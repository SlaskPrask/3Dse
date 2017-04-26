#include "../include/enginelayer.h"
#include "../include/enginecallback.h"
#include "../include/platformgraphics.h"
#include "../include/platform.h"
#ifdef ANDROID
#include "../include/androidnames.h"
#endif

using namespace _engineprivate;
using namespace _ENGINESPACE;

int EngineLayer::event(int id, int size, int *value)
{
	switch(id)
	{
		default:
			return 0;
		case 0://surface create
			if (!instance()->checkTextures())
			{
				instance()->contextLost();
				instance()->reloadSprites();
				instance()->reloadFonts();
			}
			return 0;
		case 1://application close
			/*not used in android, used in desktop callback*/
			instance()->state.closed=1;
			return 0;
		case 2://surface change(w,h)
			instance()->setSize(value[0],value[1]);
			instance()->state.windowResized=1;
			return 0;
		case 3://back button
			instance()->state.backButton=1;
			return 0;
		case 4://mouse down(which,x,y)
			instance()->state.mouseEvents.push_back(MouseEvent(value[0],MouseEvent::Press,value[1],value[2]));
			return 0;
		case 5://mouse move(which,x,y)
			instance()->state.mouseEvents.push_back(MouseEvent(value[0],MouseEvent::Held,value[1],value[2]));
			return 0;
		case 6://mouse up(which,x,y)
			instance()->state.mouseEvents.push_back(MouseEvent(value[0],MouseEvent::Release,value[1],value[2]));
			return 0;
		case 7://focus lost
			instance()->state.focusLost=1;
			if (instance()->gameFocusLossFunc)
			instance()->gameFocusLossFunc();
			return 0;
		case 8://run
			instance()->run(value[0]<=0?0.001:(double)value[0]/1000.0f);
			return 0;
		case 9://pause
			instance()->state.paused=1;
			return 0;
		case 10://resume
			instance()->state.resumed=1;
			return 0;
		case 11://focus gain
			instance()->state.focusGained=1;
			if (instance()->gameFocusGainFunc)
			instance()->gameFocusGainFunc();
			return 0;
		case 12://ad size change
			{
				if (value[1]<=0)
				value[1]=instance()->height<=0?1:instance()->height;
				double adRatio=(double)value[0]/(double)value[1];
				if (instance()->state.adSize!=adRatio)
				instance()->state.adChange=1;
				instance()->state.adSize=adRatio;
			}
			return 0;
	}
	return 0;
}

std::string EngineLayer::stringEvent(int id, int size, std::string *value)
{
	std::string ret="";
	switch(id)
	{
		default:
			break;
		case 0://text keyboard input
			{
				instance()->state.keyboardinput="";
				for (unsigned int i=0;i<value[0].length();i++)
					if (value[0][i]<256)
						instance()->state.keyboardinput+=value[0][i];
			}
			break;
		case 1://version report
			{
				Log::log("Engine",std::string("Platform version ")+value[0]+" "+value[1]+" "+value[2]+" SDK "+value[3]);
			}
			break;
	}
	return ret;
}

void EngineLayer::eventParser()
{
	backButton=state.backButton;
	windowResized=state.windowResized;
	closed=state.closed;
	paused=state.paused;
	resumed=state.resumed;
	focusGained=state.focusGained;
	focusLost=state.focusLost;
	keyboardinput=state.keyboardinput;
	adChange=state.adChange;
	adSize=state.adSize;
	state.adChange=state.backButton=state.windowResized=state.closed=state.paused=state.resumed=state.focusGained=state.focusLost=0;
	state.keyboardinput="";
	
	if (windowResized)
	{
		if (width!=reqWidth||height!=reqHeight)
		{
			reqWidth=width;
			reqHeight=height;
		}
	}

	if (focusGained)
	focus=1;
	if (focusLost)
	focus=0;

	for (int i=0;i<_MAX_MOUSES;i++)
	{
		mouseWheelUp[i]=0;
		mouseWheelDown[i]=0;

		for (int j=0;j<_MAX_MOUSE_BUTTONS;j++)
		{
			mousePresses[i]=-1;
			if (forcedRelease||paused)
			{
				switch (mouseState[i][j])
				{
					default:
						break;
					case MouseEvent::Release:
						mouseState[i][j]=MouseEvent::Unheld;
						break;
					case MouseEvent::Press:
					case MouseEvent::Held:
						mouseState[i][j]=MouseEvent::Release;
						break;
				}
				forcedRelease=0;
			}
			else
			switch (mouseState[i][j])
			{
				default:
					break;
				case MouseEvent::Release:
					mouseState[i][j]=MouseEvent::Unheld;
					break;
				case MouseEvent::Press:
					mouseState[i][j]=MouseEvent::Held;
					break;
			}
		}
	}

	keyboardstr=keyboardinput;

	#ifndef ANDROID
	sf::Event event;

	anykeyreleased=anykeypressed=0;

	//0 is not pressed, 1 is initial press, 2 is held down, -1 is initial release
	for (int i=0;i<MAX_KEYBOARD_KEYS;i++)
	{
		if (key[i]==1)
		key[i]=2;
		else
		if (key[i]==-1)
		key[i]=0;
	}

	if (window)
	while (window->pollEvent(event))
	{
		switch (event.type)
		{
			default:
				break;
			case sf::Event::Closed:
				instance()->state.closed=1;
				break;
			case sf::Event::Resized:
				instance()->setSize(event.size.width,event.size.height);
				instance()->state.windowResized=1;
				break;
			case sf::Event::LostFocus:
				instance()->state.focusLost=1;

				for (int i=0;i<_MAX_MOUSE_BUTTONS;i++)
				state.mouseEvents.push_back(MouseEvent(0,MouseEvent::Release,mouseX[0],mouseY[0],i));
				break;
			case sf::Event::GainedFocus:
				instance()->state.focusGained=1;
				break;
			case sf::Event::TextEntered:
				if (event.text.unicode<256)
				keyboardstr+=event.text.unicode;
				break;
			case sf::Event::KeyPressed:
				if (event.key.code>=0&&event.key.code<MAX_KEYBOARD_KEYS)
					if (key[event.key.code]<=0)
					{
						key[event.key.code]=1;
						anykeypressed=1;
						anykeyheld++;
					}
				break;
			case sf::Event::KeyReleased:
				if (event.key.code>=0&&event.key.code<MAX_KEYBOARD_KEYS)
					if (key[event.key.code]>0)
					{
						key[event.key.code]=-1;
						anykeyreleased=1;
						anykeyheld--;
					}
				break;
			case sf::Event::MouseWheelMoved:
				if (event.mouseWheel.delta<0)
				mouseWheelDown[0]=-event.mouseWheel.delta;
				else
				mouseWheelUp[0]=event.mouseWheel.delta;

				state.mouseEvents.push_back(MouseEvent(0,MouseEvent::Move,event.mouseWheel.x,event.mouseWheel.y));
				break;
			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button>=0&&event.mouseButton.button<_MAX_MOUSES)
				state.mouseEvents.push_back(MouseEvent(0,MouseEvent::Press,event.mouseButton.x,event.mouseButton.y,event.mouseButton.button));
				break;
			case sf::Event::MouseButtonReleased:
				if (event.mouseButton.button>=0&&event.mouseButton.button<_MAX_MOUSES)
				state.mouseEvents.push_back(MouseEvent(0,MouseEvent::Release,event.mouseButton.x,event.mouseButton.y,event.mouseButton.button));
				break;
			case sf::Event::MouseMoved:
				state.mouseEvents.push_back(MouseEvent(0,MouseEvent::Move,event.mouseMove.x,event.mouseMove.y));
				break;
			case sf::Event::MouseEntered:
				break;
			case sf::Event::MouseLeft:
				break;
			case sf::Event::TouchBegan:
				break;
			case sf::Event::JoystickButtonPressed:
				break;
			case sf::Event::JoystickButtonReleased:
				break;
			case sf::Event::JoystickMoved:
				break;
			case sf::Event::JoystickConnected:
				break;
			case sf::Event::JoystickDisconnected:
				break;
		}
	}
	#endif

	int newPresses=0;
	MouseEvent me;

	while (!(state.mouseEvents.empty()))
	{
		me=*(state.mouseEvents.begin());
		if (me.which<_MAX_MOUSES)
		{
			//could ignore this if the "if"'s in cases fail
			mouseX[me.which]=me.x;
			mouseY[me.which]=me.y;
			if (getCameraW()==getWidth())
			mouseTX[me.which]=getCameraX()+(double)me.x;
			else
			mouseTX[me.which]=-getHorBar()+getCameraX()+(double)me.x/(double)getWidth()*getRegionW();

			if (getCameraH()==getHeight())
			mouseTY[me.which]=getCameraY()+(double)me.y;
			else
			mouseTY[me.which]=-getVerBar()+getCameraY()+(double)me.y/(double)getHeight()*getRegionH();

			switch (me.type)
			{
				default:
				case MouseEvent::Move:
				case MouseEvent::Held:
				case MouseEvent::Unheld:
					break;
				case MouseEvent::Release:
					if (mouseState[me.which][me.button]==MouseEvent::Held||mouseState[me.which][me.button]==MouseEvent::Press)
					mouseState[me.which][me.button]=MouseEvent::Release;
					break;
				case MouseEvent::Press:
					if (mouseState[me.which][me.button]==MouseEvent::Unheld||mouseState[me.which][me.button]==MouseEvent::Release)
					{
						mouseState[me.which][me.button]=MouseEvent::Press;
						for(int i=0;i<_MAX_MOUSES;i++)
						{
							if (mousePresses[i]==me.which)
							break;
							if (mousePresses[i]==-1)
							{
								mousePresses[i]=me.which;
								newPresses++;
								break;
							}
						}
					}
					break;
			}
		}
		state.mouseEvents.erase(state.mouseEvents.begin());
	}
}

void EngineLayer::message(const std::string &m)
{
	CallbackMessage(m);
}

#ifdef ANDROID
void EngineLayer::setJVM(JNIEnv *env)
{
	jint rs=env->GetJavaVM(&(instance()->jvm));
	if (rs!=JNI_OK)
	Log::error("Engine","JVM unavailable");

	jclass localclass;
	localclass=env->FindClass(JNILAYER_______);
	if (!localclass)
		Log::error("Engine","Unable to find JNI engine");
	else
	{
		instance()->JNIEngineLayer=(_jclass*)env->NewGlobalRef(localclass);
		env->DeleteLocalRef(localclass);
	}

	localclass=env->FindClass(JNIASSETS______);
	if (!localclass)
		Log::error("Engine","Unable to find JNI assets");
	else
	{
		instance()->JNIAssetManager=(_jclass*)env->NewGlobalRef(localclass);
		env->DeleteLocalRef(localclass);
	}
}
#endif

#ifdef ANDROID
JNIEnv* EngineLayer::getEnv(bool *attached)
{
	JNIEnv *env;
	if (instance()->jvm->GetEnv((void **)&env,JNI_VERSION_1_6)==JNI_EDETACHED)
	{
		instance()->jvm->AttachCurrentThread(&env,NULL);
		*attached=1;
	}
	else
	*attached=0;
	return env;
}
void EngineLayer::giveEnv(bool *attached)
{
	if (*attached)
	instance()->jvm->DetachCurrentThread();
}
#endif

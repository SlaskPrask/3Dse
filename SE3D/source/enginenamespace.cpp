#include "../include/enginenamespace.h"

using namespace _engineprivate;
using namespace _ENGINESPACE;

int _ENGINESPACE::getSceneWidth()
{
	if (getScene())
	return getScene()->width;
	else
	return 1;
}

int _ENGINESPACE::getSceneHeight()
{
	if (getScene())
	return getScene()->height;
	else
	return 1;
}

int _ENGINESPACE::run(int argc,char* argv[],void(*func)()=NULL)
{
	if (func)
	setGameStartFunction(func);
	return EngineLayer::instance()->runGame(argc,argv);
}

std::string _ENGINESPACE::getKeyName(int keycode)
{
	if (keycode<0||keycode>=EngineLayer::MAX_KEYBOARD_KEYS)
	return "";

	switch (keycode)
	{
		default:
			{
				std::ostringstream s("Key ");
				s<<to_string(keycode);
				return s.str();
			}
			break;
		case _ENGINESPACE::Key::A:
			return "A";
		case _ENGINESPACE::Key::B:
			return "B";
		case _ENGINESPACE::Key::C:
			return "C";
		case _ENGINESPACE::Key::D:
			return "D";
		case _ENGINESPACE::Key::E:
			return "E";
		case _ENGINESPACE::Key::F:
			return "F";
		case _ENGINESPACE::Key::G:
			return "G";
		case _ENGINESPACE::Key::H:
			return "H";
		case _ENGINESPACE::Key::I:
			return "I";
		case _ENGINESPACE::Key::J:
			return "J";
		case _ENGINESPACE::Key::K:
			return "K";
		case _ENGINESPACE::Key::L:
			return "L";
		case _ENGINESPACE::Key::M:
			return "M";
		case _ENGINESPACE::Key::N:
			return "N";
		case _ENGINESPACE::Key::O:
			return "O";
		case _ENGINESPACE::Key::P:
			return "P";
		case _ENGINESPACE::Key::Q:
			return "Q";
		case _ENGINESPACE::Key::R:
			return "R";
		case _ENGINESPACE::Key::S:
			return "S";
		case _ENGINESPACE::Key::T:
			return "T";
		case _ENGINESPACE::Key::U:
			return "U";
		case _ENGINESPACE::Key::V:
			return "V";
		case _ENGINESPACE::Key::W:
			return "W";
		case _ENGINESPACE::Key::X:
			return "X";
		case _ENGINESPACE::Key::Y:
			return "Y";
		case _ENGINESPACE::Key::Z:
			return "Z";
		case _ENGINESPACE::Key::Num0:
			return "0";
		case _ENGINESPACE::Key::Num1:
			return "1";
		case _ENGINESPACE::Key::Num2:
			return "2";
		case _ENGINESPACE::Key::Num3:
			return "3";
		case _ENGINESPACE::Key::Num4:
			return "4";
		case _ENGINESPACE::Key::Num5:
			return "5";
		case _ENGINESPACE::Key::Num6:
			return "6";
		case _ENGINESPACE::Key::Num7:
			return "7";
		case _ENGINESPACE::Key::Num8:
			return "8";
		case _ENGINESPACE::Key::Num9:
			return "9";
		case _ENGINESPACE::Key::Left:
			return "Left";
		case _ENGINESPACE::Key::Right:
			return "Right";
		case _ENGINESPACE::Key::Up:
			return "Up";
		case _ENGINESPACE::Key::Down:
			return "Down";
		case _ENGINESPACE::Key::Escape:
			return "Escape";
		case _ENGINESPACE::Key::LControl:
			return "Left Control";
		case _ENGINESPACE::Key::RControl:
			return "Right Control";
		case _ENGINESPACE::Key::LShift:
			return "Left Shift";
		case _ENGINESPACE::Key::RShift:
			return "Right Shift";
		case _ENGINESPACE::Key::LAlt:
			return "Left Alt";
		case _ENGINESPACE::Key::RAlt:
			return "Right Alt";
		case _ENGINESPACE::Key::LSystem:
			return "Left System";
		case _ENGINESPACE::Key::RSystem:
			return "Right System";
		case _ENGINESPACE::Key::Menu:
			return "Menu";
		case _ENGINESPACE::Key::LBracket:
			return "[";
		case _ENGINESPACE::Key::RBracket:
			return "]";
		case _ENGINESPACE::Key::SemiColon:
			return ";";
		case _ENGINESPACE::Key::Comma:
			return ",";
		case _ENGINESPACE::Key::Period:
			return ".";
		case _ENGINESPACE::Key::Quote:
			return "'";
		case _ENGINESPACE::Key::Slash:
			return "/";
		case _ENGINESPACE::Key::BackSlash:
			return "\\";
		case _ENGINESPACE::Key::Tilde:
			return "~";
		case _ENGINESPACE::Key::Equal:
			return "=";
		case _ENGINESPACE::Key::Space:
			return "Space";
		case _ENGINESPACE::Key::Return:
			return "Return";
		case _ENGINESPACE::Key::BackSpace:
			return "Backspace";
		case _ENGINESPACE::Key::Tab:
			return "Tab";
		case _ENGINESPACE::Key::PageUp:
			return "Page Up";
		case _ENGINESPACE::Key::PageDown:
			return "Page Down";
		case _ENGINESPACE::Key::End:
			return "End";
		case _ENGINESPACE::Key::Insert:
			return "Insert";
		case _ENGINESPACE::Key::Delete:
			return "Delete";
		case _ENGINESPACE::Key::Add:
			return "+";
		case _ENGINESPACE::Key::Subtract:
			return "-";
		case _ENGINESPACE::Key::Multiply:
			return "*";
		case _ENGINESPACE::Key::Divide:
			return "Numpad /";
		case _ENGINESPACE::Key::Numpad0:
			return "Numpad 0";
		case _ENGINESPACE::Key::Numpad1:
			return "Numpad 1";
		case _ENGINESPACE::Key::Numpad2:
			return "Numpad 2";
		case _ENGINESPACE::Key::Numpad3:
			return "Numpad 3";
		case _ENGINESPACE::Key::Numpad4:
			return "Numpad 4";
		case _ENGINESPACE::Key::Numpad5:
			return "Numpad 5";
		case _ENGINESPACE::Key::Numpad6:
			return "Numpad 6";
		case _ENGINESPACE::Key::Numpad7:
			return "Numpad 7";
		case _ENGINESPACE::Key::Numpad8:
			return "Numpad 8";
		case _ENGINESPACE::Key::Numpad9:
			return "Numpad 9";
		case _ENGINESPACE::Key::F1:
			return "F1";
		case _ENGINESPACE::Key::F2:
			return "F2";
		case _ENGINESPACE::Key::F3:
			return "F3";
		case _ENGINESPACE::Key::F4:
			return "F4";
		case _ENGINESPACE::Key::F5:
			return "F5";
		case _ENGINESPACE::Key::F6:
			return "F6";
		case _ENGINESPACE::Key::F7:
			return "F7";
		case _ENGINESPACE::Key::F8:
			return "F8";
		case _ENGINESPACE::Key::F9:
			return "F9";
		case _ENGINESPACE::Key::F10:
			return "F10";
		case _ENGINESPACE::Key::F11:
			return "F11";
		case _ENGINESPACE::Key::F12:
			return "F12";
		case _ENGINESPACE::Key::F13:
			return "F13";
		case _ENGINESPACE::Key::F14:
			return "F14";
		case _ENGINESPACE::Key::F15:
			return "F15";
		case _ENGINESPACE::Key::Pause:
			return "Pause";
	}
	return "";
}

std::string _ENGINESPACE::getMouseName(int i)
{
	if (i<0||i>=_MAX_MOUSE_BUTTONS)
	return "";

	switch (i)
	{
		default:
			{
				std::ostringstream s("Mouse ");
				s<<to_string(i);
				return s.str();
			}
			break;
		case _ENGINESPACE::Mouse::MouseLeft:
			return "Left Mouse";
		case _ENGINESPACE::Mouse::MouseRight:
			return "Right Mouse";
		case _ENGINESPACE::Mouse::MouseMiddle:
			return "Middle Mouse";
		case _ENGINESPACE::Mouse::Mouse4:
			return "Mouse 4";
		case _ENGINESPACE::Mouse::Mouse5:
			return "Mouse 5";
	}
	return "";
}

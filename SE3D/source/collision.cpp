#include "../include/collision.h"
#include "../include/enginelayer.h"
#include "../include/object.h"

using namespace _engineprivate;
using namespace _ENGINESPACE;

void Collision::split()
{
	child[0]=new Collision(x,y,(x+x2)/2.0f,(y+y2)/2.0f,depth+1,this);
	child[1]=new Collision((x+x2)/2.0f,y,x2,(y+y2)/2.0f,depth+1,this);
	child[2]=new Collision(x,(y+y2)/2.0f,(x+x2)/2.0f,y2,depth+1,this);
	child[3]=new Collision((x+x2)/2.0f,(y+y2)/2.0f,x2,y2,depth+1,this);
}

void Collision::remove(Hitbox* h)
{
	removeHitbox(h);

	if (children())
	attemptCombine();
	else
	if (parent)
	parent->attemptCombine();
}

void Collision::move(Hitbox* h)
{
	int q;
	if (children())
	q=h->where(this);
	else
	q=h->inside(this)?4:-1;

	switch (q)
	{
		default:
		case 4://stay
			break;
		case -1://outside
			removeHitbox(h);

			if (parent)
			EngineLayer::instance()->getMasterCollision()->add(h);
			else
			expand(h);

			if (children())
			attemptCombine();
			else
			if (parent)
			parent->attemptCombine();
			break;
		case 0:
		case 1:
		case 2:
		case 3:
			removeHitbox(h);
			child[q]->add(h);
			break;
	}
}

void Collision::attemptCombine()
{
	//assuing children exist
	if (!child[0]->children()&&!child[1]->children()&&!child[2]->children()&&!child[3]->children()
	&&  hitbox.size()+child[0]->hitbox.size()+child[1]->hitbox.size()+child[2]->hitbox.size()+child[3]->hitbox.size()<=EngineLayer::instance()->DEFAULT_COLLISION_OBJECTS)
	{
		for (int i=0;i<4;i++)
		{
			for (std::vector<Hitbox*>::iterator it=child[i]->hitbox.begin();it!=child[i]->hitbox.end();++it)
			addHitbox(*it);
			delete child[i];
			child[i]=NULL;
		}
		if (parent)
		parent->attemptCombine();
	}
}

void Collision::addHitbox(Hitbox *h)
{
	//include split and split check when !children()

	if (children())
	insert(h);
	else
	if (hitbox.size()>=EngineLayer::DEFAULT_COLLISION_OBJECTS&&depth<EngineLayer::DEFAULT_COLLISION_DEPTH)
	{
		split();
		std::vector<Hitbox*> temp(hitbox.begin(),hitbox.end());
		temp.push_back(h);
		hitbox.clear();

		for (std::vector<Hitbox*>::iterator it=temp.begin();it!=temp.end();++it)
		add(*it);
	}
	else
	insert(h);
}

void Collision::removeHitbox(Hitbox *h)
{
	std::vector<Hitbox*>::iterator it=std::find(hitbox.begin(),hitbox.end(),h);
	if (it!=hitbox.end())
	hitbox.erase(it);
}


void Collision::expand(Hitbox *h)
{
	switch (h->getExpansion(this))
	{
		default:
		case 0:
			parent=new Collision(x,y,x2*2-x,y2*2-y,depth-1);
			parent->giveChild(this,0);
			break;
		case 1:
			parent=new Collision(x*2-x2,y,x2,y2*2-y,depth-1);
			parent->giveChild(this,1);
			break;
		case 2:
			parent=new Collision(x,y*2-y2,x2*2-x,y2,depth-1);
			parent->giveChild(this,2);
			break;
		case 3:
			parent=new Collision(x*2-x2,y*2-y2,x2,y2,depth-1);
			parent->giveChild(this,3);
			break;
	}
	EngineLayer::instance()->setMasterCollision(parent);
	parent->reverseAdd(h);
}

void Collision::giveChild(Collision* c,int index)
{
	switch (index)
	{
		default:
		case 0:
			child[0]=c;
			child[1]=new Collision(c->getRight(),y,x2,c->getBottom(),depth+1,this);
			child[2]=new Collision(x,c->getBottom(),c->getRight(),y2,depth+1,this);
			child[3]=new Collision(c->getRight(),c->getBottom(),x2,y2,depth+1,this);
			break;
		case 1:
			child[0]=new Collision(x,y,c->getLeft(),c->getBottom(),depth+1,this);
			child[1]=c;
			child[2]=new Collision(x,c->getBottom(),c->getLeft(),y2,depth+1,this);
			child[3]=new Collision(c->getLeft(),c->getBottom(),x2,y2,depth+1,this);
			break;
		case 2:
			child[0]=new Collision(x,y,c->getRight(),c->getTop(),depth+1,this);
			child[1]=new Collision(c->getRight(),y,x2,c->getTop(),depth+1,this);
			child[2]=c;
			child[3]=new Collision(c->getRight(),c->getTop(),x2,y2,depth+1,this);
			break;
		case 3:
			child[0]=new Collision(x,y,c->getLeft(),c->getTop(),depth+1,this);
			child[1]=new Collision(c->getLeft(),y,x2,c->getTop(),depth+1,this);
			child[2]=new Collision(x,c->getTop(),c->getLeft(),y2,depth+1,this);
			child[3]=c;
			break;
	};
}

void Collision::insert(Hitbox *h)
{
	h->setMap(this);
	hitbox.push_back(h);
}

Collision::Collision(double fx,double fy,double fx2,double fy2,int d,Collision *par):x(fx),y(fy),x2(fx2),y2(fy2),parent(par),depth(d)
{
	child[0]=child[1]=child[2]=child[3]=NULL;
}

void Collision::reverseAdd(Hitbox *h)
{
	if (h->inside(this))
	addHitbox(h);
	else
	{
		if (parent)
		parent->reverseAdd(h);
		else
		expand(h);
	}
}

void Collision::add(Hitbox *h)
{
	int q;
	if (children())
	q=h->where(this);
	else
	q=h->inside(this)?4:-1;

	switch (q)
	{
		default:
		case -1:
			if (parent)
			parent->reverseAdd(h);
			else
			expand(h);
			break;
		case 4: //this only
			addHitbox(h);
			break;
		case 0:
		case 1:
		case 2:
		case 3:
			//if (children())
			child[q]->add(h);
			/*else
			addHitbox(h);*/
			break;
	}
}

bool Collision::collide(Hitbox *h,std::vector<Object*> *vec)
{
	std::vector<Hitbox*>::iterator it;
	Collision *p=parent;
	while (p)
	{
		for (it=p->hitbox.begin();it!=p->hitbox.end();++it)
		if (h->getCollision(*it))
		{
			std::vector<Object*>::iterator check=std::find(vec->begin(),vec->end(),(*it)->getObject());
			if (check==vec->end())
			vec->push_back((*it)->getObject());
		}

		p=p->parent;
	}

	collideChildren(h,vec);

	return !vec->empty();
}

void Collision::collideChildren(Hitbox *h,std::vector<Object*> *vec)
{
	for (std::vector<Hitbox*>::iterator it=hitbox.begin();it!=hitbox.end();++it)
	if (h->getCollision(*it))
	{
		std::vector<Object*>::iterator check=std::find(vec->begin(),vec->end(),(*it)->getObject());
		if (check==vec->end())
		vec->push_back((*it)->getObject());
	}

	if (children())
	{
		child[0]->collideChildren(h,vec);
		child[1]->collideChildren(h,vec);
		child[2]->collideChildren(h,vec);
		child[3]->collideChildren(h,vec);
	}
}

#ifdef DEBUG
void Collision::draw(bool c)
{
	EngineLayer::instance()->drawRectangle(x,y,x2-x,y2-y,0,c?0:1,0,c?1:0,.15);
	if (EngineLayer::instance()->resourceSet(0)&&EngineLayer::instance()->resourceSet(0)->getFont(0))
	EngineLayer::instance()->drawText(EngineLayer::instance()->resourceSet(0)->getFont(0),to_string(hitbox.size()),x+(x2-x)/2,y+(x2-x)/2-(x2-x)/20,(x2-x)/10,0,1,1,1,1,1);

	if (children())
	{
		child[0]->draw(0);
		child[1]->draw(1);
		child[2]->draw(1);
		child[3]->draw(0);
	}

	for (std::vector<Hitbox*>::iterator it=hitbox.begin();it!=hitbox.end();++it)
	(*it)->draw();
}
#endif

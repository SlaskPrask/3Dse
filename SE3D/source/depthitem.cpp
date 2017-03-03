#include "../include/depthitem.h"
#include "../include/enginelayer.h"

using namespace _engineprivate;
using namespace _ENGINESPACE;

DepthItem::DepthItem(Object* i)
{
	nextNode=0;
	prevNode=0;
	item=i;
	item->_depthItem=this;
}

void DepthItem::addAbove(DepthItem *t)
{
	if (getDepth()>=t->getDepth())
	{
		prevNode=t->getPrevious();
		nextNode=t;
		if (prevNode)
		prevNode->next(this);
		else
		{
			prevNode=0;
			EngineLayer::instance()->setFirstDepth(this);
		}
		nextNode->prev(this);
	}
	else
	if (t->getNext())
	{
		addAbove(t->getNext());
	}
	else
	{
		nextNode=0;
		prevNode=t;
		t->next(this);
	}
}

void DepthItem::addBelow(DepthItem *t)
{
	if (getDepth()<=t->getDepth())
	{
		nextNode=t->getNext();
		prevNode=t;
		if (nextNode)
		nextNode->prev(this);
		prevNode->next(this);
	}
	else
	if (t->getPrevious())
	{
		addBelow(t->getPrevious());
	}
	else
	{
		prevNode=0;
		nextNode=t;
		EngineLayer::instance()->setFirstDepth(this);
		t->prev(this);
	}
}

void DepthItem::moveUp()
{
	if (prevNode)
	{
		if (prevNode->getDepth()<getDepth())
		{
			detach();
			addBelow(prevNode);
		}
	}
}

void DepthItem::moveDown()
{
	if (nextNode)
	{
		if (nextNode->getDepth()>getDepth())
		{
			detach();
			addAbove(nextNode);
		}
	}
}

void DepthItem::detach()
{
	if (prevNode)
	prevNode->next(nextNode);
	else
	EngineLayer::instance()->setFirstDepth(nextNode);
	
	if (nextNode)
	nextNode->prev(prevNode);
}

DepthItem::~DepthItem()
{
	detach();
}

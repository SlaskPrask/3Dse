#pragma once

#include "enginesettings.h"

namespace _engineprivate
{
	template <class T>
	class LinkedList;

	template <class T>
	class ListHandle
	{
		friend class _engineprivate::LinkedList<T>;
		
		private:
		LinkedList<T> *firstNode;
		LinkedList<T> *lastNode;
		
		inline void setFirst(T *t)
		{
			firstNode=t;
		}
		inline void setLast(T *t)
		{
			lastNode=t;
		}
		
		public:
		inline LinkedList<T> *first()
		{
			return firstNode;
		}
		inline LinkedList<T> *last()
		{
			return lastNode;
		}
		void add(T* t)
		{
			if (lastNode)
			lastNode->_add(t);
			else
			{
				t->_listHandle=this;
				firstNode=t;
				lastNode=t;
			}
		}
		ListHandle()
		{
			firstNode=0;
			lastNode=0;
		}
		~ListHandle()
		{
			if (firstNode)
			firstNode->_removeHandleDown();
		}
	};
}

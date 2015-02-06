#pragma once
#include "cinder/ImageIO.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "Dispatcher.h"

using namespace ci::signals;
using namespace ci::app;
using namespace std;
using namespace ci;

namespace kubik
{	
	class CompositeDispatcher: public Dispatcher
	{
	public:
		CompositeDispatcher():Dispatcher()
		{
		}

		void addChild(DispatcherRef child)
		{
			child->setChildPosition(_localPosition);
			child->setAlpha(alpha);
			child->setParent(this);
			displayList.push_back(child);
		}	

		void removeChild(DispatcherRef child)
		{
			displayList.remove(child);
		}

		virtual void draw()
		{
			for (auto comp : displayList)			
				comp->draw();
			Dispatcher::draw();
		}	

		virtual void setPosition(ci::Vec2i position)		
		{
			_localPosition = position;		
			_parentPosition = getFullPosition();
		
			updateGlobalPosition();
			for (auto comp : displayList)			
				comp->setChildPosition(position);			
		}	

		virtual void setChildPosition(ci::Vec2i position)
		{
			_parentPosition = getFullPosition();
		
			updateGlobalPosition();
			for (auto comp : displayList)			
				comp->setChildPosition(position);
		}

		virtual void setParentPosition(ci::Vec2i position)		
		{
			_parentPosition = position;
			updateGlobalPosition();
			for (auto comp : displayList)			
				comp->setParentPosition(position);	
		}	

		virtual void setAlpha(float alpha)
		{
			this->alpha = alpha;
			for (auto comp : displayList)			
				comp->setAlpha(alpha);
		}

		/*virtual void mouseUp(MouseEvent &event)
		{	
			mouseUp(event);
			for (auto comp : displayList)			
				comp->mouseUp(event);					
		}	*/	

		virtual void unActivateListeners()
		{
			for (auto comp : displayList)
				comp->unActivateListeners();

			Dispatcher::unActivateListeners();
		}

		virtual void activateListeners()
		{			
			for (auto comp : displayList)
				comp->activateListeners();

			Dispatcher::activateListeners();
		}			

	protected:
		list<shared_ptr<Dispatcher>> displayList;
	};

	typedef shared_ptr<CompositeDispatcher> CompositeDispatcherRef;	
}
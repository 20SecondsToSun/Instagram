#pragma once

#include "EventGUI.h"

using namespace ci::signals;
using namespace ci::app;
using namespace std;
using namespace ci;

namespace kubik
{
	class Dispatcher 
	{

	public:

		Dispatcher()
			:isMouseUpSet(false),
			localPosition(Vec2f::zero()),
			position(Vec2f::zero()),
			_globalPosition(Vec2f::zero()),
			_localPosition(Vec2f::zero()),
			_parentPosition(Vec2f::zero()),
			alpha(1),
			name("default"),
			parent(NULL)
		{
		}

		void addChild(shared_ptr<Dispatcher> child)
		{
			//1/0;
		}

		virtual void setParent(Dispatcher* parent)
		{
			this->parent = parent;
		}

		virtual void draw()
		{
			gl::pushMatrices();				
			gl::translate(getGlobalPosition());
				drawLayout();
			gl::popMatrices();
		}

		virtual void drawLayout()
		{

		}

		Vec2f getAbsolutePosition()
		{
			return localPosition + position;
		}

		virtual void setLocalPosition(ci::Vec2i position)
		{
			localPosition = position;
			_localPosition = position;
		}

		virtual void setChildPosition(ci::Vec2i position)
		{
			_parentPosition = getFullPosition();
			updateGlobalPosition();
		}

		virtual void setPosition(ci::Vec2i position)		
		{
			_localPosition = position;
			updateGlobalPosition();
		}

		virtual ci::Vec2f getFullPosition()		
		{
			Vec2f _position = Vec2f::zero();
			
			if(parent)
				_position = parent->getGlobalPosition();

			return _position;
		}

		virtual void setParentPosition(ci::Vec2i position)		
		{
			_parentPosition = position;
			updateGlobalPosition();
		}

		virtual void updateGlobalPosition()
		{
			_globalPosition = _localPosition + _parentPosition;
		}

		Vec2f getGlobalPosition()
		{
			return _globalPosition;
		}

		Vec2f getLocalPosition()
		{
			return _localPosition;
		}
		
		Vec2f getParentPosition()
		{
			return _parentPosition;
		}

		virtual Vec2f getPosition()		
		{
			return _localPosition;			
		}

		virtual void setAlpha(float alpha)
		{
			this->alpha = alpha;	
		}

		template<typename T, typename Y>
		void addMouseUpListener( T fn, Y *inst )
		{
			if(isMouseUpSet) return;

			isMouseUpSet = true;
			
			mouseUpListener = getWindow()->connectMouseUp(&Dispatcher::mouseUp, this);							
			mouseUpSignalListener = mouseUpSignal.connect(std::bind( fn, inst, std::placeholders::_1 ));
		}
	
		void removeMouseUpListener()
		{
			mouseUpListener.disconnect();
			mouseUpSignalListener.disconnect();
			isMouseUpSet = false;
		}	

		virtual void mouseUp(MouseEvent &_event)
		{
			//console()<<"seeet"<<endl;
			//mouseUpSignal(event);
		}

		/*virtual void mouseUpHandler(ci::Vec2i pos)
		{	
			mouseUpSignal(event);
		}*/	

		virtual void mouseUpFunction(EventGUIRef event )
		{
			mouseUpSignal(event);
		}

		virtual void activateListeners()
		{

		}	

		virtual void unActivateListeners()
		{			
			removeMouseUpListener();			
		}

		Dispatcher* getParent()
		{			
			return parent;			
		}
			
		boost::signals2::signal<void(EventGUIRef &)> mouseUpSignal;			

	protected:
		Vec2i position, localPosition;
		float alpha;
		bool isMouseUpSet;	
		connection mouseUpListener, mouseUpSignalListener;
		EventGUIRef event;	
		string name;
		Dispatcher* parent;

		Vec2f _localPosition, _parentPosition, _globalPosition;
	};
	typedef shared_ptr<Dispatcher> DispatcherRef;	
}
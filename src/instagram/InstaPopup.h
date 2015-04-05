#pragma once
#include "cinder/app/AppNative.h"
#include "CompositeDispatcher.h"
#include "cinder/Timeline.h"
#include "InstagramClient.h"

using namespace kubik;

namespace instagram
{
	typedef std::shared_ptr<class InstaPopup> InstaPopupRef;

	class InstaPopup: public CompositeDispatcher
	{
		typedef ci::signals::signal<void(void)> SignalVoid;

	public:	
		InstaPopup(InstagramClientRef client):
			client(client),
			CompositeDispatcher(),
			bgColor(ColorA(1,1,1,0.0)),
			alpha(0),
			showing(false),
			connected(false)
		{

		}			

		virtual void draw()
		{
			using namespace ci;	

			if(!showing)
				return;
			
			gl::pushMatrices();				
			gl::translate(getGlobalPosition());
			gl::color(ColorA(bgColor.r, bgColor.g, bgColor.b, alpha));
			gl::drawSolidRect(getWindowBounds());
			gl::color(Color::white());

			ci::gl::Texture tex = image.getStandartResImage();

			if (tex)
				gl::draw(tex, Vec2f(0.5*(getWindowWidth() - tex.getWidth()), 0.5*(getWindowHeight() - tex.getHeight())));
			

			gl::popMatrices();
			CompositeDispatcher::draw();
		}

		void show(const ImageGraphic& image, ci::EaseFn eFunc = ci::EaseOutCubic(), float time = 0.7f)
		{
			this->image = image;
			showing = true;
			alpha = 0.0f;
			timeline().apply(&alpha, 0.85f, time, eFunc)
				.updateFn(bind( &InstaPopup::alphAnimationUpdate, this))
				.finishFn( bind( &InstaPopup::showAnimationFinish, this));
		}

		void hide(ci::EaseFn eFunc = ci::EaseOutCubic(), float time = 0.7f)
		{	
			showing = false;
			timeline().apply(&alpha, 0.0f, time, eFunc)
				.updateFn(bind( &InstaPopup::alphAnimationUpdate, this))
				.finishFn( bind( &InstaPopup::hideAnimationFinish, this));
		}		

		void alphAnimationUpdate()
		{
			setAlpha(alpha.value());
		}

		void showAnimationFinish()
		{

		}

		void hideAnimationFinish()
		{
			//HideCompleteSignal();
		}

		virtual void setAlpha(float  alpha)
		{
			//bgColor = Utils::colorAlpha(bgColor, alpha);
			//closeBlock->setAlpha(alpha);
			//CompositeDispatcher::setAlpha(alpha);
		}

		void connect()
		{
			if(connected) return;

			using namespace ci::app;
			mouseUpCon   = getWindow()->getSignalMouseUp().connect(std::bind( &InstaPopup::mouseUp, this,std::placeholders::_1) );
			connected = true;
		}
		
		void disconnect()
		{
			if(!connected) return;

			mouseUpCon.disconnect();
			connected = false;
		}

		void mouseUp( MouseEvent event )
		{
			event.setHandled(true);
			touchedEvent();
		}

		SignalVoid touchedEvent;

	private:
		ci::gl::Texture tex;
		ImageGraphic image;
		ci::signals::scoped_connection	mouseUpCon;			

		ci::ColorA bgColor;
		ci::Anim<float> alpha;
		InstagramClientRef client;

		bool showing, connected;
	};
}

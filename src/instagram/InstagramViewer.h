#pragma once
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Json.h"
#include "cinder/Timeline.h"
#include "InstagramClient.h"

namespace instagram
{
	class InstagramViewer
	{
		typedef ci::signals::signal<void(void)> SignalVoid;
		std::vector<ImageGraphic> images;
	public:	
		InstagramViewer(InstagramClientRef client)
			:client(client),
			currentPos(Vec2f::zero()),
			futureCurrentPos(Vec2f::zero()),
			blockDrag(false),
			oneImageWidth(306),
			oneImageHeight(306),
			countInRaw(4),
			connected(false),
			mainHeight(0),
			marginBottom(400),
			marginTop(400),
			animTime(0.3f),
			animFunc(EaseOutQuad())
		{
		}

		void connect()
		{
			if(connected) return;

			using namespace ci::app;
			mouseDownCon = getWindow()->getSignalMouseDown().connect(std::bind( &InstagramViewer::mouseDown, this,std::placeholders::_1) );
			mouseUpCon   = getWindow()->getSignalMouseUp().connect(std::bind( &InstagramViewer::mouseUp, this,std::placeholders::_1) );
			mouseDragCon = getWindow()->getSignalMouseDrag().connect(std::bind( &InstagramViewer::mouseDrag, this,std::placeholders::_1));
			updateCon    = App::get()->getSignalUpdate().connect(std::bind( &InstagramViewer::update, this));
			connected = true;
		}

		void disconnect()
		{
			if(!connected) return;

			mouseDownCon.disconnect();
			mouseDragCon.disconnect();
			mouseUpCon.disconnect();
			updateCon.disconnect();
			connected = false;
		}

		void update()
		{
			if (!client->isLoading() && client->needSynch())
			{
				auto _newImages = client->getImages();
				images.insert(images.end(), _newImages.begin(), _newImages.end());	
				client->setSynch(false);
			}

			mainHeight = (images.size() / countInRaw) * oneImageWidth;			
		}

		void draw()
		{
			gl::pushMatrices();
			gl::translate(currentPos.value());
			
			int x = 0, y = 0;
			
			for (int i = 0; i < images.size(); i++)
			{
				x = oneImageWidth  * (i % countInRaw);
				y = oneImageWidth  * (i / countInRaw);
				images[i].draw(Vec2f(x, y));
			}
			gl::popMatrices();	
		}	

		void mouseDown( MouseEvent event )
		{
			currentMousePos = event.getPos();
			wasDrag = false;
			downSecond =  getElapsedSeconds();
		}

		void mouseUp( MouseEvent event )
		{
			int bottomCorner = futureCurrentPos.y + mainHeight;

			if(futureCurrentPos.y > 0)
			{
				futureCurrentPos = Vec2i::zero();
				blockDrag = true;
				timeline().apply(&currentPos, futureCurrentPos, animTime, animFunc)
					.finishFn(bind( &InstagramViewer::animComplete, this));
			}
			else if(bottomCorner < getWindowHeight())
			{
				futureCurrentPos = Vec2i(0, getWindowHeight() - mainHeight);
				blockDrag = true;
				timeline().apply(&currentPos, futureCurrentPos, animTime, animFunc)
					.finishFn( bind( &InstagramViewer::animComplete, this));
			}

			//touchDelta = getElapsedSeconds() - downSecond;
			//ci::app::console()<<":::  "<<touchDelta<<std::endl;
			if(!wasDrag)
				getTouchedImage(event.getPos());
		}

		void mouseDrag( MouseEvent event )
		{
			wasDrag = true;

			delta = currentMousePos - event.getPos();			

			if(blockDrag)
				return;
		
			futureCurrentPos -= Vec2i(0, delta.y);
			
			if(currentPos.value().y > marginTop)
			{
				futureCurrentPos = Vec2i::zero();
				blockDrag = true;
			}
			else if(getWindowHeight() - mainHeight - marginBottom > currentPos.value().y)
			{
				futureCurrentPos = Vec2i(0, getWindowHeight() - mainHeight);
				blockDrag = true;

				client->loadNextMedia();
			}

			timeline().apply(&currentPos, futureCurrentPos, animTime, animFunc)
				.finishFn( bind( &InstagramViewer::animComplete, this));

			currentMousePos = event.getPos();
		}
		
		

		void getTouchedImage(ci::Vec2f pos)
		{
			//client
			int xCoord = (int)(pos.x  / oneImageWidth);
			int yCoord = (int)((pos.y - currentPos.value().y) / oneImageHeight);
			lastImageIndexTouched = yCoord * countInRaw + xCoord;
			ci::app::console()<<"lastImageIndexTouched:-------------------  "<<lastImageIndexTouched<<std::endl;

			touchedEvent();
		}

		void animComplete()
		{
			blockDrag = false;
		}

		ImageGraphic getImageGraphic()
		{
			return images[lastImageIndexTouched];
		}

		SignalVoid touchedEvent;

	protected:
		InstagramClientRef client;		
		ci::Vec2i currentMousePos, delta, futureCurrentPos;
		ci::Anim<ci::Vec2i> currentPos;

		bool blockDrag;
		bool connected;	
		bool wasDrag;

		int mainHeight;
		int oneImageWidth, oneImageHeight;
		int	countInRaw;		
		int lastImageIndexTouched;

		float marginBottom, marginTop, animTime;
		float downSecond, touchDelta;

		ci::EaseFn animFunc;	

		ci::signals::scoped_connection	mouseDownCon, mouseUpCon, mouseDragCon, updateCon;			

		void complete();
	};

	typedef shared_ptr<InstagramViewer> InstagramViewerRef;
}
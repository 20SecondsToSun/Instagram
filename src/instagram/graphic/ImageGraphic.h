#pragma once
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Json.h"
#include "cinder/Timeline.h"
#include "cinder/Channel.h"
#include "InstagramClient.h"

namespace instagram
{
	class ImageGraphic
	{

	public:
		ImageGraphic():
			animTime(.7f),
			animFunc(ci::EaseOutQuad())
		{

		}
		void setLowResSurface(ci::Surface surf)
		{
			lowSurface = surf;
		}

		ci::Surface getLowResSurface()
		{
			return lowSurface;
		}

		void fadeIn(float from, float to)
		{
			alpha = from;
			ci::app::timeline().apply(&alpha, to, animTime, animFunc);
		}

		float getAlpha()
		{
			return alpha;
		}

		void setSize(int size)
		{
			width = height = size;
		}

		void draw(ci::Vec2f vec)
		{
			using namespace ci;
			
			if(lowSurface)
			{
				gl::color(ColorA(1, 1, 1, alpha));
				gl::draw(lowSurface, vec);
			}
			else
			{
				gl::pushMatrices();
				gl::translate(vec);
				gl::color(ColorA(0.1, 0.1, 0.1, 1.0));
				gl::drawSolidRect(Rectf(5, 5, width - 5, height - 5));
				gl::popMatrices();
			}
		}

		void drawStandartResImage(ci::Vec2f vec)
		{
			using namespace ci;
			
			if(standartSurface)
			{
				gl::color(ColorA(1, 1, 1, alpha));
				gl::draw(standartSurface, vec);
			}
		}

		void setLowResURL(const std::string& url)
		{
			lowResURL = url;
		}

		const std::string& getLowResURL()
		{
			return lowResURL;
		}
		
		void setStandartResURL(const std::string& url)
		{
			standartResURL = url;
		}		

		void loadLowRes()
		{
			if(lowSurface) return;
			ci::app::console()<<lowResURL<<std::endl;
			try
			{
				lowSurface = ci::loadImage(ci::loadUrl(lowResURL));
				if(lowSurface) 
				{
					ci::app::console()<<"loaded"<<std::endl;
				}
			}
			catch(...)
			{
			}

			fadeIn(0, 1);
		}

		void setResImage(ci::Surface surf)
		{			
			lowSurface = surf;
			fadeIn(0, 1);
		}


		void loadStandartRes()
		{
			if(standartSurfaceTex)
				return;

			try
			{
				standartSurface = ci::loadImage(ci::loadUrl(standartResURL));
				standartSurfaceTex = ci::gl::Texture(standartSurface);
			}
			catch(...)
			{
			}
		}

		const ci::gl::Texture getStandartResImage()
		{
			if(standartSurface)
				return standartSurface;			
			
			return ci::gl::Texture();
		}

	protected:
		ci::Surface lowSurface;
		ci::Surface standartSurface;

		ci::gl::Texture standartSurfaceTex;
		ci::Anim<float> alpha;

		float animTime;
		ci::EaseFn animFunc;
		int width, height;

		std::string lowResURL;
		std::string standartResURL;

	};
}
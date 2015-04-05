#pragma once
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Json.h"
#include "cinder/Timeline.h"
#include "cinder/Channel.h"
#include "InstagramClient.h"
#include "TextureStore.h"

namespace instagram
{
	class ImageGraphic
	{

	public:
		ImageGraphic():
			animTime(.7f),
			animFunc(ci::EaseOutQuad()),
			loadedthumb(false)
		{

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
			if (tex)
			{
				gl::color(ColorA(1, 1, 1, 1));
				gl::draw(tex, vec);
			}				
			else
			{
				tex = ph::fetchTexture(lowResURL);
				gl::pushMatrices();
				gl::translate(vec);
				gl::color(ColorA(0.1, 0.1, 0.1, 1.0));
				gl::drawSolidRect(Rectf(5, 5, width - 5, height - 5));
				gl::popMatrices();
			}	
		}

		void setLowResURL(const std::string& url)
		{
			lowResURL = url;
		}

		std::string getLowResURL()
		{
			return lowResURL;
		}
		
		void setStandartResURL(const std::string& url)
		{
			standartResURL = url;
		}

		std::string getStandartResURL()
		{
			return standartResURL;
		}

		ci::gl::Texture getStandartResImage()
		{
			if (!bigtex)		
				bigtex = ph::fetchTexture(standartResURL);
			
			return bigtex;
		}

	protected:
		ci::gl::Texture tex, bigtex;
		bool loadedthumb;
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
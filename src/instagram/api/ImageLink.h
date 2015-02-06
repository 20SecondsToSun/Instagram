#pragma once
#include "cinder/app/AppNative.h"
#include "InstBaseObject.h"

namespace instagram
{   
    class ImageLink : public InstBaseObject
    {
	public:
		std::string getURL() const
		{
			return url;
		}

		int getWidth() const
		{
			return width;
		}

		int getHeight() const
		{
			return height;
		}

		void setURL(std::string value)
		{
			url = value;
		}

		void setWidth(int value)
		{
			width = value;
		}

		void setHeight(int value)
		{
			height = value;
		}

		void parse(ci::JsonTree tree)
		{
			url    = tree.getChild("url").getValue<std::string>();	
			width  = tree.getChild("width").getValue<int>();	
			height = tree.getChild("height").getValue<int>();
		}

	private:
		std::string url;        
		int width;
        int height;
	};
}
   
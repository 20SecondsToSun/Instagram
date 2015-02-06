#pragma once
#include "cinder/app/AppNative.h"
#include "ImageLink.h"

namespace instagram
{   
    class ImagesList : public InstBaseObject
    {
	public:
		ImageLink getLowResolution() const
		{
			return lowResolution;
		}

		ImageLink getThumbnail() const
		{
			return thumbnail;
		}

		ImageLink getStandardResolution() const
		{
			return standardResolution;
		}

		void setLowResolution(ImageLink value)
		{
			lowResolution = value;
		}

		void setThumbnail(ImageLink value)
		{
			thumbnail = value;
		}

		void setStandardResolution(ImageLink value)
		{
			standardResolution = value;
		}

		void parse(ci::JsonTree tree)
		{
			lowResolution.parse(tree.getChild("low_resolution"));
			thumbnail.parse(tree.getChild("thumbnail"));
			standardResolution.parse(tree.getChild("standard_resolution"));
		}

	private:
		ImageLink lowResolution;
		ImageLink thumbnail;
		ImageLink standardResolution;
	};
}
   
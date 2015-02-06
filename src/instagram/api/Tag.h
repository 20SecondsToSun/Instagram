#pragma once
#include "cinder/app/AppNative.h"

namespace instagram
{   
    class Tag
    {
		int media_count;
		std::string name;

	public:
		std::string getName()
		{
			return name;
		}

		int getMediaCount()
		{
			return media_count;
		}

		void setName(std::string value)
		{
			this->name = value;
		}

		void setMediaCount(int value)
		{
			this->media_count = value;
		}
	
	};
}

#pragma once
#include "cinder/app/AppNative.h"
#include "cinder/Json.h"

namespace instagram
{   
    class InstBaseObject
    {
	public:
		virtual void parse(ci::JsonTree tree) = 0;	
	};
}
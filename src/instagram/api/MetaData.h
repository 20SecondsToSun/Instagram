#pragma once
#include "cinder/app/AppNative.h"

namespace instagram
{   
	class Metadata : public InstBaseObject
	{	
		int code;

	public:
		void parse(ci::JsonTree tree)
		{
			code = tree.getChild("code").getValue<int>();
		}

		bool okCode()
		{
			return code == 200;
		}

		int getCode()
		{
			return code;
		}


	};
}

#pragma once
#include "cinder/app/AppNative.h"
#include "InstBaseObject.h"

namespace instagram
{   
    class Pagination : public InstBaseObject
    {
		std::string next_url;

	public:
		const std::string& getNextURL()
		{
			return  next_url;
		}

		void setNextURL(std::string value)
		{
			next_url = value;
		}

		void parse(ci::JsonTree tree)
		{			
			next_url = tree.getChild("next_url").getValue<std::string>();
			ci::app::console()<<"next_url :: "<<next_url<<std::endl;
		}
	};
}
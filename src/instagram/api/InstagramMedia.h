#pragma once
#include "cinder/app/AppNative.h"
#include "ImagesList.h"
#include "User.h"

namespace instagram
{   
    class InstagramMedia : public InstBaseObject
    {
		//CommentList comments;
       // Caption caption;
        //LikesList likes;
        std::string link;
        User user;
        double created_time;
        ImagesList images;
        std::string type;
        std::string filter;
        std::vector<std::string> tags;
        std::string id;
       // Location location;
        bool user_has_liked;	

	public:
		const std::string& getLink()
		{
			return link;
		}

		void getLink(std::string value)
		{
			this->link = value;
		}

		///
		const User& getUser()
		{
			return user;
		}

		void setUser(User value)
		{
			this->user = value;
		}
		
		///
		const ImagesList& getImagesList()
		{
			return images;
		}

		void setImagesList(ImagesList value)
		{
			this->images = value;
		}

		void parse(ci::JsonTree tree)
		{
			images.parse(tree.getChild("images"));			
		}
	
	};
}
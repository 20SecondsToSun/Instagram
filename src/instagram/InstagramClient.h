#pragma once
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Json.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "Curl.h"
#include "ImagesList.h"
#include "Pagination.h"
#include "Tag.h"
#include "User.h"
#include "InstagramResponse.h"
#include "InstagramMedia.h"
#include "InstBaseObject.h"
#include "MetaData.h"
#include "ImageGraphic.h"

#include <boost/thread/thread.hpp>

using namespace std;
using namespace mndl::curl;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;

namespace instagram
{
	typedef boost::shared_ptr<boost::thread> ThreadRef;

		namespace API
		{
			const static string POPULAR		 = "https://api.instagram.com/v1/media/popular/";
			const static string MEDIA_ID	 = "https://api.instagram.com/v1/media/";
			const static string USER_SEARCH  = "https://api.instagram.com/v1/users/search";
			const static string USERS		 = "https://api.instagram.com/v1/users/";
			const static string TAGS		 = "https://api.instagram.com/v1/tags/";
			const static string SEARCH		 = "search/";			
			const static string MEDIA_RECENT = "media/recent/";			
			const static string CLIENT_ID	 = "client_id";
			const static string QUERY		 = "q";
		}

	class InstagramClient  
	{
	public:	
		InstagramClient(string clientID)
			:clientID(clientID),
			_loading(false),
			loadImagesImmediately(true),
			lastHiResIndex(0)
		{

		}

		/*void loadPopularMedias()
		{		
			string request = API::POPULAR + "?" + API::CLIENT_ID + "=" + clientID;
			console()<<"request-------:  "<<request<<endl;

			string json    =  Curl::get(request);
			JsonTree data  = JsonTree(json).getChild("data");				

			medialist.clear();

			for(auto it : data)				
				medialist.push_back(parseImagesList(it));				

			loadImages();
		}

		void userSearch(string name)
		{
			string request = API::USER_SEARCH + "?" + API::QUERY + "=" + name + "&" + API::CLIENT_ID + "=" + clientID;
			console()<<"request-------:  "<<request<<endl;

			string json	   =  Curl::get(request);
			JsonTree data  = JsonTree(json).getChild("data");	

			userList.clear();
			for(auto it : data)
			{	
				User user;
				user.setID(it.getChild("id").getValue<int>());
				userList.push_back(user);
			}
			console()<<"userList-------:  "<<userList.size()<<endl;
		}

		void loadUserFeed(int id)
		{
			string request = API::USERS + to_string(id) + "/" + API::MEDIA_RECENT + "?" + API::CLIENT_ID + "=" + clientID + "&count=10";
			console()<<"request-------:  "<<request<<endl;
			string json =  Curl::get(request);
			JsonTree data  = JsonTree(json).getChild("data");
			console()<<"----------------:  "<<endl;	
			JsonTree pagination  = JsonTree(json).getChild("pagination");					

			console()<<"pagination-------:  "<<pagination<<endl;	

			medialist.clear();
			for(auto it : data)				
				medialist.push_back(parseImagesList(it));	

			console()<<"medialist-------:  "<<medialist.size()<<endl;	

			loadImages();
		}

		void tagSearch(string tagName)
		{
			string request = API::TAGS + API::SEARCH + "?" + API::QUERY + "=" + tagName + "&" + API::CLIENT_ID + "=" + clientID;
			console()<<"request-------:  "<<request<<endl;
			string json =  Curl::get(request);
			JsonTree data  = JsonTree(json).getChild("data");
			console()<<"----------------:  "<<data<<endl;				
		}
*/	
		


		////////////////////////////////////////////////////////////////////////////
		//
		//					LOAD BY TAG
		//
		////////////////////////////////////////////////////////////////////////////

		void loadTagMedia(string tagName, int count = 20)
		{
			if(_loading) return;

			_loading = true;
			ThreadRef(new boost::thread( bind( &InstagramClient::_loadTagMedia, this, tagName, count)));
		}

		void _loadTagMedia(string tagName, int count = 20)
		{
			string request = API::TAGS + tagName + "/" +
							 API::MEDIA_RECENT + "?" +
							 API::CLIENT_ID + "=" + clientID +
							 "&COUNT=" + to_string(count);

			loadMediaRequest(request);
		}

		////////////////////////////////////////////////////////////////////////////
		//
		//					PAGINATION
		//
		////////////////////////////////////////////////////////////////////////////

		void loadNextMedia()
		{
			if(_loading) return;

			_loading = true;
			ThreadRef( new boost::thread( bind( &InstagramClient::_loadNextMedia, this) ) );
		}

		void _loadNextMedia()
		{			
			loadMediaRequest(lastMediaResponse.getPagination().getNextURL());
		}	

		void loadMediaRequest(string request)
		{
			string json = Curl::get(request);
	
			lastMediaResponse.parse(json);	
		
			if(loadImagesImmediately)
				loadImages();

			_loading = false;
		}

		void loadStandartResImageByIndex(int index)
		{	
			lastHiResIndex = index;
			ThreadRef( new boost::thread(bind( &InstagramClient::_loadStandartResImageByIndex, this) ) );
		}

		void _loadStandartResImageByIndex()
		{			
			if(lastHiResIndex < cashedTextures.size())
			{
				cashed_mutex.lock();
				ImageGraphic img = cashedTextures[lastHiResIndex];
				cashed_mutex.unlock();
				img.loadStandartRes();

				cashed_mutex.lock();
				cashedTextures[lastHiResIndex] = img;
				cashed_mutex.unlock();
			}			
		}

		const ci::gl::Texture getLastStandartResTexture()
		{	
			cashed_mutex.lock();
			ci::gl::Texture tex = cashedTextures[lastHiResIndex].getStandartResImage();
			cashed_mutex.unlock();
			return tex;
		}

		////////////////////////////////////////////////////////////////////////////
		//
		//					GETTERS
		//
		////////////////////////////////////////////////////////////////////////////

		bool isLoading()
		{
			return _loading;
		}

		std::vector<ImageGraphic> getImages()
		{
			std::vector<ImageGraphic> images;
			cashed_mutex.lock();
			images = cashedTextures;
			cashed_mutex.unlock();
			return images;			
		}

		const list<User>& getUsers()
		{
			return userList;
		}

	protected:
		string clientID;

		int lastHiResIndex;
		bool _loading, loadImagesImmediately;		

		std::vector<ImageGraphic> cashedTextures;
		boost::mutex cashed_mutex;

		std::shared_ptr<ci::Surface> lastLoadedStandartResImage;
		boost::mutex standartImageMutex;

		std::list<User> userList;		
		
		std::string nextRequest;
		InstagramResponse<InstagramMedia> lastMediaResponse;
	
		void loadImages()
		{
			std::vector<ImageGraphic> textures_temp;		
			list<InstagramMedia> mediaList = lastMediaResponse.getData();

			for (auto image : mediaList)
			{					
				ImageGraphic imageGr;
				imageGr.setLowResURL(image.getImagesList().getLowResolution().getURL());
				imageGr.setStandartResURL(image.getImagesList().getStandardResolution().getURL());
				imageGr.setSize(306);
				textures_temp.push_back(imageGr);										
			}

			cashed_mutex.lock();
			for (auto imageGr : textures_temp)	
				cashedTextures.push_back(imageGr);	
			cashed_mutex.unlock();

			int startIndex = cashedTextures.size() - textures_temp.size();

			for (int i = 0; i < textures_temp.size(); i++)
			{
				textures_temp[i].loadLowRes();

				cashed_mutex.lock();
				cashedTextures[startIndex + i] = textures_temp[i];			
				cashed_mutex.unlock();	
			}			
		}
	};

	typedef shared_ptr<InstagramClient> InstagramClientRef;
}
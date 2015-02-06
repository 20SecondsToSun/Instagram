#pragma once
#include "cinder/app/AppNative.h"
#include "Pagination.h"
#include "Metadata.h"

namespace instagram
{   
	template<class T> class InstagramResponse
	{	
		Pagination pagination;
		Metadata meta;
		std::list<T> data;

	public:
		// getters
		Pagination getPagination()
		{
			return pagination;
		}

		Metadata getMetadata()
		{
			return meta;
		}

		const std::list<T>& getData()
		{
			return data;
		}

		// setters
		void setPagination(Pagination value)
		{
			this->pagination = value;
		}

		void setMetadata(Metadata meta)
		{
			this->meta = meta;
		}

		void setData(T data)
		{
			this->data = data;
		}

		void clear()
		{
			data.clear();
		}

		void parse(std::string json)
		{
			clear();

			meta.parse(JsonTree(json).getChild("meta"));

			if(meta.okCode())
			{
				pagination.parse(JsonTree(json).getChild("pagination"));			
				JsonTree jdata = JsonTree(json).getChild("data");
		
				for (auto _data : jdata)
				{
					T object;			
					object.parse(_data);
					data.push_back(object);				
				}
			}
			else
			{
				ci::app::console()<<"code:  "<<meta.getCode()<<std::endl;
			}

		}
	};
}
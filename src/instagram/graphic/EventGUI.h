#pragma once

using namespace ci::signals;
using namespace ci::app;
using namespace std;
using namespace ci;

namespace kubik
{
	enum KubikEvent 
	{
		NONE,
		MENU_TOUCH
	};

	enum InteractEvent 
	{
		TOUCH_UP
	};

	class EventGUI
	{
	public:
		EventGUI(KubikEvent type = KubikEvent::NONE):type(type)
		{

		}

		virtual KubikEvent getType()
		{
			return type;
		}

		virtual void setType(KubikEvent value)
		{
			type = value;
		}

	protected:
		KubikEvent type;
	};

	typedef shared_ptr<EventGUI> EventGUIRef;	
}
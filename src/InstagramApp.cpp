#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "Curl.h"
#include "InstagramClient.h"
#include "InstagramViewer.h"
#include "InstaPopup.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace instagram;

class InstagramApp : public AppNative {
  public:
	void setup();	
	void draw();

	InstagramClientRef instClient;
	InstagramViewerRef instaViewer;
	InstaPopupRef instaPopup;
	void touchedHandler();
	void closePopupHandler();
	void create();
	void kill();
	void keyDown(KeyEvent event);

	bool created;
};

void InstagramApp::setup()
{
	setWindowSize(1224, 1000);
	create();
	gl::enableAlphaBlending();
}

void InstagramApp::create()
{
	string clientID = "6ac8af15a5d341e9802c8d1a26899ae3";
	instClient = InstagramClientRef(new InstagramClient(clientID));
	instaViewer = InstagramViewerRef(new InstagramViewer(instClient));
	instaPopup = InstaPopupRef(new InstaPopup(instClient));

	instaViewer->touchedEvent.connect(bind(&InstagramApp::touchedHandler, this));
	instaViewer->connect();

	//instClient->LoadingCompleteSignal.connect(bind(&InstagramApp::loadingDataComplete, this));
	//instClient->userSearch("metalcorehero");
	//instClient->loadUserFeed(instClient->getUsers().front().getID());	
	instClient->loadTagMedia("beauty");
	//instClient->loadPopularMedias();	
	created = true;
}

void InstagramApp::kill()
{
	instaViewer->disconnect();
	//instaViewer->touchedEvent.disconnect_all_slots();
	instaPopup->disconnect();
	created = false;
}

void InstagramApp::touchedHandler()
{
	instaViewer->disconnect();

	instaPopup->show(instaViewer->getImageGraphic());
	instaPopup->touchedEvent.connect(bind(&InstagramApp::closePopupHandler, this));
	instaPopup->connect();
}

void InstagramApp::closePopupHandler()
{
	instaPopup->disconnect();
	instaPopup->hide();
	instaViewer->connect();
}

void InstagramApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
	if (created)
	{
		instaViewer->draw();
		instaPopup->draw();
	}	
}

void InstagramApp::keyDown(KeyEvent event)
{
	switch (event.getCode())
	{
	case KeyEvent::KEY_q:
		if (!created)			
			create();
		else if (!instClient->isLoading())
			kill();
		break;	
	}
}

CINDER_APP_NATIVE( InstagramApp, RendererGl )
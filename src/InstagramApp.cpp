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
};

void InstagramApp::setup()
{
	setWindowSize(1224, 1000);

	string clientID = "6ac8af15a5d341e9802c8d1a26899ae3";
	instClient	= InstagramClientRef(new InstagramClient(clientID));
	instaViewer = InstagramViewerRef(new InstagramViewer(instClient));
	instaPopup  = InstaPopupRef(new InstaPopup(instClient));

	instaViewer->touchedEvent.connect(bind(&InstagramApp::touchedHandler, this));
	instaViewer->connect();

	//instClient->LoadingCompleteSignal.connect(bind(&InstagramApp::loadingDataComplete, this));
	//instClient->userSearch("metalcorehero");
	//instClient->loadUserFeed(instClient->getUsers().front().getID());	
	instClient->loadTagMedia("beauty");	
	//instClient->loadPopularMedias();	
	gl::enableAlphaBlending();
}

void InstagramApp::touchedHandler()
{
	instaViewer->disconnect();
	int index = instaViewer->getLastImageIndexTouched();	
	instaPopup->show();
	instClient->loadStandartResImageByIndex(index);
	

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
	instaViewer->draw();
	instaPopup->draw();
}

CINDER_APP_NATIVE( InstagramApp, RendererGl )
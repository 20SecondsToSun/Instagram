#include <sstream>
#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
#include "Curl.h"

using namespace std;

namespace mndl { namespace curl {

CURL* Curl::maincurl = NULL;

string Curl::post( const string &url, vector<string> &m )
{
	bool   usePost    = true;
	string postString = "";
	stringstream st;
	int    count = 0;

	vector<string>::iterator act, end;
	for( act = m.begin(), end = m.end(); act != end; ++act )
	{
		st.clear();
		st.str( string());
		st << count++;

		postString += st.str() + "=" + Curl::urlEncode( *act ) + "&";
	}

	return easyCurl( url, usePost, postString );
}

string Curl::post( const string &url, map<string, string> &m )
{
	bool   usePost    = true;
	string postString = "";

	map<string, string>::iterator act, end;
	for( act = m.begin(), end = m.end(); act != end; ++act )
	{
		postString += act->first + "=" + Curl::urlEncode( act->second ) + "&";
	}

	return easyCurl( url, usePost, postString );
}

string Curl::postImage( const string &Serverurl, const string &Imageurl )
{
	 return easyCurl(Serverurl,true,Imageurl);
}

string Curl::postUploadFB( const string &url, const string &  ACCESSTOKEN, const string &  path, const string &  message)
{
	string buffer = "";
	CURLcode result;

	struct curl_httppost *post=NULL;  
	struct curl_httppost *last=NULL;  
	struct curl_slist *headers=NULL;  

	char errorBuffer[CURL_ERROR_SIZE];

	errorBuffer[0] = 0;

	CURL * easyhandle;  
	easyhandle = curl_easy_init();  

	if( ! easyhandle ) return "";

	curl_easy_setopt(easyhandle, CURLOPT_VERBOSE, 1); // do this to make debugging easier  

	headers = curl_slist_append(headers, "Expect:");  

	curl_formadd(&post, &last,  
				 CURLFORM_COPYNAME, "access_token",  
				 CURLFORM_COPYCONTENTS, ACCESSTOKEN.c_str(), CURLFORM_CONTENTHEADER, headers, CURLFORM_END);  

	curl_formadd(&post, &last,  
				 CURLFORM_COPYNAME, "source",  
				 CURLFORM_FILE, path.c_str(), CURLFORM_CONTENTTYPE, "image/jpg", CURLFORM_END); 
	
	curl_formadd(&post, &last,  
				 CURLFORM_COPYNAME, "message",  
				 CURLFORM_COPYCONTENTS, message.c_str(), CURLFORM_CONTENTHEADER, headers, CURLFORM_END); 

	curl_easy_setopt( easyhandle, CURLOPT_ERRORBUFFER   , errorBuffer );
	curl_easy_setopt( easyhandle, CURLOPT_WRITEDATA     , &buffer     );
	curl_easy_setopt( easyhandle, CURLOPT_WRITEFUNCTION , writer      );
	curl_easy_setopt( easyhandle, CURLOPT_SSL_VERIFYPEER, 0L                                                 ); // this line makes it work under https
	curl_easy_setopt( easyhandle, CURLOPT_SSL_VERIFYHOST, 0L                                                 );
	curl_easy_setopt(easyhandle, CURLOPT_URL, url.c_str()); 

	/* Set the form info */  
	curl_easy_setopt(easyhandle, CURLOPT_HTTPPOST, post);  
	/* post away! */  
	result = curl_easy_perform(easyhandle);   

	curl_easy_cleanup(easyhandle);  

	if( result == CURLE_OK )
	{
		//console() << "buffer:" << buffer << endl;
		return buffer;
	}
	else
	{
		//console() << "error:" << errorBuffer << endl;
		return "";
	}
}

string Curl::postUpload( const string &url, map<string, string> &m, string path )
{
	struct curl_httppost* post =    NULL;
    struct curl_httppost* last =    NULL;
	string buffer = "";
	char errorBuffer[CURL_ERROR_SIZE];

	errorBuffer[0] = 0;

	CURL *curl = curl_easy_init();
	CURLcode result;

	if( ! curl ) return "";

	curl_formadd(&post, &last, 
			CURLFORM_COPYNAME, "photo",
			CURLFORM_FILE,  path.c_str(),
			CURLFORM_END);
 
	curl_easy_setopt( curl, CURLOPT_ERRORBUFFER   , errorBuffer );
	curl_easy_setopt( curl, CURLOPT_URL           , url.c_str() );
	curl_easy_setopt( curl, CURLOPT_FOLLOWLOCATION, 1           );
	curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION , writer      );
	curl_easy_setopt( curl, CURLOPT_WRITEDATA     , &buffer     );
	curl_easy_setopt( curl, CURLOPT_POST      , 1                      );
	curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);	

	curl_easy_setopt( curl, CURLOPT_SSL_VERIFYPEER, 0L                                                 ); // this line makes it work under https
	curl_easy_setopt( curl, CURLOPT_SSL_VERIFYHOST, 0L                                                 );
	
	result = curl_easy_perform( curl );
	curl_easy_cleanup( curl );

	if( result == CURLE_OK )
	{
		return buffer;
	}
	else
	{
		cerr << "error:" << errorBuffer << endl;
		return "";
	}
}

string Curl::deleteRequest( const string &url, map<string, string> &m )
{
	CURL *curl = curl_easy_init();
	CURLcode result;

	string buffer = "";
	char errorBuffer[CURL_ERROR_SIZE];

   if( ! curl ) return "";    
    
   curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
   curl_easy_setopt(curl,CURLOPT_CUSTOMREQUEST, "DELETE");

   
	curl_easy_setopt( curl, CURLOPT_ERRORBUFFER   , errorBuffer );
	curl_easy_setopt( curl, CURLOPT_URL           , url.c_str() );
	curl_easy_setopt( curl, CURLOPT_FOLLOWLOCATION, 1           );
	curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION , writer      );
	curl_easy_setopt( curl, CURLOPT_WRITEDATA     , &buffer     );

	curl_easy_setopt( curl, CURLOPT_SSL_VERIFYPEER, 0L                                                 ); // this line makes it work under https
	curl_easy_setopt( curl, CURLOPT_SSL_VERIFYHOST, 0L                                                 );


	string postString = "";

	map<string, string>::iterator act, end;
	for( act = m.begin(), end = m.end(); act != end; ++act )
	{
		postString += act->first + "=" + Curl::urlEncode( act->second ) + "&";
	}

	curl_easy_setopt( curl, CURLOPT_POSTFIELDS, postString.c_str());		


    result = curl_easy_perform(curl);

    curl_easy_cleanup( curl );

	if( result == CURLE_OK )
	{
		return buffer;
	}
	else
	{
		cerr << "error:" << errorBuffer << endl;
		return errorBuffer;
	}
}

string Curl::get( const string &url )
{
	bool   usePost = false;
	string params  = "";

	return easyCurl( url, usePost, params );
}

bool Curl::ftpUpload( const std::string &ftpUrl, const std::string &userName, const std::string &password, const ci::fs::path &path )
{
	bool ret = true;
	CURL *curl = curl_easy_init();

	CURLcode  res;

	if( ! curl )
		return false;

	FILE        *fileSource;
	curl_off_t   fileSize;
	std::string  fileName = path.filename().string();
	std::string  tempName = fileName + ".uploading";
	std::string  renameFromCommand = "RNFR " + tempName;
	std::string  renameToCommand   = "RNTO " + fileName;
	std::string  urlFullPath   = ftpUrl + "/" + tempName;
	std::string  userPass = userName + ":" + password;

	struct curl_slist *headerList = NULL;

	if( ! ci::fs::exists( path ))
	{
		cerr << "error: " << "Couldn't open file: " << path << endl;
		curl_easy_cleanup( curl );
		curl_global_cleanup();
		return false;
	}

	fileSize = (curl_off_t)ci::fs::file_size( path );

	fileSource = fopen( path.string().c_str(), "rb" ); // get a FILE * of the same file

	curl_global_init( CURL_GLOBAL_ALL ); // In windows, this will init the winsock stuff

	// build a list of commands to pass to libcurl
	headerList = curl_slist_append( headerList, renameFromCommand.c_str());
	headerList = curl_slist_append( headerList, renameToCommand.c_str());

	curl_easy_setopt( curl, CURLOPT_UPLOAD          , 1L                        ); // enable uploading
	curl_easy_setopt( curl, CURLOPT_URL             , urlFullPath.c_str()       ); // specify target
	curl_easy_setopt( curl, CURLOPT_USERPWD         , userPass.c_str()          ); // specify user/password
	curl_easy_setopt( curl, CURLOPT_POSTQUOTE       , headerList                ); // pass in that last of FTP commands to run after the transfer
	curl_easy_setopt( curl, CURLOPT_READDATA        , fileSource                ); // now specify which file to upload
	curl_easy_setopt( curl, CURLOPT_INFILESIZE_LARGE, fileSize                  ); // Set the size of the file to upload (optional).  If you give a *_LARGE option you MUST make sure that the type of the passed-in argument is a curl_off_t. If you use CURLOPT_INFILESIZE (without _LARGE) you must make sure that to pass in a type 'long' argument

	res = curl_easy_perform( curl );

	if( res != CURLE_OK )
	{
		cerr << "error: " << curl_easy_strerror( res ) << endl;
		ret = false;
	}

	curl_slist_free_all( headerList ); // clean up the FTP commands list
	curl_easy_cleanup( curl );
	fclose( fileSource );
	curl_global_cleanup();

	return ret;
}

int Curl::writer( char *data, size_t size, size_t nmemb, string *buffer )
{
	int result = 0;
	if( buffer != NULL )
	{
		buffer->append( data, size * nmemb );
		result = size * nmemb;
	}
	return result;
}

string Curl::easyCurl( const string &url, bool post, const string &postParamString)
{
	string buffer = "";
	char errorBuffer[CURL_ERROR_SIZE];

	errorBuffer[0] = 0;

	if(!maincurl)
		maincurl = curl_easy_init();
	CURLcode result;

	if( ! maincurl )
		return "";

	curl_easy_setopt( maincurl, CURLOPT_ERRORBUFFER   , errorBuffer );
	curl_easy_setopt( maincurl, CURLOPT_URL           , url.c_str() );
	//curl_easy_setopt( curl, CURLOPT_HEADER        , 0           );
	curl_easy_setopt( maincurl, CURLOPT_FOLLOWLOCATION, 1           );
	curl_easy_setopt( maincurl, CURLOPT_WRITEFUNCTION , writer      );
	curl_easy_setopt( maincurl, CURLOPT_WRITEDATA     , &buffer     );
	if( post )
	{
		curl_easy_setopt( maincurl, CURLOPT_POST      , 1                      );
		curl_easy_setopt( maincurl, CURLOPT_POSTFIELDS, postParamString.c_str());	
	}	
	
	curl_slist *headers=NULL; // init to NULL is important 
 
	curl_slist_append(headers, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*;q=0.8"); 
	curl_slist_append(headers, "Accept-Language: ru,en-us;q=0.7,en;q=0.3"); 
	curl_slist_append(headers, "Accept-Encoding: identity"); 
	curl_slist_append(headers, "Accept-Charset: windows-1251,utf-8;q=0.7,*;q=0.7"); 

    curl_easy_setopt(maincurl, CURLOPT_HTTPHEADER, headers); 

	curl_easy_setopt( maincurl, CURLOPT_USERAGENT     , "Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)" );
	curl_easy_setopt( maincurl, CURLOPT_COOKIEFILE    , "cookies.txt"                                         ); // read from
	curl_easy_setopt( maincurl, CURLOPT_COOKIEJAR     , "cookies.txt"                                         ); // write to

	curl_easy_setopt( maincurl, CURLOPT_SSL_VERIFYPEER, 0L                                                 ); // this line makes it work under https
	curl_easy_setopt( maincurl, CURLOPT_SSL_VERIFYHOST, 0L                                                 );

	result = curl_easy_perform( maincurl );

	//curl_easy_cleanup( maincurl );	
	
	if( result == CURLE_OK )
	{
		return buffer;
	}
	else
	{
		//cerr << "error:" << errorBuffer << endl;
		return "ERROR";
	}
}

string Curl::easyCurlUpload( const string &url, bool post, const string &postParamString, FILE* file )
{
	string buffer = "";
	char errorBuffer[CURL_ERROR_SIZE];

	errorBuffer[0] = 0;

	CURL *curl = curl_easy_init();
	CURLcode result;

	if( ! curl )
		return "";

	curl_easy_setopt( curl, CURLOPT_ERRORBUFFER   , errorBuffer );
	curl_easy_setopt( curl, CURLOPT_URL           , url.c_str() );
	curl_easy_setopt( curl, CURLOPT_HEADER        , 0           );
	curl_easy_setopt( curl, CURLOPT_FOLLOWLOCATION, 1           );
	curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION , writer      );
	curl_easy_setopt( curl, CURLOPT_WRITEDATA     , &buffer     );
	if( post )
	{
		curl_easy_setopt( curl, CURLOPT_POST      , 1                      );
		curl_easy_setopt( curl, CURLOPT_POSTFIELDS, postParamString.c_str());	
	}
	
	curl_easy_setopt( curl, CURLOPT_USERAGENT     , "Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)" );
	curl_easy_setopt( curl, CURLOPT_COOKIEFILE    , "cookies.txt"                                         ); // read from
	curl_easy_setopt( curl, CURLOPT_COOKIEJAR     , "cookies.txt"                                         ); // write to

	curl_easy_setopt( curl, CURLOPT_SSL_VERIFYPEER, 0L                                                 ); // this line makes it work under https
	curl_easy_setopt( curl, CURLOPT_SSL_VERIFYHOST, 0L                                                 );


	
	struct stat file_info;

	/* to get the file size */ 
	if(fstat(fileno(file), &file_info) != 0)
	{ 
		curl_easy_setopt( curl, CURLOPT_UPLOAD          , 1L                        ); // enable uploading

		/* set where to read from (on Windows you need to use READFUNCTION too) */ 
		curl_easy_setopt(curl, CURLOPT_READDATA, file);
 
		/* and give the size of the upload (optional) */ 
		curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,           (curl_off_t)file_info.st_size);
 
		/* enable verbose for easier tracing */ 
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	}	

	result = curl_easy_perform( curl );
	curl_easy_cleanup( curl );	

	if( result == CURLE_OK )
	{
		return buffer;
	}
	else
	{
		cerr << "error:" << errorBuffer << endl;
		return "";
	}
}

string Curl::urlEncode( const string &c )
{
	string escaped = "";
	int max = c.length();
	for( int i = 0; i < max; ++i )
	{
		if( ( '0' <= c[i] && c[i] <= '9' )
		 || ( 'a' <= c[i] && c[i] <= 'z' )
		 || ( 'A' <= c[i] && c[i] <= 'Z' )
		 || ( c[i] == '~' || c[i] == '!' || c[i] == '*' || c[i] == '(' || c[i] == ')' || c[i] == '\''))
		{
			escaped.append( &c[i], 1 );
		}
		else
		{
			escaped.append( "%" );
			escaped.append( char2Hex( c[i] )); // converts char 255 to string "ff"
		}
	}
	return escaped;
}

string Curl::char2Hex( char dec )
{
	char dig1 = ( dec & 0xF0 ) >> 4;
	char dig2 = ( dec & 0x0F );

	if(  0 <= dig1 && dig1 <=  9 ) dig1 += '0';
	if( 10 <= dig1 && dig1 <= 15 ) dig1 += 'a' - 10;
	if(  0 <= dig2 && dig2 <=  9 ) dig2 += '0';
	if( 10 <= dig2 && dig2 <= 15 ) dig2 += 'a' - 10;

	string r;
	r.append( &dig1, 1);
	r.append( &dig2, 1);
	return r;
}

}} // namespace mndl::curl
#if PLATFORM_WINDOWS
#include <windows.h>
#include <shellapi.h>
#endif
#include <string.h>

#include "tinyHTTP.h"
static const char* webroot = ".";

static bool testEndpoint(const httpparser::Request& request, httpparser::Response& response)
{
    static const char* testMsg = "Hello There!";
    response.status = "OK";
    response.statusCode = 200;
    
    response.content.reserve(strlen(testMsg));
    response.content.resize(strlen(testMsg));
    memcpy(&response.content[0], testMsg, strlen(testMsg));

    httpparser::Response::HeaderItem hi; 
    hi.name = "Content-Type";
    hi.value = "text/plain";
    response.headers.push_back(hi);
    hi.name = "Content-Length";
    char buf[210];
    sprintf(buf, "%d", (int) response.content.size());
    hi.value = buf;
    response.headers.push_back(hi);

    return true;
}

#if PLATFORM_WINDOWS
DWORD WINAPI ServerThreadFunction( LPVOID lpParam )
{
    tinyHTTP* pServer = new tinyHTTP;
    pServer->RegisterEndpoint("/test", testEndpoint);
    pServer->Start(webroot);  //does not return


    delete pServer;
    return 0;
}

int main(int argc, char const *argv[])
{
    DWORD dwThreadId;
    HANDLE hThread = CreateThread( 
            NULL,                   // default security attributes
            0,                      // use default stack size  
            ServerThreadFunction,       // thread function name
            nullptr,          // argument to thread function 
            0,                      // use default creation flags 
            &dwThreadId);   // returns the thread identifier 

    Sleep(500);

    ShellExecuteW(0, 0, L"http://localhost:8080/index.html", 0, 0 , SW_SHOW );
    while(true);
//   use beforeunload event in the client to signal exit
}
#else
int main(int argc, char const *argv[])
{
    tinyHTTP* pServer = new tinyHTTP;
    pServer->RegisterEndpoint("/test", testEndpoint);
    pServer->Start(webroot);  //does not return


    delete pServer;
    return 0;
}
#endif
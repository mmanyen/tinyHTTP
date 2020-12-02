#ifdef PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#include <io.h>
#define socket_write(a,b,c) send(a,b,c,0)
#define socket_read(a,b,c) recv(a,b,c,0)
#else
#define closesocket close
#define socket_write(a,b,c) send(a,b,c,0)
#define socker_read(a,b,c) recv(a,b,c,0)
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>

#include "tinyHTTP.h"

using namespace httpparser;

// Files to be served are here
const char* webroot = "./webroot";

#define PORT 8080


static bool sendFile(const httpparser::Request& request, httpparser::Response& response)
{
    bool bRet = false;
    std::string localFile = webroot;

    localFile += request.path;
    FILE* f = fopen (localFile.c_str(), "rb");
    if (f)
    {
        fseek(f, 0, SEEK_END);
        size_t s = ftell(f);
        fseek(f, 0, SEEK_SET);
        response.content.reserve(s);
        response.content.resize(s);
        fread(&response.content[0], 1, s, f);
        fclose(f);  
        bRet = true;
    }

    if(bRet)
    {
        response.status = "OK";
        response.statusCode = 200;
    }
    else
    {
        response.statusCode = 404;
        response.status = request.uri + " not found";
    }
    return bRet;
}

tinyHTTP::tinyHTTP()
{
    mEndpoints["*"] = sendFile;
}

tinyHTTP::~tinyHTTP()
{

}


bool tinyHTTP::RegisterEndpoint(std::string name, EndPointHandlerType handler)
{
    // TODO: Check if unique
    mEndpoints[name] = handler;
    return true;
}

void tinyHTTP::dispatchRequest(Request request, Response& response)
{
    // Check list of endpoints 
    std::map<std::string, EndPointHandlerType>::iterator it = mEndpoints.find(request.path);
    if(it == mEndpoints.end())
    {
        // not found
        it = mEndpoints.find("*");
        if(it != mEndpoints.end())
        {
            (*it).second(request, response);
        }
    }
    else
    {
        // found
        (*it).second(request, response);
    }
}

void sendResponse(const Response& response, int s)
{
    std::string r = response.inspect();
    socket_write(s , r.c_str() , r.size());
}

enum 
{
    SRV_NO_ERROR = 0,
    SRV_WINDOWS_ERR = -200,
    SRV_SOCKET_ERR,
    SRV_BIND_ERR,
    SRV_LISTEN_ERR 
};



int tinyHTTP::Start(const char* pWebRootFolder)
{
    int server_fd, new_socket; 
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    bool bInitSuccess = true;
    int retVal = SRV_NO_ERROR;

#if PLATFORM_WINDOWS
    WORD wVersionRequested;
    WSADATA wsaData;

    wVersionRequested = MAKEWORD(2, 2);

    int err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        /* Tell the user that we could not find a usable */
        /* Winsock DLL.                                  */
        printf("WSAStartup failed with error: %d\n", err);
        bInitSuccess = false;
        retVal = SRV_WINDOWS_ERR;
    }
#endif

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        bInitSuccess = false;
        retVal = SRV_SOCKET_ERR;
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    if (bInitSuccess && bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {        
        bInitSuccess = false;
        retVal = SRV_BIND_ERR;
    }

    if (bInitSuccess && listen(server_fd, 10) < 0)
    {
        bInitSuccess = false;
        retVal = SRV_LISTEN_ERR;
    }

    if (bInitSuccess)
    {
        RegisterEndpoint("*", sendFile); // define the thunk
        while(1)
        {
            printf("\n+++++++ Waiting for new connection ++++++++\n\n");
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                printf("Error in In accept");
                continue;
            }
            
            char buffer[30000] = {0};
            socket_read(new_socket , buffer, 30000);

            Request request;
            HttpRequestParser inParser;
            Response response;

            HttpRequestParser::ParseResult res = inParser.parse(request, buffer, buffer + sizeof(buffer));

            if( res == HttpRequestParser::ParsingCompleted )
            {
                printf("%s request on uri: %s\n", request.method.c_str(), request.uri.c_str());
                // Default to all is well
                response.statusCode = 200;
                response.status = "OK";
                dispatchRequest(request, response);
            }
            else
            {
                // error send response
                printf("Parse Error\n");
                response.statusCode = 500;
                response.status = "Http parse error";
                response.content.clear();
            }
                
            // send response
            sendResponse(response, new_socket);

            closesocket(new_socket);
        }
    }
#ifdef PLATFORM_WINDOWS
    WSACleanup();
#endif 

    return retVal;
}


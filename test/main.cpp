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

int main(int argc, char const *argv[])
{
    tinyHTTP* pServer = new tinyHTTP;
    pServer->RegisterEndpoint("/test", testEndpoint);
    pServer->Start(webroot);


    delete pServer;
    
}
#ifndef TINYHTTP_HEADER
#define TINYHTTP_HEADER

#include "httpparser/request.h"
#include "httpparser/httprequestparser.h"
#include "httpparser/response.h"
#include "httpparser/httpresponseparser.h"
#include <vector>
#include <map>

typedef bool (*EndPointHandlerType)(const httpparser::Request&, httpparser::Response&);


class tinyHTTP
{
public: 
    tinyHTTP();
    virtual ~tinyHTTP();

    bool RegisterEndpoint(std::string name, EndPointHandlerType handler);
    int Start(const char* pWebRootFolder);
    bool Stop();

protected:
    void dispatchRequest(httpparser::Request request, httpparser::Response& response);

private:
    std::map<std::string, EndPointHandlerType> mEndpoints;
};


#endif // TINYHTTP_HEADER
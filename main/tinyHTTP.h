#ifndef TINYHTTP_HEADER
#define TINYHTTP_HEADER

#include "httpparser/request.h"
#include "httpparser/httprequestparser.h"
#include "httpparser/response.h"
#include "httpparser/httpresponseparser.h"
#include <cstdint>
#include <vector>
#include <map>
#include <string>
#include <functional>
#include <thread>
#include <atomic>

class tinyHTTP
{
public: 
    tinyHTTP(const char* webroot = "./webroot", uint16_t port = 8080);
    virtual ~tinyHTTP();

    bool RegisterEndpoint(std::string name,  std::function<bool(const httpparser::Request&, httpparser::Response&)> handler);
    int Start();
    void Stop();

protected:
    void dispatchRequest(httpparser::Request request, httpparser::Response& response);

private:
    std::map<std::string, std::function<bool(const httpparser::Request&, httpparser::Response&)>> mEndpoints;
    std::string mWebroot;
    uint16_t mPort;
    std::atomic_bool mbKeepRunning;

    int Process();
    std::thread* mProcessThread;
};


#endif // TINYHTTP_HEADER
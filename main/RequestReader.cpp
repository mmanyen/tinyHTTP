#include "RequestReader.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#include <io.h>
#else
#define closesocket close
#include <sys/socket.h>
#endif

#include <string>
#include <vector>
#include <list>

static std::string readALine(int soc)
{
    std::string line;
    char buf[50];
    char c;
    int numRead = recv(soc, &c, 1, 0);
    while (c != '\n')
    {
        if (c != '\r')
            line += c;
        numRead = recv(soc, &c, 1, 0);
    }
    return line;
}

bool ReadRequest(int soc, httpparser::Request &request)
{
    bool bRet {true};

    std::string line{readALine(soc)}; // read the medthod line
    size_t pos = line.find(" ");
    request.method = line.substr(0, pos);

    // Get the colon seperated items
    line = readALine(soc);
    while (line != "")
    {
        size_t colonPos = line.find(":");
        httpparser::Request::HeaderItem hi;
        hi.name = line.substr(0, colonPos);
        hi.value = line.substr(line.find_first_not_of(' ', colonPos + 1));
        request.headers.push_back(hi);
        line = readALine(soc);
    }
    // find content len
    bool bFound{false};
    size_t s = 0;
    for (auto x : request.headers)
    {
        if (std::string("Content-Length") == x.name)
        {
            s = std::stoi(x.value);
            // read content into request.content
            request.content.resize(s);
            size_t numread = 0;
            while ((numread < s) && bRet)
            {
                size_t oneread = recv(soc, &request.content[0], s - numread, 0);
                numread += oneread;
                if (oneread == 0)
                {
                    // error
                    bRet = false;
                }
            }
        }
    }

    return bRet;
}

/*
 * Copyright (C) Alex Nekipelov (alex@nekipelov.net)
 * License: MIT
 */

#ifndef HTTPPARSER_RESPONSE_H
#define HTTPPARSER_RESPONSE_H

#include <string>
#include <vector>
#include <sstream>

namespace httpparser
{

struct Response {
    Response()
        : versionMajor(1), versionMinor(1), keepAlive(false), statusCode(0)
    {}
    
    struct HeaderItem
    {
        std::string name;
        std::string value;
    };

    int versionMajor;
    int versionMinor;
    std::vector<HeaderItem> headers;
    std::vector<char> content;
    bool keepAlive;
    
    unsigned int statusCode;
    std::string status;

    std::string inspect() const
    {
        std::stringstream stream;
        stream << "HTTP/" << versionMajor << "." << versionMinor
               << " " << statusCode << " " << status << "\n";

        for(std::vector<Response::HeaderItem>::const_iterator it = headers.begin();
            it != headers.end(); ++it)
        {
            stream << it->name << ": " << it->value << "\n";
        }
        stream << "\n";   // end of http header
        std::string data(content.begin(), content.end());
        stream << data;
//        stream << "\n" << data;
        return stream.str();
    }
};

} // namespace httpparser

#endif // HTTPPARSER_RESPONSE_H


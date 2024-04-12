add_library(tinyHTTP tinyHTTP.cpp tinyHTTP.h)
if(WIN32)
    target_link_libraries(tinyHTTP Ws2_32.lib)
endif(WIN32)

target_include_directories(tinyHTTP PUBLIC . httpparser)

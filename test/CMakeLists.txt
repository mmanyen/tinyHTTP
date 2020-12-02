add_executable(test main.cpp)
target_include_directories(test PUBLIC  ../main)
target_link_libraries(test tinyHTTP)
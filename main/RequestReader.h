#include "httpparser/request.h"

//! @brief Read an HTTP request from the socket until complete
bool ReadRequest(int soc, httpparser::Request& request);


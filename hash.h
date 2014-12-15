#ifndef HASH_H
#define HASH_H

#include <iostream> // For C++ strings

// This generates a hash signature verification
// Note: It should *not* be considered cryptographically secure!
std::string genHash(const std::string &input);

// Returns whether a message has been seen before (and should be dropped)
bool messageSeen(const std::string &msg);

#endif

#ifndef HASH_H
#define HASH_H

#include <iostream> // For C++ strings

#define HASH_LENGTH 64

// This generates a hash for signature verification
std::string genHash(const std::string &input);

// Returns whether a message has been seen before (and should be dropped)
bool messageSeen(const std::string &msg);

#endif

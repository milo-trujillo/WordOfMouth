#ifndef HASH_H
#define HASH_H

#include <iostream> // For C++ strings

// This generates a hash signature verification
// Note: It should *not* be considered cryptographically secure!
std::string genHash(const std::string &input);

// Returns whether a message has been seen before (and should be dropped)
bool messageSeen(const std::string &msg);

// Generates a 24 byte digest based on any key
// NOTE: Digest is placed on the stack, remember to call delete!
unsigned char* generate192BitDigest(const std::string &key);

#endif

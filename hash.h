#ifndef HASH_H
#define HASH_H

#include <iostream> // For C++ strings

// This generates a hash signature verification
// Note: It should *not* be considered cryptographically secure!
std::string genHash(std::string input);

#endif

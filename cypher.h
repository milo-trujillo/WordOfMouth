#ifndef CYPHER_H
#define CYPHER_H

#include <iostream> // For strings

std::string cypher(const std::string &msg, const std::string &key);
std::string decypher(const std::string &msg, const std::string &key);
std::string base64Encode(const std::string &str);
std::string base64Decode(const std::string &str);

bool isReadableText(const std::string &msg);

#endif

#ifndef CYPHER_H
#define CYPHER_H

#include <map>      // For pairs
#include <iostream> // For strings

// Stores an encrypted message and its initialization vector
struct cypheredMessage
{
	std::string msg;
	std::string iv;
};

// Encrypts a message, including an integrity check hash
cypheredMessage cypherMessage(std::string msg, const std::string &key);
// Decrypts a message, stripping the hash and returning <success, message>
std::pair<bool, std::string> decypherMessage(const cypheredMessage &cyphertext,
	const std::string &key);

// These use a hard-coded initialization vector and should be avoided
// They are needed for the config files, where stronger crypto is not an option
std::string cypherWeak(const std::string &msg, const std::string &key);
std::string decypherWeak(const std::string &msg, const std::string &key);

// Ascii armor data to make it safe for transit in a more complex protocol
std::string base64Encode(const std::string &str);
std::string base64Decode(const std::string &str);

// Returns if a message is ASCII text
bool isReadableText(const std::string &msg);

#endif

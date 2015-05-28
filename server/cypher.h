#ifndef CYPHER_H
#define CYPHER_H

#include <map>      // For pairs
#include <iostream> // For strings

class cypheredMessage;

// Ascii armor data to make it safe for transit in a more complex protocol
std::string base64Encode(const std::string &str);
std::string base64Decode(const std::string &str);

// Encrypts a message, including an integrity check hash
cypheredMessage cypherMessage(std::string msg, const std::string &key);
// Decrypts a message, stripping the hash and returning <success, message>
std::pair<bool, std::string> decypherMessage(const cypheredMessage &cyphertext,
	const std::string &key);

// These use a hard-coded initialization vector and should be avoided
// They are needed for the config files, where stronger crypto is not an option
std::string cypherWeak(const std::string &msg, const std::string &key);
std::string decypherWeak(const std::string &msg, const std::string &key);

// Returns if a message is ASCII text
bool isReadableText(const std::string &msg);

// Stores an encrypted message and its initialization vector
class cypheredMessage
{
	public:
		// This is the public interface for people outside the cypher code
		cypheredMessage(std::string encodedString);
		std::string toString(); // For saving message to disk, network, etc
		bool isInvalid(); // Returns true if message is corrupt or illegal

	private:
		// And here's the interface internal to the cypher code
		cypheredMessage(){}

		std::string iv;
		std::string msg;

		friend cypheredMessage cypherMessage(std::string, const std::string&);
		friend std::pair<bool, std::string> decypherMessage(
			const cypheredMessage&, const std::string&);
};

#endif

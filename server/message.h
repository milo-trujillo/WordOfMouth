#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream> // For std::string

/*
	The Message class keeps track of Message data and the current encryption
	state, providing methods to modify that state and dump the results as a
	string. It also handles all memory allocation and deallocation.
*/

typedef enum {PLAINTEXT, CYPHERED, KEYED} cryptoType;

class Message
{
	public:
		Message(std::string plaintext);
		Message(const char* plaintext);
		Message(const char *, const int size, cryptoType state);
		Message(const Message& m);
		Message& operator=(const Message& m);
		~Message();

		const char& operator[](size_t i) const { return data[i]; }
		int size() const { return _size; }
		cryptoType getStatus() const { return status; }
		std::string getAsciiArmor() const;

		// These functions all change the internal state of the Message
		// and change its status, returning true / false on success.
		bool cypher(const char* key);
		bool decypher(const char* key);
		bool sign(const char* privkey);
		bool unsign(const char* pubkey);
		bool encrypt(const char* pubkey);
		bool decrypt(const char* privkey);

	private:
		char* data;
		int _size;
		cryptoType status;
};

#endif

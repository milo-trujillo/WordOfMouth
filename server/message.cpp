#include <string.h> // For strlen

#include "message.h"

using namespace std;

/*
	Note: We make it a habit to use for-loops to copy data rather than strcpy
	because we are often storing binary data and don't want anything odd to
	happen if we hit a null byte.
*/

message::message(string plaintext)
{
	int len = plaintext.length();
	data = new char[len];
	for( int i = 0; i < len; i++ )
		data[i] = plaintext[i];
	_size = len;
	status = PLAINTEXT;
}

message::message(const char* plaintext)
{
	int len = strlen(plaintext);
	data = new char[len];
	for( int i = 0; i < len; i++ )
		data[i] = plaintext[i];
	_size = len;
	status = PLAINTEXT;
}

message::message(const char* src, const int s, cryptoType state)
{
	data = new char[s];
	for( int i = 0; i < s; i++ )
		data[i] = src[i];
	_size = s;
	status = state;
}

message::message(const message& m)
{
	_size = m._size;
	data = new char[m._size];
	for( int i = 0; i < _size; i++ )
		data[i] = m.data[i];
	status = m.status;
}

message& message::operator=(const message& m)
{
	_size = m._size;
	data = new char[m._size];
	for( int i = 0; i < _size; i++ )
		data[i] = m.data[i];
	status = m.status;
	return *this;
}

message::~message()
{
	delete [] data;
}

// For now we just throw the char* into a string, later we'll Base64 encode it.
string message::getAsciiArmor() const
{
	return string(data);
}

//
// These are all stub functions that will later call our crypto functions
// from other source files and return success / error.
//

bool message::cypher(const char* key)
{
	return false;
}

bool message::decypher(const char* key)
{
	return false;
}

bool message::sign(const char* privkey)
{
	return false;
}

bool message::unsign(const char* pubkey)
{
	return false;
}

bool message::encrypt(const char* pubkey)
{
	return false;
}

bool message::decrypt(const char* privkey)
{
	return false;
}

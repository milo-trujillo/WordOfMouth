#include <string.h> // For strlen

#include "message.h"

using namespace std;

/*
	Note: We make it a habit to use for-loops to copy data rather than strcpy
	because we are often storing binary data and don't want anything odd to
	happen if we hit a null byte.
*/

Message::Message(string plaintext)
{
	int len = plaintext.length();
	data = new char[len];
	for( int i = 0; i < len; i++ )
		data[i] = plaintext[i];
	_size = len;
	status = PLAINTEXT;
}

Message::Message(const char* plaintext)
{
	int len = strlen(plaintext);
	data = new char[len];
	for( int i = 0; i < len; i++ )
		data[i] = plaintext[i];
	_size = len;
	status = PLAINTEXT;
}

Message::Message(const char* src, const int s, cryptoType state)
{
	data = new char[s];
	for( int i = 0; i < s; i++ )
		data[i] = src[i];
	_size = s;
	status = state;
}

Message::Message(const Message& m)
{
	_size = m._size;
	data = new char[m._size];
	for( int i = 0; i < _size; i++ )
		data[i] = m.data[i];
	status = m.status;
}

Message& Message::operator=(const Message& m)
{
	if( data != NULL )
	{
		scrub();
		delete [] data;
	}
	_size = m._size;
	data = new char[m._size];
	for( int i = 0; i < _size; i++ )
		data[i] = m.data[i];
	status = m.status;
	return *this;
}

Message::~Message()
{
	scrub();
	delete [] data;
}

// For now we just throw the char* into a string, later we'll Base64 encode it.
string Message::getAsciiArmor() const
{
	return string(data);
}

// TODO: Implement scrubbing
void Message::scrub()
{
	
}

//
// These are all stub functions that will later call our crypto functions
// from other source files and return success / error.
//
// Note: Before changing 'data' they should always scrub the current contents.
//

bool Message::cypher(const char* key)
{
	return false;
}

bool Message::decypher(const char* key)
{
	return false;
}

bool Message::sign(const char* privkey)
{
	return false;
}

bool Message::unsign(const char* pubkey)
{
	return false;
}

bool Message::encrypt(const char* pubkey)
{
	return false;
}

bool Message::decrypt(const char* privkey)
{
	return false;
}

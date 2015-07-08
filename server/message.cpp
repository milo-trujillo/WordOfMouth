#include <string.h> // For strlen

#include "log.h"
#include "encode.h"
#include "message.h"

using namespace std;

/*
	Note: We make it a habit to use for-loops or memcpy to copy data rather
	than strcpy because we are often storing binary data and don't want 
	anything odd to happen if we hit a null byte.
*/

Message::Message(string plaintext)
{
	int len = plaintext.length() + 1; // Add one to get the null terminator
	data = new char[len];
	for( int i = 0; i < len; i++ )
		data[i] = plaintext[i];
	_size = len;
	status = PLAINTEXT;
}

Message::Message(const char* plaintext)
{
	int len = strlen(plaintext) + 1; // Add one to get the null terminator
	data = new char[len];
	for( int i = 0; i < len; i++ )
		data[i] = plaintext[i];
	_size = len;
	status = PLAINTEXT;
}

// If size is given *explicitly* then we assume we're copying binary data and
// do *not* add a null terminator
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
	if( data != nullptr )
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

// We attempt to base64 encode a message 
string Message::getAsciiArmor() const
{
	char* armor = nullptr;
	if( asciiEncode(data, _size, &armor) )
	{
		string encoded(armor);
		delete [] armor;
		return encoded;
	}
	else
	{
		log(LOG_ERROR, "Failed to ascii encode message!");
		return string("ERROR");
	}
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

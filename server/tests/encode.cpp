#include <iostream>
#include <string.h>

#include "../encode.h"

#define TEST_BASE64 0
#define TEST_HEX 1

using namespace std;

void test(int test)
{
	#ifdef VERBOSE
	if( test == TEST_BASE64 )
		cout << "\tTesting base64 encoding..." << endl;
	if( test == TEST_HEX )
		cout << "\tTesting hex encoding..." << endl;
	#endif

	const char* msg = "Testing123...";
	char* encoded;
	bool error = false;
	bool success;

	if( test == TEST_BASE64 )
		success = asciiEncode(msg, strlen(msg), &encoded);
	if( test == TEST_HEX )
		success = hexEncode(msg, strlen(msg), &encoded);
	if( !success )
	{
		#ifdef VERBOSE
		if( test == TEST_BASE64 )
			cout << "\tError base64 encoding raw string!" << endl;
		if( test == TEST_HEX )
			cout << "\tError hex encoding raw string!" << endl;
		#endif
		error = true;
	}
	char* decoded;
	int len;
	if( test == TEST_BASE64 )
		success = asciiDecode(encoded, &len, &decoded);
	if( test == TEST_HEX )
		success = hexDecode(encoded, &len, &decoded);
	if( !success )
	{
		#ifdef VERBOSE
		if( test == TEST_BASE64 )
			cout << "\tError base64 decoding raw string!" << endl;
		if( test == TEST_HEX )
			cout << "\tError hex decoding raw string!" << endl;
		#endif
		error = true;
	}
	else
	{
		if( len != strlen(decoded) )
		{
			#ifdef VERBOSE
			cout << "\tReported length does not match decoded length!" << endl;
			cout << "\t\tReported length: " << len << " Decoded length: " 
				<< strlen(decoded) << endl;
			#endif
			error = true;
		}
		else if( len != strlen(msg) )
		{
			#ifdef VERBOSE
			cout << "\tReported length does not match original message length!" << endl;
			cout << "\t\tReported length: " << len << " Original length: " 
				<< strlen(decoded) << endl;
			#endif
			error = true; 
		}
	}

	#ifdef VERBOSE
	cout << "\tMessage: " << msg << endl;
	if( test == TEST_BASE64 )
		cout << "\tBase64: " << encoded << endl;
	if( test == TEST_HEX )
		cout << "\tHex: " << encoded << endl;
	if( success )
		cout << "\tDecoded: " << decoded << endl;
	#endif

	if( strcmp(msg, decoded) != 0 )
	{
		#ifdef VERBOSE
		cout << "\tDecoded message does not match original!" << endl;
		cout << "\tLength of original: " << strlen(msg) << endl;
		cout << "\tLength of decoded: " << strlen(decoded) << endl;
		#endif
		error = true;
	}
	if( strcmp(msg, encoded) == 0 )
	{
		#ifdef VERBOSE
		cout << "\tEncoded and original messages should not match!" << endl;
		#endif
		error = true;
	}
	if( error )
	{
		if( test == TEST_BASE64 )
			cout << "[-] Problem with base64 raw strings!" << endl;
		if( test == TEST_HEX )
			cout << "[-] Problem with hex raw strings!" << endl;
	}
	else
	{
		if( test == TEST_BASE64 )
			cout << "[+] Base64 encoding for raw strings successful." << endl;
		if( test == TEST_HEX )
			cout << "[+] Hex encoding for raw strings successful." << endl;
	}
}

int main()
{
	test(TEST_BASE64);
	test(TEST_HEX);
	return 0;
}

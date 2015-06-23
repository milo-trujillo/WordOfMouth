#include <iostream>
#include <string.h>

#include "../encode.h"

using namespace std;

int main()
{
	#ifdef VERBOSE
	cout << "\tTesting base64 encoding..." << endl;
	#endif
	const char* msg = "Testing123...";
	char* encoded;
	bool error = false;
	bool success = asciiEncode(msg, strlen(msg), &encoded);
	if( !success )
	{
		#ifdef VERBOSE
		cout << "\tError encoding raw string!" << endl;
		#endif
		error = true;
	}
	char* decoded;
	int len;
	success = asciiDecode(encoded, &len, &decoded);
	if( !success )
	{
		#ifdef VERBOSE
		cout << "\tError decoding raw string!" << endl;
		#endif
		error = true;
	}

	#ifdef VERBOSE
	cout << "\tMessage: " << msg << endl;
	cout << "\tBase64: " << encoded << endl;
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
		cout << "[-] Problem with base64 raw strings!" << endl;
	else
		cout << "[+] Base64 encoding for raw strings successful." << endl;
	
	return 0;
}

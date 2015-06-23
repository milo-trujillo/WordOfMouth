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
		cout << "[-] Error encoding raw string!" << endl;
		error = true;
	}
	char* decoded;
	int len;
	success = asciiDecode(encoded, &len, &decoded);
	if( !success )
	{
		cout << "[-] Error decoding raw string!" << endl;
		error = true;
	}
	if( strcmp(msg, decoded) != 0 || strcmp(msg, encoded) == 0 )
	{
		cout << "[-] Error in string comparison!" << endl;
		error = true;
	}
	#ifdef VERBOSE
	cout << "\tMessage: " << msg << endl;
	cout << "\tBase64: " << encoded << endl;
	cout << "\tDecoded: " << decoded << endl;
	#endif
	if( error )
		cout << "[-] Problem with base64 raw strings!" << endl;
	else
		cout << "[+] Base64 encoding for raw strings successful." << endl;
	
	return 0;
}

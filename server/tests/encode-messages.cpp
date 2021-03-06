#include <iostream>
#include <string.h>

#include "../encode.h"
#include "../message.h"

using namespace std;

int main()
{
	// Debugging message ascii encoding / decoding
	string msg = "Testing 123...";
	Message m(msg);
	string aa = m.getAsciiArmor();
	#ifdef VERBOSE
	cout << "\tEncoding message with text: " << msg << endl;
	cout << "\tGot encoded data: " << aa << endl;
	#endif
	size_t len;
	char* decoded;
	bool success = asciiDecode(aa.c_str(), &len, &decoded);
	if( !success || (strcmp(msg.c_str(), decoded) != 0) )
	{
		#ifdef VERBOSE
		if( !success )
		{
			cout << "\tAscii decode returned internal error." << endl;
		}
		else if( strcmp(msg.c_str(), decoded) != 0 )
		{
			cout << "\tDecoded string does not match original." << endl;
			cout << "\tOriginal: " << msg << " (Length: " << msg.length()
				<< ")" << endl;
			cout << "\tDecoded:  " << decoded << " (Length: " << len 
				<< ")" << endl;
		}
		#endif
		cout << "[-] Error ascii decoding messages." << endl;
		return 0;
	}

	#ifdef VERBOSE
	cout << "\tData decodes to: " << decoded << endl;
	#endif

	cout << "[+] Messages ascii encode and decode correctly." << endl;

	return 0;
}

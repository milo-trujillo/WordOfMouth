#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../hash.h"
#include "../encode.h"

int main()
{
	bool errors = false;

	const char* str = "Testing123...";
	const char* correct_digest = "493b5c09b3d6d3581590f139740"
		"d2c8344a0f77940238e692248bd3e2c22730e57ae98f341462e6"
		"f8f4e8bc12a4d1e693f1580220e9da2aaa89106202b349bd5";
	char* dgst;
	bool success = hash(str, strlen(str), &dgst);
	if( !success )
		errors = true;

	char* ascii;
	success = hexEncode(dgst, getHashLength(), &ascii);
	if( !success )
	{
		errors = true;
		printf("[!] WARNING! Problem with ascii encoding "
			"-- this may not be a problem with hashing!\n");
	}

	#ifdef VERBOSE
	printf("\tHashing: %s\n", str);
	printf("\tGot digest: %s\n", ascii);
	printf("\tCorrect digest is: %s\n", correct_digest);
	#endif

	if( strcmp(ascii, correct_digest) != 0 )
	{
		#ifdef VERBOSE
		printf("\tHashes do not match!\n");
		#endif
		errors = true;
	}
	else
	{
		#ifdef VERBOSE
		printf("\tHashes match!\n");
		#endif
	}

	if( errors )
		printf("[-] Error with SHA512 hashing.\n");
	else
		printf("[+] Hashing algorithm is accurate.\n");

	// Cleanup from hashing and ascii encoding
	delete [] dgst;
	delete [] ascii;
	
	return 0;
}

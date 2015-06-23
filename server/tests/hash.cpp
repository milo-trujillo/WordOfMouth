#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../hash.h"
#include "../encode.h"

int main()
{
	bool errors = false;

	const char* str = "Testing123...";
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
	#endif

	// Now let's ask the local system to verify our results
	FILE* fp;
	char cmd[200];
	sprintf(cmd, "echo -n \"%s\" | shasum -a 512", str);
	fp = popen(cmd, "r");
	if( fp == NULL )
	{
		printf("\tError running local shasum program!\n");
		errors = true;
	}
	else
	{
		char data[500];
		fgets(data, sizeof(data) - 1, fp);
		#ifdef VERBOSE
		printf("\tLocal system got digest: %s\n", data);
		#endif
		int i;
		for( i = 0; data[i] != ' '; i++ )
		{
			if( ascii[i] != data[i] )
			{
				errors = true;
				printf("\tERROR: Hashes do not match!\n");
			}
		}
		if( i != strlen(ascii) )
		{
			errors = true;
			printf("\tERROR: Hash lengths do not match!\n");
		}
	}
	pclose(fp);

	if( errors )
		printf("[-] Error with SHA hashing.\n");
	else
		printf("[+] Hashing algorithm matches system implementation.\n");

	// Cleanup from hashing and ascii encoding
	delete [] dgst;
	delete [] ascii;
	
	return 0;
}

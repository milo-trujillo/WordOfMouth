#include <sstream>
#include <iostream>
#include "relay.h"
#include <fstream>
#include <stdio.h>
#include <time.h>
#include <map>
#include <unistd.h>
#include "log.h"
#include "cypher.h"
#include <sys/stat.h>
#include <cmath>

#define VERBOSE 0
//if you want all the debugging couts turned on, set this to 1, else 0

using namespace std;

bool fileExists(const string& fileName);
//string inPass;
RelayConfig inputPassword();
pair<bool,RelayConfig> relayDecrypt(string inPass);
void encrypt(string inPass);
RelayConfig test();
//pair<return1,return2> foo
//return make_pair(return1,return2);
//if foo.first


//RelayConfig(int l, std::string h, int p, std::string a)
//std::string encrypt(std::string alias, std::string message);
//std::string decrypt(std::string alias, std::string message);
//alias is the thing that is used for encrypting/decrypting
//The test files will be called Information.txt and Encrypted.txt

//logErr
//logWarn
//logDebug
//logInfo

//Song lyrics from OMI's "Cheerleader", just for Milo
/*
"She walks like a model,
She grants my wishes like a genie in a bottle,
'Cause I'm the wizard of love,
And I've got the magic wand,
All these other girls are tempting,
But I'm empty when you're gone.
And they're like, 
"Do you need me?
Do you think I'm pretty?
Do I make you feel like cheating?"
And I'm like "No, not really!"
*/


/*Things that this program needs to be able to do:
	-check to make sure that there isn't an empty password file
		-if there is, make one, and request that the user give a password, which will then be used to encrypt and decrypt the information that they give.
	-ask the user to input their password
		-if the user inputs the correct password, decrypt the information and return that info
		-if the user inputs the wrong password, return an error
		-side note: make sure that there's a delay between confirming if the password is correct or not
	-because of the fact that the information is encrypted with the person's "password" the password isn't actually compared to anything,
	and instead relies on the fact that decrypting with the wrong password will return some very odd strings that don't work.

*/

//int main()
//{
//	inputPassword();
	
//	return 0;
//}

RelayConfig inputPassword()
{
	string configFile="enco.txt";//enco for encryptedconfig
	int decrypted=0,input=0;
	string inPass="";//this is supposed to be the password that the user inputs to begin the program
//	pair<bool,RelayConfig()> decryptReturn;
	if(!fileExists(configFile))//fileExists is a bool, returning true if it exists, so this chunk of code only applies if the file doesn't exist.
	{
		//This bit here just defines the large number of strings that I needed for this bit of code
		//Anything with the prefix "en" is the encrypted version of the string.
		string foreignListen="",enForeignListen;
		string ipAddress="",enIpAddress;
		string foreignOut="",enForeignOut;
		string alias="",enAlias;
		string localListen="",enLocalListen;
		string localOut="",enLocalOut;
		string logFile="",enLogFile;
		string concatenated,enConcatenated;
		//The following requests input from the user to fill in the config file
		cout << "It appears that you do not have a file configured with the information you need to start the program." << endl;
		cout << "Please input the correct information according to the following prompts: " << endl;
		while(inPass=="")//doesn't accept an empty string
		{
			cout << "Choose a password for startup: "; // << endl;
			getline( cin,inPass);
			if(VERBOSE)
			{
				cout << "inPass: " << inPass << " length: " << inPass.length() << endl;
			}
		}
		while(foreignListen=="")
		{
			cout << "Enter the port other relays will contact you on: "; // << endl;
			getline( cin,foreignListen );
			if(VERBOSE)
			{
				cout << "foreignListen: " << foreignListen << " length: " << foreignListen.length() << endl;
			}
		}
		while(ipAddress=="")
		{
			cout << "Enter the IP of the next relay: "; // << endl;
			getline( cin,ipAddress );
			if(VERBOSE)
			{
				cout << "ipAddress: " << ipAddress << " length: " << ipAddress.length() <<  endl;
			}
		}
		while(foreignOut=="")
		{
			cout << "Enter the port you contact the next relay on: "; // << endl;
			getline( cin,foreignOut );
			if(VERBOSE)
			{
				cout << "foreignOut: " << foreignOut << " length: " << foreignOut.length() << endl;
			}
		}
		while(alias=="")
		{
			cout << "Enter the alias of the local user: "; // << endl;
			getline( cin,alias );
			if(VERBOSE)
			{
				cout << "alias: " << alias << " length: " << alias.length() << endl;
			}
		}
		while(localListen=="")
		{
			cout << "Enter the port your relay will listen on for new messages: "; // << endl;
			getline( cin,localListen );
			if(VERBOSE)
			{
				cout << "localListen: " << localListen << " length: " << localListen.length() << endl;
			}
		}
		while(localOut=="")
		{
			cout << "Enter the port you will display messages on: "; // << endl;
			getline( cin,localOut );
			if(VERBOSE)
			{
				cout << "localOut: " << localOut << " length: " << localOut.length() << endl;
			}
		}
		while(logFile=="")
		{
			cout << "Finally, input the name of the logfile that you use: "; // << endl;
			getline( cin,logFile );
			if(VERBOSE)
			{
				cout << "logFile: " << logFile << " length: " << logFile.length() << endl;
			}
		}

		//This takes all of the input that the user just gave, and puts it all into one long string for encryption.
		concatenated=foreignListen;
		concatenated.push_back('\n');
		concatenated.append(ipAddress);
		concatenated.push_back('\n');
		concatenated.append(foreignOut);
		concatenated.push_back('\n');
		concatenated.append(alias);
		concatenated.push_back('\n');
		concatenated.append(localListen);
		concatenated.push_back('\n');
		concatenated.append(localOut);
		concatenated.push_back('\n');
		concatenated.append(logFile);
		concatenated.push_back('\n');

		//The following encrypts the input given by the user above
		if(inPass!="")
		{
			if( decypher( cypher( concatenated,inPass ),inPass )==concatenated)
			{
				enConcatenated = cypher( concatenated,inPass );
			}
			else
			{
				cout << "Error encrypting." << endl;
			}
		}
		else
		{
			cout << "Error, please input password again." << endl;
			getline( cin,inPass );
			enConcatenated = cypher( concatenated,inPass );
		}


		//The following creates the encrypted file using the encrypted information attained above.
		ofstream enco;//opens up an output file stream
		enco.open("enco.txt");//opens the file enco.txt
		if(enco.is_open())//true unless there was a problem, like permission denied
		{
			enco << enConcatenated;
		}
		enco.close();
	}

	while(inPass=="")
	{
		cout << "Please input your password: " << endl;	
		getline( cin,inPass );
		if(inPass!="")
		{
			cout << "Thank you, attempting to decrypt the information" << endl;
			usleep(3000000);
		}
		else
		{
			cout << "Error, empty password field." << endl;
			usleep(3000000);
		}
	}
	pair<bool,RelayConfig> decryptReturn=relayDecrypt(inPass);
	if(decryptReturn.first==true)
	{
		decrypted=1;
	}
	else
	{
		decrypted=0;
		input=0;
		inPass="";
	}
	RelayConfig whatYouNeed=decryptReturn.second;
	//This portion requests that the user input their password to receive the information that they need to open the program
	while(1)//repeats until the password is correct
	{
		if(inPass=="")//Because it's possible that the user has just created their password, this checks that first. If the user inputs the password incorrectly, it returns to this state.
		{
			inPass="";
			cout << "Please input your password: ";//this requests their password on-screen
			getline(cin, inPass);
			cout << "Thank you, attempting to decrypt the information" << endl;
		}
		//cin >> inPass;//this here asks for input and puts it into the variable inPass
		if(inPass.size()!=0)//checks to make sure that the password input isn't completely empty
		{
			usleep(3000000);//waits 3 seconds before allowing the user to try the password again
			decryptReturn=relayDecrypt(inPass);
			if(decryptReturn.first==true)
			{
				whatYouNeed=decryptReturn.second;
				break;
			}
			else
			{
				cout << "Error. Incorrect password." << endl;
				decrypted=0;
				inPass="";
			}
		}
		else
		{
			cout << "Error, empty password field." << endl;
			usleep(3000000);//waits 3 seconds before allowing the user to try the password again
			inPass="";
		}
	}

	//This little function here simply redacts the password after it has been used and is no longer needed.
//	for(int i=0;i<inPass.length();i++)
//	{
//		inPass[i]="X";
//	}
	if(VERBOSE)
	{
		cout << "Relay successfully decrypted" << endl;
	}

	return whatYouNeed;
	
}







pair<bool,RelayConfig> relayDecrypt(string inPass)
{
	int i=0;
	string enForeignListen = "enerror";
	string enIpAddress = "enerror";
	string enForeignOut = "enerror";
	string enAlias = "enerror";
	string enLocalListen = "enerror";
	string enLocalOut = "enerror";
	string enLogFile = "enerror";
	int deForeignListen = -1;
	string deIpAddress = "";
	int deForeignOut = -1;
	string deAlias = "";
	int deLocalListen = -1;
	int deLocalOut = -1;
	string deLogFile = "";
	string extra1,extra2,extra3;
	string deConcatenated="";
	string deForeignListenString,deForeignOutString,deLocalListenString,deLocalOutString;
	//good grief, that's way too many variables


	//Opens up a stringstream and clears it
	stringstream ss;
	ss.str(""); // Clear the stringstream
	ss.clear(); // And any weird state it may have entered

	if(inPass=="")
	{
		cout << "Error, password string empty for relayDecrypt function." << endl;
		return make_pair( false,RelayConfig(-1,"Error",-1,"Error",-1,-1,"Error") );
	}
	ifstream myfile;//this takes and decrypts the information that its given, whoo!
	myfile.open("enco.txt");//opens the file enco.txt
	if(myfile.is_open())
	{

		//Reads from the encrypted config file
		string enConcatenated((istreambuf_iterator<char>(myfile)),istreambuf_iterator<char>());
		if( isReadableText( decypher( enConcatenated,inPass ) ) )
		{
		deConcatenated=decypher( enConcatenated,inPass );	
		}
		else
		{
			if(VERBOSE)
			{
				cout << "File non-decryptable." << endl;
			}
			return make_pair( false,RelayConfig(-1,"Error",-1,"Error",-1,-1,"Error") );
			i++;
		}

		//Decrypts each piece using the input password, checking at each step to make sure that it's human-readable
		//Theoretically, this check should prevent crashes that would occur if the wrong password is entered, and it tries to store a non-integer to an integer variable..
	}
	myfile.close();
	//My logic for this is that if all 7 pieces are human-readable, then I should get 7 

/*
	cout << "deForeignListen: " << deForeignListen << endl;
	cout << "deIpAddress: " << deIpAddress << endl;
	cout << "deForeignOut: " << deForeignOut << endl;
	cout << "deAlias: " << deAlias << endl;
	cout << "deLocalListen: " << deLocalListen << endl;
	cout << "deLocalOut: " << deLocalOut << endl;
	cout << "deLogFile: " << deLogFile << endl;
*/


	


	int decryptlength=deConcatenated.length(),j=0;
	for(i=0;i<decryptlength;i++)
	{
		if(j==0)
		{
			if(deConcatenated[i]!='\n')
			{
				deForeignListenString.append(deConcatenated,i,1);
			}
			else
			{
				i++;
				j++;
			}
		}
		if(j==1)
		{
			if(deConcatenated[i]!='\n')
			{
				deIpAddress.append(deConcatenated,i,1);
			}
			else
			{
				i++;
				j++;
			}
		}
		if(j==2)
		{
			if(deConcatenated[i]!='\n')
			{
				deForeignOutString.append(deConcatenated,i,1);
			}
			else
			{
				i++;
				j++;
			}
		}
		if(j==3)
		{
			if(deConcatenated[i]!='\n')
			{
				deAlias.append(deConcatenated,i,1);
			}
			else
			{
				i++;
				j++;
			}
		}
		if(j==4)
		{
			if(deConcatenated[i]!='\n')
			{
				deLocalListenString.append(deConcatenated,i,1);
			}
			else
			{
				i++;
				j++;
			}
		}
		if(j==5)
		{
			if(deConcatenated[i]!='\n')
			{
				deLocalOutString.append(deConcatenated,i,1);
			}
			else
			{
				i++;
				j++;
			}
		}
		if(j==6)
		{
			if(deConcatenated[i]!='\n')
			{
				deLogFile.append(deConcatenated,i,1);
			}
			else
			{
				i++;
				j++;
			}
		}
			
	}


	ss << deForeignListenString;
	ss >> deForeignListen;
	ss.str("");
	ss.clear();
	ss << deForeignOutString;
	ss >> deForeignOut;
	ss.str("");
	ss.clear();
	ss << deLocalListenString;
	ss >> deLocalListen;
	ss.str("");
	ss.clear();
	ss << deLocalOutString;
	ss >> deLocalOut;
	ss.str("");
	ss.clear();

	if(VERBOSE)
	{
		cout << "Decryption successful, returning RelayConfig." << endl;
	}

	return make_pair( true,RelayConfig(deForeignListen,deIpAddress,deForeignOut,deAlias,deLocalListen,deLocalOut,deLogFile) );
}




/*
	Oh hey, I found this online and it looks like it'll work for checking if a file exists, yay!
	This function requires the inclusion of <sys/stat.h>
	According to people in the comments, this method is faster than using the options available from fstream
	Now, for the comments included in the code for this:


	Check if a file exists
@param[in] filename - the name of the file to check

@return		 true if the file exists, else false


*/

bool fileExists(const string& filename)
{
	struct stat buf;
	if(stat(filename.c_str(), &buf) != -1)
	{
		return true;
	}

	return false;
}

#include <sstream>
#include <iostream>
#include "relay.h"
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <map>
#include <unistd.h>
#include "log.h"
#include "cypher.h"
#include <sys/stat.h>
#include <cmath>

using namespace std;

bool fileExists(const string* fileName);
string inPass;
RelayConfig inputPassword();
pair<bool,RelayConfig> decrypt(string inPass);
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

RelayConfig inputPassword()
{
	string configFile="enco.txt";//enco for encryptedconfig
	int decrypted=0,input=0;
	string inPass="";//this is supposed to be the password that the user inputs to begin the program
	if(!fileExists(configFile))//fileExists is a bool, returning true if it exists, so this chunk of code only applies if the file doesn't exist.
	{
		//This bit here just defines the large number of strings that I needed for this bit of code
		//Anything with the prefix "en" is the encrypted version of the string.
		string foreignListen,enForeignListen;
		string ipAddress,enIpAddress;
		string foreignOut,enForeignOut;
		string alias,enAlias;
		string localListen,enLocalListen;
		string localOut,enLocalOut;
		string logFile,enLogFile;
		string inPass;


		//The following requests input from the user to fill in the config file
		cout << "It appears that you do not have a file configured with the information you need to start the program." << endl;
		cout << "Please input the correct information according to the following prompts: " << endl;
		cout << "First, please input the password that you would like to use." << endl;
		getline( cin,inPass);
		cout << "Next, input the port that you listen for incoming messages on: " << endl;
		getline( cin,foreignListen );
		cout << "Next, input the ip address that you send information to: " << endl;
		getline( cin,ipAddress );
		cout << "Next, input the port that you send to for outgoing messages: " << endl;
		getline( cin,foreignOut );
		cout << "Next, input the alias of the person that you send your messages to: " << endl;
		getline( cin,alias );
		cout << "Next, input the port that you listen on locally (for communication within your machine): " << endl;
		getline( cin,localListen );
		cout << "Next, input the port that you sent to locally (for communication within your machine): " << endl;
		getline( cin,localOut );
		cout << "Finally, input the name of the logfile that you use: " << endl;
		getline( cin,logFile );

		//The following encrypts the input given by the user above
		enForeignListen = cypher( foreignListen,inPass );
		enIpAddress = cypher( enIpAddress,inPass );
		enForeignOut = cypher( foreignOut,inPass );
		enAlias = cypher( alias,inPass );
		enLocalListen = cypher( localListen,inPass );
		enLocalOut = cypher( localOut,inPass );
		enLogFile = cypher( logFile,inPass );

		//The following creates the encrypted file using the encrypted information attained above.
		ofstream enco;//opens up an output file stream
		enco.open("enco.txt");//opens the file enco.txt
		if(enco.is_open())//because we just opened it, this should be true.
		{
			enco << enForeignListen << endl;//puts the encrypted listen port in the top line
			enco << enIpAddress << endl;//ip address in next
			enco << enForeignOut << endl;//and so on
			enco << enAlias << endl;
			enco << enLocalListen << endl;
			enco << enLocalOut << endl;
			enco << enLogFile << endl;
			enco.close();
		}
	}

	//This portion requests that the user input their password to receive the information that they need to open the program
	while(decrypted==0)//repeats until the password is correct
	{
		while(input==0)//while loop to ensure that the user has provided non-empty input
		{
			input=0;
			if(passIn="")//Because it's possible that the user has just created their password, this checks that first. If the user inputs the password incorrectly, it returns to this state.
			{
				cout << "Please input your password: ";//this requests their password on-screen
				getline(cin, inPass);
			}
			//cin >> inPass;//this here asks for input and puts it into the variable inPass
			if(inPass.size()!=0)//checks to make sure that the password input isn't completely empty
			{
					cout << "Thank you, attempting to decrypt the information" << endl;//this is pretty straightforward, just tells the user that it's checking whether the password worked
					input=1;
					usleep(3000000);//waits 3 seconds before allowing the user to try the password again
					if(relayDecrypt.first)
					{
						whatYouNeed=relayDecrypt.second;
						decrypted=1;
					}
					else
					{
						cout << "Error. Incorrect password." << endl;
						decrypted=0;
						passIn="";
					}
			}
			else
			{
				cout << "Error, empty password field." << endl;
				usleep(3000000);//waits 3 seconds before allowing the user to try the password again
				passIn="";
			}
		}
		input=0;
//		decrypted=1;
	}
//	RelayConfig whatYouNeed=test();

	//This little function here simply redacts the password after it has been used and is no longer needed.
	for(i=0;i<passIn.length();i++)
	{
		passIn[i]="X";
	}

	return whatYouNeed;
	
}

pair<bool,RelayConfig> relayDecrypt(string passIn)
{
	int i;
	string enForeignListen;
	string enIpAddress;
	string enForeignOut;
	string enAlias;
	string enLocalListen;
	string enLocalOut;
	string enLogFile;
	int deForeignListen;
	string deIpAddress;
	int deForeignOut;
	string deAlias;
	int deLocalListen;
	int deLocalOut;
	string deLogFile;
//good grief, that's way too many variables

	ifstream myfile("enco.txt");//this takes and decrypts the information that its given, whoo!
	if(myfile.is_open())
	{
		//Opens up a stringstream
		stringstream ss;

		//Reads from the encrypted config file
		getline( myfile,enForeignListen );
		getline( myfile,enIpAddress );
		getline( myfile,enForeignOut );
		getline( myfile,enAlias );
		getline( myfile,enLocalListen );
		getline( myfile,enLocalOut );
		getline( myfile,enLogFile );

		//Decrypts each piece using the input password, checking at each step to make sure that it's human-readable
		//Theoretically, this check should prevent crashes that would occur if the wrong password is entered, and it tries to store a non-integer to an integer variable..
		if( isReadableText( decypher( enForeignListn,passIn ) ) )
		{
			ss << decypher( enForeignListen,passIn );//reads it line by line, the 1st saved to a variable enForeignListen, and then decrypts with the password, saving that to return it
			ss >> deForeignListen;
			ss.str(""); // Clear the stringstream
			ss.clear(); // And any weird state it may have entered
		}
		else
		{
			return make_pair( false,RelayConfig(-1,"Error",-1,"Error",-1,-1,"Error") );
		}
		if( isReadableText( decypher( enIpAddress,passIn ) ) )
		{
			deIpAddress=decypher( enIpAddress,passIn );
			ss << decypher( enForeignOut,passIn );
			ss >> deForeignOut;
			ss.str(""); // Clear the stringstream
			ss.clear(); // And any weird state it may have entered
		}
		else
		{
			return make_pair( false,RelayConfig(-1,"Error",-1,"Error",-1,-1,"Error") );
		}
		if( isReadableText( decypher( enAlias,passIn ) ) )
		{
			deAlias=decypher( enAlias,passIn );
		}
		else
		{
			return make_pair( false,RelayConfig(-1,"Error",-1,"Error",-1,-1,"Error") );
		}
		if( isReadableText( decypher( enLocalListen,passIn ) ) )
		{
		ss << decypher( enLocalListen,passIn );
		ss >> deLocalListen;
		ss.str(""); // Clear the stringstream
		ss.clear(); // And any weird state it may have entered
		}
		else
		{
			return make_pair( false,RelayConfig(-1,"Error",-1,"Error",-1,-1,"Error") );
		}
		if( isReadableText( decypher( enLocalOut,passIn ) ) )
		{
		ss << decypher( enLocalOut,passIn );
		ss >> deLocalOut;
		ss.str(""); // Clear the stringstream
		ss.clear(); // And any weird state it may have entered
		}
		else
		{
			return make_pair( false,RelayConfig(-1,"Error",-1,"Error",-1,-1,"Error") );
		}
		if( isReadableText( decypher( enLogFile,passIn ) ) )
		{
		deLogFile=decypher( enLogFile,passIn );
		}
		else
		{
			return make_pair( false,RelayConfig(-1,"Error",-1,"Error",-1,-1,"Error") );
		}
		myfile.close();
	}

	//My logic for this is that if all 7 pieces are human-readable, then I should get 7 

	return make_pair( true,RelayConfig(deForeignListen,deIpAddress,deForeignOut,deAlias,deLocalListen,deLocalOut,deLogFile) );
}



//between this and the last comment is all that's needed to decrypt a txt file that's already encrypted
/*This is now taken care of in the main call to this thing.
void relayEncrypt(string passIn)
{	
//after this line is the stuff that encrypts an unencrpted file	
	string foreignListen;
	string ipAddress;
	string foreignOut;
	string alias;
	string localListen;
	string localOut;
	string logFile;
	string enForeignListen;
	string enIpAddress;
	string enForeignOut;
	string enAlias;
	string enLocalListen;
	string enLocalOut;
	string enLogFile;
	int i;

	ifstream infofile("Information.txt");//reads through Information.txt and stores each line into a variable for use in creating the encrypted information
	if(infofile.is_open())//this for sure works, so no problems here
	{
		getline( infofile,foreignListen );
		getline( infofile,ipAddress );
		getline( infofile,foreignOut );
		getline( infofile,alias );
		getline( infofile,localListen );
		getline( infofile,localOut );
		getline( infofile,logFile );
		infofile.close();
		enForeignListen = cypher( foreignListen,passIn );
		enIpAddress = cypher( ipAddress,passIn );
		enForeignOut = cypher( foreignOut,passIn );
		enAlias = cypher( alias,passIn );
		enLocalListen = cypher( localListen,passIn );
		enLocalOut = cypher( localOut,passIn );
		enLogFile = cypher( logFile,passIn );
		
//		cout << enForeignListen << endl;
//		cout << enIpAddress << endl;
//		cout << enForeignOut << endl;
//		cout << enAlias << endl;
	}
	
	ofstream encryptedfile("Encrypted.txt");//this is for taking the stuff that was read from Information.txt and storing it into the encrypted txt file
	if(encryptedfile.is_open())
	{
		encryptedfile << enForeignListen << endl;
		encryptedfile << enIpAddress << endl;
		encryptedfile << enForeignOut << endl;
		encryptedfile << enAlias << endl;
		encryptedfile << enLocalListen << endl;
		encryptedfile << enLocalOut << endl;
		encryptedfile << enLogFile << endl;
		encryptedfile.close();
	}
	
	
	
	for(i=0;i<inPass.size();i++)//since the password is no longer needed at all, we redact it
	{
		inPass[i]='X';
	}//end of the redaction function
//	cout << deForeignListen << endl;
//	cout << deIpAddress << endl;



	return;
}*/

RelayConfig test()
{
	
	string enForeignListen="Error";
	string enIpAddress="Error";
	string enForeignOut="Error";
	string enAlias="Error";
	string enLocalListen="Error";
	string enLocalOut="Error";
	string enLogFile="Error";
	string log="";
	int deLocalListen = -1;
	int deLocalOut = -1;
	int deForeignListen = -1;
	string deIpAddress="Error";
	int deForeignOut = -1;
	string deAlias="Error";
	string deLogFile="Error";
	ifstream myfile("test.txt");//this takes and decrypts the information that its given, whoo!
	stringstream ss;
	if(myfile.is_open())
	{
		getline( myfile,enForeignListen );//gets the first line,saves it to enForeignListen
		// We cannot print encrypted information, so these logs have been disabled
		/*
		if(enForeignListen!="Error")
		{
			log="Input encrypted foreign listen port: ";
			log+=enForeignListen;
			logInfo(log);
			log="";
		}
		*/
		//cout << "enForeignListen: " << enForeignListen << endl;//prints this out
		getline( myfile,enIpAddress );//same
		/*
		if(enIpAddress!="Error")
		{
			log="Input encrypted ip address: ";
			log+=enIpAddress;
			logInfo(log);
			log="";
		}
		*/
		//cout << "enIpAddress: " << enIpAddress << endl;//same
		getline( myfile,enForeignOut );//same
		/*
		if(enForeignOut!="Error")
		{
			log="Input encrypted foreign ouput port: ";
			log+=enForeignOut;
			logInfo(log);
			log="";
		}
		*/
		//cout << "enForeignOut: " << enForeignOut << endl;//same
		getline( myfile,enAlias );//same
		/*
		if(enAlias!="Error")
		{
			log="Input encrypted foreign ouput port: ";
			log+=enAlias;
			logInfo(log);
			log="";
		}
		*/
		//cout << "enAlias: " << enAlias << endl;//same
		getline( myfile,enLocalListen );
		getline( myfile,enLocalOut );
		getline( myfile,enLogFile );
		myfile.close();
		ss << enForeignListen;//reads it line by line, the 1st saved to a variable enForeignListen, and then decrypts with the password, saving that to return it
		ss >> deForeignListen;
		ss.str(""); // Clear the stringstream
		ss.clear(); // And any weird state it may have entered
		deIpAddress=enIpAddress;
		ss << enForeignOut;
		deAlias=enAlias;
		ss >> deForeignOut;
		ss.str("");
		ss.clear();
		ss << enLocalListen;//reads it line by line, the 1st saved to a variable enForeignListen, and then decrypts with the password, saving that to return it
		ss >> deLocalListen;
		ss.str(""); // Clear the stringstream
		ss.clear(); // And any weird state it may have entered
		ss << enLocalOut;//reads it line by line, the 1st saved to a variable enForeignListen, and then decrypts with the password, saving that to return it
		ss >> deLocalOut;
		ss.str(""); // Clear the stringstream
		ss.clear(); // And any weird state it may have entered
		ss << enLogFile;
		ss >> deLogFile;
		ss.str("");
		ss.clear();
		/*
		cout << "Gives:" << deForeignListen << endl;
		cout << "Gives:" << deIpAddress << endl;
		cout << "Gives:" << deForeignOut << endl;
		cout << "Gives:" << deAlias << endl;
		*/
	}

	return RelayConfig(deForeignListen,deIpAddress,deForeignOut,deAlias,deLocalListen,deLocalOut,deLogFile);
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

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
#define FILENAME "enco.txt"
//this allows the compiler to change the name of the encrypted config file to whatever they'd like (perhaps for obscurity?)
//if this is done, be sure to keep the quotation marks around the name, as well as the .txt at the end.
//for example, changing "enco.txt" to "encryptedconfig" is not valid, but changing it to "thisisuseless.txt" is valid.

using namespace std;

bool fileExists(const string& fileName);//this function checks to see whether a certain file exists, and is more efficient than using fstream
RelayConfig inputPassword();//this is the main part of the NextNodeInfo function, requesting the user's password or other information
pair<bool,RelayConfig> relayDecrypt(string inPass);//this is what decrypts the information in the encrypted file containing the config for WOM


//The following is just notes on how to make pairs
//pair<return1,return2> foo
//return make_pair(return1,return2);
//if foo.first



//The following is notes regarding the log and its usage
//logErr(string)
//logWarn(string)
//logDebug(string)
//logInfo(string)



//The following is song lyrics for Milo
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

/*Things that this program can already do:
	-Detect whether there is a config file that has been created, and if it hasn't, prompts the user for input regarding the config file.
		-in addition, it can also detect whether or not a file exists, as a side function useable by other parts of the program by including the header.
	-Requests user password.
		-either decrypts the config successfully or unsuccessfully, and returns that result.
			-if unsuccessful, requests the password to be input again
		-detects whether the password field was blank, and requests input again if it is.
			-empties the password variable after each iteration.
	-Regardless of whether the result was successful, it waits 3 seconds before continuing through another iteration, or continuing to completion.
	-If a config file doesn't exits, encrypts the information that the user inputs to create the config file, and read from an encrypted config file if the correct password is given.
	-Detect whether or not an empty password field has been input.
*/




RelayConfig inputPassword()
{
	string configFile=FILENAME;//this stores away the name of the config file
	string inPass="";//this is supposed to be the password that the user inputs to begin the program
	if(!fileExists(configFile))//fileExists returnss a boolean, returning true if it exists, so this chunk of code only applies if the file doesn't exist.
	{
		//This bit here just defines the large number of strings that I needed for this bit of code, for the input, encryption, and concatenation.
		string foreignListen="";
		string ipAddress="";
		string foreignOut="";
		string alias="";
		string localListen="";
		string localOut="";
		string logFile="";
		//up until this point, this is just for user input, and the strings are unnecessary anywhere else in the code, which is why they're declared within the if statement
		//after taking in this input, it's all saved into the string concatenated
		//this is done by taking one piece, pushing a newline character to the end of it, then taking the next piece, and so on (for later separation of the parts after decryption)
		string concatenated;
		string enConcatenated;//This is the string that the encrypted version of concatenated is saved to before putting it in the text file


		//The following requests input from the user to fill in the config file, and this code will only be executed if a config file doesn't exist.
		cout << "It appears that you do not have a file configured with the information you need to start the program." << endl;
		cout << "Please input the correct information according to the following prompts: " << endl;

		//The following is how the request for information to fill the config file is made, up until the next comment block.
		//Because each part of the config follows a standard method of input gathering, I'm only commenting the password part.

		while(inPass=="")//doesn't accept an empty string for the input
		{
			cout << "Choose a password for startup: ";//Because of the fact that the program hasn't daemonized at this point, cout tells the user what is being requested
			getline( cin,inPass);//This takes in the user input from cin, and stores it to the variable inPass (in order to store away the password they use for encryption later)
			if(VERBOSE)//this statement is handled by a preprocessor macro at the top, where VERBOSE is defined as 0 or 1 to give fairly complete debugging info
			{
				cout << "inPass: " << inPass << " length: " << inPass.length() << endl;
				//This couts the input password (repeats it back to the user) as well as its length (there were issues before, so this is mostly just leftover.
			}
		}

		while(foreignListen=="")
		{
			cout << "Enter the port other relays will contact you on: ";
			getline( cin,foreignListen );
			if(VERBOSE)
			{
				cout << "foreignListen: " << foreignListen << " length: " << foreignListen.length() << endl;
			}
		}

		while(ipAddress=="")
		{
			cout << "Enter the IP of the next relay: ";
			getline( cin,ipAddress );
			if(VERBOSE)
			{
				cout << "ipAddress: " << ipAddress << " length: " << ipAddress.length() <<  endl;
			}
		}

		while(foreignOut=="")
		{
			cout << "Enter the port you contact the next relay on: ";
			getline( cin,foreignOut );
			if(VERBOSE)
			{
				cout << "foreignOut: " << foreignOut << " length: " << foreignOut.length() << endl;
			}
		}

		while(alias=="")
		{
			cout << "Enter the alias of the local user: ";
			getline( cin,alias );
			if(VERBOSE)
			{
				cout << "alias: " << alias << " length: " << alias.length() << endl;
			}
		}

		while(localListen=="")
		{
			cout << "Enter the port your relay will listen on for new messages: ";
			getline( cin,localListen );
			if(VERBOSE)
			{
				cout << "localListen: " << localListen << " length: " << localListen.length() << endl;
			}
		}

		while(localOut=="")
		{
			cout << "Enter the port you will display messages on: ";
			getline( cin,localOut );
			if(VERBOSE)
			{
				cout << "localOut: " << localOut << " length: " << localOut.length() << endl;
			}
		}

		while(logFile=="")
		{
			cout << "Finally, input the name of the logfile that you use: ";
			getline( cin,logFile );
			if(VERBOSE)
			{
				cout << "logFile: " << logFile << " length: " << logFile.length() << endl;
			}
		}

		/*
			If ever needed, more pieces can be added to the config fairly easily. In this section of the code, what needs to happen is that
			a small block of code needs to be written as follows:
			while(NEWINFO=="")
			{
				cout << "Enter the [NEWINFO DESCRIPTION]: ";
				getline( cin,NEWINFO );
				if(VERBOSE)
				{
					cout << "NEWINFO: " << NEWINFO << " length: " << NEWINFO.length() << endl;
				}
			}

			Where NEWINFO is the new piece of information that is appended to the RelayConfig.
		*/

		//This takes all of the input that the user just gave, and puts it all into one long string for encryption.
		concatenated=foreignListen;//Keep in mind that this has to be in the order of RelayConfig in order to work. It starts by setting concatenated to the first part of RelayConfig
		concatenated.push_back('\n');//Then it appends a newline character to it.
		concatenated.append(ipAddress);//Then the next part of RelayConfig as added
		concatenated.push_back('\n');//And so on.
		concatenated.append(foreignOut);//The newline characters are used as separaters for later when the config file needs to be decrypted
		concatenated.push_back('\n');//And then when the decrypted file needs to be separated into each piece again.
		concatenated.append(alias);
		concatenated.push_back('\n');
		concatenated.append(localListen);
		concatenated.push_back('\n');
		concatenated.append(localOut);
		concatenated.push_back('\n');
		concatenated.append(logFile);
		concatenated.push_back('\n');

		//The following encrypts the input given by the user above
		if(inPass!="")//theoretically this will ALWAYS succeed because of the fact that the code doesn't accept a null password, but you never know.
		{
			if( decypherWeak( cypherWeak( concatenated,inPass ),inPass )==concatenated)
			{//This checks to make sure that decrypting the encrypted version of the concatenation is the same as the unencrypted form, kind of as a failsafe to make sure that nothing bad happened
				enConcatenated = cypherWeak( concatenated,inPass );//if encryption doesn't fail, then it stores the encrypted version into the enConcatenated string.
			}
			else
			{
				cout << "Error encrypting. Please delete your encrypted config file and try again." << endl;//Like I said, if something weird happened, the user wants to know.
			}
		}
		else
		{
			cout << "Error, please input password again: ";//If for some reason the password is gone, it asks the user to input it again, which really shouldn't happen.
			getline( cin,inPass );//stores the user's input into the terminal into the variable inPass.
			if( decypherWeak( cypherWeak( concatenated,inPass ),inPass )==concatenated)
			{//This checks to make sure that decrypting the encrypted version of the concatenation is the same as the unencrypted form, kind of as a failsafe to make sure that nothing bad happened
				enConcatenated = cypherWeak( concatenated,inPass );//if encryption doesn't fail, then it stores the encrypted version into the enConcatenated string.
			}
			else
			{
				cout << "Error encrypting. Please delete your encrypted config file and try again." << endl;//Like I said, if something weird happened, the user wants to know.
			}
		}


		//The following creates the encrypted file using the encrypted information attained above.
		ofstream enco;//opens up an output file stream in order to store enConcatenated to a new file
		enco.open(FILENAME);//opens the config file, which in turn creates the file
		if(enco.is_open())//should be true unless there was some weird problem, like file access permission being denied
		{
			enco << enConcatenated;//stores enConcatenated directly into the file.
		}
		enco.close();//closes the file
	}

	//for some reason, not having this little bit of code here caused inconsistencies with sleep times, which means somebody could potentially bruteforce the password by
	//repeatedly opening the program and inputting the password.
	while(inPass=="")//assuming the file was just made, this will be skipped over and the program will jump right to the next part of the code.
	{//otherwise, this little piece will be executed
		cout << "Please input your password: ";//requests the user for their password
		getline( cin,inPass );//retrieves the password from cin and stores it to inPass
		if(inPass!="")//if they actually put in a password, this is entered
		{
			cout << "Thank you, attempting to decrypt the information" << endl;//tells the user that it's attempting to decrypt the config file
			usleep(3000000);//waits for 3 seconds before continuing out of the loop
		}
		else//otherwise if they entered a null string
		{
			cout << "Error, empty password field." << endl;//the program tells them
			usleep(3000000);//and still waits for 3 seconds before continuing
		}
	}


	pair<bool,RelayConfig> decryptReturn=relayDecrypt(inPass);//this declaration was necessary in order to compile, and so the following bit of code makes up
	//for the weirdness by checking to make sure that it was successful before continuing on
	if(decryptReturn.first==false)//if the decryption was successful, the program can continue on freely without entering the while loops at all
	{
		inPass="";
		cout << "Error. Incorrect password." << endl;
	}
	RelayConfig whatYouNeed=decryptReturn.second;


	//This portion requests that the user input their password to receive the information that they need to open the program
	while(1)//repeats until the password is correct
	{
		if(inPass=="")//Because it's possible that the user has just created their password, this checks that first. If the user inputs the password incorrectly, it returns to this state.
		{
			cout << "Please input your password: ";//this requests their password on-screen
			getline( cin,inPass );//this takes in the user's password typed into the terminal and stores it to inPass
			cout << "Thank you, attempting to decrypt the information" << endl;//tells the user that it is attempting to decrypt the information
		}
		if(inPass.size()!=0)//checks to make sure that the password input isn't completely empty so that it doesn't attempt to decrypt with a null string
		{
			usleep(3000000);//waits 3 seconds before allowing the user to try the password again
			decryptReturn=relayDecrypt(inPass);//saves off the result of relayDecrypt into the decryptReturn
			if(decryptReturn.first==true)//if successful, this statement will be entered
			{
				whatYouNeed=decryptReturn.second;//it stores off the RelayConfig for returning from the function
				break;//then exits out of the while loop with this
			}
			else
			{
				cout << "Error. Incorrect password." << endl;//otherwise it tells the user that the encryption was unsuccessful
				inPass="";//resets the inPass variable to 0, and continues through the while loop again
			}
		}
		else
		{
			cout << "Error, empty password field." << endl;//if the user input an empty password field, it tells them so before repeating the loop
			usleep(3000000);//waits 3 seconds before allowing the user to try the password again
			inPass="";//just in case something funky happened, this reset the inPass field to being empt
		}
	}

	if(VERBOSE)//if descriptive debugging is enabled, this is entered
	{
		cout << "Relay successfully decrypted" << endl;//and it tells the user that the decryption was successful.
	}

	return whatYouNeed;
	
}







pair<bool,RelayConfig> relayDecrypt(string inPass)
{
	int i=0;//generic variable

	//The following are what get used for the RelayConfigReturn
	int deForeignListen = -1;
	string deIpAddress = "";
	int deForeignOut = -1;
	string deAlias = "";
	int deLocalListen = -1;
	int deLocalOut = -1;
	string deLogFile = "";

	//these are for the decryption of the config file itself
	string deConcatenated="";
	string deForeignListenString,deForeignOutString,deLocalListenString,deLocalOutString;


	//Opens up a stringstream and clears it
	stringstream ss;
	ss.str("");
	ss.clear();

	if(inPass=="")//checks to make sure that this function wasn't called with an empty inPass
	{
		cout << "Error, password string empty for relayDecrypt function." << endl;//if it was, it prints out an error
		return make_pair( false,RelayConfig(-1,"Error",-1,"Error",-1,-1,"Error") );//and returns a pair that should error out any function that calls this with an empty inPass
	}

	//this opens up the encrypted file and attempts to decrypt it
	ifstream myfile;
	myfile.open(FILENAME);//opens the config file
	if(myfile.is_open())//checks to make sure that the file opened correctly (could be issues with perms or something)
	{

		//Reads from the encrypted config file and stores it into the string enConcatenated
		string enConcatenated((istreambuf_iterator<char>(myfile)),istreambuf_iterator<char>());
		if( isReadableText( decypherWeak( enConcatenated,inPass ) ) )//checks to make sure that decrypting gives a readable text
		{
			deConcatenated=decypherWeak( enConcatenated,inPass );//if it is, then it stores the results of decryption to deConcatenated
		}
		else//otherwise an error is returned
		{
			if(VERBOSE)
			{
				cout << "File non-decryptable." << endl;//if verbosia is turned on, then it tells the user that decryption was unsuccessful
			}
			return make_pair( false,RelayConfig(-1,"Error",-1,"Error",-1,-1,"Error") );//returns an error RelayConfig pair
		}

	}
	myfile.close();

	int decryptlength=deConcatenated.length();//this only iterates over the length of the string, so i have to know its length
	int j=0;//this is used in order to keep track of which part of the RelayConfig the decrypt function is on when splitting the deConcatenated string apart
	for(i=0;i<decryptlength;i++)//iterates over the length of the string
	{
		//alright, this next part is a bit tricky and relies on the fact that each piece of the RelayConfig was previously separated by newlines
		//since each part follows the same procedure, I'm only going to comment one example of it to explain how this works
		//if more parts are added to RelayConfig, following the pattern below will account for it
		if(j==0)//this simply checks which part of the RelayConfig that the separation is currently on, the ForeignListen port is 0, ipAddress is 1, and so on
		{
			if(deConcatenated[i]!='\n')//it only executes this portion of the code up until the newline character
			{
				deForeignListenString.append(deConcatenated,i,1);
				//ok, so the append method of strings is kind of weird since there are like. 7 ways it can work.
				//what this does is it appends a portion of deConcatenated to deForeignListenString
				//by looking at deConcatenated[i] to begin, and taking in one character
				//in essence only appending a single character to the deForeignListenString
			}
			else//if it reaches a newline character, we want to skip over it and move onto the next part of the RelayConfig
			{
				i++;//this skips over the newline character
				j++;//this moves to the next part of the RelayConfig
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

	//because of the fact that some of the RelayConfig pieces are integers and not strings, we have to convert them with stringstream
	ss << deForeignListenString;//this takes in the string
	ss >> deForeignListen;//and this stores it t an integer
	ss.str("");//empties stringstream
	ss.clear();//and any weird state it might have entered
	ss << deForeignOutString;//rinse and repeat for all the ints
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
		cout << "Decryption successful, returning RelayConfig." << endl;//once again, this only prints out if full debugging couts are turned on in the preprocessors up top
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

bool fileExists(const string& filename)//I'm not entirely sure how stat works, so I won't pretend to know. I just know that this does check to see if a file exists, and it works.
{
	struct stat buf;
	if(stat(filename.c_str(), &buf) != -1)
	{
		return true;
	}

	return false;
}

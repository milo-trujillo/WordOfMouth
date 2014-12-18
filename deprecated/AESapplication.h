#ifndef AESAPPLICATION_H
#define AESAPPLICATION_H
#include <iostream>//for strings

std::string encrypt(std::string message,std::string password)
//this function takes in a message and uses AES to encrypt it with a given password, both in string form. 
//there are some known bizarre bugs with the thing, and all the examples that I've managed to come up with
//have something to do with a lower-case letter n in the password conflicting with something in the message
//so it currently attempts to encrypt, then decrypt, and compare the result with original message. if the
//encryption/decryption was successful, it repeats the encryption and returns the encrypted message. If it
//was not successful, then it couts that there was an error, and returns a string that contains the message
//"Error."

std::string decrypt(std::string message,std::string password)
//similar to the encrypt function, this uses AES in order to decrypt messages using a given password.
//it doesn't appear to have the same problems that the encrypt function has, but it's hard to tell because
//of the fact that those errors would likely stem from a similar issue with the encrypt function, so I
//imagine that fixing the encryption function in those weird cases would also fix any issues with this

#endif

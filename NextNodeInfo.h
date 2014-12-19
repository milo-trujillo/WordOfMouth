#ifndef NEXTNODEINFO_H
#define NEXTNODEINFO_H

#include "relay.h"
#include <map>
//This one asks the user to input their password to decrypt the encrypted config file
relay::RelayConfig inputPassword();

//This fancy function is supposed to be pretty good at checking whether or not a file exists or not
bool fileExists(const std::string& filename);

//This decrypts the encrypted config file with a given password; if successful, it will return
//A true value for the boolean, and a filled in RelayConfig. Otherwise, it will return a false
//Value for the boolean, and a RelayConfig filled with "Error" strings where there are strings,
//And -1 where there are integers (to avoid crashes during decryption)
std::pair<bool,relay::RelayConfig> relayDecrypt(std::string passIn);





#endif

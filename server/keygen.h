#ifndef KEYGEN_H
#define KEYGEN_H

void keyGen(char** privKey, char** pubKey);
void keyDelete(char* privKey, char* pubKey);

char* encrypt(char* key, unsigned char* plaintext);
char* decrypt(char* key, unsigned char* cyphertext);

#endif

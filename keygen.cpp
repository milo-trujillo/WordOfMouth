#include <gcrypt.h>
#include <map>
#include <cstdlib>
#include <stdio.h>
#include <gpg-error.h>
#include <iostream>
#include <strings.h>

using namespace std;

//everything in caps is some kind of format setter mandated by gcrypt
gcry_sexp_t sexp_new(const char* lisp)//basically turns a char* into a s-expression
{
  /*the documented example of RSA keygen was in lisp,
  this should help get an s-expression with c++*/
  gcry_error_t err;
  gcry_sexp_t sexp;
  long unsigned int length = strlen(lisp);
  if (err = gcry_sexp_new(&sexp,lisp,length,1))
  {//found here: https://www.gnupg.org/documentation/manuals/gcrypt/Working-with-S_002dexpressions.html
    cerr << "error in gcry_sexp_new" << endl;
  }
  return sexp;
}

char* makeStringFromSexp(gcry_sexp_t sexp)//reverse of above, makes s-expression into char*
{
  unsigned int bufferLength = gcry_sexp_sprint(sexp, GCRYSEXP_FMT_ADVANCED, NULL, 0);//remember to put a mode here not sure which is best yet
  char* buffer = new char[bufferLength];
  if (buffer != NULL)
  {
    if (0!=gcry_sexp_sprint(sexp, GCRYSEXP_FMT_ADVANCED, buffer, bufferLength))
    {//found here: https://www.gnupg.org/documentation/manuals/gcrypt/Working-with-S_002dexpressions.html
      return buffer;
    }
    else
    {
      cerr << "error in gcry_sexp_sprint" << endl;
      exit(1);
    }
  }
  else cerr << "null buffer in gcry_sexp_sprint" << endl;
  exit(1);
}

void keyGen(char **privKey, char **pubKey)
{
  gcry_error_t err;
  gcry_sexp_t parms = sexp_new("(genkey(rsa(transient-key)(nbits 4:2048)))");
  gcry_sexp_t r_key;
  if (err = gcry_pk_genkey(&r_key,parms))
  {
    cerr << "error generating key" << endl;
    exit(1);
  }
  gcry_sexp_t pubSexp = gcry_sexp_nth(r_key,1);
  gcry_sexp_t privSexp = gcry_sexp_nth(r_key,2);

  *privKey = makeStringFromSexp(privSexp);
  *pubKey = makeStringFromSexp(pubSexp);
}

char* encrypt(char* key,unsigned char* plain)
{
  gcry_error_t err;
  gcry_mpi_t r_mpi;
  if (err = gcry_mpi_scan(&r_mpi,GCRYMPI_FMT_STD,plain,0,NULL))
  {//found here:https://www.gnupg.org/documentation/manuals/gcrypt/MPI-formats.html
      cerr << "error in gcry_mpi_scan " << endl;
      exit(1);
  }

  gcry_sexp_t r_sexp;
  long unsigned int erroff;
  string tmp = (char*) plain;
  tmp+="(flags raw) (value %m)";
  if (err = gcry_sexp_build(&r_sexp,&erroff,tmp.c_str(),r_mpi))//another one of those weird things in lisp?
  {//found here:https://www.gnupg.org/documentation/manuals/gcrypt/Working-with-S_002dexpressions.html
      cerr << "error in gcry_sexp_build" << endl;
      exit(1);
  }

  gcry_sexp_t pkey = sexp_new(key);
  gcry_sexp_t r_ciph;
  if (err = gcry_pk_encrypt(&r_ciph,r_sexp,pkey))
  {//found here:https://www.gnupg.org/documentation/manuals/gcrypt/Cryptographic-Functions.html
      cerr << "error in gcry_pk_encrypt" << endl;
      exit(1);
  }

  return makeStringFromSexp(r_ciph);
}

unsigned char* decrypt(char* key,char* cipher)
{
  gcry_error_t err;
  gcry_sexp_t data = sexp_new(cipher);
  gcry_sexp_t skey = sexp_new(key);
  gcry_sexp_t r_plain;
  if (err = gcry_pk_decrypt(&r_plain,data,skey))
  {//found here:https://www.gnupg.org/documentation/manuals/gcrypt/Cryptographic-Functions.html#Cryptographic-Functions
      cerr << "error in gcry_pk_decrypt" << endl;
      exit(1);
  }

  gcry_mpi_t r_mpi = gcry_sexp_nth_mpi(r_plain,0,GCRYMPI_FMT_USG);
  //found here:https://www.gnupg.org/documentation/manuals/gcrypt/Working-with-S_002dexpressions.html

  unsigned char *buffer;
  long unsigned int bufferLength;
  if (err = gcry_mpi_aprint(GCRYMPI_FMT_STD,&buffer,&bufferLength,r_mpi))
  {//found here:https://www.gnupg.org/documentation/manuals/gcrypt/MPI-formats.html
      cerr << "error in gcry_mpi_aprint" << endl;
      exit(1);
  }
  return buffer;
}

int main()
{
  char *pubMsgKey, *privMsgKey, *pubRelayKey, *privRelayKey;
  keyGen(&privMsgKey,&pubMsgKey);
  keyGen(&privRelayKey,&pubRelayKey);

  unsigned char* plainAlias = (unsigned char*) "harrypotter";
  cout << "Plaintext Alias: " << plainAlias << endl;
  char* encryptedAlias;
  encryptedAlias = encrypt(pubRelayKey,plainAlias);
  cout << "Cipher Alias: " << encryptedAlias << endl;
  unsigned char* decryptedAlias;
  decryptedAlias = decrypt(privRelayKey,encryptedAlias);
  cout << "Decrypted Alias: " << decryptedAlias << endl;
  return 0;
}

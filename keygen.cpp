#include <gcrypt.h>
#include <map>
#include <cstdlib>
#include <stdio.h>
#include <gpg-error.h>
#include <iostream>

using namespace std;

gcry_sexp_t sexp_new(const char* lisp)
{
  /*the documented example of RSA keygen was in lisp,
  this should help get an s-expression with c++*/
  gcry_error_t err;
  gcry_sexp_t sexp;
  unsigned int length = strlen(lisp);
  if (err = gcry_sexp_new(&sexp,lisp,length,1))
  {
    cerr << "error here write better msg later" << endl;
  }
  return sexp;
}

char* makeStringFromSexp(gcry_sexp_t sexp)
{
  unsigned int bufferLength = gcry_sexp_sprint(sexp, GCRYSEXP_FMT_ADVANCED, NULL, 0);//remember to put a mode here not sure which is best yet
  char* buffer = new char[bufferLength];
  if (buffer != NULL)
  {
    if (0!=gcry_sexp_sprint(sexp, GCRYSEXP_FMT_ADVANCED, buffer, bufferLength))
    {
      return buffer;
    }
    else cerr << "something's funky here. put a better error msg" << endl;
  }
  else cerr << "null buffer" << endl;
  exit(1);
}

void keyGen(char **privKey, char **pubKey)
{
  gcry_error_t err;
  gcry_sexp_t parms = sexp_new("(genkey(rsa(transient-key)(nbits 4:2048)))");
  gcry_sexp_t r_key;
  err = gcry_pk_genkey(&r_key,parms);
  if (err != 0) {
    cerr << "error generating key" << endl;
    exit(1);
  }
  gcry_sexp_t pubSexp = gcry_sexp_nth(r_key,1);
  gcry_sexp_t privSexp = gcry_sexp_nth(r_key,2);

  *privKey = makeStringFromSexp(privSexp);
  *pubKey = makeStringFromSexp(pubSexp);
}

int main()
{
  char *pubMsgKey, *privMsgKey, *pubRelayKey, *privRelayKey;
  keyGen(&privMsgKey,&pubMsgKey);
  keyGen(&privRelayKey,&pubRelayKey);
  cout << privMsgKey << endl;
  return 0;
}

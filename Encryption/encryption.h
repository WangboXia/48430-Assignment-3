#ifndef ENCRYPTION
#define ENCRYPTION

/*Inclusion*/
#include <stdio.h>  /* printf,scanf ,fgets,fread,fwrite,stdin*/
#include <string.h> /* strlen, strcpy,strncpy,strcmp*/
#include <stdlib.h> /* sizeof,NULL */

/* Function Prototypes */
void encryptFile();
void decryptFile();
unsigned enterCipher();
void XOR(FILE* inFileP, FILE* outFileP, char* key);
void subtitutionEncrypt(FILE* inFileP, FILE* outFileP);
void subtitutionDecrypt(FILE* inFileP, FILE* outFileP);

#endif

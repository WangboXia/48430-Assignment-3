#include "encryption.h"

/*encrypt the file*/
void encryptFile() {
    char inFileName[1024];
    char outFileName[1024];
    FILE *inFileP, *outFileP;
    
    /*Request the file name*/
    printf("Input the file name for encryption:\n");
    scanf("%s", inFileName);
    
    /*open the target file*/
    inFileP = fopen(inFileName, "r");
    if(inFileP == NULL) {
        printf("Error! That file does not exist!\n");   
        return;             
    }

    strcpy(outFileName, inFileName);
    
    /*get the cipher choice*/
    unsigned cipher = enterCipher();

    /*change the file suffix and do the encryption*/
    switch (cipher) {
        case 1:
            strcat(outFileName, ".sub");
            outFileP = fopen(outFileName, "w");
            subtitutionEncrypt(inFileP, outFileP);
            break;
        case 2:
            strcat(outFileName, ".xor");
            outFileP = fopen(outFileName, "w");
            char password[1024];
            printf("Set your password:\n");
            scanf("%s", password);
            XOR(inFileP, outFileP, password);
            break;
        default: break;
    }

    printf("Encrypting...\n");
    printf("Encryted as %s", outFileName);
    fclose(inFileP);
    fclose(outFileP);
    remove(inFileName);
}

/*get the cipher choice*/
unsigned enterCipher(){
    unsigned cipher;
    int res = 0;	
	while(res == 0)
	{
		printf("Choose the cipher: 1.Subtitution 2.XOR\n");
		res = scanf("%u", &cipher);
		/*check the type*/
		if(res==0)
		{
			printf("Invalid choice\n");
			setbuf(stdin, NULL);
		} 
		/*Check the range*/
		else if (cipher != 1 && cipher != 2)
		{
			printf("Invalid choice\n");
			res = 0;
		}
	}
    return cipher;
}

/*decrypt the file*/
void decryptFile() {
    char inFileName[1024];
    char outFileName[1024];
    FILE *inFileP, *outFileP;
    
    /*request the file name*/
    printf("Input the file name for decryption:\n");
    scanf("%s", inFileName);
    
    /*open the target file*/
    inFileP = fopen(inFileName, "r");
     if(inFileP == NULL) {
        printf("Error! That file does not exist!\n");   
        return;             
    }
    
    /*get the suffix of the encrypted file*/
    char suffix[5];
    strncpy(suffix, inFileName + (strlen(inFileName) - 4), 4);

    /*do the decryption according the suffix*/
    if (!strcmp(suffix, ".sub")){
        strncpy(outFileName, inFileName, strlen(inFileName)-strlen(".sub"));
        outFileP = fopen(outFileName, "w");
        subtitutionDecrypt(inFileP, outFileP);
    } 
    
    else if (!strcmp(suffix, ".xor")){
        strncpy(outFileName, inFileName, strlen(inFileName)-strlen(".xor"));
        outFileP = fopen(outFileName, "w");
        char password[1024];
        printf("Input your password:\n");
        scanf("%s", password);
        XOR(inFileP, outFileP, password);
    }

    else {
        printf("Not a crypted file");
        fclose(inFileP);
        return;
    }
    
    printf("Decrypting...\n");
    printf("Decrypted as %s", outFileName);
    fclose(inFileP);
    fclose(outFileP);
}

/*subtitution cipher encryption*/
void subtitutionEncrypt(FILE* inFileP, FILE* outFileP) {
    int c;
    int key = 33;/*the shift number*/
    
    /*shift the charachter in file within the printable charachter table*/
    while((c = fgetc(inFileP)) != EOF){
        if (c >= ' ' && c <= '~')
            c = (c - ' ' + key + 95) % 95 + ' ';
        fputc(c, outFileP); 
    }
}

/*subtitution cipher decryption*/
void subtitutionDecrypt(FILE* inFileP, FILE* outFileP) {
    int c;
    int key = 33;
  
    /*recover the charachter in file within the printable charachter table*/
    while((c = fgetc(inFileP)) != EOF){
        if (c >= ' ' && c <= '~')
            c = (c - ' ' - key + 95) % 95 + ' ';
        fputc(c, outFileP); 
    }     
}

/*
XOR cipher encryption/decryption
for the XOR cipher, encryption and decryption processes are the same
*/
void XOR(FILE* inFileP, FILE* outFileP, char* key) {
    int c, ckey;
    int keylen, index=0;

    keylen = strlen(key);
    
    /*do the xor operation of each charachter in file and charachter in key*/
    while((c = fgetc(inFileP)) != EOF){
        ckey = key[index%keylen];     
        c = c ^ ckey;   
        fputc(c, outFileP); 
        index ++; 
    }
}

/*Double Inclusion Guards*/
#ifndef COMPRESSION
#define COMPRESSION

/* Inclusions */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/io.h>

/* Definitions */
#define NUM_CHARS 257
#define SENTINEL 256
#define BIT_SIZE 8
#define MAX_FILE_NAME 1024

/* Structs for Huffman Tree */
typedef struct node huff_node;
int access(const char *filename, int amode);  
struct node {
    int c;
    size_t n;
    huff_node* left;
    huff_node* right;
};

/* Relevant Function Prototypes */
const char* END_PAD = "0000000";
void staticsAndClean(const char* inFile, const char* outFile);
void Compress(const char *inFileName); /* Core function used to compress a user-defined text file */
void Decompress(const char* fileName); /* Core function used to decompress a file with comp */
void heapInsert(huff_node** node_list, int n, huff_node * );
huff_node* popMin(huff_node** node_list, int  n);
huff_node* getRoot(int frequenciesArray[]);
void getBitsForChars( int frequenciesArray[]);
void computeBinaryRepr(huff_node* treeP, char *toFindP);
void writeHeader(FILE *outFile, int frequenciesArray[]);
void readHeader(FILE *inFile, int *);
void writeBit(const char *bitArrayP, FILE *outFileP);
int readBit(FILE *inFileP);
int recoveFromBitSequance(FILE *inFileP, huff_node *treeP);
void huffCompressFile(FILE *in, FILE *out);
void huffDecompressFile(FILE *in, FILE *out);
void clearTree(huff_node *treeP);
void clearTable(char *tableP[]);

#endif

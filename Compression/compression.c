/*Inclusions*/
#include "compression.h"

static int comp_bit = 0, comp_bitCount = 0;
static int de_bit = 0, de_bitCount = 0;
static char * binaryTable[NUM_CHARS];

/* Used to calculate the compression ratio */ 
/* Verify whether a text file is worth to be compressed or not */
void staticsAndClean(const char* inFile, const char* outFile){
    struct stat inStat;
    struct stat outStat;
    stat(inFile, &inStat);
    stat(outFile, &outStat);
    if(outStat.st_size >= inStat.st_size){
        remove(outFile);
        printf("Warning: Before compressed: %ld bytes after compressed: %ld bytes\n", inStat.st_size, outStat.st_size);
        printf("File is too small. It is not compressed!\n");
        return;
    }
    remove(inFile);
    double ratio;
    int sizeReduced;
    sizeReduced = inStat.st_size - outStat.st_size;
    ratio = (double)sizeReduced / (double)inStat.st_size;
    printf("\nCompression successful!\n");
    printf("%s is compressed from %ld bytes to %ld bytes! \n",
           inFile, inStat.st_size, outStat.st_size);
    printf("The compression ratio is about %.2f%%!\n",
           ratio * 100);
    printf("The compressed file can now be found at: %s\n\n", outFile);
}

/* Decompression Algorithm */
/* Verify whether a compressed file exists or not */
void Decompress(const char *inFile) {

    if(access(inFile, 0) != 0){
        printf("Error! The file %s does not exist!\n", inFile);
        return;
    }
    de_bit = 0;
    de_bitCount = 0;
    FILE *in, *out;
    in = fopen(inFile, "rb");
    if(in == NULL) {
        printf("Opening %s failed\n", inFile);
        return;
    }
    char outFile[MAX_FILE_NAME];

    printf("\nDecompressing...\n");
    strcpy(outFile, inFile);
    char* dot = strstr(outFile, ".");
    *dot = '\0';
    strcat(outFile, ".txt");
    out = fopen(outFile, "wb");
    huffDecompressFile(in, out);
    printf("Decompression succeeds!\nThe decompressed file can now be found at: %s\n\n", outFile);
    fclose(in);
    fclose(out);
    remove(inFile);
}

void swap(huff_node** node_list, int first, int second){
    huff_node* tmp = node_list[first];
    node_list[first] = node_list[second];
    node_list[second] = tmp;
}

void writeHeader(FILE *out, int occurens[]) {
    int index;
    int chars = 0;
    for (index = 0; index < NUM_CHARS; ++index) {
        if (occurens[index]) {
            ++chars;
        }
    }

    fprintf(out, "%d\n", chars);

    for (index = 0; index < NUM_CHARS; ++index) {
        if (occurens[index]) {
            fprintf(out, "%d %d\n", index, occurens[index]);
        }
    }
}

void heapInsert(huff_node ** node_list, int n, huff_node * new_node){
    node_list[n] = new_node;
    int p = (n-1) / 2;
    int  cur = n;
    huff_node* parent;
    while(p >= 0){
        parent = node_list[p];
        if(parent->n > new_node->n){
            swap(node_list, p, cur);
            cur = p;
            p = (p - 1) / 2;
        }
        else{
            break;
        };
    }
}

huff_node* popMin(huff_node** node_list, int  n){
    huff_node* top = node_list[0];
    node_list[0] = node_list[n-1];
    int  child = 1;
    int p = 0;
    while(child < n - 1){
        if(child + 1 < n - 1){
            if(node_list[child + 1]->n < node_list[child]->n){
                ++child;
            }
        }
        if(node_list[p]-> n > node_list[child]->n){
            swap(node_list, p, child);
            p = child;
            child = 2 * child + 1;
        }
        else{
            break;
        }
    }

    return top;
}

/* Huffman Compressin Algorithm */
void Compress(const char *inFile) {
    if(access(inFile, 0) != 0){
        printf("Error! The file %s does not exist!\n", inFile);
        return;
    }
    clearTable(binaryTable);

    comp_bit = 0;
    comp_bitCount = 0;
    FILE *in, *out;
    in = fopen(inFile, "rb");
    if(inFile == NULL) {
        printf("Opening %s failed!\n", inFile);
        return;
    }
    char outFile[MAX_FILE_NAME];
    /* long int inFileSize, outFileSize; */
    printf("\nCompressing...");
    strcpy(outFile, inFile);
    char* dot = strstr(outFile, ".");
    *dot = '\0';
    strcat(outFile, ".comp");
    out = fopen(outFile, "wb");
    huffCompressFile(in, out);
    fclose(in);
    fclose(out);
    staticsAndClean(inFile, outFile);
}

void getBitsForChars(int occurens[]) {
    char *toFindP = (char *)calloc(1, sizeof(char));
    huff_node* treeP = getRoot(occurens);
    computeBinaryRepr(treeP, toFindP);
    clearTree(treeP);
}

void computeBinaryRepr(huff_node* huff_tree, char *prefix) {
    if (huff_tree->left == NULL && huff_tree->right == NULL) {
        binaryTable[huff_tree->c] = prefix;
    }

    else {

        if(huff_tree->left != NULL) {
            char * newPrefix = (char *)malloc(strlen(prefix) + 2);
            sprintf(newPrefix, "%s%c", prefix, '0');
            computeBinaryRepr(huff_tree->left, newPrefix);
        }
        if(huff_tree->right != NULL) {
            char * newPrefix = (char *)malloc(strlen(prefix) + 2);
            sprintf(newPrefix, "%s%c", prefix, '1');
            computeBinaryRepr(huff_tree->right, newPrefix);
        }
        free(prefix);
    }
}

void clearTable(char *table[]) {
    int index;
    for (index = 0; index < NUM_CHARS; ++index) {
        if (table[index] != NULL) {
            table[index] = NULL;
        }
    }
}

huff_node* getRoot(int occurens[]) {
    int cha;
    int size = 0;
    huff_node* huff_tree[NUM_CHARS];

    for(cha = 0; cha < NUM_CHARS; cha++) {
        if(occurens[cha]) {
            huff_node* currentNodeP = (huff_node* )malloc(sizeof(huff_node));
            currentNodeP->c = cha;
            currentNodeP->n = occurens[cha];
            currentNodeP->left = NULL;
            currentNodeP->right = NULL;
            heapInsert(huff_tree, size, currentNodeP);
            ++size;
        }
    }

    while(size > 1) {
        huff_node* currentNodeP = (huff_node* )malloc(sizeof(huff_node));
        currentNodeP->left = popMin(huff_tree, size);
        --size;
        currentNodeP->right = popMin(huff_tree, size);
        --size;
        currentNodeP->n = currentNodeP->left->n + currentNodeP->right->n;
        heapInsert(huff_tree, size, currentNodeP);
        ++size;
    }

    return huff_tree[0];
}

void readHeader(FILE *in, int * occurens) {
    int index;
    int chars;
    int range;
    int occurs;

    if(fscanf(in, "%d", &chars) != 1) {
        printf("File format is not correct!");
    }

    for(index = 0; index < chars; ++index) {
        if((fscanf(in, "%d %d", &range, &occurs) != 2) || range >= NUM_CHARS || range < 0 ) {
            printf("File format incorrect.");
        }
        occurens[range] = occurs;
    }
    fgetc(in);
}

void writeBit(const char *chars, FILE *out) {
    while(*chars) {
        comp_bit = comp_bit * 2 + *chars - '0';
        comp_bitCount++;

        if(comp_bitCount == 8) {
            fputc(comp_bit, out);
            comp_bit = 0;
            comp_bitCount = 0;
        }
        ++chars;
    }
}

int readBit(FILE *in) {
    int bit;
    if (de_bitCount == 0) {
        de_bit = fgetc(in);
        de_bitCount = (1 << (BIT_SIZE - 1));
    }

    bit = de_bit/de_bitCount;
    de_bit %= de_bitCount;
    de_bitCount /= 2;

    return bit;
}

void huffDecompressFile(FILE *in, FILE *out) {

    int * occurens = (int*)calloc(NUM_CHARS, sizeof(int));
    int cha;
    huff_node* huff_tree;

    readHeader(in, occurens);
    huff_tree = getRoot(occurens);

    while(1){
        cha = recoveFromBitSequance(in, huff_tree);
        if(cha == SENTINEL){
            break;
        }
        fputc(cha, out);
    }

    clearTree(huff_tree);
    free(occurens);
}

int recoveFromBitSequance(FILE *inFileP, huff_node* treeP) {
    while (treeP->left || treeP->right) {
        if(!treeP) {
            printf("Error file format");
            exit(1);;
        }
        else if (readBit(inFileP)) {
            treeP = treeP->right;
        }
        else {
            treeP = treeP->left;
        }
    }
    return treeP->c;
}

void huffCompressFile(FILE *in, FILE *out) {
    int occurens[NUM_CHARS] = {0};
    int cha;
    /* char **tablePP; */
    while(1) {
        cha = fgetc(in);
        if(cha == EOF){
            break;
        }
        occurens[cha]++;
    }

    occurens[SENTINEL] = 1;
    rewind(in);
    getBitsForChars(occurens);
    writeHeader(out, occurens);

    while(1){
        cha = fgetc(in);
        if(cha == EOF){
            break;
        }
        writeBit(binaryTable[cha], out);
    }
    writeBit(binaryTable[SENTINEL], out);
    writeBit(END_PAD, out);
    clearTable(binaryTable);
}

void clearTree(huff_node* huff_tree) {
    if (huff_tree != NULL) {
        clearTree(huff_tree->left);
        clearTree(huff_tree->right);
        free (huff_tree);
    }
}

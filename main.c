/* Lab 07 Group 06 */
/* 
    13100072 Wangbo Xia
    13078985 Siyu Fang
    13317540 Bo Peng
    13485952 Zhengxin Ma
    13482397 Nuojin Zhang
*/
/*******************************************************************************
 * List header files - do NOT use any other header files. Note that stdlib.h is
 * included in case you want to use any of the functions in there. However the
 * task can be achieved with stdio.h and string.h only.
*******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Compression/compression.c"
#include "Encryption/encryption.c"

/*******************************************************************************
 * List preprocessing directives - you may define your own.
*******************************************************************************/
#define MAX_TRAINNUM_LEN 6
#define MAX_SUBCODE_LEN 99
#define MAX_NUM_TRAINS 5
#define DB_NAME "database.txt"
/*#define LINE_WIDTH 103*/

/*******************************************************************************
 * List structs - you may define struct date_time and struct train only. Each
 * struct definition should have only the fields mentioned in the assignment
 * description.
*******************************************************************************/
typedef struct {
    int month;
    int day;
    int hour;
    int minute;
} date_time_t;

typedef struct {
    char traincode[MAX_TRAINNUM_LEN + 1];
    date_time_t departure_date;
    char departure_suburb[MAX_SUBCODE_LEN + 1];
    char arrival_suburb[MAX_SUBCODE_LEN + 1];
    date_time_t arrival_date;
} train_t;

/*******************************************************************************
 * Function prototypes - do NOT change the given prototypes. However you may
 * define your own functions if required.
*******************************************************************************/
void print_menu ();
void sorting();
int cmpByTraincode(const void *train1,const void *train2);
void addTrain();
void displayAllTrains();
void saveAllTrains();
void loadDatabase();
void compressOnly(); /* Compression */
void decompressOnly(); /* Decompression */

train_t trains[MAX_NUM_TRAINS];
int num = 0;

/*******************************************************************************
 * Main
*******************************************************************************/
int main(void)
{
    /*initialise the local with the database*/
    loadDatabase();
    
    /*showing the menu and waiting for a request until the user ask for quit*/
    int choice;
    while (1) {
        print_menu();
        printf("Enter your choice>\n");
	    if (scanf("%d", &choice) == 1){
            switch(choice)
            {
                case 1: addTrain();
                break;
                case 2: displayAllTrains();
                break;
                case 3: saveAllTrains();
                break;
                case 4: sorting();
                case 5:loadDatabase();
                break;
                case 6: compressOnly();
                break;
                case 7: decompressOnly();
                break;
                case 8: encryptFile();
                break;
                case 9: decryptFile();
                break;
                case 10: exit(0);
                default: printf("Invalid choice\n");
                }
            }
        else
	    {
            printf("Invalid choice.\n"); 
            /*clear the buff*/
		    setbuf(stdin, NULL);
	    }
    }
    return 0;
}


/*******************************************************************************
 * This function prints the initial menu with all instructions on how to use
 * this program.
 * inputs:
 * - none
 * outputs:
 * - none
*******************************************************************************/
void print_menu (void)
{
    printf("\n"
    "1. Add a train infomation\n"
    "2. Display all trains to a destination\n"
    "3. Store the train information into the database file\n"
    "4. Sort train number in ascending order\n"
    "5. Load the train information from the database\n"
    "6. Compress the database file\n"
    "7. Decompress the database file\n"
    "8. Encrypt the target file\n"
    "9. Decrypt the target file\n"
    "10. Exit the program\n"
    "Enter choice (number between 1-10)>\n");
}

/*******************************************************************************
 * This function add a train to the list
*******************************************************************************/
void addTrain()
{
	if (num == MAX_NUM_TRAINS) {
        printf("Cannot add more trains - memory full\n");
        return;
    }
    /* read the '\n' after choice */
    getchar();
    /* deal with train code */
    while (1) {
        printf("Enter train number(for example G0000)>\n");
        char code[1000];
        fgets(code, 999, stdin);
        int len;
        len = strlen(code);
        if (code[len - 1] == '\n') {
            len--;
            code[len] = '\0';
        }
        int format_flag = 1; /* 1 : the format is correct */
        /* too long or too short */
        if (len < 3 || len > 6) {
            format_flag = 0; /* 0 : the format is incorrect */
        }
        else {
        	/* copy the code to train */
            strcpy(trains[num].traincode, code);
        }
        /* [A-Z], [0-9] */
        const char* left = "G0000";
        const char* right = "G9999";
        const char* str = trains[num].traincode;
        int i;
        for (i = 0; format_flag && i < len; i++) {
            if (!(str[i] >= left[i] && str[i] <= right[i])) {
                format_flag = 0; /* 0 : the format is incorrect */
            }
        }
        if (format_flag) break;
        printf("Invalid input\n");
    }

    printf("Enter Departure Station>\n");
    scanf("%12s", trains[num].departure_suburb);
	while(getchar() != '\n');

    printf("Enter departure info for the train leaving %-3s.\n",trains[num].departure_suburb);
    /* deal with date */
    while (1) {
        printf("Enter month, date, hour and minute separated by spaces>(must be 4 values)\n");
        int date_num[4]; /* month, day, hour, minute */
        /* [1-12], [1-31], [0-23], [0-59] */
        const int left[4] = {1, 1, 0, 0};
        const int right[4] = {12, 31, 23, 59};
        int i;
        for (i = 0; i < 4; i++) {
            scanf("%d", date_num + i);
        }
        int format_flag = 1;
        for (i = 0; format_flag && i < 4; i++) {
            if (!(date_num[i] >= left[i] && date_num[i] <= right[i])) {
                format_flag = 0;
            }
        }
        if (format_flag) {
            trains[num].departure_date.month = date_num[0];
            trains[num].departure_date.day = date_num[1];
            trains[num].departure_date.hour = date_num[2];
            trains[num].departure_date.minute = date_num[3];
            break;
        }
        printf("Invalid input\n");
    }

    printf("Enter arrival station>\n");
    scanf("%12s", trains[num].arrival_suburb);
    while(getchar() != '\n');

    printf("Enter arrival info.\n");
    /* deal with date */
    while (1) {
        printf("Enter month, date, hour and minute separated by spaces(must be 4 values)>\n");
        int date_num[4];
        const int left[4] = {1, 1, 0, 0};
        const int right[4] = {12, 31, 23, 59};
        int i;
        for (i = 0; i < 4; i++) {
            scanf("%d", date_num + i);
        }
        int format_flag = 1;
        for (i = 0; format_flag && i < 4; i++) {
            if (!(date_num[i] >= left[i] && date_num[i] <= right[i])) {
                format_flag = 0;
            }
        }
        if (format_flag) {
            trains[num].arrival_date.month = date_num[0];
            trains[num].arrival_date.day = date_num[1];
            trains[num].arrival_date.hour = date_num[2];
            trains[num].arrival_date.minute = date_num[3];
            break;
        }
        printf("Invalid input\n");
    }
    num += 1;
}

/*******************************************************************************
 * This function display trains
*******************************************************************************/
void displayAllTrains()
{
    char code[MAX_SUBCODE_LEN + 1];
    printf("Enter arrival station or enter * to show all destinations>\n");
    scanf("%s", code);
    int len = strlen(code);
    int i;
    for (i = 0; i < num; i++) {
    	/* show all trains or the suburb is same */
        if ((len == 1 && code[0] == '*') || 
            (strcmp(code, trains[i].arrival_suburb) == 0)) {
        	/* show title */
            if (i == 0) {
                printf("Train  Departure City  Departure Time  Arrival City  Arrival Time\n");
                printf("------ --------------  --------------  ------------  ------------\n");
            }
            printf("%-6s %-14s  %02d-%02d-%02d:%02d     %-12s  %02d-%02d-%02d:%02d\n",
                trains[i].traincode, trains[i].departure_suburb, trains[i].departure_date.month, trains[i].departure_date.day, 
                trains[i].departure_date.hour, trains[i].departure_date.minute, trains[i].arrival_suburb, 
                trains[i].arrival_date.month, trains[i].arrival_date.day, 
                trains[i].arrival_date.hour, trains[i].arrival_date.minute);
        }
    }
    if (i == 0) printf("No trains\n");
}

/*******************************************************************************
 * This function save trains to database
*******************************************************************************/
void saveAllTrains()
{
	/*Open a file named "database" for writing*/
	FILE *fp = NULL;
	fp = fopen(DB_NAME, "w");

	/*Check if no train*/
	if (num == 0)
	{
		/*input this information to the file*/
		fprintf(fp, "No train.");
		fclose(fp);
		return;
	}

    fprintf(fp,"The project is designed and developed for the subject called 48430.\n");
    fprintf(fp,"The project can be used for storing a specific train information.  \n");
    fprintf(fp,"Users can also retrieve the information of a specific train from the database.txt file.\n");
    fprintf(fp,"The project is able to compress and decompress the database.txt file. \n");
    fprintf(fp,"The project is able to encrypt and decrypt the database.txt file as well. \n");
    fprintf(fp,"The developers for the Train Mangement Software listed below: \n");
    fprintf(fp,"Wangbo Xia 13100072; Zhengxin Ma 1348952; Siyu Fang 13078985; Bo Peng 13317540; Nuojin Zhang 13482397; \n");
    fprintf(fp,"Train  Departure City  Departure Time  Arrival City  Arrival Time\n");
    fprintf(fp,"------ --------------  --------------  ------------  ------------\n");

	/*Input the information of each train*/
	int i=0;
	for(;i<num;i++)
	            fprintf(fp,"%-6s %-14s  %02d-%02d-%02d:%02d     %-12s  %02d-%02d-%02d:%02d\n",
                trains[i].traincode, trains[i].departure_suburb,trains[i].departure_date.month, trains[i].departure_date.day, 
                trains[i].departure_date.hour, trains[i].departure_date.minute, trains[i].arrival_suburb, 
                trains[i].arrival_date.month, trains[i].arrival_date.day, 
                trains[i].arrival_date.hour, trains[i].arrival_date.minute);
	/*Close the file*/
	fclose(fp);
}

/*******************************************************************************
 * This function sort trains from database
*******************************************************************************/

int cmpByTraincode(const void *train1,const void *train2){
    train_t t1 = *(train_t*)train1;
    train_t t2 = *(train_t*)train2;
    return strcmp(t1.traincode,t2.traincode);
}
void sorting()
{
    int i;
    qsort(trains,num,sizeof(trains[0]),cmpByTraincode);    
  
    for (i = 0; i < num; i++) {
        
            printf("%-6s %-14s  %02d-%02d-%02d:%02d     %-12s  %02d-%02d-%02d:%02d\n",
                trains[i].traincode, trains[i].departure_suburb, trains[i].departure_date.month, trains[i].departure_date.day, 
                trains[i].departure_date.hour, trains[i].departure_date.minute, trains[i].arrival_suburb, 
                trains[i].arrival_date.month, trains[i].arrival_date.day, 
                trains[i].arrival_date.hour, trains[i].arrival_date.minute);
    }
}

/*******************************************************************************
 * This function load the train information from the existing database
*******************************************************************************/
void loadDatabase(void)
{
	/*A buffle to store the line*/
	char line[128];
	
	/*Open the file named "database" for reading*/
	FILE *fp = NULL;
	if((fp = fopen(DB_NAME, "r"))!= NULL)
	{
	    /*Get the first line*/
	    fgets(line, 128, fp);
	    /*If file ends, it means no trains*/
	    if (feof(fp))
		{
			fclose(fp);
			return;
		}

	    /*Skip the 2-9 line*/
	    fgets(line, 128, fp);
        fgets(line, 128, fp);
        fgets(line, 128, fp);
        fgets(line, 128, fp);
        fgets(line, 128, fp);
        fgets(line, 128, fp);
        fgets(line, 128, fp);
        fgets(line, 128, fp);
	
	    /*Read each line and transform the formatted string to expected values */
	    int i = 0;
        while(!feof(fp))
	    {
		    fscanf(fp, "%s", trains[i].traincode);
            fscanf(fp, "%s", trains[i].departure_suburb);
		    fscanf(fp, "%d-%d-%d:%d", 
			    &trains[i].departure_date.month,
                &trains[i].departure_date.day,
                &trains[i].departure_date.hour,
                &trains[i].departure_date.minute);
        
            fscanf(fp, "%s", trains[i].arrival_suburb);
		    fscanf(fp, "%d-%d-%d:%d", 
			    &trains[i].arrival_date.month,
                &trains[i].arrival_date.day,
                &trains[i].arrival_date.hour,
                &trains[i].arrival_date.minute);
            fgets(line, 1, fp); /*Skip "\n"*/
            i++;
        }

        /*assign the number of trains in the database*/
        num = i-1;
	    /*Close file*/
	    fclose(fp); 
    }
}

/*******************************************************************************
 * This function used for compression
*******************************************************************************/
void compressOnly() {
	char inFileName[1024]; /* text filename */
    printf("Enter the filename you wish to compress?\n");
    scanf("%s", inFileName);
	Compress(inFileName); /* Huffman Compression Algorithm */
}
/*******************************************************************************
 * This function used for decompression
*******************************************************************************/
void decompressOnly() {
	char inFileName[1024]; /* The file compressed before */
    printf("Enter the filename you wish to decompress?\n");
    scanf("%s", inFileName);
	Decompress(inFileName); /* Decompression Algorithm */
}



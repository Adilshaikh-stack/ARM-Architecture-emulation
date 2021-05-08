/*
 * File: loader_dumper.c
 * Author: M.Adil Shaikh
 * School: Dalhousie University
 * Dept : Electrical and Computer engineering
 * Course : ECED3403 Computer Architecture
 
 * Contains:    loader_dump.c contains two fucntions:
 *               1)void load(char *filename) calls following;
 *                       |---->check_Validity_Srecord
 *                       |        |--->checkSUM 
 *                       |        |---->check_HEADER
 *                       |---->bus 
 *                       |---->hex_to_ascii 
 *                              |---->hex_to_in
 *               2)void dump(char start_addr[], char endaddr[]);
 *                           |--->bus  
 * 
 * symbol |---> (calls)      
 *------------------------------------------------------------------------------------
 * Note: Ive tried to use byte as unsigned int in load function when using the sscanf,it 
 * takes hex value from the dataByte but it doesnt work iam not sure why is it happenning
 * it is only working if iam using char byte[] of size of 2.
 *			
 * Last Modified: 2020.08.01
 *
 *
 */

#include "A2.h"
#include "functions.h"
#include "instruction.h"
unsigned int dataBytes = 0;
/*
 * Function: load
 * ---------------------
 * filename: takes the filename.xme to be loaded
 * 
 * ---------------------
 * Reading Valid S-records from the filename.xme
 * and separating data bytes and loading into the 
 * bus fucntion from start addrees of the same record
 * till the there is no more data to load.
 * 
 */
int start_address = 0;
void load(char filename[])
{
    /*declaration of variables nneddded in load fucntion*/

    char record[record_Size];
    unsigned int address;
    /*j,x are incrementers used in various loops*/
    int j = 0;
    int x = 0;
    char data[data_SIZE];
    unsigned int byte = 0;

    /*filename.xme that was passed through load fucntions*/
    FILE *xmEfile;
    xmEfile = fopen(filename, "r");

    if (xmEfile == NULL)
    {
        perror("Error opening file!");
        exit(0);
    }
    /*reading Srecords from filename.xme line by line*/
    while (fgets(record, record_Size, xmEfile) != NULL)
    {
        /*reinitializing for next record*/
        address = 0;
        sscanf(record + 4, "%04x", &address);
        /*calling the check_Validity_Srecord which 
        * check for validity for Srecords and makes terminate = T 
        * which eventually stops the emulator or debugger
        */
        check_Validity_Srecord(record);

        /* printing the sourcefilename from S0 record
         * if it exists
         */
        if ((strcmp(header, "S0") == 0))
        {
            unsigned int sourceFile;
            printf("Source filename: ");
            for (j = 0; j < CountByte_Int; j += 2)

            { /*+8 to start copying from first data byte*/
                sscanf(record + 8 + j, "%02x", &sourceFile);
                printf("%c", sourceFile);
            }
            printf(" ");
        }

        if (strcmp(header, "S9") == 0)
        { /* getting the starting address from S9 record*/
            sscanf(record + 4, "%04X", &start_address);
            RG[R7].word = start_address;
        }

        /* storing the databytes of S1 record in data array*/
        if (strcmp(header, "S1") == 0)
        {
            /* take example S1record 
             S10F100030285028292848283F285F2861 
             data[dataCount] = 30285028292848283F285F28
             */
            strncpy(data, record + 8, CountByte_Int);
            // '/0' to determine end of string
            data[CountByte_Int] = '\0';

            for (x = 0; x < CountByte_Int; x = x + 2)
            {
                /* incrementing address after one char is stored in that address
                 * of memory array in bus function
                 *
                 * from previous example os S1 record
                 * byte = 30 for first iteration and so on
                 */
                sscanf(&data[x], "%02X", &byte);
                /* bus fucntion call to write the bytes to memory
                 * from start address till the end address
                 */
                bus(address, &byte, WRITE, BYTE);
                address++;
            }

            x = 0; //reinitialzing
        }

        /*If S9-record is present write the starting address*/
        if (strcmp(header, "S9") == 0)
        {
            printf("\nFile read - no error detected. ");
            printf("Starting address: %04X\n", start_address);
        }
    }
    fclose(xmEfile);
}
/*
 * Function: dump
 * ---------------------
 * start_addr:  takes the start address
 * endaddr: takes the end address.
 *---------------------
 * Takes start and end address and displays the 
 * data bytes from location start to end address
 * through bus fucntion where dataBytes is taken as lvalue
 * in bus fucntion (meaning pointing to that data in that 
 * memory location).
 * 
 */
void dump(char start_addr[], char endaddr[])
{
    int start_addrINT, endaddrINT;
    int counterLoop = 0;
    int factor = 0;
    int k = 0;
    int n = 0;
    int current_addr = 0;
    display_WORD = False; //SIZE is

    /*storing the start and end address as integer*/
    /*example 1000 in hex == 4069 int*/
    sscanf(start_addr, "%4X", &start_addrINT);
    sscanf(endaddr, "%4X", &endaddrINT);

    /*check for if starting address is less then end address*/
    if (endaddrINT > start_addrINT)
    {

        /*factor is used to give the number to dispaly the right
         * 16 locations.
         */
        factor = (endaddrINT - start_addrINT) / 16;
        /* running  do while to print the data inside the locations
         * from start to end address as hex values
         */
        current_addr = start_addrINT;
        do
        {
            printf("\n%4X: ", current_addr);
            /*printing dta bytes in hex, adding 16 to print */
            for (k = current_addr; k < (current_addr + 16); k++)
            {
                bus((current_addr + n), &dataBytes, READ, BYTE);
                if (display_WORD == True) //displays word
                {
                    printf(" %02X ", dataBytes);
                    n += 2;
                } //display_WORD = False means display BYTE
                else
                {
                    printf(" %02X ", dataBytes);
                    n++;
                }
            }
            n = 0;
            printf(" ");
            /*printing the databytes as ASCII chararcters*/
            for (k = current_addr; k < (current_addr + 16); k++)
            {
                bus((current_addr + n), &dataBytes, READ, BYTE);
                /*since ascii char doesnt exits below 36 and greater then 126*/
                if ((dataBytes < LOwASCII_Not_exist) || (dataBytes > HighASCII_Not_exist))
                    printf(".");
                else
                    printf("%c", dataBytes);
                n++;
            }
            current_addr = k;
            counterLoop++;
            n = 0;
        } while (counterLoop < factor);
        //reinitializing the counterLoop
        printf("\n");
        k = 0;
        counterLoop = 0;
        factor = 0;
    }
    else
    {
        printf("end address should be greater then start address!");
        terminate = False;
        exit(-1);
    }
}
/* File: A2.c
 * Notes:   A2.c contains following fucntions definitions:
 *          1) void siginthandler(int param)
 *          2) void displayRegisters()
 *          3) unsigned char checkSUM(char recordByte[]);
 *          4) void emulator();
 *          5) void bus(unsigned short mar, unsigned short *mbr, ACTION rw, SIZE bw);
 *------------------------------------------------------------------------------------
 *
 *			
 * Last Modified: 2020.08.01
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include "A2.h"
#include "bus.h"
#include "functions.h"
#include "instruction.h"

/*
 * Function: siginthandler
 * ------------------------------------
 * purpose:Stop and return the control 
 * to the program when Ctrl-C is entered during
 * the instruction cycle
 * 
 */
void siginthandler(int param)
{
    ctrl_c_fnd = True;
    printf("User pressed Ctrl+C\n");
    //return control to the program
}
int CountByte_Int = 0;

/*
 * Function: displayRegisters
 * -------------------------------------------------
 * purpose: print Registers from R0-R7
 * 
 */

void displayRegisters()
{
    for (int i = 0; i < RegSIZE; i++)
    {
        printf("R%d: %04X\n", i, RG[i].word);
    }
}
/*
 * Function: checkSUM
 * -------------------------------------------------
 * recordByte: character sring bytes from the records
 * -------------------------------------------------
 * Calculates the check sum of the records and returns the sum 
 * 
 */

unsigned char checkSUM(char recordByte[record_Size], int CountcheckSum)
{

    unsigned char sum = 0;
    int byte;

    int i;

    for (i = 0; i < CountcheckSum; i = i + 2)
    {
        sscanf(recordByte + i, "%2X", &byte);
        sum = sum + (byte & 0xFF);
    }
    return sum;
}

int check_HEADER()
{
    if (header[0] != 'S')
    {
        printf("Invald header %s !\n", header);
    }
    return -1;
}

/*
 * Function: check_Validity_Srecord
 * ---------------------
 * filename: takes the char string of record
 * 
 * ---------------------
 * It is called in Load(filename) and is to check the validity of
 * S record three checks are being perfomed:
 * 1) if the record starts with S char
 * 2) if the checksum of Srecord is correct or not
 * 3) if the Countbyte shows the accurate size of the srecord 
 * terminate  = T stops the while loop where the record is being read. 
 */

void check_Validity_Srecord(char record[record_Size])
{
    unsigned char sum;
    char checkSUMrecord[record_Size];

    int off = 8; //(offset)

    /*reading hex value and storing as integer 
     * to CountByte_Int
     */
    sscanf(record + 2, "%2X", &CountByte_Int);
    CountByte_Int = (2 * CountByte_Int) + 2;

    strncpy(checkSUMrecord, record + 2, (CountByte_Int));
    checkSUMrecord[CountByte_Int] = '\0';
    sum = checkSUM(checkSUMrecord, CountByte_Int);
    CountByte_Int = CountByte_Int - off;
    strncpy(header, record, 2);
    header[2] = '\0';
    check_HEADER();
    /*Checking if header first character is S or not*/

    /*if the length of srecord is not as indicated by count byte*/
    /*if (CountByte_Int != (strlen(record) - (2)))
    {
        printf("\nInvalid size of %s record!\n", header);
        printf("It should be %d as indicated by countbyte\n", CountByte_Int);
    }*/

    /*if checksum of S1 or S9 records are not equal to 0xff(maxbyte)
    *Dat is corrupted
    *checksum byte of S0 record doesnt really matter beacuse it only 
     provides source filename */
    if (strcmp(header, "S1") == 0 || strcmp(header, "S9") == 0)
    {
        if (sum != 0xff)
        {
            printf("\nChecksum is %x (invalid) of %s record: Data corrupted\n", sum, header);
        }
    }
}
/*
 * Function: bus
 * ---------------------
 * mar : takes mmeory address as unsigned short
 * *mbr : pointing towards the address, treated as lvalue
 *        when reading and rvalue when writing the data.
 * rw :  type enum containing READ and WRITE option
 * bw :  type enum containing BYTE and WORD address accesable.
 *---------------------
 * Taking the address where the dataBytes (*mbr) are being stored 
 * incase of when the action is WRITE.
 * Taking the address where the (dataBytes (*mbr) as lvalue) are 
 * already stored incase of when the it the action is READ.
 * It is both BYTE  and WORD addressable which can be chose
 * by using SIZE as BYTE or WORD.
 */

void bus(unsigned short mar, unsigned short *mbr, enum ACTION rw, enum SIZE bw)
{
    if (rw == READ)
    {
        if (bw == WORD)
        {
            display_WORD = True;
            *mbr = (((mem[(mar + 1)]) << 8) | (mem[mar]));
        }
        else
        { //bw = Byte
            *mbr = mem[mar];
        }
    }
    else
    {
        if (bw == WORD)
        {
            mem[(mar & 0xFFFE)] = *mbr & 0xFF;
            mem[(mar & 0xFFFE) + 1] = (*mbr >> 8) & 0xFF;
        }
        else
        {
            mem[mar] = *mbr;
        }
    }

    Clkcycle += 3; //memory access Clkcycle increases by 3
}

/*
 * Function: debugger
 * ---------------------
 * filedebug: name of the filename.dbg 
 *-------------------
 * Incase of reading the options l,d x from 
 * filename.dbg we used debugger fucntion
 * which calls the load, dump function depending
 * on the option in the file.
 * 
 */
void debugger(char *filedebug)
{
    char line[LineSize];
    char *token = NULL;
    char *fileXme = NULL;
    char *start_add = NULL;
    char *end_add = NULL;
    char *breakpoint = NULL;
    unsigned int breakpoint_int = 0;
    terminate = False;
    FILE *file = fopen(filedebug, "r");

    while ((fgets(line, LineSize, file) != NULL) && (terminate != True))
    {

        token = strtok(line, " \t\n");

        if (strcasecmp(token, "l") == 0)
        {
            fileXme = strtok(NULL, "\n");
            printf("%s\n", fileXme);
            load(fileXme);
        }
        else if (strcasecmp(token, "d") == 0)
        {
            /*taking the next token being start address*/
            start_add = strtok(NULL, " \t");
            /*takingng the next token being end address*/
            end_add = strtok(NULL, " \t");
            dump(start_add, end_add);
        }
        else if (strcasecmp(token, "r") == 0)
        {
            displayRegisters();
        }
        else if (strcasecmp(token, "b") == 0)
        {
            breakpoint = strtok(NULL, " \t");
            sscanf(breakpoint, "%04X", &breakpoint_int);
            printf("Current breakpoint address: %04X\n", breakpoint_int);
        }
        else if (strcasecmp(token, "g") == 0)
        {
            instruction_Cyle(breakpoint_int);
        }
        else if (strcasecmp(token, "x") == 0)
        {
            terminate = True;
        }
        else
        {
            printf("Invalid option : %s\n", token);
            terminate = True;
        }

        token = strtok(NULL, " \t");
    }

    fclose(file);
}

/*
 * Function: emulator
 * ---------------------------
 * Takes no arguments
 *----------------------------
 * Emulator function only takes place if there is one 
 * commandline argument and thus takes user 
 * input from keyboard and execute the fucntions,
 * load an ddump depending on the user input.
 * 
 */
void emulator()
{

    char filename[file_Size];
    char Option;
    terminate = False;
    char start_addr[addrSize];
    char endaddr[addrSize];
    int brkpt;

    while (terminate == False)
    {
        printf("Option: ");
        scanf(" %c", &Option);

        if (Option == 'l' || Option == 'L')
        {
            printf("Enter .XME file to load\n");
            scanf("%s", filename);
            load(filename);
        }
        else if (Option == 'd' || Option == 'D')
        {
            printf("Enter lower and upper bounds\n");
            scanf("%s %s", start_addr, endaddr);
            dump(start_addr, endaddr);
        }
        else if (Option == 'g' || Option == 'G')
        {
            printf("brkpoint: %04x\n", brkpt);
            instruction_Cyle(brkpt);
        }
        else if (Option == 'b' || Option == 'B')
        {
            scanf("%4X", &brkpt);
            printf("Current breakpoint address: %04X\n", brkpt);
        }
        else if (Option == 'r' || Option == 'R')
        {
            displayRegisters();
        }
        else if (Option == 'X' || Option == 'x')
        {
            terminate = True;
            exit(0);
        }
    }
}
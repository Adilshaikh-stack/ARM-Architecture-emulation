/* File: A2.h
 * Notes:  header file containning global variables and fucntions declarations.
 *------------------------------------------------------------------------------------
 *
 *			
 * Last Modified: 2020.08.01
 */

#ifndef A2
#define A2
#include <stdint.h>
/*Declaring global values used by A2.c and loade_dump.c*/
#define file_Size 100
#define record_Size 250
#define HexSize 2
#define countSize 2
#define headerSize 3
#define addrSize 4
#define MAXMEM 65535 /* 2^16 bytes*/
#define LineSize 20
#define data_SIZE 200
#define Offst 12
#define LOwASCII_Not_exist 36
#define HighASCII_Not_exist 126

char header[headerSize];
extern unsigned int dataBytes;
extern int CountByte_Int;
extern int start_address;

/* Definitions of functions are defined in A2.c
 * and loader_dump.c
 */
void debugger(char *filedebug);
void emulator();
void load(char filename[]);
void dump(char start_addr[], char endaddr[]);
void check_Validity_Srecord(char record[record_Size]);
void displayRegisters();
int check_HEADER();

unsigned char mem[MAXMEM];

/*
 * Function: typedef enum of ACTION
 * ---------------------
 * boolean datatype containning two constants:
 * READ & WRITE serving the purpose of boolean function.
 * It is used by bus fucntion.
 */
enum ACTION
{
    READ,
    WRITE
};
/*
 * Function: typedef enum of SIZE
 * ---------------------
 * boolean datatype containning two constants:
 * BYTE & WORD serving the purpose of boolean function.
 * It is used by bus fucntion.
 */
enum SIZE
{
    BYTE,
    WORD
};

/*
 * Function: typedef enum
 * ---------------------
 * There is no boolean function in C therefore I made
 * boolean datatype containning two constants:
 * F,T serving the purpose of boolean function.
 */
typedef enum
{
    False,
    True
} boolean;
boolean terminate;
boolean display_WORD;
boolean Invalid_ins;
boolean ctrl_c_fnd;

#endif
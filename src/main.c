/* File: main.c 
 * Author: M.Adil Shaikh
 * School: Dalhousie University
 * Dept : Electrical and Computer engineering
 * Course : ECED3403 Computer Architecture
 * 
 * Purpose: This emulator reads filename.xme containing valid Srecords,
 *          and then WRITES the data bytes from the starting addresses of that 
 *          record till the end of the data bytes in that record into the memroy
 *          array of 64Kib thtough bus fucntion. It reads the data as word or byte but
 *          WORD or BYTE has to be changed inside the bus function manually.Three new 
 *          options are introduced being “g”, “r” and “b”. We will fetch the instruction
 *          as word from databytes we extracted from S1 record and decode it into its opcodes 
 *          and operands and depending on the opcode we get we will execute that instruction.
 *          During this instruction cycle, Program status Word (PSW) is changing its sate and the
 *          Clock cycle is also incrementing.
 *     
 *          This program works in two ways: 
 *          if there is one argument in command line then:
 *          $ ./A3 it will call emulator: The functions calling works as below
 *           depending on the option entered or read from filename.dbg:
 *          * emulator--->load---->check_Validity_Srecord
 *          *       |       |        |--->checkSUM 
 *          *       |       |           |--->check_Header
 *          *       |       |---->bus 
 *          *       |       
 *          *       |----->instruction_Cycle--->fucntion calls to various instructions                
 *          *       |
 *          *       |---->dump--->bus 
 *         if there is three arguments in commandline then:
 *         $ ./A3 -d filename.dbg. This will call debugger not emulator and the rest of
 *         the calling structure remains same as in case of one argument.
 *         
 *        if there are more then 3 or wrong args passed then it will show invalid option
 *          
 *  ----------------------------------------------------------------------------------------------
 * Notes:  1) The .h files are main.c, A2.h,instruction.h and functions.h
 *         2) The .c files are main.c,functions.c, instructionCycle.c, loader_dump.c.and A2.c file.
 *         3) CEX instruction cannot be implemented and some parts of program could not be optimized
 *            due to non availability of time.
 *------------------------------------------------------------------------------------------------
 * (calls: --->) symbol
 * Project A3: files included are PolledKB.xme , SRA.xme TEST2.dbg, LDRSTR-A3.list(to compare results) 
 *             LDRSTRA3.xme TEST3.dbg and Strings.xme and testfile.doc.
 *             Other sample files are also included provided by professor.
 * 
 *			
 * Last Modified: 2020.08.01
 *
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <A2.h>

int main(int argc, char *argv[])
{
    /*
     - Sample mainline checking for:
     - Argc = 1 - kb input
     - Argc = 3 - file input
     - Argv[1] = "-d" or error
     - Argv[2] (filename) - exists or error
     - 
     */
    if (argc == 1)
        /* Default - kb input */
        emulator();
    else if (argc == 3)
    {
        /* Possibly "-d" and filename */
        if (strcmp(argv[1], "-d") == 0)
        {
            /*passes the filename.dbg to the debugger*/
            debugger(argv[2]);
        }
        else
            printf("Invalid option: %s\n", argv[1]);
    }
    else
        printf("Invalid number of arguments\n");

    return 0;
}
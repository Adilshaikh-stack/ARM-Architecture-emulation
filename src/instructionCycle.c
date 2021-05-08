/*
 * File: instructionCycle.c
 * Author: M.Adil Shaikh
 * School: Dalhousie University
 * Dept : Electrical and Computer engineering
 * Course : ECED3403 Computer Architecture
 
 * Purpose: Fetch and decodes the instruction as word from mem array until 
 * PC(Program counter) being R7 register reaches the breakpoint address, 
 * and then compares the opcodes and executes the instructions from different 
 * fucntions tables and hence PSW and program counter (R7 is changed). The
 * instruction is also stopped when ctrl-c is encountered.
 * ------------------------------------------------------------------------------------
 *			
 * Last Modified: 2020.08.01
 *
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>

#include "functions.h"
#include "instruction.h"
#include "A2.h"
#include "bus.h"

/*Functions table for arithematic instructions*/
void (*Function_Arith_table[SIZE1])(uint8_t SRC,
                                    uint8_t DES,
                                    uint8_t bw,
                                    uint8_t rc) = {ADDC, SUB, SUBC, CMP,
                                                   XOR, AND, BIT, BIC, BIS};

int table_OF_ARith[SIZE1] = {0x41, 0x42, 0x43, 0x45, 0x46, 0x47, 0x48,
                             0x49, 0x4A};
/*Functions table for swpb & sxt instructions*/
void (*Function_SWPB_SXT[SIZE2])() = {SWPB, SXT};
int table_OF_SWPB_SXT[SIZE2] = {0x4E0, 0x4E1};

/*Functions table for MOVL instructions*/
void (*Function_MOVL[SIZE3])() = {MOVL, MOVLZ, MOVLS, MOVH};
int table_OF_MOVL[SIZE3] = {0xC, 0xD, 0xE, 0xF};

/*Functions table for relative memory LDR,STR*/
void (*Function_RelMem[SIZE2])() = {LDR, STR};
int table_OF_RelMem[SIZE2] = {0x2, 0x3};

/*Functions table for MOV,SRA,RRC*/
void (*Function_table2[SIZE3])() = {MOV, SRA, RRC};
int table_2[SIZE3] = {0x98, 0x9A, 0x9B};

/*Functions table for BRA &CEX*/
void (*Function_table3[SIZE2])() = {BRA, CEX};
int table_3[SIZE2] = {0x8, 0x9};

boolean ctrl_c_fnd = False; //initial Ctrl-C to false

void instruction_Cyle(int breakpt)
{
    signal(SIGINT, siginthandler);
    Clkcycle = 0; //initialize the clkcycle

    printf("Start: PC: %04X PSW: %04X Brkpt:%04X Clk:%d\n", RG[R7].word,
           PSW.word, breakpt, Clkcycle);

    uint16_t SRC1, DES1, bw1, RC1; //for arithematic instructions
    Invalid_ins = True;            // initialize invalid_ins flag to true

    //if breakpoint address is less then Program counter
    if (breakpt < RG[R7].word)
    {
        printf("Enter breakpoiont greater then starting address\n");
    }

    /* run the loop RG[R7].word(Program counter) is eual to breakpoint address
     * or ctrl-c signal is entered
     */
    while ((RG[R7].word != breakpt) && (!ctrl_c_fnd))
    {
        bus(RG[R7].word, &dataBytes, READ, WORD);
        Clkcycle++; //fetch of instruction clkcycle increment by 1
        instruction.i = dataBytes;
        /*Arithematic operands src, des, byte/word, register/constant*/
        SRC1 = instruction.Arithm_instr.SC;
        DES1 = instruction.Arithm_instr.des;
        bw1 = instruction.Arithm_instr.bw;
        RC1 = instruction.Arithm_instr.rc;

        /*check if the instruction opcode is qual to 0x40(ADD opcode)*/
        (instruction.Arithm_instr.opcode == 0x40) ? (ADD(SRC1, DES1, bw1, RC1)) : (printf(""));
        /*checking arithematic instructions opcode and calling the functions*/
        if ((instruction.CH_INST_i.bit9til12 > dec_0) && (instruction.CH_INST_i.bit9til12 < dec_11))
        {
            for (int i = 0; i < SIZE1; i++)
            {
                if (instruction.Arithm_instr.opcode == table_OF_ARith[i])
                {
                    (*Function_Arith_table[i])(SRC1, DES1, bw1, RC1);
                    ClkCycle_InvalidFlag();
                }
            }
        }
        if (instruction.CH_INST_i.bit9til12 == dec_14)
        {
            for (int i = 0; i < SIZE2; i++)
            {
                if (instruction.Swap_extend.opcode == table_OF_SWPB_SXT[i])
                {
                    (*Function_SWPB_SXT[i])();
                    ClkCycle_InvalidFlag();
                }
            }
        }
        if (instruction.CH_INST_i.bit14till15 == dec_3)
        {
            for (int j = 0; j < SIZE3; j++)
            {
                if (instruction.movl.opcode == table_OF_MOVL[j])
                {

                    (*Function_MOVL[j])();
                    ClkCycle_InvalidFlag();
                }
            }
        }
        if (instruction.CH_INST_i.bit16 == dec_1)
        {
            for (int i = 0; i < SIZE2; i++)
            {
                if (instruction.ldr_str.opcode == table_OF_RelMem[i])
                {
                    (*Function_RelMem[i])();
                    ClkCycle_InvalidFlag();
                }
            }
        }
        if ((instruction.CH_INST_i.bit9til12 == dec_12) ||
            (instruction.CH_INST_i.bit9til12 == dec_13))
        {
            for (int i = 0; i < SIZE3; i++)
            {
                if (instruction.Shi_Rot.opcode == table_2[i])
                {
                    (*Function_table2[i])();
                    ClkCycle_InvalidFlag();
                }
                else if (instruction.mov_inst.opcode == table_2[i])
                {
                    (*Function_table2[i])();
                    ClkCycle_InvalidFlag();
                }
            }
        }
        if (instruction.CH_INST_i.bit14till15 == dec_1)
        {
            for (int i = 0; i < SIZE2; i++)
            {
                if (instruction.bra.opcode == table_3[i])
                {
                    (*Function_table3[i])();
                    ClkCycle_InvalidFlag();
                }
                else if (instruction.cex.opcode == table_3[i])
                {
                    (*Function_table3[i])();
                    ClkCycle_InvalidFlag();
                }
            }
        }

        if (instruction.CH_INST_i.bit14till15 == dec_0)
        {
            if (instruction.bl.opcode == dec_0)
            {
                BL();
                ClkCycle_InvalidFlag();
            }
        }
        if (instruction.CH_INST_i.bit9til12 == dec_12)
        {
            if (instruction.swap_inst.opcode == opcode_SWAP)
            {
                SWAP();
                ClkCycle_InvalidFlag();
            }
        }
        if (Invalid_ins == True)
        {
            printf("INvalid instructions %04X\n", instruction.i);
            PSW.PSWbits.FLT = SET; //set fault when instruction is invalid;
        }
        RG[R7].word += 2;
    }

    printf("END: PC: %04X Clk:%d\n", RG[R7].word, Clkcycle);
}
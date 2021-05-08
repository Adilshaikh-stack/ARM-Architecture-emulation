#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>

/*declaraions of all the instructions which are defined
 *in the fucntions.c
 */
void BL();
void BRA();
void CEX();
void ADD(uint8_t SRC, uint8_t DES, uint8_t bw, uint8_t rc);
void ADDC(uint8_t SRC, uint8_t DES, uint8_t bw, uint8_t rc);
void SUB(uint8_t SRC, uint8_t DES, uint8_t bw, uint8_t rc);
void SUBC(uint8_t SRC, uint8_t DES, uint8_t bw, uint8_t rc);
void CMP(uint8_t SRC, uint8_t DES, uint8_t bw, uint8_t rc);
void XOR(uint8_t SRC, uint8_t DES, uint8_t bw, uint8_t rc);
void AND(uint8_t SRC, uint8_t DES, uint8_t bw, uint8_t rc);
void BIT(uint8_t SRC, uint8_t DES, uint8_t bw, uint8_t rc);
void BIC(uint8_t SRC, uint8_t DES, uint8_t bw, uint8_t rc);
void BIS(uint8_t SRC, uint8_t DES, uint8_t bw, uint8_t rc);
void MOV();
void SWAP();
void SRA();
void RRC();
void SWPB();
void SXT();
void MOVL();
void MOVLZ();
void MOVLS();
void MOVH();
void LDR();
void STR();
void PROGRAM_STATUS_WORD(uint32_t DES_R);
void PROGRAM_STATUS_WORD_byte(uint16_t DES_RByt);
void ClkCycle_InvalidFlag();
/*useful declarations use over in program*/
#define MAXBYTE 0xff
#define LOWBYTE 0x00
#define MAXWORD 0xffff
#define MINWORD 0x0000
#define SIZE1 12
#define SIZE2 2
#define SIZE3 5
#define dec_0 0
#define dec_11 11
#define dec_14 14
#define dec_3 3
#define dec_1 1
#define dec_12 12
#define dec_13 13
#define opcode_SWAP 306

enum CLR_SET
{
    CLR,
    SET
};

enum REGISTERS
{
    R0, //GEneral purpose register(R0-R4)
    R1,
    R2,
    R3,
    R4,
    R5, //Link register
    R6, //Stack pointer
    R7  //Program counter

};
//union registersmap RG_Const[RegSIZE] = {0, 1, 2, 4, 8, 16, 32, -1};
union PSWmap PSW;

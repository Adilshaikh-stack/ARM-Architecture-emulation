#pragma once

#include <stdint.h>
#include <signal.h>

/* File: instruction.h
 * Notes:  header file struct declartions of duiferent instructions with unions.
 * useful meanings:
 * SC =soutrce/constant
 * RC =register/constant
 * OFF =offset
 * B = Bits
 * src = source register
 * des =destination register
 *------------------------------------------------------------------------------------
 *
 *			
 * Last Modified: 2020.08.01
 */

/*struct containning the bitfields in order
 *to determine which instruction it is
 */
struct checkINST_i
{
    uint16_t bitanony : 8;
    uint16_t bit9til12 : 4;
    uint16_t bit13 : 1;
    uint16_t bit14till15 : 2;
    uint16_t bit16 : 1;
};
struct Arithm_Opera_i
{
    uint16_t des : 3;
    uint16_t SC : 3;
    uint16_t bw : 1;
    uint16_t rc : 1;
    uint16_t opcode : 8;
};
/* struct containning the bitfields of LDR & STR 
 * operations
 */
struct LDR_STR_i
{
    uint16_t des : 3;
    uint16_t src : 3;
    uint16_t bw : 1;
    uint16_t OFF : 7;
    uint16_t opcode : 2;
};
/*Struct for accessing bitfields for MOV*/
struct MOVL_i
{
    uint16_t des : 3;
    uint16_t B : 8;
    uint16_t opcode : 5;
};

/*struct for swap bytes in Des and sign exten byte 
 *to word in des
 */

struct Swap_sign_i
{

    uint16_t des : 3;
    uint16_t anony : 1;
    uint16_t opcode : 12;
};

/*struct for arithematic shift rightt 1 bit of des
 * and instruction for rotate right with carry.
 */

struct shift_rotate_i
{
    uint16_t des : 3;
    uint16_t constant : 3;
    uint16_t bw : 1;
    uint16_t opcode : 9;
};

/*struct for swapping src and des register 
 *contents
 */

struct SWAP_i
{
    uint16_t des : 3;
    uint16_t src : 3;
    uint16_t opcode : 10;
};

/*instruction for moving src register 
 *contents to des
 */

struct MOV_i
{

    uint16_t des : 3;
    uint16_t src : 3;
    uint16_t bw : 1;
    uint16_t opcode : 9;
};
/*conditional execution instruction*/
struct CEX_i
{
    uint16_t False : 3;
    uint16_t True : 3;
    uint16_t carry : 4;
    uint16_t opcode : 6;
};
/*instruction forbranch with link(BL)*/
struct BL_i
{
    uint16_t OFF : 13;
    uint16_t opcode : 3;
};
/*instruction for unconditional branching*/
struct BRA_i
{
    uint16_t OFF : 10; //offset
    uint16_t opcode : 6;
};
union registersmap {
    struct registers_map
    {
        uint16_t LSBYTE : 8;
        uint16_t MSBYTE : 8;
        uint16_t ovf : 1;
    } Reg;

    uint16_t word;
    uint32_t words;
};

union results {
    struct value
    {
        uint16_t bits7 : 7;
        uint16_t sign_B : 1; //sign bit in byte
        uint16_t ovf_B : 1;  //overflow bit in Byte
        uint16_t bits15 : 6;
        uint16_t sign_W : 1; //sign bit in word
        uint16_t ovf_W : 1;  //overflow bit in word

    } bitmap;

    uint32_t words; //32 bits
    uint16_t word;  //16 bits
    uint8_t byte;   //8 bits
};
/* unionizing the struct with i*/
union PSWmap {
    struct PSW_Map
    {
        uint16_t C : 1; //carry bit
        uint16_t Z : 1; //zero bit
        uint16_t N : 1; //negative bit
        uint16_t SLP : 1;
        uint16_t V : 1; //overflow bit
        uint16_t Curr_Prio : 3;
        uint16_t FLT : 1;
        uint16_t Not_def : 4;
        uint16_t Prev_Prio : 3;
    } PSWbits;

    uint16_t word; //16-bits
};

union field {

    struct checkINST_i CH_INST_i;
    struct LDR_STR_i ldr_str;
    struct Arithm_Opera_i Arithm_instr;
    struct MOVL_i movl;
    struct Swap_sign_i Swap_extend;
    struct shift_rotate_i Shi_Rot;
    struct SWAP_i swap_inst;
    struct MOV_i mov_inst;
    struct CEX_i cex;
    struct BL_i bl;
    struct BRA_i bra;

    uint16_t i; //i is 16 bits wide
} instruction;
#define RegSIZE 8
union registersmap RG[RegSIZE]; //array of registers R0-R7
void instruction_Cyle(int breakpt);
int Clkcycle; //clock sycle
              /*When accessing Mem_Array Clockcycle is increment by 3
              *When fetching instruction Clockcycle is increment by 1
              *When decoding instruction Clockcycle is increment by 1
              *When executing instruction Clockcycle is increment by 1
              */

void siginthandler(int param); //handling the ctrl-c
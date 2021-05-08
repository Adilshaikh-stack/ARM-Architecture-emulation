/*
 * File: Functions.c
 * Author: M.Adil Shaikh
 * School: Dalhousie University
 * Dept : Electrical and Computer engineering
 * Course : ECED3403 Computer Architecture
 * 
 * Purpose: Functions definitions of different instructions, declarations of these
 * functions is in fucntions.h.
 * ------------------------------------------------------------------------------------
 *			
 * Last Modified: 2020.08.01
 *
 *
 */

#include "instruction.h"
#include "functions.h"
#include "A2.h"
#include "bus.h"

unsigned int RG_Const[RegSIZE] = {0, 1, 2, 4, 8, 16, 32, -1};

/*Clkcycle increments when execution and decode takes place*/
void ClkCycle_InvalidFlag()
{
    Clkcycle += 2; //decode and execute
    Invalid_ins = False;
}
/*Branch with link where R5 (Link register)
 *gets the R7 value and R7 increments by offset
 *provided by the BL instruction
 */
void BL()
{
    RG[R5].word = RG[R7].word;
    RG[R7].word = RG[R7].word + instruction.bl.OFF;
}
/*unconditional branching R7 increments by offset
 *provided by the BRA instruction
 */
void BRA()
{
    RG[R7].word = RG[R7].word + instruction.bra.OFF;
}

void CEX()
{
}
/*Addition of des and src/constant registers(as byte/word)
 * provided by the instructions and store it in des register
 */
void ADD(uint8_t SRC, uint8_t DES, uint8_t bw, uint8_t rc)
{
    uint32_t result;
    uint16_t resultbyte;
    if (bw == CLR)
    {
        if (rc == CLR)
            RG[DES].words = RG[DES].word + RG[SRC].word;
        else
            RG[DES].words = RG[DES].word + RG_Const[SRC];

        result = RG[DES].words;
        PROGRAM_STATUS_WORD(result);
    }
    else
    {
        if (rc == CLR)
            RG[DES].Reg.LSBYTE = RG[DES].Reg.LSBYTE + RG[SRC].Reg.LSBYTE;
        else
            RG[DES].Reg.LSBYTE = RG[DES].Reg.LSBYTE + RG_Const[SRC];

        resultbyte = RG[DES].word;
        PROGRAM_STATUS_WORD_byte(resultbyte);
    }
}
/*Addition of des and src/constant +carry registers(as byte/word)
 * provided by the instructions and store it in des register
 */
void ADDC(uint8_t SRC, uint8_t DES, uint8_t bw, uint8_t rc)
{
    uint32_t result;
    uint16_t resultbyte;
    if (bw == CLR)
    {
        if (rc == CLR)
            RG[DES].words = RG[DES].word + RG[SRC].word + PSW.PSWbits.C;
        else
            RG[DES].words = RG[DES].word + RG_Const[SRC] + PSW.PSWbits.C;

        result = RG[DES].words;
        PROGRAM_STATUS_WORD(result);
    }
    else
    {
        if (rc == CLR)
            RG[DES].Reg.LSBYTE = RG[DES].Reg.LSBYTE +
                                 RG[SRC].Reg.LSBYTE + PSW.PSWbits.C;
        else
            RG[DES].Reg.LSBYTE = RG[DES].Reg.LSBYTE + RG_Const[SRC] +
                                 PSW.PSWbits.C;
        resultbyte = RG[DES].word;
        PROGRAM_STATUS_WORD_byte(resultbyte);
    }
}
/* subtraction of src/constant +1from des register(as byte/word)
 * provided by the instructions and store it in des register
 */
void SUB(uint8_t SRC, uint8_t DES, uint8_t bw, uint8_t rc)
{
    uint32_t result;
    uint16_t resultbyte;
    if (bw == CLR)
    {
        if (rc == CLR)
            RG[DES].words = RG[DES].word + (~RG[SRC].word) + 1;
        else
            RG[DES].words = RG[DES].word + (~RG_Const[SRC]) + 1;

        result = RG[DES].words;

        PROGRAM_STATUS_WORD(result);
    }
    else
    {
        if (rc == CLR)
            RG[DES].Reg.LSBYTE = RG[DES].Reg.LSBYTE + (~RG[SRC].Reg.LSBYTE) + 1;
        else
            RG[DES].Reg.LSBYTE = RG[DES].Reg.LSBYTE + (~RG_Const[SRC]) + 1;

        resultbyte = RG[DES].word;
        PROGRAM_STATUS_WORD_byte(resultbyte);
    }
}
/* subtraction of src/constant + carrybit from des register(as byte/word)
 * provided by the instructions and store it in des register
 */
void SUBC(uint8_t SRC, uint8_t DES, uint8_t bw, uint8_t rc)
{

    uint32_t result;
    uint16_t resultbyte;

    if (bw == CLR)
    {
        if (rc == CLR)
            RG[DES].words = RG[DES].word + (~RG[SRC].word) + PSW.PSWbits.C;
        else
            RG[DES].words = RG[DES].word + (~RG_Const[SRC]) + PSW.PSWbits.C;

        result = RG[DES].words;
        PROGRAM_STATUS_WORD(result);
    }
    else
    {
        if (rc == CLR)
            RG[DES].Reg.LSBYTE = RG[DES].Reg.LSBYTE + (~RG[SRC].Reg.LSBYTE) +
                                 PSW.PSWbits.C;
        else
            RG[DES].Reg.LSBYTE = RG[DES].Reg.LSBYTE + (~RG_Const[SRC]) +
                                 PSW.PSWbits.C;
        resultbyte = RG[DES].word;
        PROGRAM_STATUS_WORD_byte(resultbyte);
    }
}
/*comparision of destination and src register values
 * and then setting and clearing the PSW V,N, C bits
 */
void CMP(uint8_t SRC, uint8_t DES, uint8_t bw, uint8_t rc)
{
    uint32_t result;
    uint16_t resultbyte;

    if (bw == CLR)
    {
        if (rc == CLR)
            result = RG[DES].word + (~RG[SRC].word) + 1;
        else
            result = RG[DES].word + (~RG_Const[SRC]) + 1;

        PROGRAM_STATUS_WORD(result);
    }
    else
    {
        if (rc == CLR)
            resultbyte = RG[DES].Reg.LSBYTE + (~RG[SRC].Reg.LSBYTE) +
                         1;
        else
            resultbyte = RG[DES].Reg.LSBYTE + (~RG_Const[SRC]) +
                         1;

        resultbyte = RG[DES].word;
        PROGRAM_STATUS_WORD_byte(resultbyte);
    }
}
/* store destination and src register values
 * and then setting and clearing the PSW V,N, C bits
 */
void XOR(uint8_t SRC, uint8_t DES, uint8_t bw, uint8_t rc)
{
    uint32_t result;
    uint16_t resultbyte;

    /*EXCLUSIVEOR the DES and SRC register / Constant
     *Store the result in DST register 
     */
    if (bw == CLR)
    {
        if (rc == CLR)
        {
            RG[DES].word = (RG[DES].word) ^ ((RG[SRC].word));
        }
        else
        {
            RG[DES].word = (RG[DES].word) ^ (RG_Const[SRC]);
        }
        result = RG[DES].words;
        PROGRAM_STATUS_WORD(result);
    }
    else
    {
        /*EXCLUSIVEOR the LSB DES and SRC register/Constant
         *Store the result in DST register
         */
        if (rc == CLR)
            RG[DES].Reg.LSBYTE = (RG[DES].Reg.LSBYTE) ^ (RG[SRC].Reg.LSBYTE);
        else
            RG[DES].Reg.LSBYTE = (RG[DES].Reg.LSBYTE) ^ (RG_Const[SRC]);

        resultbyte = RG[DES].word;
        PROGRAM_STATUS_WORD_byte(resultbyte);
    }
}

void AND(uint8_t SRC, uint8_t DES, uint8_t bw, uint8_t rc)
{
    uint32_t result;
    uint16_t resultbyte;

    /* DES and SRC register/Constant are logicalANDED together as WORD
     * Store the result in DST register
     */
    if (bw == CLR)
    {
        if (rc == CLR)
            RG[DES].word = (RG[DES].word) & ((RG[SRC].word));
        else
            RG[DES].word = (RG[DES].word) & (RG_Const[SRC]);

        result = RG[DES].words;
        PROGRAM_STATUS_WORD(result);
    }
    else
    { /* LSB of DES and SRC register/Constant are logicalANDED together
       * Store the result in DST register
       */
        if (rc == CLR)
            RG[DES].Reg.LSBYTE = (RG[DES].Reg.LSBYTE) & (RG[SRC].Reg.LSBYTE);
        else
            RG[DES].Reg.LSBYTE = (RG[DES].Reg.LSBYTE) & (RG_Const[SRC]);

        resultbyte = RG[DES].word;
        PROGRAM_STATUS_WORD_byte(resultbyte);
    }
}

void BIT(uint8_t SRC, uint8_t DES, uint8_t bw, uint8_t rc)
{
    uint32_t result;
    uint16_t resultbyte;
    /*DES and SRC register/Constant are logicalANDED together as WORD*/
    if (bw == CLR)
    {
        if (rc == CLR)
            result = (RG[DES].word) & ((RG[SRC].word));

        else
            result = (RG[DES].word) & (RG_Const[SRC]);

        PROGRAM_STATUS_WORD(result);
    }
    else
    { /*LSB of DES and SRC register/Constant are logicalANDED together*/
        if (rc == CLR)
            resultbyte = (RG[DES].Reg.LSBYTE) & (RG[SRC].Reg.LSBYTE);
        else
            resultbyte = (RG[DES].Reg.LSBYTE) & (RG_Const[SRC]);

        PROGRAM_STATUS_WORD_byte(resultbyte);
    }
}

void BIC(uint8_t SRC, uint8_t DES, uint8_t bw, uint8_t rc)
{
    /* CLEAR the bit by logicalANDing DES and complement
     * of SRC register/Constant as WORD.
     * store the result in DST register.
     */
    if (bw == CLR)
    {
        if (rc == CLR)
            RG[DES].word = (RG[DES].word) & (~(RG[SRC].word));

        else
            RG[DES].word = (RG[DES].word) & (~(RG_Const[SRC]));
        PROGRAM_STATUS_WORD(RG[DES].word);
    }
    else
    {
        /* CLEAR the bit by logicalANDing the LSB of DES and complement
         * of SRC register/Constant
         * store the result in LSB of DST register.
         */
        if (rc == CLR)
            RG[DES].Reg.LSBYTE = (RG[DES].Reg.LSBYTE) & (~RG[SRC].Reg.LSBYTE);
        else
            RG[DES].Reg.LSBYTE = (RG[DES].Reg.LSBYTE) & (~RG_Const[SRC]);

        PROGRAM_STATUS_WORD_byte(RG[DES].Reg.LSBYTE);
    }
}

void BIS(uint8_t SRC, uint8_t DES, uint8_t bw, uint8_t rc)
{
    /*Set bits in DST specified by SRC*/
    /* SET the bit by logicalORing the DEST and
     * SRC/Constant register as WORD and store the result in DST
     * register.
     */
    if (bw == 0)
    {
        if (rc == CLR)
            RG[DES].word = (RG[DES].word) | (RG[SRC].word);

        else
            RG[DES].word = (RG[DES].word) | (RG_Const[SRC]);

        PROGRAM_STATUS_WORD(RG[DES].word);
    }
    else
    { /*SET the bit by logicalORing the LSB of DEST and
        *SRC/Constant register and store the result in DST
        *register.
        */
        if (rc == CLR)
            RG[DES].Reg.LSBYTE = (RG[DES].Reg.LSBYTE) | (RG[SRC].Reg.LSBYTE);
        else
            RG[DES].Reg.LSBYTE = (RG[DES].Reg.LSBYTE) | (RG_Const[SRC]);

        PROGRAM_STATUS_WORD_byte(RG[DES].word);
    }
}

void MOV()
{
    uint8_t SRC, DES, bw;
    SRC = instruction.mov_inst.src;
    DES = instruction.mov_inst.des;
    bw = instruction.mov_inst.bw;

    /*contents of DST as WORD EQUALS the SRC register /Constant*/
    if (bw == 0)
        RG[DES].word = RG[SRC].word;

    else /*contents of LSB(LsByte) DST EQUALS the SRC/Constant register*/
        RG[DES].Reg.LSBYTE = RG[SRC].Reg.LSBYTE;
}

void SWAP()
{
    /*SWAP the contents of DST and SRC register
     *Function SRA
     */
    uint8_t SRC, DES;
    uint16_t temp;
    SRC = instruction.swap_inst.src;
    DES = instruction.swap_inst.des;
    temp = RG[SRC].word;
    RG[SRC].word = RG[DES].word;
    RG[DES].word = temp;
}

void SRA()
{
    uint8_t DES, bw, signbit;
    DES = instruction.Shi_Rot.des;
    bw = instruction.Shi_Rot.bw;

    if (bw == CLR)
    /*LogicalSHIFTright the DES register(1 bit) arithmetic as WORD*/
    {
        signbit = (RG[DES].word >> 15);
        RG[DES].word = (signbit << 15) | (RG[DES].word >> 1);
    }
    else /*LogicalSHIFTright the LSB of DES register (1 bit) arithmetic*/
    {
        signbit = (RG[DES].Reg.LSBYTE >> 7);
        RG[DES].Reg.LSBYTE = (signbit << 7) | (RG[DES].Reg.LSBYTE >> 1);
    }
    /*check if signbit is set*/
    (signbit == SET) ? (PSW.PSWbits.C = SET) : (PSW.PSWbits.C = CLR);
}

void RRC()
{
    uint8_t DES, temp, bw;
    uint16_t mask = 0x0001;
    DES = instruction.Shi_Rot.des;
    temp = RG[DES].word;
    bw = instruction.Shi_Rot.bw;
    /*Logical Rotate DES register right (1 bit) through
      Carry as WORD*/
    if (bw == 0)
    {
        if (PSW.PSWbits.C == 1)
            RG[DES].word = (RG[DES].word) * 2;
        else
            RG[DES].word = ((RG[DES].word) / 2);
    }
    else /*Logical Rotate LSB of DES register right (1 bit) through
          *Carry
          */
    {
        if (PSW.PSWbits.C == 1)
            RG[DES].Reg.LSBYTE = (RG[DES].Reg.LSBYTE) * 2;
        else
            RG[DES].Reg.LSBYTE = ((RG[DES].Reg.LSBYTE) / 2);
    }
    mask = mask & temp;
    (mask == 0x0001) ? (PSW.PSWbits.C = SET) : (PSW.PSWbits.C = CLR);
}

void SWPB()
{
    /*swap the byte of DES register (WORD only)
     with other BYTE of DES register in word*/
    uint8_t DES;
    uint16_t temp;
    DES = instruction.Swap_extend.des;

    temp = RG[DES].Reg.LSBYTE;
    RG[DES].Reg.LSBYTE = RG[DES].Reg.MSBYTE;
    RG[DES].Reg.MSBYTE = temp;
}

void SXT()
{
    /*sign extend BYTE to WORD in DES register*/
    uint8_t DES, value, Setval;
    DES = instruction.Swap_extend.des;
    /*To set the value of MSBit of LSbyte of register*/
    Setval = 0x80;
    value = (RG[DES].Reg.LSBYTE) & (Setval);

    if (value == Setval)
        RG[DES].Reg.MSBYTE = MAXBYTE;
    else
        RG[DES].Reg.MSBYTE = LOWBYTE;
}

void MOVL()
{
    /*THE lowBYTE of DST EQUALS B bits of MOVL
     *The highBYTE DST register is unchanged
     */
    uint8_t DES, Bits;
    DES = instruction.movl.des;
    Bits = instruction.movl.B;
    /*LsByte of DesRegister contains Bits*/
    RG[DES].Reg.LSBYTE = Bits;
}

void MOVLZ()
{
    /*THE lowBYTE of DST register EQUALS B bits of MOVLZ
     *The highBYTE DST register is CLEAR
     */
    uint8_t DES, Bits;
    Bits = instruction.movl.B;
    DES = instruction.movl.des;
    /*LsByte of DesRegister contains Bits*/
    RG[DES].Reg.LSBYTE = Bits;
    /*MsByte of DesRegister contains 0x00*/
    RG[DES].Reg.MSBYTE = LOWBYTE;
}

void MOVLS()
{
    /*THE lowBYTE of DST register EQUALS B bits of MOVLS
     *The highBYTE of DST register is SET
     */
    uint8_t DES, Bits;
    Bits = instruction.movl.B;
    DES = instruction.movl.des;
    /*LsByte of DesRegister contains Bits*/
    RG[DES].Reg.LSBYTE = Bits;
    /*MsByte of DesRegister contains 0xff*/
    RG[DES].Reg.MSBYTE = MAXBYTE;
}

void MOVH()
{
    /* THE lowBYTE of DST register is unchanged 
     * The highBYTE of DST register EQUALS B bits of MOVLH
     */
    uint8_t DES, Bits;
    Bits = instruction.movl.B;
    DES = instruction.movl.des;
    /*MsByte of DesRegister contains 0xff*/
    RG[DES].Reg.MSBYTE = Bits;
}
void LDR()
{

    uint8_t DES, SRC, OFF, bw;
    SRC = instruction.ldr_str.src;
    DES = instruction.ldr_str.des;
    OFF = instruction.ldr_str.OFF;
    bw = instruction.ldr_str.bw;
    /*DST as WORD EQUALS to the contents of Mem_Array at
     *location SRC + OFF
     */
    if (bw == CLR)
    {
        bus((SRC + OFF), &RG[DES].word, READ, WORD);
    }
    else
    /* DST as BYTE is EQUALS to the contents of Mem_Array at 
     *location SRC + OFF
     */
    {
        bus((SRC + OFF), &RG[DES].word, READ, BYTE);
    }
}

void STR()
{
    uint8_t DES, SRC, OFF, bw;
    SRC = instruction.ldr_str.src;
    DES = instruction.ldr_str.des;
    OFF = instruction.ldr_str.OFF;
    bw = instruction.ldr_str.bw;
    /*contents of Mem_Array as WORD at location (DES + OFF) is equal to SRC*/
    if (bw == CLR)
    {
        bus((RG[DES].word + OFF), &RG[SRC].word, WRITE, WORD);
    }
    else /*contents of Mem_Array as BYTE at location (DES + OFF) is equal to SRC*/
    {
        //printf("memory %04X src register %04X location %04X\n", mem[RG[DES].word + OFF], RG[DES].word, (RG[DES].word + OFF));
        bus((RG[DES].Reg.LSBYTE + OFF), &RG[SRC].word, WRITE, BYTE);
    }
}

void PROGRAM_STATUS_WORD(uint32_t DES_R)
{
    union results bitchk;
    bitchk.words = DES_R;
    /*IF DES value is greater than 16bit value*/
    if (bitchk.bitmap.ovf_W == SET)
    {
        PSW.PSWbits.V = SET; //overflow of PSW set
        PSW.PSWbits.C = SET; //carry bit of PW set
    }
    else
    {
        PSW.PSWbits.V = CLR; //overflow of PSW cle
        PSW.PSWbits.C = CLR; //carry bit of PW clr
    }

    /*IF DES value is zero*/
    if (bitchk.word == MINWORD)
        PSW.PSWbits.Z = SET; //set Zero bit of PSW
    else
        PSW.PSWbits.Z = CLR; //clr Zero bit of PSW

    /*IF DES value is negative value*/
    if (bitchk.bitmap.sign_W == SET)
        PSW.PSWbits.N = SET; //set N bit of PSW
    else
        PSW.PSWbits.N = CLR; //Clear N bit of PSW

    //printf("PSW.word %04X\n", PSW.word);
}

void PROGRAM_STATUS_WORD_byte(uint16_t DES_RByt)
{
    union results bitchkByte;
    bitchkByte.word = DES_RByt;
    /*IF DES value is greater than 8bit value*/
    if (bitchkByte.bitmap.ovf_B == SET)
    {
        PSW.PSWbits.V = SET;
        PSW.PSWbits.C = SET;
    }
    else
    {
        PSW.PSWbits.V = CLR;
        PSW.PSWbits.C = CLR;
    }

    if (bitchkByte.byte == LOWBYTE)
        PSW.PSWbits.Z = SET;
    else
        PSW.PSWbits.Z = CLR;

    if (bitchkByte.bitmap.sign_B == SET)
        PSW.PSWbits.N = SET;
    else
        PSW.PSWbits.N = CLR;

    //printf("PSW.word %04X\n", PSW.word);
}

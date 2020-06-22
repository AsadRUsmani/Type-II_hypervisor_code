/* 
 * File:   IInfo.cpp
 * Author: asadusmani
 * Created on February 12, 2014, 11:49 AM
 */

#include "IInfo.h"

IInfo::IInfo() {    
}

IInfo::IInfo (uint32_t anInstruction):instr() {
    encoding = anInstruction;
    parse();
}

void IInfo::parse(){
    
        uint32_t anInstruction = encoding;
        type     = getType(anInstruction);
        
        if(type == RTYPE){   
           instr.R.rs = EXTBITS(anInstruction,21,25);   // bits 25–21 (5)
           instr.R.rt = EXTBITS(anInstruction,16,20);   // bits 20–16 (5)
           instr.R.rd = EXTBITS(anInstruction,11,15);   // bits 15–11 (5)
           instr.R.shft = EXTBITS(anInstruction,6,10);   // bits 10–6  (5)
           instr.R.func = EXTBITS(anInstruction,0,5);   // bits 5–0  (6)    
           isControlInst = isCntrlInstR();
    //       printRType(this);
        }else if(type == ITYPE){   
           instr.I.rs = EXTBITS(anInstruction, 21, 25);     // bits 25–21 (5) 
           instr.I.rt = EXTBITS(anInstruction, 16, 20);     // bits 20–16 (5) 
           instr.I.imm = EXTBITS(anInstruction, 0,  15);     // bits 15–0  (16) 
           isControlInst = isCntrlInstI();
      //     printIType(this);
        }else {
           instr.j.addr = EXTBITS(anInstruction, 0,  25);   // bits 25–0 (26) 
           isControlInst = isCntrlInstJ();
        //   printJType(this);
        }
        
}

TYPES IInfo::getType(uint32_t instruction) {
    int opcode = EXTBITS(instruction, 26, 31);
    instr.opcode = opcode;
    if (opcode == 0 || (opcode > 15 && opcode < 20) || opcode == 31 || opcode == 28 )  
        return RTYPE;
    else if (opcode == 2 || opcode == 3) 
        return JTYPE;
    else
        return ITYPE;
}

bool IInfo::isCntrlInstR() {
/*  RType [if(oc is 0, 28, 31 OR (> 15 and < 20)]
=================================================
ID    oc6|rs5|rt5|rd5|sa5|func6|
0     0  |s  |0  |0  |0  |8    |jr s
1     0  |s  |0  |0  |16 |8    |jr.hb s
2     0  |s  |0  |31 |0  |9    |jalr s
3     0  |s  |0  |d  |0  |9    |jalr d,s
4     0  |s  |0  |d  |16 |9    |jalr.hb d,s
5     16 |8  |0  |broffset     |bc0f    p       Not in MIPS32/64
6     16 |8  |1  |broffset     |bc0t    p       Not in MIPS32/64
7     16 |8  |2  |broffset     |bc0fl   p       Not in MIPS32/64
8     16 |8  |3  |broffset     |bc0tl   p
9     17 |8  |N|0|broffset     |bc1f N,p 	0 => bits 16-17
10    17 |8  |N|1|broffset     |bc1t N,p 
11    17 |8  |N|2|broffset     |bc1fl N,p 
12    17 |8  |N|3|broffset     |bc1tl N,p
13    18 |8  |0  |broffset     |bc2f p 
14    18 |8  |1  |broffset     |bc2t p 
15    18 |8  |2  |broffset     |bc2fl p
16    18 |8  |3  |broffset     |bc2tl p
*/    
    int oc = instr.opcode;
    int rs = instr.R.rs;
    int func = instr.R.func;
   
 if(((oc == 0)&& (func == 8 || func == 9) )|| (((oc == 16) ||(oc == 18)) && (rs == 8 )) ||
         ((oc == 17) && (rs == 8 || rs == 9 ||rs == 10 )) ){
            return true;
        } 
 else    
    return false;
}

bool IInfo::isCntrlInstI() {
/*
I-Type [all other values of oc]
===============================
oc6|rs5|rt5|imm16|
1  |s  |0  |broffset |bltz s,p
1  |s  |1  |broffset |bgez s,p
1  |s  |2  |broffset |bltzl s,p
1  |s  |3  |broffset |bgezl s,p
1  |s  |16 |broffset |bltzal s,p
1  |s  |17 |broffset |bgezal s,p
1  |s  |18 |broffset |bltzall s,p
1  |s  |19 |broffset |bgezall s,p

4  |s  |t  |broffset |beq s,t,p
5  |s  |t  |broffset |bne s,t,p
6  |s  |0  |broffset |blez s,p
7  |s  |0  |broffset |bgtz s,p
20 |s  |t  |broffset |beql s,t,p 
21 |s  |t  |broffset |bnel s,t,p 
22 |s  |0  |broffset |blezl s,p 
23 |s  |0  |broffset |bgtzl s,p
 */   
    int oc = instr.opcode;
    int rt = instr.I.rt;
   
    if ((oc >= 4 && oc <=7 ) || (oc >= 20 && oc <=23) || oc == 50 || oc == 54 || oc == 58 ||oc == 62 
            || (oc == 1 && ((rt >= 0 && rt <= 3) || (rt >= 16 && rt <= 19))) )
        return true;
    else       
        return false;
}

bool IInfo::isCntrlInstJ(){
/*J-Type [if oc is 2 or 3]
========================
oc6|target26|
2  |target  | j    target
3  |target  | jal  target
29 |target  | jalx target MIPS16e
*/        
    int oc = instr.opcode;

    if (oc == 2 || oc == 3 ||oc == 29 ) 
         return true;
    else
        return false;
}


void IInfo::printRType(IInfo *ins){
    printf("Instruction is R-Type. encoding 0x%x\n",ins->encoding);
    printf("| opcode(6) | rs(5) | rt(5) | rd(5) | sa(5) | func(6) |\n");
    printf("| %d | %d | %d | %d | %d | %d |\n", ins->instr.opcode, ins->instr.R.rs, ins->instr.R.rt, ins->instr.R.rd, ins->instr.R.shft,ins->instr.R.func);
}
void IInfo::printIType(IInfo *ins){
    printf("Instruction is I-Type. encoding 0x%x\n",ins->encoding);
    printf("| opcode(6) | rs(5) | rt(5) | immediate(16) |\n");
    printf("| %d | %d | %d | %d |\n", ins->instr.opcode, ins->instr.I.rs, ins->instr.I.rt, ins->instr.I.imm);
}
void IInfo::printJType(IInfo *ins){
    printf("Instruction is J-Type. encoding 0x%x\n",ins->encoding);
    printf("| opcode(6) | target(26) |\n");
    printf("| %d | %d |\n", ins->instr.opcode, ins->instr.j.addr);
}



//void IInfo::decode(){    
//    if (type == RTYPE) decodeRType();
//    if (type == ITYPE) decodeIType();
//    if (type == JTYPE) decodeJType();
//    
//}


//void IInfo::decodeRType(){
//    opcode	= BitMan::extractBits(encoding, 26, 31);   // bits 31–26 (6)
//    rs		= BitMan::extractBits(encoding, 21, 25);   // bits 25–21 (5)
//    rt		= BitMan::extractBits(encoding, 16, 20);   // bits 20–16 (5)
//    rd		= BitMan::extractBits(encoding, 11, 15);   // bits 15–11 (5)
//    sa		= BitMan::extractBits(encoding, 6,  10);   // bits 10–6  (5)
//    func	= BitMan::extractBits(encoding, 0,   5);   // bits 5–0  (6)
//}


//void IInfo::decodeIType(){
//    opcode    = BitMan::extractBits(encoding, 26, 31);     // bits 31–26 (6)
//    rs        = BitMan::extractBits(encoding, 21, 25);     // bits 25–21 (5) 
//    rt        = BitMan::extractBits(encoding, 16, 20);     // bits 20–16 (5) 
//    immediate = BitMan::extractBits(encoding, 0,  15);     // bits 15–0  (16) 
//} 


//void IInfo::decodeJType(){
//    opcode = BitMan::extractBits(encoding, 26, 31);   // bits 31–26 (6)
//    target = BitMan::extractBits(encoding, 0,  25);   // bits 25–0 (26)  
//}


//IInfo::IInfo(uint32_t anInstruction)
//{
//	rs=rt=rd=sa=func=immediate=target = -1;
//
//	encoding = anInstruction;
//	opcode   = BitMan::extractBits(anInstruction, 26, 31);
//	type     = this->getType();
//
//	this->processInst();
//}
//
//
//void IInfo::processInst() {
//
//    decode();    
//}


//bool IInfo::isCntrlInst() {
//    if (type == RTYPE)  return isCntrlInstR();
//    if (type == ITYPE)  return isCntrlInstI();
//    if (type == JTYPE)  return isCntrlInstJ();    
//    printf("InstHandler::isCntrlInst(IInfo* ii): Error: Unable to determine instruction type\n");
//    exit(1);
//}


//void IInfo::print() {
//    	printf("Encoding: 0x%.8x \n",encoding);
//        printf("Decoding:\n"); 
//        if (type == RTYPE) printRType();
//        if (type == ITYPE) printIType();
//        if (type == JTYPE) printJType();
//        
//}
//void IInfo::printRType(){
//    printf("Instruction is R-Type.\n");
//    printf("| opcode(6) | rs(5) | rt(5) | rd(5) | sa(5) | func(6) |\n");
//    printf("| %d | %d | %d | %d | %d | %d |\n", opcode, rs, rt, rd, sa,func);
//}
//void IInfo::printIType(){
//    printf("Instruction is I-Type.\n");
//    printf("| opcode(6) | rs(5) | rt(5) | immediate(16) |\n");
//    printf("| %d | %d | %d | %d |\n", opcode, rs, rt, immediate);
//}
//void IInfo::printJType(){
//    printf("Instruction is J-Type.\n");
//    printf("| opcode(6) | target(26) |\n");
//    printf("| %d | %d |\n", opcode, target);
//}




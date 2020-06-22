/* 
 * File:   IInfo.h
 * Author: asadusmani
 *
 * Created on February 12, 2014, 11:49 AM
 */

#ifndef IINFO_H
#define	IINFO_H

#include "Common.h"


#define EXTBITS(val,start,end) \
    ((val >> start) & ((1 << (end - start + 1)) - 1))

enum TYPES { RTYPE , ITYPE , JTYPE ,ILLEGAL };

typedef struct{
                uint32_t opcode:6;
		struct{			
			uint32_t rs:5;
			uint32_t rt:5;
			uint32_t rd:5;
			uint32_t shft:5;
			uint32_t func:6;
			} R;
		struct{			
			uint32_t rs:5;
			uint32_t rt:5;
			uint32_t imm:16;
			} I;
		struct{
			uint32_t addr:26;
			} j;
	} INSTRUCTION;
        
class IInfo {
public:    
        INSTRUCTION instr;
        uint32_t encoding;
        TYPES type;                      /* Type of instruction (I,J.R)*/
        bool isControlInst;
        IInfo(uint32_t ins);
        IInfo();
        //bool isCntrlInst();
        void printRType(IInfo *ins);
        void printIType(IInfo *ins);
        void printJType(IInfo *ins);
private:
    
        bool inline isCntrlInstR();
        bool inline isCntrlInstI();
        bool inline isCntrlInstJ();

        TYPES inline getType(uint32_t ins);
        void inline parse();
};

//class IInfo {
//public://data
//
//    enum INSTYPES { RTYPE , ITYPE , JTYPE ,ILLEGAL };
//    //enum INSTYPES {a,b,c,d};
//    //enum abc { RTYPE = 0,ITYPE=1,JTYPE=2};
//    uint32_t encoding;                  /* Encoding of instruction as retrieved from the binary*/
//
//    INSTYPES type;                      /* Type of instruction (I,J.R)*/
//    uint8_t opcode;
//    //R-type => | opcode(6) | rs(5) | rt(5) | rd(5) | sa(5) | func(6) |
//    uint8_t rs;       // bits 25–21 (5)
//    uint8_t rt;       // bits 20–16 (5)
//    uint8_t rd;       // bits 15–11 (5)
//    uint8_t sa;       // bits 10–6  (5)
//    uint8_t func;      // bits 5–0  (6)
//    //I-type => | opcode(6) | rs(5) | rt(5) | immediate(16) |        
//    int immediate;  // bits 15–0  (16)
//    //J-type => | opcode(6) | target(26) |
//    uint32_t target;   // bits 25–0 (26)
//
//public://functions
//
//    IInfo(uint32_t anInstruction);
//    virtual ~IInfo();
//
//    void processInst();
//
//    bool isCntrlInstR();
//    bool isCntrlInstI();
//    bool isCntrlInstJ();
//    bool isCntrlInst();
//
//    INSTYPES getType();
//    void printRType();
//    void printIType();
//    void printJType();
//    void print();
//
//
//private:
//    void decode();
//    void decodeRType();
//    void decodeIType();
//    void decodeJType();
//    
//
//};

#endif	/* IINFO_H */


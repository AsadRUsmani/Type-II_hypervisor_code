#include <iostream>

#include "TLB_IPE.h"
#include "Common.h"
#include "InsDecoding.h"


InsDecoding decode_funs;

InsDecoding::InsDecoding( ) {
}

InsDecoding::InsDecoding(const InsDecoding& orig) {
}

InsDecoding::~InsDecoding( ) {
}

InsDecoding::emulate opcode_funs_array[] ={ &InsDecoding::Opcode_0,
                                                 &InsDecoding::Opcode_1,
                                                 &InsDecoding::Opcode_2,   // j    
                                                 &InsDecoding::Opcode_3,   // jal
                                                 &InsDecoding::Opcode_4,   // beq
                                                 &InsDecoding::Opcode_5,   // bne
                                                 &InsDecoding::Opcode_6,   // blez
                                                 &InsDecoding::Opcode_7,   // bgtz
                                                 &InsDecoding::Opcode_8,   // addi
                                                 &InsDecoding::Opcode_9,   // addiu
                                                 &InsDecoding::Opcode_10,  // slti
                                                 &InsDecoding::Opcode_11,  // sltiu
                                                 &InsDecoding::Opcode_12,  // andi
                                                 &InsDecoding::Opcode_13,  // ori
                                                 &InsDecoding::Opcode_14,  // xori
                                                 &InsDecoding::Opcode_15,  // lui
                                                 &InsDecoding::Opcode_16,
                                                 &InsDecoding::not_an_Ins,
                                                 &InsDecoding::Opcode_18,
                                                 &InsDecoding::not_an_Ins,
                                                 &InsDecoding::Opcode_20,  // beql
                                                 &InsDecoding::Opcode_21,  // bnel
                                                 &InsDecoding::Opcode_22,  // blezl
                                                 &InsDecoding::Opcode_23,  // bgtzl
                                                 &InsDecoding::Opcode_24,  // daddi
                                                 &InsDecoding::Opcode_25,  // daddiu
                                                 &InsDecoding::Opcode_26,  // ldl
                                                 &InsDecoding::Opcode_27,  // ldr 
                                                 &InsDecoding::Opcode_28,
                                                 &InsDecoding::not_an_Ins,
                                                 &InsDecoding::not_an_Ins,
                                                 &InsDecoding::Opcode_31,
                                                 &InsDecoding::Opcode_32, //  lb 
                                                 &InsDecoding::Opcode_33, //  lh
                                                 &InsDecoding::Opcode_34, // lwl
                                                 &InsDecoding::Opcode_35, // lw
                                                 &InsDecoding::Opcode_36, // lbu
                                                 &InsDecoding::Opcode_37, // lhu
                                                 &InsDecoding::Opcode_38, // lwr
                                                 &InsDecoding::Opcode_39, // lwu
                                                 &InsDecoding::Opcode_40, // sb
                                                 &InsDecoding::Opcode_41, // sh
                                                 &InsDecoding::Opcode_42, // swl
                                                 &InsDecoding::Opcode_43, // sw
                                                 &InsDecoding::Opcode_44, // sdl
                                                 &InsDecoding::Opcode_45, // sdr
                                                 &InsDecoding::Opcode_46, // swr
                                                 &InsDecoding::Opcode_47, // cache
                                                 &InsDecoding::Opcode_48, // ll
                                                 &InsDecoding::not_an_Ins,
                                                 &InsDecoding::Opcode_50, // bbit0
                                                 &InsDecoding::Opcode_51, // pref
                                                 &InsDecoding::Opcode_52, // lld
                                                 &InsDecoding::not_an_Ins,
                                                 &InsDecoding::Opcode_54, // bbit032
                                                 &InsDecoding::Opcode_55, // ld
                                                 &InsDecoding::Opcode_56, // sc
                                                 &InsDecoding::not_an_Ins,
                                                 &InsDecoding::Opcode_58, // bbit1
                                                 &InsDecoding::Opcode_59, // BlockTermination
                                                 &InsDecoding::Opcode_60, // scd
                                                 &InsDecoding::not_an_Ins,
                                                 &InsDecoding::Opcode_62, // bbit132
                                                 &InsDecoding::Opcode_63, // sd
                                                 
      };

InsDecoding::emulate opcode0_0to5_bits[] ={ &InsDecoding::OP0_0to5bits_0, //nop,ssnop,sll,ehb
                                            &InsDecoding::not_an_Ins,
                                            &InsDecoding::OP0_0to5bits_2, //srl, rotr
                                            &InsDecoding::OP0_0to5bits_3, //sra
                                            &InsDecoding::OP0_0to5bits_4, //sllv
                                            &InsDecoding::not_an_Ins,  
                                            &InsDecoding::OP0_0to5bits_6, // srlv, rotrv 
                                            &InsDecoding::OP0_0to5bits_7, // srav
                                            &InsDecoding::OP0_0to5bits_8, // jr, jr.hb
                                            &InsDecoding::OP0_0to5bits_9, // jalr, jalr.hb
                                            &InsDecoding::OP0_0to5bits_10, // movz
                                            &InsDecoding::OP0_0to5bits_11, // movn
                                            &InsDecoding::OP0_0to5bits_12, // syscall
                                            &InsDecoding::OP0_0to5bits_13, // break
                                            &InsDecoding::OP0_0to5bits_14, // sdbbp
                                            &InsDecoding::OP0_0to5bits_15, // sync
                                            &InsDecoding::OP0_0to5bits_16, // mfhi
                                            &InsDecoding::OP0_0to5bits_17, // mthi
                                            &InsDecoding::OP0_0to5bits_18, // mflo
                                            &InsDecoding::OP0_0to5bits_19, // mtlo
                                            &InsDecoding::OP0_0to5bits_20, // dsllv
                                            &InsDecoding::not_an_Ins,
                                            &InsDecoding::OP0_0to5bits_22, // dsrlv, drotrv
                                            &InsDecoding::OP0_0to5bits_23, // dsrav
                                            &InsDecoding::OP0_0to5bits_24, // mult
                                            &InsDecoding::OP0_0to5bits_25, // multu
                                            &InsDecoding::OP0_0to5bits_26, // div
                                            &InsDecoding::OP0_0to5bits_27, // divu
                                            &InsDecoding::OP0_0to5bits_28, // dmult
                                            &InsDecoding::OP0_0to5bits_29, // dmultu
                                            &InsDecoding::OP0_0to5bits_30, // ddiv
                                            &InsDecoding::OP0_0to5bits_31, // ddivu
                                            &InsDecoding::OP0_0to5bits_32, // add
                                            &InsDecoding::OP0_0to5bits_33, // addu
                                            &InsDecoding::OP0_0to5bits_34, // sub
                                            &InsDecoding::OP0_0to5bits_35, // subu
                                            &InsDecoding::OP0_0to5bits_36, // and
                                            &InsDecoding::OP0_0to5bits_37, // or
                                            &InsDecoding::OP0_0to5bits_38, // xor
                                            &InsDecoding::OP0_0to5bits_39, // nor
                                            &InsDecoding::not_an_Ins,
                                            &InsDecoding::not_an_Ins,
                                            &InsDecoding::OP0_0to5bits_42, // slt
                                            &InsDecoding::OP0_0to5bits_43, // sltu
                                            &InsDecoding::OP0_0to5bits_44, // dadd
                                            &InsDecoding::OP0_0to5bits_45, // daddu
                                            &InsDecoding::OP0_0to5bits_46, // dsub
                                            &InsDecoding::OP0_0to5bits_47, // dsubu
                                            &InsDecoding::OP0_0to5bits_48, // tge
                                            &InsDecoding::OP0_0to5bits_49, // tgeu
                                            &InsDecoding::OP0_0to5bits_50, // tlt
                                            &InsDecoding::OP0_0to5bits_51, // tltu
                                            &InsDecoding::OP0_0to5bits_52, // teq
                                            &InsDecoding::not_an_Ins,
                                            &InsDecoding::OP0_0to5bits_54, //tne
                                            &InsDecoding::not_an_Ins,
                                            &InsDecoding::OP0_0to5bits_56, // dsll
                                            &InsDecoding::not_an_Ins,
                                            &InsDecoding::OP0_0to5bits_58, // dsrl
                                            &InsDecoding::OP0_0to5bits_59, // dsra
                                            &InsDecoding::OP0_0to5bits_60, // dsll32
                                            &InsDecoding::not_an_Ins,    
                                            &InsDecoding::OP0_0to5bits_62, // dsrl32
                                            &InsDecoding::OP0_0to5bits_63, // dsra32
                                            
};                              

InsDecoding::emulate opcode1_16to20_bits[] ={&InsDecoding::OP1_16to20bits_0, // bltz
                                             &InsDecoding::OP1_16to20bits_1, // bgez
                                             &InsDecoding::OP1_16to20bits_2, // bltzl
                                             &InsDecoding::OP1_16to20bits_3, // bgezl
                                             &InsDecoding::not_an_Ins,
                                             &InsDecoding::not_an_Ins,
                                             &InsDecoding::not_an_Ins,
                                             &InsDecoding::not_an_Ins,
                                             &InsDecoding::OP1_16to20bits_8, //tgei
                                             &InsDecoding::OP1_16to20bits_9, // tgeiu
                                             &InsDecoding::OP1_16to20bits_10, //tlti
                                             &InsDecoding::OP1_16to20bits_11, //tltiu
                                             &InsDecoding::OP1_16to20bits_12, // teqi
                                             &InsDecoding::not_an_Ins,
                                             &InsDecoding::OP1_16to20bits_14, // tnei
                                             &InsDecoding::not_an_Ins,
                                             &InsDecoding::OP1_16to20bits_16, // bltzal
                                             &InsDecoding::OP1_16to20bits_17, // bgezal
                                             &InsDecoding::OP1_16to20bits_18, // bltzall
                                             &InsDecoding::OP1_16to20bits_19, //bgezall
                                             &InsDecoding::not_an_Ins,
                                             &InsDecoding::not_an_Ins,
                                             &InsDecoding::not_an_Ins,
                                             &InsDecoding::not_an_Ins,
                                             &InsDecoding::not_an_Ins,
                                             &InsDecoding::not_an_Ins,
                                             &InsDecoding::not_an_Ins,
                                             &InsDecoding::not_an_Ins,
                                             &InsDecoding::not_an_Ins,
                                             &InsDecoding::not_an_Ins,
                                             &InsDecoding::not_an_Ins,
                                             &InsDecoding::OP1_16to20bits_31, //synci
};

InsDecoding::emulate opcode16_21to25_bits[] ={&InsDecoding::OP16_21to25bits_0, // mfc0
                                              &InsDecoding::OP16_21to25bits_1, // dmfc0
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::OP16_21to25bits_4, // mtco
                                              &InsDecoding::OP16_21to25bits_5, // dmtc0
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::OP16_21to25bits_10, // rdpgpr
                                              &InsDecoding::OP16_21to25bits_11, // di,ei
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::OP16_21to25bits_14, // wrpgpr
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
};

InsDecoding::emulate opcode16_0to5_bits[] ={  &InsDecoding::not_an_Ins,
                                              &InsDecoding::OP16_0to5bits_1, //tlbr
                                              &InsDecoding::OP16_0to5bits_2, // tlbwi
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::OP16_0to5bits_6, // tlbwr
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::OP16_0to5bits_8, // tlbp
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::OP16_0to5bits_24, // eret
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::OP16_0to5bits_31, // deret
                                              &InsDecoding::OP16_0to5bits_32, // wait
                                              
};

InsDecoding::emulate opcode28_0to5_bits[] ={  &InsDecoding::OP28_0to5bits_0,  //  madd
                                              &InsDecoding::OP28_0to5bits_1,  // maddu
                                              &InsDecoding::OP28_0to5bits_2,  // mul
                                              &InsDecoding::OP28_0to5bits_3, //dmul
                                              &InsDecoding::OP28_0to5bits_4,  //msub
                                              &InsDecoding::OP28_0to5bits_5,  //msubu
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::OP28_0to5bits_8, // mtm0
                                              &InsDecoding::OP28_0to5bits_9, // mtp0
                                              &InsDecoding::OP28_0to5bits_10, // mtp1
                                              &InsDecoding::OP28_0to5bits_11, // mtp2
                                              &InsDecoding::OP28_0to5bits_12, // mtm1
                                              &InsDecoding::OP28_0to5bits_13, // mtm2
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::OP28_0to5bits_15, //vmulu
                                              &InsDecoding::OP28_0to5bits_16, //vmm0
                                              &InsDecoding::OP28_0to5bits_17, //v3mulu
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::OP28_0to5bits_24, // saa
                                              &InsDecoding::OP28_0to5bits_25, // saad
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::OP28_0to5bits_32,  // clz
                                              &InsDecoding::OP28_0to5bits_33,  // clo
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::OP28_0to5bits_36,  // dclz
                                              &InsDecoding::OP28_0to5bits_37,  // dclo
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::OP28_0to5bits_40, // baddu
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::OP28_0to5bits_42, // seq
                                              &InsDecoding::OP28_0to5bits_43, // sne
                                              &InsDecoding::OP28_0to5bits_44, // pop
                                              &InsDecoding::OP28_0to5bits_45, // dpop
                                              &InsDecoding::OP28_0to5bits_46, // seqi
                                              &InsDecoding::OP28_0to5bits_47, // snei
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::OP28_0to5bits_50, // cins
                                              &InsDecoding::OP28_0to5bits_51, // cins32
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::OP28_0to5bits_58, //exts
                                              &InsDecoding::OP28_0to5bits_59, //exts32
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::OP28_0to5bits_63, //sdbbp
    
};

InsDecoding::emulate opcode31_0to5_bits[] = { &InsDecoding::OP31_0to5bits_0, //ext
                                              &InsDecoding::OP31_0to5bits_1, //dextm
                                              &InsDecoding::OP31_0to5bits_2, //dextu
                                              &InsDecoding::OP31_0to5bits_3, //dext
                                              &InsDecoding::OP31_0to5bits_4, //ins
                                              &InsDecoding::OP31_0to5bits_5, //dinsm
                                              &InsDecoding::OP31_0to5bits_6, //dinsu
                                              &InsDecoding::OP31_0to5bits_7, //dins
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::OP31_0to5bits_32, //seb,seh,wsbh
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::OP31_0to5bits_36, //dsbh,dshd
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::not_an_Ins,
                                              &InsDecoding::OP31_0to5bits_59, //rdhwr

};

void InsDecoding::findInstrCase(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
       
       (decode_funs.*opcode_funs_array[InsB->instr.opcode])(InsB,InsD,vec,blockEndPC);
}

void InsDecoding::Opcode_0(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
    
    //printf("opcode 0 ..goes in %d \n", InsB->func);
    (decode_funs.*opcode0_0to5_bits[InsB->instr.R.func])(InsB,InsD,vec,blockEndPC);
    
}

void InsDecoding::Opcode_1(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
    
    uint8_t b=  EXTBITS(InsB->encoding,0,5);
    uint8_t c= InsB->instr.I.rs;
  //  printf("opcode 1 ..rt %d  rs %d\n", InsB->instr.I.rt,InsB->instr.I.rs);
    if(b==58 & c==0){              // drotr
        InsB->instr.I.imm=0;
        InsB->instr.I.rs =0;
        InsB->instr.I.rt=0;
        InsB->type = RTYPE;
        InsB->instr.R.func = b;
        InsB->instr.R.rs = EXTBITS(InsB->encoding,21,25);   
        InsB->instr.R.rt = EXTBITS(InsB->encoding,16,20);   
        InsB->instr.R.rd = EXTBITS(InsB->encoding,11,15);   
        InsB->instr.R.shft = EXTBITS(InsB->encoding,6,10);           
        Translation_IPE::shift_R(InsB,vec);
    }else{
        (decode_funs.*opcode1_16to20_bits[InsB->instr.I.rt])(InsB,InsD,vec,blockEndPC);
    }
}

void InsDecoding::Opcode_2(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
    Translation_IPE::j(InsB, InsD, vec,blockEndPC);
}

void InsDecoding::Opcode_3(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
    Translation_IPE::jal(InsB, InsD, vec,blockEndPC);   // jal
}

void InsDecoding::Opcode_4(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  //  printf("opcode 4 , beq \n");
    Translation_IPE::bne_beq(InsB, InsD, vec);    // beq
}

void InsDecoding::Opcode_5(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  //  printf("Ins %llx opcode 5 , bne \n",InsB->encoding);
    Translation_IPE::bne_beq(InsB, InsD, vec);    // bne
}

void InsDecoding::Opcode_6(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   // printf("opcode 6 , blez  \n");
    Translation_IPE::branch(InsB, InsD, vec);    // blez
}

void InsDecoding::Opcode_7(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   // printf("opcode 7 , bgtz \n");
    Translation_IPE::branch(InsB, InsD, vec);    // bgtz
}

void InsDecoding::Opcode_8(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   // printf("opcode 8 , addi  \n");
    Translation_IPE::unprev_I(InsB, vec);    // addi
}

void InsDecoding::Opcode_9(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  //  printf("opcode 9 , addiu \n");
    Translation_IPE::unprev_I(InsB, vec);  // addiu
}

void InsDecoding::Opcode_10(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
//    printf("opcode 10 , slti \n");
    Translation_IPE::unprev_I(InsB, vec);    // slti
}

void InsDecoding::Opcode_11(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   // printf("opcode 11 , sltiu \n");
    Translation_IPE::unprev_I(InsB, vec);    // sltiu
}

void InsDecoding::Opcode_12(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  //  printf("opcode 12 , andi \n");
    Translation_IPE::unprev_I(InsB, vec);    // andi
}

void InsDecoding::Opcode_13(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  //  printf("opcode 13 , ori \n");
    Translation_IPE::unprev_I(InsB, vec);    // ori
}

void InsDecoding::Opcode_14(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   // printf("opcode 14 ,xori \n");
    Translation_IPE::unprev_I(InsB, vec);    // xori
}

void InsDecoding::Opcode_15(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  //  printf("opcode 15 , lui \n");
    Translation_IPE::unprev_I(InsB, vec);    // lui
}

void InsDecoding::Opcode_16(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  //  printf("opcode 16 ..rs= %d \n", InsB->rs);
    uint8_t a= InsB->instr.R.rs;
    if (a == 16){
        (decode_funs.*opcode16_0to5_bits[InsB->instr.R.func])(InsB,InsD,vec,blockEndPC);
    }else{
        (decode_funs.*opcode16_21to25_bits[a])(InsB,InsD,vec,blockEndPC);
    }
}

void InsDecoding::Opcode_18(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  //  printf("opcode 18 ..rs %d \n", InsB->rs);
  
    uint8_t a =InsB->instr.R.rs;
    if (a==1){          // dmfc2
        printf(" dmfc2 \n");
        Translation_IPE::nop(vec);
    }else
        if(a==5){       //dmtc2
        printf(" dmtc2 \n");
            Translation_IPE::nop(vec);
    }
}

void InsDecoding::Opcode_20(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
 //   printf("opcode 20 , beql \n");
    Translation_IPE::bne_beq_likely(InsB, InsD, vec);    // beql
}

void InsDecoding::Opcode_21(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  //  printf("opcode 21 , bnel \n");
    Translation_IPE::bne_beq_likely(InsB, InsD, vec);    // bnel
}

void InsDecoding::Opcode_22(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  //  printf("opcode 22 , blezl \n");
    Translation_IPE::branch_likely(InsB, InsD, vec);    // blezl
}

void InsDecoding::Opcode_23(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  //  printf("opcode 23 , bgtzl \n");
    Translation_IPE::branch_likely(InsB, InsD, vec);    // bgtzl
}

void InsDecoding::Opcode_24(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   // printf("opcode 24 , daddi \n");
    Translation_IPE::unprev_I(InsB, vec);    // daddi
}

void InsDecoding::Opcode_25(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   // printf("opcode 25 , daddiu \n");
    Translation_IPE::unprev_I(InsB, vec);    // daddiu
}

void InsDecoding::Opcode_26(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  //  printf("opcode 26 , ldl \n");
    Translation_IPE::unprev_I_Load(InsB, vec);    // ldl
}

void InsDecoding::Opcode_27(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   // printf("opcode 27 , ldr \n");
    Translation_IPE::unprev_I_Load(InsB, vec);    // ldr
}

void InsDecoding::Opcode_28(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  //  printf("Opcode_28 ..goes in %d \n",InsB->func);
    
    (decode_funs.*opcode28_0to5_bits[InsB->instr.R.func])(InsB,InsD,vec,blockEndPC);   
}

void InsDecoding::Opcode_31(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   // printf("Opcode_31 .. goes in %d \n", InsB->func);
    (decode_funs.*opcode31_0to5_bits[InsB->instr.R.func])(InsB,InsD,vec,blockEndPC);   
}

void InsDecoding::Opcode_32(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   // printf("opcode 32 , lb \n");
    Translation_IPE::unprev_I_Load(InsB, vec);    // lb
}

void InsDecoding::Opcode_33(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   // printf("opcode 33 , lh \n");
    Translation_IPE::unprev_I_Load(InsB, vec);    // lh
}

void InsDecoding::Opcode_34(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   // printf("opcode 34 , lwl \n");
    Translation_IPE::unprev_I_Load(InsB, vec);    // lwl
}

void InsDecoding::Opcode_35(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   // printf("opcode 35 ,lw \n");
    Translation_IPE::unprev_I_Load(InsB, vec);    // lw
}

void InsDecoding::Opcode_36(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
    //lprintf("opcode 36 , lbu \n");
    Translation_IPE::unprev_I_Load(InsB, vec);    // lbu
}

void InsDecoding::Opcode_37(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   // printf("opcode 37, lhu  \n");
    Translation_IPE::unprev_I_Load(InsB, vec);    // lhu
}

void InsDecoding::Opcode_38(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   // printf("opcode 38 , lwr \n");
    Translation_IPE::unprev_I_Load(InsB, vec);    // lwr
}

void InsDecoding::Opcode_39(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   // printf("opcode 39 , lwu \n");
    Translation_IPE::unprev_I_Load(InsB, vec);    // lwu
}

void InsDecoding::Opcode_40(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   // printf("opcode 40 ,sb \n");
    Translation_IPE::unprev_I_Store(InsB, vec);    // sb
}

void InsDecoding::Opcode_41(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
    //printf("opcode 41 , sh \n");
    Translation_IPE::unprev_I_Store(InsB, vec);    // sh
}

void InsDecoding::Opcode_42(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
    //printf("opcode 42 ,swl \n");
    Translation_IPE::unprev_I_Store(InsB, vec);    // swl
}

void InsDecoding::Opcode_43(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
    //printf("opcode 43 , sw \n");
    Translation_IPE::unprev_I_Store(InsB, vec);    // sw
}

void InsDecoding::Opcode_44(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
    //printf("opcode 44 , sdl \n");
    Translation_IPE::unprev_I_Store(InsB, vec);    // sdl
}
void InsDecoding::Opcode_45(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
    //printf("opcode 45 , sdr \n");
    Translation_IPE::unprev_I_Store(InsB, vec);    // sdr
}

void InsDecoding::Opcode_46(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   // printf("opcode 46 ,swr \n");
    Translation_IPE::unprev_I_Store(InsB, vec);    // swr
}

void InsDecoding::Opcode_47(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
    //printf("opcode 47 , cache \n");
    Translation_IPE::cache( vec);    // cache
}

void InsDecoding::Opcode_48(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
    //printf("opcode 48 ,ll \n");
    Translation_IPE::ll(InsB, vec);    // ll
}

void InsDecoding::Opcode_50(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
    //printf("opcode 50 ,bbit0 \n");
    Translation_IPE::bbit(InsB, InsD, vec);    // bbit0
}

void InsDecoding::Opcode_51(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   // printf("opcode 51 ,pref \n");
    Translation_IPE::pref( vec);    //pref
}

void InsDecoding::Opcode_52(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   // printf("opcode 52 , lld \n");
    Translation_IPE::lld(InsB, vec);    // lld
}

void InsDecoding::Opcode_54(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
    //printf("opcode 54 , bbit032 \n");
    Translation_IPE::bbit(InsB, InsD, vec);    // bbit032
}

void InsDecoding::Opcode_55(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   // printf("opcode 55 ,ld \n");
    Translation_IPE::unprev_I_Load(InsB, vec);    // ld
}

void InsDecoding::Opcode_56(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   // printf("opcode 56 ,sc \n");
    Translation_IPE::sc(InsB, vec);    // sc
}

void InsDecoding::Opcode_58(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   // printf("opcode 58 ,bbit1 \n");
    Translation_IPE::bbit(InsB,InsD, vec);    // bbit1
}

void InsDecoding::Opcode_59(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
//    printf("opcode 59 ,Block Termination \n");
    Translation_IPE::TerminateBlock(vec);    // Termination for block span over multiple pages
}

void InsDecoding::Opcode_60(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
    //printf("opcode 60 ,scd  \n");
    Translation_IPE::scd(InsB, vec);    // scd
}

void InsDecoding::Opcode_62(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   // printf("opcode 62 ,bbit32 \n");
    Translation_IPE::bbit(InsB,InsD, vec);    // bbit132
}

void InsDecoding::Opcode_63(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  //  printf("opcode 63 ,sd \n");
    Translation_IPE::unprev_I_Store(InsB, vec);    // sd
}

void InsDecoding::not_an_Ins(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t blockEndPC){
    printf("Not an instruction:\n");
    (InsB->type == RTYPE) ? InsB->printRType(InsB):InsB->printIType(InsB);
    exit(1);
}


/*----------------------------------------------------------------------------------*/

void InsDecoding::OP0_0to5bits_0(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   
    //printf("instruction %lx \n", InsB->encoding);
    if(InsB->encoding == 0){           //nop
        //printf("nop  \n");
        Translation_IPE::nop(vec);
    }else
        if(InsB->instr.R.rs == 0 && InsB->instr.R.rd == 0 && InsB->instr.R.rt == 0){
            uint8_t a = InsB->instr.R.shft;
            if(a == 1){
                // printf(" ssnop \n");
                Translation_IPE::ssnop(vec);
            }
            else 
                if (a == 3){
                    // printf(" ehb \n");
                    Translation_IPE::nop(vec);
                }
        }
    /*if(InsB->encoding == 0){           //nop
        //printf("nop  \n");
        Translation_IPE::nop(vec);
    }else
       if(InsB->rs == 0 && InsB->rd == 0 && InsB->rt == 0 && a == 1){      //snnop
          // printf(" ssnop \n");
           Translation_IPE::ssnop(vec);
       }else if(InsB->rs == 0 && InsB->rd == 0 && InsB->rt == 0 && a == 3){      //ehb
          // printf(" ehb \n");
           Translation_IPE::nop(vec);
       }*/else            //sll
          {
           //printf("sll  \n"); 
           Translation_IPE::shift_R(InsB,vec); } 
    
}

void InsDecoding::OP0_0to5bits_2(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   
    /*uint8_t a = InsB->rs;
    if(a==0){           //srl
     //   printf(" srl \n");  
    }else
       if(a == 1){      //rotr
     //      printf(" rotr \n");
       }*/
    Translation_IPE::shift_R(InsB, vec);
}

void InsDecoding::OP0_0to5bits_3(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   // printf(" sra \n");      
    Translation_IPE::shift_R(InsB, vec); //sra
}

void InsDecoding::OP0_0to5bits_4(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  //  printf(" sllv \n");      
    Translation_IPE::unprev_R(InsB, vec); //sllv
}

void InsDecoding::OP0_0to5bits_6(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   
    /*uint8_t a = InsB->sa;
    if(a==0){           //srlv
   //     printf(" srlv \n");
    }else
       if(a == 1){      //rotrv
    //       printf(" rotrv \n");
       }    */ 
    Translation_IPE::unprev_R(InsB, vec);
}

void InsDecoding::OP0_0to5bits_7(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  //  printf(" srav \n");      
    Translation_IPE::unprev_R(InsB, vec); //srav
}

void InsDecoding::OP0_0to5bits_8(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
 //  printf(" jr \n");
            //jr s
       Translation_IPE::jr(InsB, InsD, vec);
}

void InsDecoding::OP0_0to5bits_9(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
            //jalr s
   // printf("jalr \n");
        Translation_IPE::jalr(InsB, InsD, vec);
}

void InsDecoding::OP0_0to5bits_10(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
 //printf(" movz \n");
    Translation_IPE::unprev_R(InsB, vec); //movz
}

void InsDecoding::OP0_0to5bits_11(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   // printf(" movn \n");
    Translation_IPE::unprev_R(InsB, vec); //movn
}

void InsDecoding::OP0_0to5bits_12(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf("syscall  \n");
        Translation_IPE::syscall( vec); // syscall    
}

void InsDecoding::OP0_0to5bits_13(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" break \n");
        Translation_IPE::br( vec); //break    
}

void InsDecoding::OP0_0to5bits_14(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" sdbbp \n");
        Translation_IPE::nop( vec); // sdbbp   
}

void InsDecoding::OP0_0to5bits_15(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
 //  printf(" sync \n");
    uint8_t a = InsB->instr.R.shft;
    if(a==0){
        Translation_IPE::sync( vec); //sync 
    }else if(a==2){     //synciobdma
        Translation_IPE::nop(vec);
        printf("SyncIOBDMA\t-----operation required-----\n");
    }else{              //syncs
      //  printf("Syncs\n");
        Translation_IPE::nop(vec);
    }
}

void InsDecoding::OP0_0to5bits_16(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" mfhi \n");
        Translation_IPE::moveFromLoHi_R(InsB, vec); //mfhi    
}

void InsDecoding::OP0_0to5bits_17(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" mthi \n");
        Translation_IPE::moveToLoHi_R(InsB,vec);    // mthi
}

void InsDecoding::OP0_0to5bits_18(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" mflo \n");
        Translation_IPE::moveFromLoHi_R(InsB, vec);    //mflo
}

void InsDecoding::OP0_0to5bits_19(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" mtlo \n");
        Translation_IPE::moveToLoHi_R( InsB, vec);  //mtlo   
}

void InsDecoding::OP0_0to5bits_20(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" dsllv \n");
        Translation_IPE::unprev_R( InsB,vec); //dsllv    
}

void InsDecoding::OP0_0to5bits_22(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   
  
    /*uint8_t a = InsB->sa;
    if(a==0){           //dsrlv
        printf("dsrlv  \n");
        
    }else
       if(a == 1){      // drotrv 
        printf(" drotrv \n");
       }  */   
    Translation_IPE::unprev_R(InsB, vec);
}

void InsDecoding::OP0_0to5bits_23(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" dsrav \n");
        Translation_IPE::unprev_R( InsB, vec); //dsrav    
}

void InsDecoding::OP0_0to5bits_24(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf("  mult \n");
        Translation_IPE::mulDiv_R( InsB, vec);  //mult    
}

void InsDecoding::OP0_0to5bits_25(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" multu \n");
        Translation_IPE::mulDiv_R( InsB, vec); //multu    
}

void InsDecoding::OP0_0to5bits_26(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" div \n");
        Translation_IPE::mulDiv_R(InsB, vec); // div   
}

void InsDecoding::OP0_0to5bits_27(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" divu \n");
        Translation_IPE::mulDiv_R(InsB, vec); //divu    
}

void InsDecoding::OP0_0to5bits_28(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" dmult \n");
        Translation_IPE::mulDiv_R(InsB, vec); //dmult    
}

void InsDecoding::OP0_0to5bits_29(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" dmultu \n");
        Translation_IPE::mulDiv_R(InsB, vec); //dmultu    
}

void InsDecoding::OP0_0to5bits_30(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" ddiv \n");
        Translation_IPE::mulDiv_R(InsB, vec); //ddiv    
}

void InsDecoding::OP0_0to5bits_31(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" ddivu \n");
        Translation_IPE::mulDiv_R(InsB, vec); //ddivu    
}

void InsDecoding::OP0_0to5bits_32(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" add \n");
        Translation_IPE::unprev_R(InsB, vec); //add    
}

void InsDecoding::OP0_0to5bits_33(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" addu \n");
        Translation_IPE::unprev_R(InsB, vec); //addu    
}

void InsDecoding::OP0_0to5bits_34(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" sub \n");
        Translation_IPE::unprev_R(InsB, vec); // sub    
}

void InsDecoding::OP0_0to5bits_35(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" subu \n");
        Translation_IPE::unprev_R(InsB, vec); //subu    
}

void InsDecoding::OP0_0to5bits_36(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" and \n");
        Translation_IPE::unprev_R(InsB, vec); // and    
}

void InsDecoding::OP0_0to5bits_37(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" or \n");
        Translation_IPE::unprev_R(InsB, vec);  //or    
}

void InsDecoding::OP0_0to5bits_38(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" xor \n");
        Translation_IPE::unprev_R(InsB, vec); // xor    
}

void InsDecoding::OP0_0to5bits_39(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" nor \n");
        Translation_IPE::unprev_R(InsB, vec); // nor    
}

void InsDecoding::OP0_0to5bits_42(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" slt \n");
        Translation_IPE::unprev_R( InsB, vec); //slt    
}

void InsDecoding::OP0_0to5bits_43(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" sltu \n");
        Translation_IPE::unprev_R( InsB, vec); //sltu   
}

void InsDecoding::OP0_0to5bits_44(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" dadd \n");
        Translation_IPE::unprev_R(InsB, vec); // dadd   
}

void InsDecoding::OP0_0to5bits_45(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
    //printf(" daddu \n");
        Translation_IPE::unprev_R(InsB, vec); // daddu    
}

void InsDecoding::OP0_0to5bits_46(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" dsub \n");
        Translation_IPE::unprev_R(InsB, vec); // dsub    
}

void InsDecoding::OP0_0to5bits_47(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" dsubu \n");
        Translation_IPE::unprev_R(InsB, vec); // dsubu    
}

void InsDecoding::OP0_0to5bits_48(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" tge \n");
        Translation_IPE::tge_tgeu_tlt_tltu_R(InsB, vec); //tge    
}

void InsDecoding::OP0_0to5bits_49(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" tgeu \n");
        Translation_IPE::tge_tgeu_tlt_tltu_R(InsB, vec);    // tgeu
}

void InsDecoding::OP0_0to5bits_50(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" tlt \n");
        Translation_IPE::tge_tgeu_tlt_tltu_R(InsB, vec);    //tlt
}

void InsDecoding::OP0_0to5bits_51(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" tltu \n");
        Translation_IPE::tge_tgeu_tlt_tltu_R(InsB, vec);    //tltu
}

void InsDecoding::OP0_0to5bits_52(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" teq \n");
        Translation_IPE::teq_tne_R(InsB, vec);    // teq
}

void InsDecoding::OP0_0to5bits_54(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" tne \n");
        Translation_IPE::teq_tne_R(InsB, vec); // tne   
}

void InsDecoding::OP0_0to5bits_56(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" dsll \n");
        Translation_IPE::shift_R(InsB, vec);  //dsll   
}

void InsDecoding::OP0_0to5bits_58(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" dsrl \n");
        Translation_IPE::shift_R(InsB, vec); //dsrl    
}

void InsDecoding::OP0_0to5bits_59(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" dsra \n");
        Translation_IPE::shift_R( InsB, vec); //dsra    
}

void InsDecoding::OP0_0to5bits_60(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" dsll32 \n");
        Translation_IPE::shift_R(InsB, vec); //dsll32   
}

void InsDecoding::OP0_0to5bits_62(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   // printf(" dsrl32 \n");
        Translation_IPE::shift_R(InsB, vec); //dsrl32    
}

void InsDecoding::OP0_0to5bits_63(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" dsra32 \n");
        Translation_IPE::shift_R(InsB, vec); //dsra32    
}

void InsDecoding::OP1_16to20bits_0(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" bltz \n");
        Translation_IPE::branch(InsB, InsD, vec); // bltz 
}

void InsDecoding::OP1_16to20bits_1(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" bgez \n");
        Translation_IPE::branch(InsB, InsD, vec); // bgez 
}

void InsDecoding::OP1_16to20bits_2(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" bltzl \n");
        Translation_IPE::branch_likely(InsB, InsD, vec); // bltzl 
}

void InsDecoding::OP1_16to20bits_3(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" bgezl \n");
        Translation_IPE::branch_likely(InsB, InsD, vec); // bgezl 
}

void InsDecoding::OP1_16to20bits_8(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" tgei \n");
        Translation_IPE::tgei_tgeiu_tlti_tltiu_I(InsB, vec); // tgei 
}

void InsDecoding::OP1_16to20bits_9(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" tgeiu \n");
        Translation_IPE::tgei_tgeiu_tlti_tltiu_I(InsB, vec); // tegiu 
}

void InsDecoding::OP1_16to20bits_10(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" tlti \n");
        Translation_IPE::tgei_tgeiu_tlti_tltiu_I(InsB, vec); // tlti 
}

void InsDecoding::OP1_16to20bits_11(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" tltiu \n");
        Translation_IPE::tgei_tgeiu_tlti_tltiu_I(InsB, vec); // tltiu 
}

void InsDecoding::OP1_16to20bits_12(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" teqi \n");
        Translation_IPE::teqi_tnei_I(InsB, vec); // teqi 
}

void InsDecoding::OP1_16to20bits_14(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" tnei \n");
        Translation_IPE::teqi_tnei_I(InsB, vec); // tnei 
}

void InsDecoding::OP1_16to20bits_16(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" bltzal \n");
        Translation_IPE::branch(InsB, InsD, vec); // bltzal 
}

void InsDecoding::OP1_16to20bits_17(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
     //   printf(" bal or bgezal\n");
        Translation_IPE::branch(InsB, InsD, vec); // bgezal         
}

void InsDecoding::OP1_16to20bits_18(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" bltzall \n");
        Translation_IPE::branch_likely(InsB, InsD, vec); // bltzall 
}

void InsDecoding::OP1_16to20bits_19(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" bgezall \n");
        Translation_IPE::branch_likely(InsB, InsD, vec); // bgezall 
}


void InsDecoding::OP1_16to20bits_31(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" synci \n");
        Translation_IPE::nop( vec); // synci 
}

void InsDecoding::OP16_0to5bits_1(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" tlbr \n");
    Translation_IPE::tlbr(vec); // tlbr
}

void InsDecoding::OP16_0to5bits_2(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" tlbwi \n");
    Translation_IPE::tlbwi(vec); // tlbwi
}

void InsDecoding::OP16_0to5bits_6(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" tlbwr \n");
    Translation_IPE::tlbwr(vec); // tlbwr
}

void InsDecoding::OP16_0to5bits_8(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" tlbp \n");
    Translation_IPE::tlbp(vec); // tlbp
}

void InsDecoding::OP16_0to5bits_24(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" eret \n");
    Translation_IPE::eret(vec); // eret
}

void InsDecoding::OP16_0to5bits_31(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" deret \n");
    Translation_IPE::deret(vec); // deret
}

void InsDecoding::OP16_21to25bits_0(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" mfc0 \n");
    Translation_IPE::mfc0(InsB, vec); // mfc0
}

void InsDecoding::OP16_21to25bits_1(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" dmfc0 \n");
    Translation_IPE::mfc0(InsB, vec); // dmfc0
}

void InsDecoding::OP16_21to25bits_4(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" mtc0 \n");
    Translation_IPE::mtc0(InsB, vec); // mtc0
}

void InsDecoding::OP16_21to25bits_5(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" dmtc0 \n");
    Translation_IPE::mtc0(InsB, vec); // dmtc0
}

void InsDecoding::OP16_21to25bits_10(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
//   printf(" rdpgpr Ins = 0x%016lx\n",InsB->encoding);
    Translation_IPE::nop( vec); // rdpgpr
}

void InsDecoding::OP16_21to25bits_11(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  
    uint8_t a= InsB->instr.R.func;
    if(a==0){            // di
   // printf(" di \n");
        Translation_IPE::di(InsB, vec); 
    }else 
        if(a==32){       //ei
    //        printf(" ei \n");
            Translation_IPE::ei(InsB, vec);
        }
}

void InsDecoding::OP16_21to25bits_14(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   printf(" wrpgpr \n");
    Translation_IPE::nop( vec); // wrpgpr
}
void InsDecoding::OP16_0to5bits_32(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" wait \n");
    Translation_IPE::wait(vec); // wait
}
void InsDecoding::OP28_0to5bits_0(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" madd \n");
    Translation_IPE::madd( InsB, vec);  // madd
}

void InsDecoding::OP28_0to5bits_1(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" maddu \n");
    Translation_IPE::madd( InsB, vec);  // maddu
}

void InsDecoding::OP28_0to5bits_2(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" mul \n");
    Translation_IPE::unprev_R( InsB, vec);  // mul
}

void InsDecoding::OP28_0to5bits_3(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" dmul \n");
    Translation_IPE::unprev_R( InsB, vec);  // dmul
}

void InsDecoding::OP28_0to5bits_4(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
 //  printf(" msub \n");
    Translation_IPE::madd( InsB, vec);  // msub
}

void InsDecoding::OP28_0to5bits_5(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" msubu \n");
    Translation_IPE::madd( InsB, vec);  // msubu
}

void InsDecoding::OP28_0to5bits_8(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" mtm0 \n");
   Translation_IPE::mtm0( InsB, vec);  // mtm0
}

void InsDecoding::OP28_0to5bits_9(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
//   printf(" mtp0 \n");
    Translation_IPE::mtp0( InsB, vec);  // mtp0
}

void InsDecoding::OP28_0to5bits_10(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" mtp1 \n");
    Translation_IPE::mtp1( InsB, vec);  // mtp1
}

void InsDecoding::OP28_0to5bits_11(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" mtp2 \n");
    Translation_IPE::mtp2( InsB, vec);  // mtp2
}

void InsDecoding::OP28_0to5bits_12(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" mtm1 \n");
    Translation_IPE::mtm1( InsB, vec);  // mtm1
}

void InsDecoding::OP28_0to5bits_13(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" mtm2 \n");
    Translation_IPE::mtm2( InsB, vec);  // mtm2
}


void InsDecoding::OP28_0to5bits_15(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   printf(" vmulu \n");
 //  Translation_IPE::unprev_R( InsB, vec);  // vmulu
}

void InsDecoding::OP28_0to5bits_16(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   printf(" vmm0 \n");
  //  Translation_IPE::unprev_R( InsB, vec);  // vmm0
}

void InsDecoding::OP28_0to5bits_17(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" v3mulu \n");
    Translation_IPE::v3mulu( InsB, vec);  // v3mulu
}

void InsDecoding::OP28_0to5bits_24(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" saa \n");
    Translation_IPE::saa( InsB, vec);  // saa
}

void InsDecoding::OP28_0to5bits_25(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" saad \n");
    Translation_IPE::saad( InsB, vec);  // saad
}

void InsDecoding::OP28_0to5bits_32(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" clz \n");
    Translation_IPE::unprev_R( InsB, vec);  // clz
}

void InsDecoding::OP28_0to5bits_33(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" clo \n");
    Translation_IPE::unprev_R( InsB, vec);  // clo
}

void InsDecoding::OP28_0to5bits_36(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" dclz \n");
    Translation_IPE::unprev_R( InsB, vec);  // dclz
}

void InsDecoding::OP28_0to5bits_37(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" dclo \n");
    Translation_IPE::unprev_R( InsB, vec);  // dclo
}

void InsDecoding::OP28_0to5bits_40(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" baddu \n");
    Translation_IPE::unprev_R( InsB, vec);  // baddu
}

void InsDecoding::OP28_0to5bits_42(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" seq \n");
    Translation_IPE::unprev_R( InsB, vec);  // seq
}

void InsDecoding::OP28_0to5bits_43(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" sne \n");
    Translation_IPE::unprev_R( InsB, vec);  // sne
}

void InsDecoding::OP28_0to5bits_44(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" pop \n");
    Translation_IPE::unprev_R( InsB, vec);  // pop
}

void InsDecoding::OP28_0to5bits_45(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" dpop \n");
    Translation_IPE::unprev_R( InsB, vec);  // dpop
}

void InsDecoding::OP28_0to5bits_46(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" seqi \n");
    Translation_IPE::seqi_snei( InsB, vec);  // seqi
}

void InsDecoding::OP28_0to5bits_47(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" snei \n");
    Translation_IPE::seqi_snei( InsB, vec);  // snei
}

void InsDecoding::OP28_0to5bits_50(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" cins \n");
    Translation_IPE::ins_R( InsB, vec);  // cins
}

void InsDecoding::OP28_0to5bits_51(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" cins32 \n");
    Translation_IPE::ins_R( InsB, vec);  // cins32
}

void InsDecoding::OP28_0to5bits_58(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" exts \n");
    Translation_IPE::ext_R( InsB, vec);  // exts
}

void InsDecoding::OP28_0to5bits_59(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  // printf(" exts32 \n");
    Translation_IPE::ext_R( InsB, vec);  // exts32
}

void InsDecoding::OP28_0to5bits_63(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   printf(" sdbbp \n");
    Translation_IPE::nop(  vec);  // sdbbp
}

void InsDecoding::OP31_0to5bits_0(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" ext \n");
    Translation_IPE::ext_R(InsB,  vec);  // ext
}

void InsDecoding::OP31_0to5bits_1(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" dextm \n");
    Translation_IPE::ext_R( InsB, vec);  // dextm
}

void InsDecoding::OP31_0to5bits_2(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" dextu \n");
    Translation_IPE::ext_R( InsB, vec);  // dextu
}

void InsDecoding::OP31_0to5bits_3(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" dext \n");
    Translation_IPE::ext_R( InsB, vec);  // dext
}

void InsDecoding::OP31_0to5bits_4(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" ins \n");
    Translation_IPE::ins_R( InsB, vec);  // ins
}

void InsDecoding::OP31_0to5bits_5(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" dinsm \n");
    Translation_IPE::ins_R( InsB, vec);  // dinsm
}

void InsDecoding::OP31_0to5bits_6(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" dinsu \n");
    Translation_IPE::ins_R( InsB, vec);  // dinsu
}

void InsDecoding::OP31_0to5bits_7(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   //printf(" dins \n");
    Translation_IPE::ins_R(InsB,  vec);  // dins
}

void InsDecoding::OP31_0to5bits_32(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
   // uint32_t a  = BitMan::extractBits(InsB->encoding, 6, 10);
    /*uint8_t a= InsB->sa;
    if(a == 2){         // wsbh
  //   printf(" wsbh \n");
    }else
        if(a == 16){        // seb
       // printf(" seb \n");   
        }else
            if(a == 24){        //seh
      //          printf(" seh \n");
            }*/
    Translation_IPE::unprev_R(InsB,  vec);
}

void InsDecoding::OP31_0to5bits_36(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
  //  uint32_t a= BitMan::extractBits(InsB->encoding, 6,10);
    /*uint8_t a= InsB->sa;
    if(a == 2){         // dsbh
        printf(" dsbh \n");
        
    }else
        if(a == 5){     //dshd
            printf(" dshd \n");
        }*/
    Translation_IPE::unprev_R(InsB, vec);
}

void InsDecoding::OP31_0to5bits_59(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t blockEndPC){
//   printf(" rdhwr \n");
    Translation_IPE::rdhwr( InsB,  vec);  // rdhwr
}

/* 
 * File:   InsDecoding.h
 * Author: asadusmani
 *
 * Created on March 27, 2014, 2:23 PM
 */

#ifndef INSDECODING_H
#define	INSDECODING_H

#include <vector>
#include "InstBlock.h"
#include "Translation_IPE.h"


class InsDecoding {
public:
    InsDecoding();
    InsDecoding(const InsDecoding& orig);
    virtual ~InsDecoding();

    typedef void (InsDecoding::*emulate) (IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);

    void findInstrCase(IInfo *InsB, IInfo *InsD, TranslationBlock & vec, uint64_t);
    void Opcode_0(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_1(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_2(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_3(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_4(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_5(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_6(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_7(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_8(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_9(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_10(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_11(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_12(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_13(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);   
    void Opcode_14(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_15(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_16(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_18(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_20(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_21(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_22(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_23(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_24(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_25(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_26(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_27(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_28(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_31(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_32(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_33(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_34(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_35(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_36(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_37(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_38(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_39(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_40(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_41(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_42(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_43(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_44(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_45(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_46(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_47(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_48(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_50(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_51(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_52(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_54(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_55(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_56(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_58(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_59(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_60(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_62(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void Opcode_63(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    
    
    void OP0_0to5bits_0(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_2(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_3(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_4(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_6(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_7(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_8(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_9(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_10(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_11(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_12(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_13(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_14(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_15(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_16(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_17(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_18(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_19(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_20(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_22(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_23(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_24(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_25(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_26(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_27(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_28(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_29(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_30(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_31(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_32(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_33(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_34(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_35(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_36(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_37(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_38(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_39(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_42(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_43(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_44(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_45(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_46(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_47(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_48(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_49(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_50(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_51(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_52(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_54(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_56(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_58(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_59(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_60(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_62(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP0_0to5bits_63(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
   
    void OP1_16to20bits_0(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP1_16to20bits_1(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP1_16to20bits_2(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP1_16to20bits_3(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP1_16to20bits_8(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP1_16to20bits_9(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP1_16to20bits_10(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP1_16to20bits_11(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP1_16to20bits_12(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP1_16to20bits_14(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP1_16to20bits_16(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP1_16to20bits_17(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP1_16to20bits_18(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP1_16to20bits_19(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP1_16to20bits_31(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
   
    void OP16_21to25bits_0(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP16_21to25bits_1(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP16_21to25bits_4(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP16_21to25bits_5(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP16_21to25bits_10(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP16_21to25bits_11(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP16_21to25bits_14(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
     
    void OP16_0to5bits_1(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP16_0to5bits_2(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP16_0to5bits_6(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP16_0to5bits_8(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP16_0to5bits_24(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP16_0to5bits_31(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP16_0to5bits_32(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);   
   
    void OP28_0to5bits_0(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP28_0to5bits_1(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP28_0to5bits_2(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP28_0to5bits_3(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP28_0to5bits_4(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP28_0to5bits_5(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP28_0to5bits_8(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP28_0to5bits_9(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP28_0to5bits_10(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP28_0to5bits_11(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP28_0to5bits_12(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP28_0to5bits_13(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP28_0to5bits_15(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP28_0to5bits_16(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP28_0to5bits_17(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP28_0to5bits_24(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP28_0to5bits_25(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP28_0to5bits_32(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP28_0to5bits_33(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP28_0to5bits_36(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP28_0to5bits_37(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP28_0to5bits_40(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP28_0to5bits_42(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP28_0to5bits_43(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP28_0to5bits_44(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP28_0to5bits_45(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP28_0to5bits_46(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP28_0to5bits_47(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP28_0to5bits_50(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP28_0to5bits_51(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP28_0to5bits_58(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP28_0to5bits_59(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP28_0to5bits_63(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    
    void OP31_0to5bits_0(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP31_0to5bits_1(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP31_0to5bits_2(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP31_0to5bits_3(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP31_0to5bits_4(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP31_0to5bits_5(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP31_0to5bits_6(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP31_0to5bits_7(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP31_0to5bits_32(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP31_0to5bits_36(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    void OP31_0to5bits_59(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    
    
    void not_an_Ins(IInfo *InsB, IInfo *InsD, TranslationBlock &, uint64_t);
    
private:
    //static emulate function_array[];
};

#endif	/* INSDECODING_H */


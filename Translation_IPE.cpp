//#include "Translation.h"

#include "Translation_IPE.h"

InsDecoding DelaySlotObj;

#define encodeI(op,rs,rt,imm) \
                (((op & 0x3f) << 26) | ((rs & 0x1f) << 21) \
                | ((rt & 0x1f) << 16) | (imm & 0xffff))
#define encodeR(op,rs,rt,rd,sa,func) \
                (((op & 0x3f) << 26) | ((rs & 0x1f) << 21) \
                | ((rt & 0x1f) << 16) | ((rd & 0x1f) << 11) \
                | ((sa & 0x1f) << 6) | (func & 0x3f))

void Translation_IPE::add_Log(TranslationBlock &transIns ) {
    transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP1, Control_from_LogWrite));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, Control_Mark));
    
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, C_LABEL_REG, C_FUNC_CALL_OFFSET));
    transIns.push_back(encodeR(OPCODE_ZERO, C_LABEL_REG,0,RET_ADDR_REG,0,9));
    transIns.push_back(0);
}

void Translation_IPE::update_pc(TranslationBlock &transIns ) {
    
//    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, PC));
//    transIns.push_back(encodeI(OPCODE_DADDI, TEMP1, TEMP1, 4));
//    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, PC ));
    transIns.push_back(encodeI(OPCODE_ADDI, 0, TEMP1, 4));
    transIns.push_back(encodeR(OPCODE_SPECIAL2, BASE_REG, TEMP1, 0, 0, OPCODE_SAAD ));
}
void Translation_IPE::checking_previlige(TranslationBlock &transIns){
    
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseCP + (64 * 12)+(8 * 0)))); //reading status   
    transIns.push_back(encodeI(OPCODE_ANDI, TEMP1, TEMP2, 0x1E)); //branch to instruction's translation ksu bits is enabled
    transIns.push_back(encodeI(14,TEMP2,TEMP2,0x10)); //xori with 0x10
    
//    transIns.push_back(encodeI(OPCODE_ANDI, TEMP1, TEMP2, 0x18)); //branch to instruction's translation ksu bits is enabled
//    transIns.push_back(encodeI(OPCODE_BEQ, 0, TEMP2, 12)); 
//    transIns.push_back(0); 
//    transIns.push_back(encodeI(OPCODE_ANDI, TEMP1, TEMP2, 0x04)); //branch to instruction's translation erl is enabled
//    transIns.push_back(encodeI(OPCODE_BNE, 0, TEMP2, 9)); 
//    transIns.push_back(0);
//    transIns.push_back(encodeI(OPCODE_ANDI, TEMP1, TEMP2, 0x02)); //branch to instruction's translation exl is enabled
    transIns.push_back(encodeI(OPCODE_BNE, 0, TEMP2, 6)); 
    transIns.push_back(0);
    transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP3, COntrol_from_RI));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP3, Control_Mark));                                                                  // storing control mark for cpu not enabled  
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, C_LABEL_REG, C_FUNC_CALL_OFFSET));
    transIns.push_back(encodeR(OPCODE_ZERO, C_LABEL_REG,0,RET_ADDR_REG,0,9));
    transIns.push_back(0);
}

void Translation_IPE::mfc0(IInfo *InsP, TranslationBlock &transIns ) {
    unsigned long source, dest;
    int sel;
    //translated instrution would be
    // lw $13, rs
    // sw $13, rd

    source = InsP->instr.R.rd;
    dest = InsP->instr.R.rt;
    sel = (InsP->instr.R.func) & 0x07;
    checking_previlige(transIns);
    
    if((source == 9) && ((sel==0)||(sel==6))){  // when either of cvmcount or count register is read
//        printf("Count read sel = %d\n",sel);
        transIns.push_back(encodeR(OPCODE_SPECIAL3, 0,TEMP1,31,0,59)); //rdhwr instruction to read hardware cvm count register.
        transIns.push_back(encodeR(0,TEMP1,TEMP1,TEMP1,0,45)); //multipling with 2 and diving by 3 to make the host clock to guest clock
        transIns.push_back(encodeI(OPCODE_ORI,0,TEMP2, 3));  // dividing the read value by time_factor
        transIns.push_back(encodeR(OPCODE_ZERO,TEMP1,TEMP2,0,0,31));
        transIns.push_back(encodeR(0,0,0,TEMP1,0,18));
        transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64*9)+(8 *6))));
         transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64*9)+(8 *0))));
    }
    
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseCP + (64 * source)+(8 * sel))));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseGP + 8 * (dest))));
}

void Translation_IPE::mtc0(IInfo *InsP,TranslationBlock &transIns ) {

   unsigned long source, dest;
   int sel;
  
    source = InsP->instr.R.rt;
    dest = InsP->instr.R.rd;
    sel = (InsP->instr.R.func) & 0x07;
checking_previlige(transIns);
 switch(dest){
     case 0: case 1:
            if(sel==0){         // index
                transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));           
                transIns.push_back(encodeI(OPCODE_ADDI, 0, TEMP2, 0x003f));
                transIns.push_back(encodeR(OPCODE_ZERO, TEMP2, TEMP1, TEMP1, 0, 36));
                transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * dest)+(8 * sel)))); 
            }else{
            transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
            transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * dest)+(8 * sel))));        
            }
            break;
       case 2: case 3:         //entryLo0 and entryLo1
           transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
           if(sel == 0)
           {
               transIns.push_back(encodeR(OPCODE_SPECIAL3, 0, TEMP1, 31, 30, OPCODE_INS));
               transIns.push_back(encodeR(OPCODE_SPECIAL3, 0, TEMP1, 31, 0, OPCODE_DINSU));
           }
           transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * dest)+(8 * sel)))); 
            break; 
        case 4:
            if(sel==0){         // context reg
                transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
                transIns.push_back(encodeI(OPCODE_LUI, 0, TEMP2, 0xffff));
                transIns.push_back(encodeI(OPCODE_ADDI, TEMP2, TEMP2, 0xfff0));
                transIns.push_back(encodeR(OPCODE_ZERO, 0, TEMP2, TEMP2, 32, 60));   // giving sign extension
                transIns.push_back(encodeR(OPCODE_ZERO, 0, TEMP2, TEMP2, 32, 63));
                transIns.push_back(encodeR(OPCODE_ZERO, TEMP2, TEMP1, TEMP1, 0, 36));
                transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * dest)+(8 * sel)))); 
                //a &= 0xfffffffffffffff0;
            }else{
            transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
            transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * dest)+(8 * sel))));        
            }
            break;
        case 5:
            transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
            if(sel==0){         //PageMask reg
                transIns.push_back(encodeR(OPCODE_ZERO, 0, TEMP1, TEMP2, 13, OPCODE_SRL));
                transIns.push_back(encodeR(OPCODE_SPECIAL3, TEMP2, TEMP1, 28, 13, OPCODE_DINS));
            }else
                if(sel ==1)    //page grain
                    transIns.push_back(encodeR(OPCODE_SPECIAL3, 0, TEMP1, 28, 0, OPCODE_DINS));
            transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * dest)+(8 * sel)))); 
            break;
        case 6:
            if(sel==0){         // wired
                transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));           
                transIns.push_back(encodeI(OPCODE_ADDI, 0, TEMP2, 0x003f));
                transIns.push_back(encodeR(OPCODE_ZERO, TEMP2, TEMP1, TEMP1, 0, 36));
                transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * dest)+(8 * sel))));
                transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP2, (BaseCP + (64 * 1)+(8 * sel)))); //writing 63 to random
            }else{
            transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
            transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * dest)+(8 * sel))));        
            }
            break;
        case 7:
            if(sel==0){ //      hwrena reg
                transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
                transIns.push_back(encodeI(OPCODE_LUI, 0, TEMP2, 0x0));
                transIns.push_back(encodeI(OPCODE_ADDI, TEMP2, TEMP2, 0x003f));
                transIns.push_back(encodeR(OPCODE_ZERO, TEMP2, TEMP1, TEMP1, 0, 36));
                transIns.push_back(encodeR(OPCODE_ZERO, 0, TEMP1, TEMP1, 32, 60));   // giving sign extension
                transIns.push_back(encodeR(OPCODE_ZERO, 0, TEMP1, TEMP1, 32, 63));
                transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * dest)+(8 * sel)))); 
         //    a &= 0xc000000f;
         //    a = (a << 32) >>32 ; //giving sign extension
            }else{
            transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
            transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * dest)+(8 * sel))));        
            }
            break;
        case 9:
            if(sel==0){ //      count should not be written 
                transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
//                transIns.push_back(encodeI(OPCODE_LUI, 0, TEMP2, 0xffff));
//                transIns.push_back(encodeI(OPCODE_ADDI, TEMP2, TEMP2, 0xffff));
//                transIns.push_back(encodeR(OPCODE_ZERO, TEMP2, TEMP1, TEMP1, 0, 36));
                transIns.push_back(encodeR(OPCODE_ZERO, 0, TEMP1, TEMP1, 0, OPCODE_DSLL32));
                transIns.push_back(encodeR(OPCODE_ZERO, 0, TEMP1, TEMP1, 0, OPCODE_DSRL32));
                transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * dest)+(8 * sel))));
                
                /*---getting control when count is written*/
//                printf("Count write\n");
                transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP3, Control_from_countwrite));
                transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP3, Control_Mark));                                                                  // storing control mark for cpu not enabled  
                transIns.push_back(encodeI(OPCODE_LD, BASE_REG, C_LABEL_REG, C_FUNC_CALL_OFFSET));
                transIns.push_back(encodeR(OPCODE_ZERO, C_LABEL_REG,0,RET_ADDR_REG,0,9));
                transIns.push_back(0);
                
             //a &= 0xffffffff;
            }else if(sel==7){   //cvmctl
                transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
                transIns.push_back(encodeI(OPCODE_LUI, 0, TEMP2, 0xb800));
                transIns.push_back(encodeR(OPCODE_ZERO, TEMP2, TEMP1, TEMP1, 0, 37));
                transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * dest)+(8 * sel))));
                /*-----intctl reg bits are copy of cvmctl[IPTI] and cvmctl[IPPCI]-----*/
                transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseCP + (64*9) + (8*7))));
                transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP3, (BaseCP + (64*12) + (8*1))));
                transIns.push_back(encodeI(OPCODE_ANDI, TEMP1,TEMP2, 0x70)); //setting ipti field in intctl
                transIns.push_back(encodeR(OPCODE_ZERO, 0, TEMP2, TEMP2, 25, 56));
                transIns.push_back(encodeR(OPCODE_ZERO, TEMP2, TEMP3, TEMP3, 0, 37));
                transIns.push_back(encodeI(OPCODE_ANDI, TEMP1, TEMP2, 0x380)); //setting Ippci field in intctl
                transIns.push_back(encodeR(OPCODE_ZERO, 0, TEMP2, TEMP2, 19, 56));
               transIns.push_back(encodeR(OPCODE_ZERO, TEMP2, TEMP3, TEMP3, 0, 37));
               transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP3, (BaseCP + (64 * 12)+(8 * 1))));
            }else{
            transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
            transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * dest)+(8 * sel))));        
            }
            break;
        case 10: //EntryHi
            transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
            if(sel == 0)
            {
                transIns.push_back(encodeR(OPCODE_SPECIAL3, 0, TEMP1, 32, 17, OPCODE_DINSU));
                transIns.push_back(encodeR(OPCODE_SPECIAL3, 0, TEMP1, 12, 8, OPCODE_DINS));
            }
            transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * dest)+(8 * sel))));
            transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP1, Control_from_ENTRYHI));
            transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, Control_Mark));

            transIns.push_back(encodeI(OPCODE_LD, BASE_REG, C_LABEL_REG, C_FUNC_CALL_OFFSET));
            transIns.push_back(encodeR(OPCODE_ZERO, C_LABEL_REG,0,RET_ADDR_REG,0,9));
            transIns.push_back(0);
            break;
        case 11:
            if(sel==0){ // compare reg, write on compare register clears the IP7 in cause
            transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseCP + (64 * 13)+(8 * 0)))); //reading cause
            transIns.push_back(encodeI(OPCODE_LUI, 0, TEMP2, 0xffff));
            transIns.push_back(encodeI(OPCODE_ADDI, TEMP2, TEMP2, 0x7fff));
            transIns.push_back(encodeR(OPCODE_ZERO, TEMP2, TEMP1, TEMP1, 0, 36));
            transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * 13)+(8 * 0)))); // writing cause   
            // writing compare
            transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
            transIns.push_back(encodeR(OPCODE_ZERO, 0, TEMP1, TEMP1, 0, OPCODE_DSLL32));
            transIns.push_back(encodeR(OPCODE_ZERO, 0, TEMP1, TEMP1, 0, OPCODE_DSRL32));
            transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * dest)+(8 * sel))));
            
                /*---getting control when count is written*/
//            printf("Compare write\n");
                transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP3, Control_from_compare));
                transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP3, Control_Mark));                                                                  // storing control mark for cpu not enabled  
                transIns.push_back(encodeI(OPCODE_LD, BASE_REG, C_LABEL_REG, C_FUNC_CALL_OFFSET));
                transIns.push_back(encodeR(OPCODE_ZERO, C_LABEL_REG,0,RET_ADDR_REG,0,9));
                transIns.push_back(0);
                
            }
            else{
               transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
                transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * dest)+(8 * sel)))); 
            }
            break;
        case 12:
            if(sel==0){ //      status
                transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
                transIns.push_back(encodeI(OPCODE_LUI, 0, TEMP2, 0x50f8));
                transIns.push_back(encodeI(OPCODE_ADDI, TEMP2, TEMP2, 0xffff));
                transIns.push_back(encodeR(OPCODE_ZERO, TEMP2, TEMP1, TEMP1, 0, 36));
                transIns.push_back(encodeR(OPCODE_ZERO, 0, TEMP1, TEMP1, 32, 60));   // giving sign extension
                transIns.push_back(encodeR(OPCODE_ZERO, 0, TEMP1, TEMP1, 32, 63));
               transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * dest)+(8 * sel))));
            }else
                if(sel == 1){ //        intctl
                transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
                transIns.push_back(encodeI(OPCODE_LUI, 0, TEMP2, 0xfc00));
                transIns.push_back(encodeI(OPCODE_ADDI, TEMP2, TEMP2, 0x0000));
                transIns.push_back(encodeR(OPCODE_ZERO, TEMP2, TEMP1, TEMP1, 0, 36));
                transIns.push_back(encodeR(OPCODE_ZERO, 0, TEMP1, TEMP1, 32, 60));   // giving sign extension
                transIns.push_back(encodeR(OPCODE_ZERO, 0, TEMP1, TEMP1, 32, 63));
                transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * dest)+(8 * sel))));
                } else
                    if(sel ==2 ){ //    srsctl
                        transIns.push_back(encodeI(OPCODE_SD, BASE_REG, 0, (BaseCP + (64 * dest)+(8 * sel)))); ;
                        //                     a &= 0x0;
                    }else{
            transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
            transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * dest)+(8 * sel))));        
            }
            break;
        case 13:
            if(sel==0){ //      cause
                transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
                transIns.push_back(encodeI(OPCODE_LUI, 0, TEMP2, 0xfcc0));
                transIns.push_back(encodeI(OPCODE_ADDI, TEMP2, TEMP2, 0xff7c));
                transIns.push_back(encodeR(OPCODE_ZERO, TEMP2, TEMP1, TEMP1, 0, 36));
                transIns.push_back(encodeR(OPCODE_ZERO, 0, TEMP1, TEMP1, 32, 60));   // giving sign extension
                transIns.push_back(encodeR(OPCODE_ZERO, 0, TEMP1, TEMP1, 32, 63));
                transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * dest)+(8 * sel))));
            }else{
            transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
            transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * dest)+(8 * sel))));        
            }
            break;
        case 15:
            if(sel==0){ //      prid
                
            }else
                if(sel ==1){ // ebase   
                    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
                    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP2, (BaseCP + (64 * dest)+(8 * sel))));
                    transIns.push_back(encodeR(OPCODE_SPECIAL3, TEMP2, TEMP1, 9, 0, OPCODE_INS)); //Writing Corenum to new value
                
                transIns.push_back(encodeI(OPCODE_LUI, 0, TEMP2, 0xbfff));
                transIns.push_back(encodeI(OPCODE_ADDI, TEMP2, TEMP2, 0xf3ff));
                transIns.push_back(encodeR(OPCODE_ZERO, TEMP2, TEMP1, TEMP1, 0, 36));
                transIns.push_back(encodeI(OPCODE_LUI, 0, TEMP2, 0x8000));
                transIns.push_back(encodeI(OPCODE_ADDI, TEMP2, TEMP2, 0x0000));
                transIns.push_back(encodeR(OPCODE_ZERO, TEMP2, TEMP1, TEMP1, 0, 37));
                transIns.push_back(encodeR(OPCODE_ZERO, 0, TEMP1, TEMP1, 32, 60));   // giving sign extension
                transIns.push_back(encodeR(OPCODE_ZERO, 0, TEMP1, TEMP1, 32, 63));
                transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * dest)+(8 * sel)))); 
               //  a &= 0xbffff3ff;
              //   a |= 0x80000000;
              //   a = (a << 32) >>32 ; //giving sign extension
                }else{
            transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
            transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * dest)+(8 * sel))));        
            }
            break;
        /*case 16:
//            if(sel==0){ //      config
//                transIns.push_back(ld.encode(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
//                transIns.push_back(lui.encode(OPCODE_LUI, 0, TEMP2, 0x8000));
//                transIns.push_back(addi.encode(OPCODE_ADDI, TEMP2, TEMP2, 0xc48f));
//                transIns.push_back(a1.encode(OPCODE_ZERO, TEMP2, TEMP1, TEMP1, 0, 36));
//                transIns.push_back(lui.encode(OPCODE_LUI, 0, TEMP2, 0x8000));
//                transIns.push_back(addi.encode(OPCODE_ADDI, TEMP2, TEMP2, 0x4488));
//                transIns.push_back(or1.encode(OPCODE_ZERO, TEMP2, TEMP1, TEMP1, 0, 36));
//                transIns.push_back(dsll32.encode(OPCODE_ZERO, 0, TEMP1, TEMP1, 32, 60));   // giving sign extension
//                transIns.push_back(dsra32.encode(OPCODE_ZERO, 0, TEMP1, TEMP1, 32, 63));
//                transIns.push_back(sd.encode(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * dest)+(8 * sel)))); 
////                 a &= 0x8000c48f;
//  //               a |= 0x80004488;
//    //             a = (a << 32) >>32 ; //giving sign extension
//            }else{
            transIns.push_back(ld.encode(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
            transIns.push_back(sd.encode(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * dest)+(8 * sel))));        
            //}
            break;*/
        case 20:
            if(sel==0){ //      xcontext
                transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
                transIns.push_back(encodeI(OPCODE_LUI, 0, TEMP2, 0xffff));
                transIns.push_back(encodeI(OPCODE_ADDI, TEMP2, TEMP2, 0xfff0));
                transIns.push_back(encodeR(OPCODE_ZERO, 0, TEMP2, TEMP2, 32, 60));   // giving sign extension
                transIns.push_back(encodeR(OPCODE_ZERO, 0, TEMP2, TEMP2, 32, 63));
                transIns.push_back(encodeR(OPCODE_ZERO, TEMP2, TEMP1, TEMP1, 0, 36));
                transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * dest)+(8 * sel)))); 
//                a &= 0xfffffffffffffff0;
            }else{
            transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
            transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * dest)+(8 * sel))));        
            }
            break;
  
        default:        // Entry Lo0, Entry Lo1, badvaddr, epc
            transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
            transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * dest)+(8 * sel))));        
            break;
    }

}

void Translation_IPE::tlbr(TranslationBlock &transIns ) {

    checking_previlige(transIns);
    transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP3, Control_from_tlbr));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP3, Control_Mark));

    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, C_LABEL_REG, C_FUNC_CALL_OFFSET));
    transIns.push_back(encodeR(OPCODE_ZERO, C_LABEL_REG,0,RET_ADDR_REG,0,9));
    transIns.push_back(0);
}

void Translation_IPE::tlbwi(TranslationBlock &transIns ) {

    checking_previlige(transIns);
    transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP3, Control_from_tlbwi));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP3, Control_Mark));

    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, C_LABEL_REG, C_FUNC_CALL_OFFSET));
    transIns.push_back(encodeR(OPCODE_ZERO, C_LABEL_REG,0,RET_ADDR_REG,0,9));
    transIns.push_back(0);

}

void Translation_IPE::tlbwr(TranslationBlock &transIns ) {
    checking_previlige(transIns);
    transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP3, Control_from_tlbwr));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP3, Control_Mark));

    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, C_LABEL_REG, C_FUNC_CALL_OFFSET));
    transIns.push_back(encodeR(OPCODE_ZERO, C_LABEL_REG,0,RET_ADDR_REG,0,9));
    transIns.push_back(0);
}

void Translation_IPE::tlbp(TranslationBlock &transIns ) {
    checking_previlige(transIns);
    transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP3, Control_from_tlbp));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP3, Control_Mark));

    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, C_LABEL_REG, C_FUNC_CALL_OFFSET));
    transIns.push_back(encodeR(OPCODE_ZERO, C_LABEL_REG,0,RET_ADDR_REG,0,9));
    transIns.push_back(0);
}

void Translation_IPE::di(IInfo * InsP,TranslationBlock &transIns ) {
    unsigned long dest;
    int sel;
    sel = (InsP->instr.R.func) & 0x07;
    dest = InsP->instr.R.rt;
    checking_previlige(transIns);
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseCP + (64 * 12)+(8 * sel))));
    if (dest != 0) {
        transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseGP + 8 * (dest))));
    }
    transIns.push_back(encodeI(OPCODE_LUI, 0, TEMP2, 0xffff));
    transIns.push_back(encodeI(OPCODE_ADDI, TEMP2, TEMP2, 0xfffe));
    transIns.push_back(encodeR(OPCODE_ZERO, TEMP2, TEMP1, TEMP1, 0, 36));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * 12)+(8 * sel))));
}

void Translation_IPE::ei(IInfo * InsP,TranslationBlock &transIns ) {

    unsigned long dest;
    int sel;
    dest = InsP->instr.R.rt;
    sel = (InsP->instr.R.func) & 0x07;
    checking_previlige(transIns);
    
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseCP + (64 * 12)+(8 * sel))));
    if (dest != 0) {
        transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseGP + 8 * (dest))));
    }
    transIns.push_back(encodeI(OPCODE_ORI, TEMP1, TEMP1, 1));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * 12)+(8 * sel))));
}

/*------trap---------------*/
void Translation_IPE::trap_control(TranslationBlock &transIns ) {
    
    transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP3, Control_from_trap));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP3, Control_Mark));

    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, C_LABEL_REG, C_FUNC_CALL_OFFSET));
    transIns.push_back(encodeR(OPCODE_ZERO, C_LABEL_REG,0,RET_ADDR_REG,0,9));
    transIns.push_back(0);
}

void Translation_IPE::teq_tne_R(IInfo * InsP,TranslationBlock &transIns ) {
   
    unsigned long source1, source2;
    //printf("teq_tne_R\n");
    source1 = InsP->instr.R.rs;
    source2 = InsP->instr.R.rt;

    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source1))));
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP2, (BaseGP + 8 * (source2))));

    if (InsP->instr.R.func == 52) {
        transIns.push_back(encodeI(OPCODE_BEQ, TEMP1, TEMP2, 3));
    } else { // for tne the func = 54
        transIns.push_back(encodeI(OPCODE_BNE, TEMP1, TEMP2, 3)); // branch to handler
    }

    transIns.push_back(0);
    transIns.push_back(encodeI(OPCODE_BEQ, 0, 0, 6)); //branch to the next instruction's translation
    transIns.push_back(0);

    trap_control(transIns);
}

void Translation_IPE::tge_tgeu_tlt_tltu_R(IInfo * InsP,TranslationBlock & transIns ) {
   
    unsigned long source1, source2;
    printf("tge_tgeu_tlt_tltu_R!\n");
    source1 = InsP->instr.R.rs;
    source2 = InsP->instr.R.rt;

    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source1))));
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP2, (BaseGP + 8 * (source2))));

    if (InsP->instr.R.func == 48) { // teg
        transIns.push_back(encodeR(OPCODE_ZERO, TEMP1, TEMP2, TEMP2, 0, 34));
    } else
        if (InsP->instr.R.func == 49) { //tegu                    
        transIns.push_back(encodeR(OPCODE_ZERO, TEMP1, TEMP2, TEMP2, 0, 35));
    } else
        if (InsP->instr.R.func == 50) { //tlt                    
        transIns.push_back(encodeR(OPCODE_ZERO, TEMP2, TEMP1, TEMP2, 0, 34));
    } else
        if (InsP->instr.R.func == 51) { //tltu                     
        transIns.push_back(encodeR(OPCODE_ZERO, TEMP2, TEMP1, TEMP2, 0, 35));
    }

    transIns.push_back(encodeI(OPCODE_BGEZ, TEMP2, 1, 3)); //branch to trap
    transIns.push_back(0);
    transIns.push_back(encodeI(OPCODE_BEQ, 0, 0, 6)); //branch to next translated instruction in the block
    transIns.push_back(0);
    trap_control(transIns);
}

void Translation_IPE::teqi_tnei_I(IInfo * InsP,TranslationBlock &transIns ) {
 
    unsigned long source1;
    printf("teqi_tnei_I!\n");
    source1 = InsP->instr.I.rs;

    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source1))));
    transIns.push_back(encodeI(OPCODE_ADDI, 0, TEMP2, InsP->instr.I.imm));
    if (InsP->instr.I.rt == 12) {
        transIns.push_back(encodeI(OPCODE_BEQ, TEMP1, TEMP2, 3));
    } else { // for tnei the rt = 14
        transIns.push_back(encodeI(OPCODE_BNE, TEMP1, TEMP2, 3)); // branch to handler
    }

    transIns.push_back(0);
    transIns.push_back(encodeI(OPCODE_BEQ, 0, 0, 6)); //branch to the next instruction's translation
    transIns.push_back(0);

    trap_control(transIns);
}

void Translation_IPE::tgei_tgeiu_tlti_tltiu_I(IInfo * InsP,TranslationBlock & transIns ) {
  
    unsigned long source1;
    printf("tgei_tgeiu_tlti_tltiu\n");
    source1 = InsP->instr.I.rs;

    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source1))));
    transIns.push_back(encodeI(OPCODE_ADDI, 0, TEMP2, InsP->instr.I.imm));

    if (InsP->instr.I.rt == 8) { // tegi
        transIns.push_back(encodeR(OPCODE_ZERO, TEMP1, TEMP2, TEMP2, 0, 34));
    } else
        if (InsP->instr.I.rt == 9) { //tegiu                     
        transIns.push_back(encodeR(OPCODE_ZERO, TEMP1, TEMP2, TEMP2, 0, 35));
    } else
        if (InsP->instr.I.rt == 10) { //tlti                     
        transIns.push_back(encodeR(OPCODE_ZERO, TEMP2, TEMP1, TEMP2, 0, 34));
    } else
        if (InsP->instr.I.rt == 11) { //tltiu                    
        transIns.push_back(encodeR(OPCODE_ZERO, TEMP2, TEMP1, TEMP2, 0, 35));
    }

    transIns.push_back(encodeI(OPCODE_BGEZ, TEMP2, 1, 3)); //branch to trap
    transIns.push_back(0);
    transIns.push_back(encodeI(OPCODE_BEQ, 0, 0, 6)); //branch to next translated instruction in the block
    transIns.push_back(0);
    trap_control(transIns);
}

/*-----------*/
void Translation_IPE::unprev_R(IInfo * InsP, TranslationBlock &transIns ) {
    /* all instruction below are Rtype and contains 3 gp reg
     * or, sllv/dsllv,srlv/dsrlv,rotrv/drotrv,srav/dsrav, movz,movn, 
     * add/dadd, addu/daddu,sub/dsub,subu/dsubu,and,xor,nor,slt,sltu,
     * mul,wsbh,seb,seh,dsbh,dshd */
   
    unsigned long source1, source2, dest;
    source1 = InsP->instr.R.rs;
    source2 = InsP->instr.R.rt;
    dest = InsP->instr.R.rd;
    if(dest != 0)
    {
        transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source1))));
        transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP2, (BaseGP + 8 * (source2))));
        transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP3, (BaseGP + 8 * (dest))));
        transIns.push_back(encodeR(InsP->instr.opcode, TEMP1, TEMP2, TEMP3, InsP->instr.R.shft, InsP->instr.R.func));
        transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP3, (BaseGP + 8 * (dest))));
    }
    else
    {
        transIns.push_back(0);
//        printf("Dest reg is 0\tOpcode = %d\tFunc = %d\tInsn = 0x%08x\n",InsP->opcode,InsP->func,InsP->encoding);
    }
    
}

void Translation_IPE::shift_R(IInfo * InsP, TranslationBlock &transIns ) {
    /* all instruction below are Rtype and shifts variant
     *  dsrl/srl, dsll/sll, drotr/rotr, dsra/sra , dsll32, dsrl32, dsra32 */
    unsigned long source1, dest;

    source1 = InsP->instr.R.rt;
    dest = InsP->instr.R.rd;

    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source1))));
    transIns.push_back(encodeR(InsP->instr.opcode, InsP->instr.R.rs, TEMP1, TEMP2, InsP->instr.R.shft, InsP->instr.R.func));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP2, (BaseGP + 8 * (dest))));
}

void Translation_IPE::mulDiv_R(IInfo * InsP, TranslationBlock &transIns ) {
    /* all instruction below are Rtype and destination is HI and Lo reg
     *  dmult/mult, dmult/multu, ddiv/div, ddivu/divu, madd, maddu, msub,msubu */
    unsigned long source1, source2;

    source1 = InsP->instr.R.rs;
    source2 = InsP->instr.R.rt;
    
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source1))));
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP2, (BaseGP + 8 * (source2))));
    transIns.push_back(encodeR(InsP->instr.opcode, TEMP1, TEMP2, InsP->instr.R.rd, InsP->instr.R.shft, InsP->instr.R.func));
    transIns.push_back(encodeR(0, 0, 0, TEMP2, 0, OPCODE_MFLO));
    transIns.push_back(encodeR(0, 0, 0, TEMP1, 0, OPCODE_MFHI));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP2, (BaseGP + 8 * (REG_LO))));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseGP + 8 * (REG_HI))));
}

void Translation_IPE::madd(IInfo * InsP, TranslationBlock &transIns ){
  
    unsigned long source1, source2;

    source1 = InsP->instr.R.rs;
    source2 = InsP->instr.R.rt;
    
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (REG_LO))));
    transIns.push_back(encodeR(OPCODE_ZERO, 0, TEMP1, TEMP1, 0, 60));
    transIns.push_back(encodeR(OPCODE_ZERO, 0, TEMP1, TEMP1, 0, 62));
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP2, (BaseGP + 8 * (REG_HI))));
    transIns.push_back(encodeR(OPCODE_ZERO, 0, TEMP2, TEMP2, 0, 60));
    transIns.push_back(encodeR(OPCODE_ZERO, TEMP2, TEMP1, TEMP3, 0, 37));
    
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source1))));
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP2, (BaseGP + 8 * (source2))));
    transIns.push_back(encodeR(OPCODE_ZERO, TEMP2, TEMP1, 0, 0, 24));
    
    transIns.push_back(encodeR(0, 0, 0, TEMP1, 0, OPCODE_MFLO));
    transIns.push_back(encodeR(OPCODE_ZERO, 0, TEMP1, TEMP1, 0, 60));
    transIns.push_back(encodeR(OPCODE_ZERO, 0, TEMP1, TEMP1, 0, 62));
    transIns.push_back(encodeR(0, 0, 0, TEMP2, 0, OPCODE_MFHI));
    transIns.push_back(encodeR(OPCODE_ZERO, 0, TEMP2, TEMP2, 0, 60));
    transIns.push_back(encodeR(OPCODE_ZERO, TEMP2, TEMP1, TEMP1, 0, 37));
    
    if(InsP->instr.R.func == 0)//madd
        transIns.push_back(encodeR(OPCODE_ZERO, TEMP3, TEMP1, TEMP1, 0, 44));
    else if(InsP->instr.R.func == 1)//maddu
        transIns.push_back(encodeR(OPCODE_ZERO, TEMP3, TEMP1, TEMP1, 0, 45));
    else if(InsP->instr.R.func == 4)//msub
        transIns.push_back(encodeR(OPCODE_ZERO, TEMP3, TEMP1, TEMP1, 0, 46));
    else if(InsP->instr.R.func == 5)//msubu
        transIns.push_back(encodeR(OPCODE_ZERO, TEMP3, TEMP1, TEMP1, 0, 47));
    
    transIns.push_back(encodeR(OPCODE_ZERO, 0, TEMP1, TEMP2, 0, 63));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP2, (BaseGP + 8 * (REG_HI))));
    transIns.push_back(encodeR(OPCODE_ZERO, 0, TEMP1, TEMP1, 0, 60));
    transIns.push_back(encodeR(OPCODE_ZERO, 0, TEMP1, TEMP1, 0, 63));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseGP + 8 * (REG_LO))));       
}

void Translation_IPE::moveFromLoHi_R(IInfo * InsP,TranslationBlock &transIns ) {
    /* all instruction below are Rtype and destination is HI and Lo reg
     *  mflo, mfhi  */
    unsigned long dest;
   
    dest = InsP->instr.R.rd;
    if (InsP->instr.R.func == 16) {
        transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (REG_HI))));
        if(dest != 0){
        transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseGP + 8 * (dest))));
        }
    }
    if (InsP->instr.R.func == 18) {
        transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (REG_LO))));
        if(dest != 0){
        transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseGP + 8 * (dest))));
        }
    }
}

void Translation_IPE::moveToLoHi_R(IInfo * InsP, TranslationBlock &transIns ) {
    /* all instruction below are Rtype and destination is HI and Lo reg
     *  mtlo, mthi  */

    unsigned long source1;
    //printf("moveToLoHi_R!\n");
    source1 = InsP->instr.R.rs;
    if (InsP->instr.R.func == 16) {
        transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source1))));
        transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseGP + 8 * (REG_HI))));
    }
    if (InsP->instr.R.func == 18) {
        transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source1))));
        transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseGP + 8 * (REG_LO))));
    }
}

void Translation_IPE::ext_R(IInfo * InsP, TranslationBlock &transIns ) {
    /* all instruction below are Rtype 
     * ext, dextm,dextu,dext, exts, ext32 */
    int source, dest;

    source = InsP->instr.R.rs;
    dest = InsP->instr.R.rt;

    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
    transIns.push_back(encodeR(InsP->instr.opcode, TEMP1, TEMP2, InsP->instr.R.rd, InsP->instr.R.shft, InsP->instr.R.func));

    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP2, (BaseGP + 8 * (dest))));
}

void Translation_IPE::ins_R(IInfo * InsP, TranslationBlock &transIns ) {
    /* all instruction below are Rtype 
     *  ins, dinsm,dins,dinsu,cins, cins32 */
    int source, dest;

    source = InsP->instr.R.rs;
    dest = InsP->instr.R.rt;

    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP2, (BaseGP + 8 * (dest))));
    transIns.push_back(encodeR(InsP->instr.opcode, TEMP1, TEMP2, InsP->instr.R.rd, InsP->instr.R.shft, InsP->instr.R.func));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP2, (BaseGP + 8 * (dest))));

}

void Translation_IPE::unprev_I(IInfo * InsP,TranslationBlock &transIns ) {
    /* all instruction that are Itype and use 2 gp reg (1 source, 1 dest)
     * and take the immediate field as a constant
     * addiu,slti,sltiu,andi,ori,xori, lui
     * excepeional case would be lui in which only dest reg is required, 
     * but it can be handled in this function without causing error*/
    unsigned long source1 = InsP->instr.I.rs;
    unsigned long dest = InsP->instr.I.rt;
  
    if (InsP->instr.opcode != 15) {
        transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source1))));
        transIns.push_back(encodeI(InsP->instr.opcode, TEMP1, TEMP2, InsP->instr.I.imm));
        transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP2, (BaseGP + 8 * (dest))));
    } else {
        transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source1))));
        transIns.push_back(encodeI(InsP->instr.opcode, 0, TEMP2, InsP->instr.I.imm));
        transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP2, (BaseGP + 8 * (dest))));
    }
}

int Translation_IPE::common_in_LS(IInfo * InsP,TranslationBlock &transIns, int store ) {

    unsigned long rt = InsP->instr.I.rt, rs = InsP->instr.I.rs;
    
    /*first 3 instructions put the memory address in reg 12 for
     tlb handler to find its equivalent memory address  */    
    
    transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP2, store)); // 1 indicating store                                                           
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP2, OP_Indicator));
    
    if (store == 1) {
        transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (rt)))); // for uart buffer
        transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, LDST_Data));
    }

    //read gp "rs"
    //translate the address 
    //put that address in the instruction
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (rs))));
    transIns.push_back(encodeI(OPCODE_DADDIU, TEMP1, TEMP1, InsP->instr.I.imm));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, Addr_Store));

    transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP2, Control_from_LS)); /* zero indicating the control is coming
                                                           *  from load or store instruction, 
                                                           * where only address translation is required */
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP2, Control_Mark));

    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, C_LABEL_REG, C_FUNC_CALL_OFFSET));
    transIns.push_back(encodeR(OPCODE_ZERO, C_LABEL_REG,0,RET_ADDR_REG,0,9));
    transIns.push_back(0);
    
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, Trans_Addr));
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP2, (BaseGP + 8 * (rt))));
    return 1;
}

void Translation_IPE::unprev_I_Load(IInfo * InsP,TranslationBlock &transIns ) {
    /* all instruction that are Itype and use 2 gp reg (1 source other used as base address)
     * the immediate field is address offset.
     * ld, lw  */
    int store = 0;
    /*first four instructions put the memory address in reg 12 for
     tlb handler to find its equivalent memory address and 
      next four for actual load operation */
    
    if (InsP->instr.I.rt == 0) {
        transIns.push_back(0);
        return;
    }
    common_in_LS(InsP, transIns, store);
    transIns.push_back(encodeI(InsP->instr.opcode, TEMP1, TEMP2, 0));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP2, (BaseGP + 8 * (InsP->instr.I.rt))));
}

void Translation_IPE::unprev_I_Store(IInfo * InsP, TranslationBlock &transIns ) {
    /* all instruction that are Itype and use 2 gp reg (1 source other used as base address)
     * the immediate field is address offset.
     * sd, sw  */
    int store = 1;
    
    common_in_LS(InsP, transIns, store);
    transIns.push_back(encodeI(InsP->instr.opcode, TEMP1, TEMP2, 0));
}

void Translation_IPE::common_in_sc(IInfo * InsP, TranslationBlock &transIns ) {
    /* used for atomic operation on a memory location started by LLD
     * 1 is written to rt reg in case of successful store and zero in case of failure  */
    
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP3, LLBIT_Flag)); //Getting LLbit flag
    transIns.push_back(encodeI(OPCODE_BEQ, 0, TEMP3, 14));
    transIns.push_back(encodeR(OPCODE_ZERO, 0, 0, TEMP1, 0, OPCODE_ADD));
    
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (InsP->instr.I.rs))));
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP2, Addr_Store));
    transIns.push_back(encodeI(OPCODE_DADDIU, TEMP1, TEMP1, InsP->instr.I.imm));
    transIns.push_back(encodeI(OPCODE_BNE, TEMP1, TEMP2, 9));
    transIns.push_back(encodeR(OPCODE_ZERO, 0, 0, TEMP1, 0, OPCODE_ADD));
    
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP3, Trans_Addr));
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, LL_Data));
}

void Translation_IPE::sc(IInfo * InsP, TranslationBlock &transIns ) {
    /* used for atomic operation on a memory location started by LLD
     * 1 is written to rt reg in case of successful store and zero in case of failure  */
    
    common_in_sc(InsP, transIns);
    
    transIns.push_back(encodeI(OPCODE_LW, TEMP3, TEMP2, 0));
    
    transIns.push_back(encodeI(OPCODE_BNE, TEMP1, TEMP2, 4));
    transIns.push_back(encodeR(OPCODE_ZERO, 0, 0, TEMP1, 0, OPCODE_ADD));
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP2, (BaseGP + 8 * (InsP->instr.I.rt))));
    
    transIns.push_back(encodeI(OPCODE_SW, TEMP3, TEMP2, 0));
    transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP1, 1));
     
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseGP + 8 * (InsP->instr.I.rt))));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, 0, LLBIT_Flag));  //Clearing LLbit flag
}

void Translation_IPE::scd(IInfo * InsP, TranslationBlock &transIns ) {
    /* used for atomic operation on a memory location started by LLD
     * 1 is written to rt reg in case of successful store and zero in case of failure  */
    
    common_in_sc(InsP, transIns);
    
    transIns.push_back(encodeI(OPCODE_LD, TEMP3, TEMP2, 0));
    
    transIns.push_back(encodeI(OPCODE_BNE, TEMP1, TEMP2, 4));
    transIns.push_back(encodeR(OPCODE_ZERO, 0, 0, TEMP1, 0, OPCODE_ADD));
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP2, (BaseGP + 8 * (InsP->instr.I.rt))));
    
    transIns.push_back(encodeI(OPCODE_SD, TEMP3, TEMP2, 0));
    transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP1, 1));
     
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseGP + 8 * (InsP->instr.I.rt))));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, 0, LLBIT_Flag));  //Clearing LLbit flag
}

void Translation_IPE::common_in_ll(IInfo * InsP, TranslationBlock &transIns ) {
    /* used for atomic operation on a memory location followed by SCD */
    //IType ld,sd,addi;
    uint8_t rs = InsP->instr.I.rs, rt = InsP->instr.I.rt;

    transIns.push_back(encodeI(OPCODE_ADDI, 0, TEMP3, 1));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP3, LLBIT_Flag)); //Setting LLbit flag
    
    if (rt == 0) {
        printf("LL loading in zero register\n");
        transIns.push_back(0);
        return;
    }
    
    transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP2, 0));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP2, OP_Indicator));
    
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * rs)));
    transIns.push_back(encodeI(OPCODE_DADDIU, TEMP1, TEMP1, InsP->instr.I.imm));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, Addr_Store));

    transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP2, Control_from_LS)); /* 11 indicating the control is coming
                                                           *  from atomic load or store instruction, 
                                                           * where address translation is required with memory lock*/
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP2, Control_Mark));
        
        /*jalr instruction will jump to AssemblyToC label */
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, C_LABEL_REG, C_FUNC_CALL_OFFSET));
    transIns.push_back(encodeR(OPCODE_ZERO, C_LABEL_REG,0,RET_ADDR_REG,0,9));
    transIns.push_back(0);
    
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, Trans_Addr));
}

void Translation_IPE::ll(IInfo * InsP, TranslationBlock &transIns ) {
    /* used for atomic operation on a memory location followed by SCD */

    uint8_t rt = InsP->instr.I.rt;
    common_in_ll(InsP, transIns);
    
    transIns.push_back(encodeI(OPCODE_LW, TEMP1, TEMP2, 0));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP2, (BaseGP + 8 * rt)));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP2, LL_Data));
}

void Translation_IPE::lld(IInfo * InsP, TranslationBlock &transIns ) {
    /* used for atomic operation on a memory location followed by SCD */
    
    uint8_t rt = InsP->instr.I.rt;
    common_in_ll(InsP, transIns);
    
    transIns.push_back(encodeI(OPCODE_LD, TEMP1, TEMP2, 0));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP2, (BaseGP + 8 * rt)));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP2, LL_Data));
}

void Translation_IPE::saa(IInfo * InsP, TranslationBlock &transIns ) {
    /* this is a cavium specific instruction
     * it reads from a memory location and add the value from gp register
     * and store back the result on the same place. this all should be done atomically*/
   
    unsigned long b = InsP->instr.R.rs;
    unsigned long t = InsP->instr.R.rt;
    
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (b))));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, Addr_Store));

    transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP2, Control_from_LS)); /* zero indicating the control is coming
                                                           *  from load or store instruction, 
                                                           * where only address translation is required */
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP2, Control_Mark));

    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, C_LABEL_REG, C_FUNC_CALL_OFFSET));
    transIns.push_back(encodeR(OPCODE_ZERO, C_LABEL_REG,0,RET_ADDR_REG,0,9));
    transIns.push_back(0);

    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, Trans_Addr));  
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP3, (BaseGP + 8 * (t))));
    transIns.push_back(encodeR(OPCODE_CAVIUMADDITION, TEMP1, TEMP3, InsP->instr.R.rd, InsP->instr.R.shft, InsP->instr.R.func));
    
}

void Translation_IPE::saad(IInfo * InsP, TranslationBlock &transIns ) {
    /* this is a cavium specific instruction
     * it reads from a memory location and add the value from gp register
     * and store back the result on the same place. this all should be done atomically*/
 
     unsigned long b = InsP->instr.R.rs;
     unsigned long t = InsP->instr.R.rt;
    
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (b))));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, Addr_Store));

    transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP2, Control_from_LS)); /* zero indicating the control is coming
                                                           *  from load or store instruction, 
                                                           * where only address translation is required */
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP2, Control_Mark));

    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, C_LABEL_REG, C_FUNC_CALL_OFFSET));
    transIns.push_back(encodeR(OPCODE_ZERO, C_LABEL_REG,0,RET_ADDR_REG,0,9));
    transIns.push_back(0);

    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, Trans_Addr));
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP3, (BaseGP + 8 * (t))));
    transIns.push_back(encodeR(OPCODE_CAVIUMADDITION, TEMP1, TEMP3, InsP->instr.R.rd, InsP->instr.R.shft, InsP->instr.R.func));
    
}

void Translation_IPE::seqi_snei(IInfo * InsP, TranslationBlock &transIns ) {

    unsigned long source1 = InsP->instr.R.rs;
    unsigned long dest = InsP->instr.R.rt;
   
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source1))));
    transIns.push_back(encodeR(InsP->instr.opcode, TEMP1,TEMP2, InsP->instr.R.rd, InsP->instr.R.shft,InsP->instr.R.func));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP2, (BaseGP + 8 * (dest))));    
}

void Translation_IPE::mtm0(IInfo * InsP, TranslationBlock &transIns){
  
    unsigned long source = InsP->instr.R.rs;
     
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseGP + 8 * (MPL0))));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, 0 , (BaseGP + 8 * (P0))));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, 0 , (BaseGP + 8 * (P1))));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, 0 , (BaseGP + 8 * (P2))));
}

void Translation_IPE::mtm1(IInfo * InsP, TranslationBlock &transIns){

    unsigned long source = InsP->instr.R.rs;
     
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseGP + 8 * (MPL1))));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, 0 , (BaseGP + 8 * (P0))));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, 0 , (BaseGP + 8 * (P1))));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, 0 , (BaseGP + 8 * (P2))));
}

void Translation_IPE::mtm2(IInfo * InsP, TranslationBlock &transIns){

    unsigned long source = InsP->instr.R.rs;
     
        transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
        transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseGP + 8 * (MPL2))));
        transIns.push_back(encodeI(OPCODE_SD, BASE_REG, 0 , (BaseGP + 8 * (P0))));
        transIns.push_back(encodeI(OPCODE_SD, BASE_REG, 0 , (BaseGP + 8 * (P1))));
        transIns.push_back(encodeI(OPCODE_SD, BASE_REG, 0 , (BaseGP + 8 * (P2))));
}

void Translation_IPE::mtp0(IInfo * InsP, TranslationBlock &transIns){
    
    unsigned long source = InsP->instr.R.rs;
     
        transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
        transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseGP + 8 * (P0))));
}

void Translation_IPE::mtp1(IInfo * InsP, TranslationBlock &transIns){
  
    unsigned long source = InsP->instr.R.rs;
     
        transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
        transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseGP + 8 * (P1))));
}

void Translation_IPE::mtp2(IInfo * InsP, TranslationBlock &transIns){
  
    unsigned long source = InsP->instr.R.rs;
     
        transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
        transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseGP + 8 * (P2))));
}

void Translation_IPE::v3mulu(IInfo * InsP, TranslationBlock &transIns ){

    /*--first moving all the six multiplication registers---*/
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (MPL0))));
    transIns.push_back(encodeR(OPCODE_CAVIUMADDITION, TEMP1, 0, 0, 0, 8));  // mtm0
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (MPL1))));
    transIns.push_back(encodeR(OPCODE_CAVIUMADDITION, TEMP1, 0, 0, 0, 12));  // mtm1
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (MPL2))));
    transIns.push_back(encodeR(OPCODE_CAVIUMADDITION, TEMP1, 0, 0, 0, 13));  // mtm2
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (P0))));
    transIns.push_back(encodeR(OPCODE_CAVIUMADDITION, TEMP1, 0, 0, 0, 9));  // mtp0
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (P1))));
    transIns.push_back(encodeR(OPCODE_CAVIUMADDITION, TEMP1, 0, 0, 0, 10));  // mtp1
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (P2))));
    transIns.push_back(encodeR(OPCODE_CAVIUMADDITION, TEMP1, 0, 0, 0, 11));  // mtp2
    
    /*---perform multiplication---*/
    unprev_R(InsP, transIns);
}

//--------------------------------------

void Translation_IPE::BranchPath(uint16_t imm, TranslationBlock &transIns ) {

    /* all instruction that are branches and use 2 gp reg 
     bne, beq,  */

    imm = 0xffff & (imm * 4 + 4);
    //----------branch not taken-----

    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP3, Block_End_PC));
    transIns.push_back(encodeI(OPCODE_DADDI, TEMP3, TEMP3, 8));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP3, PC));
    
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, C_LABEL_REG, C_FUNC_CALL_OFFSET));
    transIns.push_back(encodeR(OPCODE_ZERO, C_LABEL_REG,0,0,0,8));
    transIns.push_back(0);
    //---------branch taken-----------------
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP3, Block_End_PC));
    transIns.push_back(encodeI(OPCODE_DADDI, TEMP3, TEMP3, imm));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP3, PC));
    
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, C_LABEL_REG, C_FUNC_CALL_OFFSET));
    transIns.push_back(encodeR(OPCODE_ZERO, C_LABEL_REG,0,0,0,8));
    transIns.push_back(0);
}

void Translation_IPE::BranchPath_likely(IInfo *InsD, uint16_t imm, TranslationBlock &transIns ) {

    /* all instruction that are branches and use 2 gp reg 
     bne, beq,  */;
    imm = 0xffff & (imm * 4 + 4);
//----------branch not taken-----
    transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP3, Control_from_EndOfBlock)); /* 1 indicating the control is coming
                                                           *  from branch/jump instruction, 
                                                           * where it is decided that where the control is shifted next*/
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP3, Control_Mark));
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP3, Block_End_PC));
    transIns.push_back(encodeI(OPCODE_DADDI, TEMP3, TEMP3, 8));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP3, PC));
    
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, C_LABEL_REG, C_FUNC_CALL_OFFSET));
    transIns.push_back(encodeR(OPCODE_ZERO, C_LABEL_REG,0,0,0,8));
   transIns.push_back(0);
    //---------branch taken-----------------
  
    DelaySlotObj.findInstrCase(InsD, NULL ,transIns,0);
    transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP3, Control_from_EndOfBlock)); /* 1 indicating the control is coming
                                                           *  from branch/jump instruction, 
                                                           * where it is decided that where the control is shifted next*/
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP3, Control_Mark));
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP3, Block_End_PC));
    transIns.push_back(encodeI(OPCODE_DADDI, TEMP3, TEMP3, imm));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP3, PC));
    
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, C_LABEL_REG, C_FUNC_CALL_OFFSET));
    transIns.push_back(encodeR(OPCODE_ZERO, C_LABEL_REG,0,0,0,8));
    transIns.push_back(0);
}

//void Translation_IPE::DelaySlot(IInfo *InsD, TranslationBlock &transIns ) {
//    uint8_t op;
//    op = InsD->instr.opcode;
//    
//    if (InsD->type == 1) { //IType
//
//        if ((op >= 32 && op <= 39) || (op == 48) || (op == 55) || (op == 52) || (op == 26) || (op == 27)) {
//            unprev_I_Load(InsD, transIns);
//        } else 
//            if ((op >= 40 && op <= 46) || (op == 56) || (op == 63) || (op == 60)) {
//            unprev_I_Store(InsD, transIns);
//        } else 
//            if ((op == 1) && (InsD->instr.I.rt == 12 || InsD->instr.I.rt == 14)) { 
//            teqi_tnei_I(InsD, transIns);
//        } else
//            if ((op == 1) && (InsD->instr.I.rt >= 8 && InsD->instr.I.rt <= 11)) { 
//            tgei_tgeiu_tlti_tltiu_I(InsD, transIns);
//        } 
//        else 
//            if(op == 47 || op==51){
//            transIns.push_back(0);
//        }else{
//            unprev_I(InsD, transIns);
//        }
//
//    } else { //RType
//
//        if (InsD->encoding == 0 ) { //nop  
//            transIns.push_back(0);
//        } else 
//            if (op == 0 && InsD->instr.R.func==15) { //sync
//                if(InsD->instr.R.shft == 0x2)
//                  printf("SyncIOBDMA!\t ~~~~~~IOBDMA operation required~~~~~~\n");
//            transIns.push_back(0);
//        } else
//            if (op == 0 && InsD->instr.R.func==1) { //ssnop
//            ssnop(transIns);
//        } else
//            if ((op == 0 && ((InsD->instr.R.func >= 0 && InsD->instr.R.func <= 3) ||
//                    (InsD->instr.R.func >= 56 && InsD->instr.R.func <= 63))) || (op == 1 && (InsD->instr.R.func == 58))) {
//            shift_R(InsD, transIns);
//        } else
//            if (op == 0 && (InsD->instr.R.func >= 24 && InsD->instr.R.func <= 31)) { // muldiv
//            mulDiv_R(InsD, transIns);
//        } else
//            if (op == 28 && (InsD->instr.R.func == 0 || InsD->instr.R.func == 1 || InsD->instr.R.func == 4 || InsD->instr.R.func == 5)) { // muldiv
//            madd(InsD, transIns);
//        }else
//            if ((op == 31 && (InsD->instr.R.func >= 0 && InsD->instr.R.func <= 3)) || (op == 28 && (InsD->instr.R.func == 58 || InsD->instr.R.func == 59))) { //ext
//            ext_R(InsD, transIns);
//        } else
//            if ((op == 31 && (InsD->instr.R.func >= 4 && InsD->instr.R.func <= 7)) || (op == 28 && (InsD->instr.R.func == 50 || InsD->instr.R.func == 51))) { //Ins
//            ins_R(InsD, transIns);
//        } else
//            if ((op == 0) && (InsD->instr.R.func == 52 || InsD->instr.R.func == 54)) { //teq_tne_R
//            teq_tne_R(InsD, transIns);
//        } else
//            if ((op == 0) && (InsD->instr.R.func >= 48 && InsD->instr.R.func <= 51)) { //tge_tgeu_tlt_tltu_R
//            tge_tgeu_tlt_tltu_R(InsD, transIns);
//        } else
//            if (op == 0 && (InsD->instr.R.func == 19 || InsD->instr.R.func == 17)) { //mtlo
//            moveToLoHi_R(InsD, transIns);
//        } else
//            if (op == 0 && (InsD->instr.R.func == 18 || InsD->instr.R.func == 16)) { //mflo
//            moveFromLoHi_R(InsD, transIns);
//        } else
//            if (op == 16 && (InsD->instr.R.rs == 0 || InsD->instr.R.rs == 1)) { //mfc0
//            mfc0(InsD, transIns);
//        } else
//            if (op == 16 && (InsD->instr.R.rs == 4 || InsD->instr.R.rs == 5)) { //mtc0
//            mtc0(InsD, transIns);
//        } else
//            if (op == 16 && (InsD->instr.R.rs == 11 && InsD->instr.R.func == 0)) { //di
//                //printf("di!\n");
//            di(InsD, transIns);
//        } else
//            if (op == 16 && (InsD->instr.R.rs == 11 && InsD->instr.R.func == 32)) { //ei
//                //printf("ei!\n");
//            ei(InsD, transIns);
//        } else
//            if (op == 31 && InsD->instr.R.func==59) { //rdhwr
//                //printf("Read Hardware Reg!\n");
//            rdhwr(InsD, transIns);
//        } else
//            if (op == 0 && InsD->instr.R.func==13) { //break
//            br(transIns);
//        }  else
//            if (op == 0 && InsD->instr.R.func==12) { //syscall
//            syscall(transIns);
//        } 
//            else 
//            if(op == 28 && InsD->instr.R.func==24){
//                //printf("SAA!\n");
//            saa(InsD, transIns);  
//        }else 
//            if(op == 28 && InsD->instr.R.func==25){
//               // printf("SAad!\n");
//            saad(InsD, transIns);  
//        }else 
//            if(op == 28 && (InsD->instr.R.func==47 || InsD->instr.R.func== 46 )){
//               // printf("seqi_snei!\n");
//            seqi_snei(InsD, transIns);  
//        }else
//            if(op == 28 && InsD->instr.R.func==8){
//               // printf("mtm0\n");
//            mtm0(InsD, transIns);  
//        }else
//            if(op == 28 && InsD->instr.R.func==12){
//               // printf("mtm1\n");
//            mtm1(InsD, transIns);  
//        }else
//            if(op == 28 && InsD->instr.R.func==13){
//               // printf("mtm2\n");
//            mtm2(InsD, transIns);  
//        }else
//            if(op == 28 && InsD->instr.R.func==9){
//               // printf("mtp0\n");
//            mtp0(InsD, transIns);  
//        }else
//            if(op == 28 && InsD->instr.R.func==10){
//               // printf("mtp1\n");
//            mtp1(InsD, transIns);  
//        }else
//            if(op == 28 && InsD->instr.R.func==11){
//               // printf("mtp2!\n");
//            mtp2(InsD, transIns);  
//        }else
//            if(op == 28 && InsD->instr.R.func==17){
//               // printf("v3mulu!\n");
//            v3mulu(InsD, transIns);  
//        }else {                        // unprev_r
//            unprev_R(InsD, transIns);
//        }
//
//    }
//
//}
//------------------------------------------

void Translation_IPE::bne_beq(IInfo *InsB, IInfo *InsD, TranslationBlock &transIns ) {

    unsigned long source1 = InsB->instr.I.rs; // saving the sources of branch
    unsigned long source2 = InsB->instr.I.rt;

    if (LOG_GEN)        add_Log(transIns);
    
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP3, (BaseGP + 8 * (source1))));
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP2, (BaseGP + 8 * (source2))));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP3, Memory_Temp4));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP2, Memory_Temp5));

    DelaySlotObj.findInstrCase(InsD, NULL ,transIns,0);
    //-------branch---------
    transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP1, Control_from_EndOfBlock)); /* 1 indicating the control is coming
                                                           *  from branch/jump instruction, 
                                                           * where it is decided that where the control is shifted next*/
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, Control_Mark));
    
//    transIns.push_back(sd.encode(1, TEMP1, 0, 5));
//    transIns.push_back(0);        transIns.push_back(0);        transIns.push_back(0);
//    transIns.push_back(addiu.encode(OPCODE_ADDIU, 0, TEMP2, 2));
//    transIns.push_back(sub.encode(0, TEMP2, TEMP1, TEMP3,0,34));
//    transIns.push_back(sd.encode(7, TEMP3, 0, 3));
//    transIns.push_back(0);        transIns.push_back(0);        transIns.push_back(0);
//    transIns.push_back(addiu.encode(OPCODE_ADDIU, 0, TEMP1, 0));
//    transIns.push_back(sub.encode(0, TEMP2, TEMP1, 0,0,26));
//    transIns.push_back(0x200d0000);
//    transIns.push_back(0x01cd001a);
    
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP3, Memory_Temp4));
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP2, Memory_Temp5));
    
    transIns.push_back(encodeI(InsB->instr.opcode, TEMP3, TEMP2, 7));
    transIns.push_back(0);

    BranchPath(InsB->instr.I.imm, transIns);
}

void Translation_IPE::bbit(IInfo *InsB, IInfo *InsD, TranslationBlock &transIns ) {
 
    unsigned long source1 = InsB->instr.I.rs; // saving the sources of branch

    if (LOG_GEN)        add_Log(transIns);

    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP3, (BaseGP + 8 * (source1))));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP3, Memory_Temp4));
   
    DelaySlotObj.findInstrCase(InsD, NULL ,transIns,0);
    transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP1, Control_from_EndOfBlock)); /* 1 indicating the control is coming
                                                           *  from branch/jump instruction, 
                                                           * where it is decided that where the control is shifted next*/
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, Control_Mark));
    //-------branch----'-----

    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP3, Memory_Temp4));
    transIns.push_back(encodeI(InsB->instr.opcode, TEMP3, InsB->instr.I.rt, 7));
    transIns.push_back(0);

    BranchPath(InsB->instr.I.imm, transIns);

}

void Translation_IPE::branch(IInfo *InsB, IInfo *InsD, TranslationBlock &transIns ) {
    unsigned long source1 = InsB->instr.I.rs; // saving the sources of branch

    //in case of bgezal, btlzal
    if (InsB->instr.I.rt == 16 || InsB->instr.I.rt == 17)    And_Link(transIns);
    
    if (LOG_GEN)   add_Log(transIns);

    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP3, (BaseGP + 8 * (source1))));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP3, Memory_Temp4));
    DelaySlotObj.findInstrCase(InsD, NULL ,transIns,0);
    transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP1, Control_from_EndOfBlock)); /* 1 indicating the control is coming
                                                           *  from branch/jump instruction, 
                                                           * where it is decided that where the control is shifted next*/
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, Control_Mark));
    //-------branch----'-----

    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP3, Memory_Temp4));
    transIns.push_back(encodeI(InsB->instr.opcode, TEMP3, InsB->instr.I.rt, 7));
    transIns.push_back(0);

    BranchPath(InsB->instr.I.imm, transIns);

}

void Translation_IPE::bne_beq_likely(IInfo *InsB, IInfo *InsD,  TranslationBlock &transIns ) {
    /* all instruction that are branches and use 2 gp reg 
     bnel, beql,  */

    unsigned long source1 = InsB->instr.I.rs; // saving the sources of branch
    unsigned long source2 = InsB->instr.I.rt;

    if (LOG_GEN)   add_Log(transIns);
    
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source1))));
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP2, (BaseGP + 8 * (source2))));

    //---Saving from where the control is coming from------
    transIns.push_back(encodeI(InsB->instr.opcode, TEMP1, TEMP2, 9));
    transIns.push_back(0);

    BranchPath_likely(InsD, InsB->instr.I.imm, transIns);
}

void Translation_IPE::branch_likely(IInfo *InsB, IInfo *InsD, TranslationBlock &transIns ) {


    /* all instruction that are branches and use 1 gp reg 
     bltz, bgez, bltzl, bgezl, bltzal, bgezal, bltzall, bgezall, blez, bgtz  */

    unsigned long source1 = InsB->instr.I.rs; // saving the sources of branch

    if (InsB->instr.I.rt == 18 || InsB->instr.I.rt == 19)   And_Link(transIns);

    if (LOG_GEN)    add_Log(transIns);
    
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source1))));
    //---Saving from where the control is coming from------
    transIns.push_back(encodeI(InsB->instr.opcode, TEMP1, InsB->instr.I.rt, 9));
    transIns.push_back(0);
    BranchPath_likely(InsD, InsB->instr.I.imm, transIns);
}

void Translation_IPE::And_Link(TranslationBlock &transIns ) {

    //-----"and link"-- storing pc in 31 reg---
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, Block_End_PC));
    transIns.push_back(encodeI(OPCODE_DADDI, TEMP1, TEMP1, 8));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseGP + (8 * 31))));

}

void Translation_IPE::j(IInfo *InsJ, IInfo *InsD, TranslationBlock &transIns, uint64_t blockEndPcValue ) {

    unsigned long addr = (blockEndPcValue & 0xf0000000) | (InsJ->instr.j.addr << 2);
    unsigned long addr_up = addr >> 16;
    unsigned long addr_low = addr & 0x0000ffff;

    if (LOG_GEN)        add_Log(transIns);

    DelaySlotObj.findInstrCase(InsD, NULL,transIns,0);
    //--storing the address, where to jump-----  
    transIns.push_back(encodeI(OPCODE_LUI, 0, TEMP1, addr_up));
    transIns.push_back(encodeI(OPCODE_ORI, TEMP1, TEMP1, addr_low));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, PC));

    //---Saving from where the control is coming from------
    transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP1, Control_from_EndOfBlock)); /* 1 indicating the control is coming
                                                           *  from branch/jump instruction, 
                                                           * where it is decided that where the control is shifted next*/
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, Control_Mark));
    
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, C_LABEL_REG, C_FUNC_CALL_OFFSET));
    transIns.push_back(encodeR(OPCODE_ZERO, C_LABEL_REG,0,0,0,8));
    transIns.push_back(0);
}

void Translation_IPE::jal(IInfo * InsJ, IInfo * InsD, TranslationBlock &transIns, uint64_t blockEndPcValue ) {

    unsigned long addr = (blockEndPcValue & 0xf0000000) | (InsJ->instr.j.addr << 2);
    unsigned long addr_up = addr >> 16;
    unsigned long addr_low = addr & 0x0000ffff;

    And_Link(transIns);

    if (LOG_GEN)    add_Log(transIns);

    DelaySlotObj.findInstrCase(InsD, NULL ,transIns,0);
    //--storing the address, where to jump-----  
    transIns.push_back(encodeI(OPCODE_LUI, 0, TEMP1, addr_up));
    transIns.push_back(encodeI(OPCODE_ORI, TEMP1, TEMP1, addr_low));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, PC));

    //---Saving from where the control is coming from------
    transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP1, Control_from_EndOfBlock)); /* 1 indicating the control is coming
                                                           *  from branch/jump instruction, 
                                                           * where it is decided that where the control is shifted next*/
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, Control_Mark));
    
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, C_LABEL_REG, C_FUNC_CALL_OFFSET));
    transIns.push_back(encodeR(OPCODE_ZERO, C_LABEL_REG,0,0,0,8));
    transIns.push_back(0);
}

void Translation_IPE::jr(IInfo *InsJ, IInfo *InsD, TranslationBlock &transIns ) {

    unsigned long source = InsJ->instr.R.rs;

    if (LOG_GEN)        add_Log(transIns);

    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, Memory_Temp4));

    DelaySlotObj.findInstrCase(InsD, NULL ,transIns,0);
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, Memory_Temp4));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, PC));

    //---Saving from where the control is coming from------
    transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP1, Control_from_EndOfBlock)); /* 1 indicating the control is coming
                                                           *  from branch/jump instruction, 
                                                           * where it is decided that where the control is shifted next*/
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, Control_Mark));

    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, C_LABEL_REG, C_FUNC_CALL_OFFSET));
    transIns.push_back(encodeR(OPCODE_ZERO, C_LABEL_REG,0,0,0,8));
    transIns.push_back(0);
}

void Translation_IPE::jalr(IInfo * InsJ, IInfo * InsD, TranslationBlock &transIns ) {

    unsigned long source = InsJ->instr.R.rs;
    int d = InsJ->instr.R.rd;
    /*---linking---*/

    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, Block_End_PC));
    transIns.push_back(encodeI(OPCODE_DADDI, TEMP1, TEMP1, 8));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseGP + (8 * d))));
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseGP + 8 * (source))));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, Memory_Temp4));
    if (LOG_GEN) {
        add_Log(transIns);
    }

    DelaySlotObj.findInstrCase(InsD, NULL ,transIns,0);
    
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, Memory_Temp4));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, PC));

    //---Saving from where the control is coming from------
    transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP1, Control_from_EndOfBlock)); /* 1 indicating the control is coming
                                                           *  from branch/jump instruction, 
                                                           * where it is decided that where the control is shifted next*/
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, Control_Mark));

    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, C_LABEL_REG, C_FUNC_CALL_OFFSET));
    transIns.push_back(encodeR(OPCODE_ZERO, C_LABEL_REG,0,0,0,8));
    transIns.push_back(0);
}

void Translation_IPE::syscall(TranslationBlock &transIns ) {

    transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP3, Control_from_syscall));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP3, Control_Mark));

    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, C_LABEL_REG, C_FUNC_CALL_OFFSET));
    transIns.push_back(encodeR(OPCODE_ZERO, C_LABEL_REG,0,RET_ADDR_REG,0,9));    
    transIns.push_back(0);
}

void Translation_IPE::sync(TranslationBlock &transIns) {

    transIns.push_back(0);
}

void Translation_IPE::nop(TranslationBlock &transIns) {

    transIns.push_back(0);
}

void Translation_IPE::cache(TranslationBlock &transIns) {
    transIns.push_back(0);
}

void Translation_IPE::rdhwr(IInfo *InsP, TranslationBlock &transIns ) {
 
    unsigned long dest = InsP->instr.R.rt;
    if(InsP->instr.R.rd == 0)
    {
        transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP2, (BaseCP + (64 * 15)+(8 * 1))));
        transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP3, 0x3FF));
        transIns.push_back(encodeR(OPCODE_ZERO, TEMP2, TEMP3, TEMP3, 0,36)); //Retrieve corenum
    }else if(InsP->instr.R.rd == 31)   //Icvmcount register should be read
        transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP3, (BaseCP + (64 * 9)+(8 * 6))));
    else if(InsP->instr.R.rd == 2 || InsP->instr.R.rd == 3)   //count register should be read
        transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP3, (BaseCP + (64 * 9)+(8 * 0))));
    else{
        printf("Rdhwr with rd = %d ~~~~~~~~~~Not implemented yet~~~~~~~\n",InsP->instr.R.rd);
        transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP3, 0));
    }
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP3, (BaseGP + 8 * (dest))));
}

void Translation_IPE::br(TranslationBlock &transIns ) {

    transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP3, Control_from_break));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP3, Control_Mark));

    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, C_LABEL_REG, C_FUNC_CALL_OFFSET));
    transIns.push_back(encodeR(OPCODE_ZERO, C_LABEL_REG,0,RET_ADDR_REG,0,9));
    transIns.push_back(0);
}

void Translation_IPE::pref(TranslationBlock &transIns){
    transIns.push_back(0);
    transIns.push_back(0);
}
void Translation_IPE::eret(TranslationBlock &transIns ) {
   
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseCP + (64 * 12)+(8 * 0))));
    transIns.push_back(encodeI(OPCODE_ANDI,TEMP1,TEMP2,0x4));
    transIns.push_back(encodeI(OPCODE_BEQ, TEMP2, 0, 12)); //checking whether erl bit of status is set or not
    transIns.push_back(0);
    //if set
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP2, (BaseCP + (64 * 30)+(8 * 0)))); //reading error epc
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP2, PC));              // storing error epc in addr_store
    transIns.push_back(encodeI(OPCODE_LUI, 0, TEMP2, 0xffff));
    transIns.push_back(encodeI(OPCODE_ADDI, TEMP2, TEMP2, 0xfffb));
    transIns.push_back(encodeR(OPCODE_ZERO, TEMP2, TEMP1, TEMP1, 0, 36));              
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * 12)+(8 * 0)))); //clearing erl bit of status
    transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP1, Control_from_EndOfBlock));                                                           
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, Control_Mark)); 
    
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, C_LABEL_REG, C_FUNC_CALL_OFFSET));
    transIns.push_back(encodeR(OPCODE_ZERO, C_LABEL_REG,0,0,0,8));
    
//    transIns.push_back(j.encode(OPCODE_J, target)); 
    transIns.push_back(0);
    //if clear
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP2, (BaseCP + (64 * 14)+(8 * 0)))); //reading epc
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP2, PC));
    transIns.push_back(encodeI(OPCODE_LUI, 0, TEMP2, 0xffff));
    transIns.push_back(encodeI(OPCODE_ADDI, TEMP2, TEMP2, 0xfffd));
    transIns.push_back(encodeR(OPCODE_ZERO, TEMP2, TEMP1, TEMP1, 0, 36));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, (BaseCP + (64 * 12)+(8 * 0)))); //clearing exl bit of status
    transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP1, Control_from_EndOfBlock));                                                           
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP1, Control_Mark));
    
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, C_LABEL_REG, C_FUNC_CALL_OFFSET));
    transIns.push_back(encodeR(OPCODE_ZERO, C_LABEL_REG,0,0,0,8));
    
//    transIns.push_back(j.encode(OPCODE_J, target)); 
    transIns.push_back(0);

}
void Translation_IPE::ssnop(TranslationBlock &transIns){
    transIns.push_back(0);
}
void Translation_IPE::deret(TranslationBlock &transIns){
    transIns.push_back(0);
}
void Translation_IPE::wait(TranslationBlock &transIns ) {
 
    /*transIns.push_back(encodeI(OPCODE_LD, BASE_REG, TEMP1, (BaseCP + (64 * 13)+(8 * 0))));
    transIns.push_back(encodeI(OPCODE_ANDI,TEMP1,TEMP1,0xff00));
//    transIns.push_back(dsll32.encode(OPCODE_ZERO, 0, TEMP1, TEMP1, 8, 60));
    transIns.push_back(encodeI(OPCODE_BEQ, TEMP1, 0, -3));
    transIns.push_back(0);*/
    transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP3, Control_from_EXT_INT));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP3, Control_Mark));

    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, C_LABEL_REG, C_FUNC_CALL_OFFSET));
    transIns.push_back(encodeR(OPCODE_ZERO, C_LABEL_REG,0,RET_ADDR_REG,0,9));
    transIns.push_back(0);
}
void Translation_IPE::TerminateBlock(TranslationBlock &transIns){
    transIns.push_back(encodeI(OPCODE_ADDIU, 0, TEMP3, Control_from_EndOfBlock));
    transIns.push_back(encodeI(OPCODE_SD, BASE_REG, TEMP3, Control_Mark));
    
    transIns.push_back(encodeI(OPCODE_LD, BASE_REG, C_LABEL_REG, C_FUNC_CALL_OFFSET));
    transIns.push_back(encodeR(OPCODE_ZERO, C_LABEL_REG,0,0,0,8));
    
    transIns.push_back(0);
}
/* 
 * File:   Translation_IPE.h
 * Author: asadusmani
 *
 * Created on June 5, 2014, 4:35 PM
 */

#ifndef TRANSLATION_IPE_H
#define	TRANSLATION_IPE_H

#include <vector>

#include "IInfo.h"
#include "Processor_IPE.h"
#include "InsDecoding.h"
#include "timer.h"

#define BaseGP          GP_REGS_OFFSET //this constant will be assigned the starting memory address of gp register
#define BaseCP          CP0_REGS_OFFSET //this constant will be assigned the starting memory address of cp register
#define Addr_Store      LDST_ADDR_OFFSET // this is offset from base where the translated address will be placed for load/store instructions
#define Trans_Addr      TRANS_ADDR_OFFSET
#define Control_Mark    CONTROL_FLAG_OFFSET // this is offset from base where a no. is placed to indicate from where the control is coming from 
#define Memory_Temp4    TEMP_REG1_OFFSET //temporary memory 
#define Memory_Temp5    TEMP_REG2_OFFSET //temporary memory
#define Block_End_PC    BLOCK_END_PC_OFFSET // this is offset from base where pc of block branch/jump instruction is placed 
#define LDST_Data       LDST_DATA_OFFSET
#define OP_Indicator    LDST_OP_OFFSET
#define LLBIT_Flag      LLBIT_OFFSET
#define LL_Data         LL_DATA_OFFSET
#define PC              PROG_COUNTER_OFFSET

#define BASE_REG        12
#define TEMP1           13
#define TEMP2           14
#define TEMP3           15
#define C_LABEL_REG     24
#define RET_ADDR_REG    25

#define REG_HI          32
#define REG_LO          33
#define MPL0            35
#define MPL1            36
#define MPL2            37
#define P0              38
#define P1              39
#define P2              40

#define OPCODE_LW       35
#define OPCODE_LD       55
#define OPCODE_SW       43
#define OPCODE_SD       63
#define OPCODE_LUI      15
#define OPCODE_JAL      3
#define OPCODE_J        2
#define OPCODE_BEQ      4
#define OPCODE_BNE      5
#define OPCODE_BGEZ     1
#define OPCODE_ADDIU    9
#define OPCODE_DADDI    24
#define OPCODE_DADDIU   25
#define OPCODE_ZERO     0
#define OPCODE_ADDI     8
#define OPCODE_ANDI     12
#define OPCODE_ORI      13
#define OPCODE_SPECIAL2         28
#define OPCODE_SPECIAL3         31


#define OPCODE_SRL              2
#define OPCODE_INS              4
#define OPCODE_DINSU            6
#define OPCODE_DINS             7
#define OPCODE_JR               8
#define OPCODE_JALR             9
#define OPCODE_SAAD             25
#define OPCODE_CAVIUMADDITION   28
#define OPCODE_ADD              32
#define OPCODE_AND              36
#define OPCODE_RDHWR            59
#define OPCODE_DSLL32           60
#define OPCODE_DSRL32           62

#define OPCODE_MFHI     16
#define OPCODE_MTHI     17
#define OPCODE_MFLO     18
#define OPCODE_MTLO     19

#define Control_from_LS                 CHECK_TLB_DATA
#define Control_from_EndOfBlock         FETCH_NEXT_BLOCK
#define Control_from_LogWrite           WRITE_LOG_FILES
#define Control_from_trap               HANLDE_TRAP_INT
#define Control_from_tlbr               HANLDE_TLB_READ
#define Control_from_tlbwi              HANLDE_TLB_WRITEI
#define Control_from_tlbwr              HANLDE_TLB_WRITE
#define Control_from_tlbp               HANLDE_TLB_PROBE
#define Control_from_syscall            HANLDE_SYSCALL_INT
#define Control_from_break              HANLDE_BR
#define Control_from_EXT_INT            HANLDE_EXTERNAL_INT
#define COntrol_from_RI                 EXP_RI
#define Control_from_ENTRYHI            HANDLE_ASID_CHANGE
#define Control_from_compare            HANDLE_Compare_Write
#define Control_from_countRead          HANDLE_count_read
#define Control_from_countwrite         HANDLE_count_write
class Translation_IPE {

public:


static void add_Log(TranslationBlock &transIns );
static void update_pc(TranslationBlock &transIns );
static void checking_previlige(TranslationBlock &transIns);
/*---privileged----*/
static void mfc0(IInfo * InsP, TranslationBlock &transIns );
static void mtc0(IInfo * InsP, TranslationBlock &transIns );
static void tlbr( TranslationBlock &transIns );
static void tlbwi( TranslationBlock &transIns );
static void tlbwr(TranslationBlock &transIns );
static void tlbp(TranslationBlock &transIns );
static void di(IInfo * InsP, TranslationBlock &transIns );
static void ei(IInfo * InsP, TranslationBlock &transIns );
/*----trap---*/
static void trap_control(TranslationBlock &transIns );
static void teq_tne_R(IInfo * InsP, TranslationBlock &transIns );
static void tge_tgeu_tlt_tltu_R(IInfo * InsP, TranslationBlock &transIns );

static void teqi_tnei_I(IInfo * InsP, TranslationBlock &transIns );
static void tgei_tgeiu_tlti_tltiu_I(IInfo * InsP, TranslationBlock &transIns );

/*---unprivileged---*/
static void unprev_R(IInfo * InsP, TranslationBlock &transIns );
static void shift_R(IInfo * InsP, TranslationBlock &transIns );
static void mulDiv_R(IInfo * InsP, TranslationBlock &transIns );
static void madd(IInfo * InsP, TranslationBlock &transIns );
static void moveFromLoHi_R(IInfo * InsP, TranslationBlock &transIns );
static void moveToLoHi_R(IInfo * InsP, TranslationBlock &transIns );
static void ext_R(IInfo * InsP, TranslationBlock &transIns );
static void ins_R(IInfo * InsP, TranslationBlock &transIns );
static void unprev_I(IInfo * InsP, TranslationBlock &transIns );
static int common_in_LS(IInfo * InsP, TranslationBlock &transIns, int load );
static void unprev_I_Load(IInfo * InsP, TranslationBlock &transIns );
static void unprev_I_Store(IInfo * InsP, TranslationBlock &transIns );
static inline void common_in_sc(IInfo * InsP, TranslationBlock &transIns );
static void sc(IInfo * InsP, TranslationBlock &transIns );
static void scd(IInfo * InsP, TranslationBlock &transIns );
static inline void common_in_ll(IInfo * InsP, TranslationBlock &transIns );
static void ll(IInfo * InsP, TranslationBlock &transIns );
static void lld(IInfo * InsP, TranslationBlock &transIns );
static void saa(IInfo * InsP, TranslationBlock &transIns );
static void saad(IInfo * InsP, TranslationBlock &transIns );
static void seqi_snei(IInfo * InsP, TranslationBlock &transIns );
static void mtm0(IInfo * InsP, TranslationBlock &transIns);
static void mtm1(IInfo * InsP, TranslationBlock &transIns);
static void mtm2(IInfo * InsP, TranslationBlock &transIns);
static void mtp0(IInfo * InsP, TranslationBlock &transIns);
static void mtp1(IInfo * InsP, TranslationBlock &transIns);
static void mtp2(IInfo * InsP, TranslationBlock &transIns);
static void v3mulu(IInfo * InsP, TranslationBlock &transIns );
/*----branch----*/
static void BranchPath(uint16_t imm, TranslationBlock &transIns );
static void BranchPath_likely(IInfo *InsD, uint16_t imm, TranslationBlock &transIns );
static void DelaySlot(IInfo *InsD, TranslationBlock &transIns );
static void bbit(IInfo *InsB, IInfo *InsD, TranslationBlock &transIns );
static void bne_beq(IInfo *InsB, IInfo *InsD, TranslationBlock &transIns );
static void branch(IInfo *InsB, IInfo *InsD, TranslationBlock &transIns );
static void bne_beq_likely(IInfo *InsB, IInfo *InsD, TranslationBlock &transIns );
static void branch_likely(IInfo *InsB, IInfo *InsD, TranslationBlock &transIns );

/*----jump-----*/
static void jump(IInfo *InsJ, TranslationBlock &transIns );
static void jumpReg(IInfo *InsJ, TranslationBlock &transIns );
static void And_Link(TranslationBlock &transIns );

static void j(IInfo *InsJ, IInfo *InsD, TranslationBlock &transIns, uint64_t );
static void jr(IInfo * InsJ, IInfo * InsD, TranslationBlock &transIns );
static void jal(IInfo * InsJ, IInfo * InsD, TranslationBlock &transIns, uint64_t );
static void jalr(IInfo * InsJ, IInfo * InsD, TranslationBlock &transIns );

/*----*/
static void syscall(TranslationBlock &transIns );
static void sync(TranslationBlock &transIns);
static void nop(TranslationBlock &transIns);
static void cache(TranslationBlock &transIns);
static void rdhwr(IInfo *InsP, TranslationBlock &transIns );
static void br(TranslationBlock &transIns );
static void eret(TranslationBlock &transIns );
static void deret(TranslationBlock &transIns);
static void pref(TranslationBlock &transIns);
static void ssnop(TranslationBlock &transIns);
static void wait(TranslationBlock &transIns );
static void TerminateBlock(TranslationBlock &transIns );
private://don't want to instantiate
	Translation_IPE ();
	virtual ~Translation_IPE ();
};

#endif	/* TRANSLATION_IPE_H */


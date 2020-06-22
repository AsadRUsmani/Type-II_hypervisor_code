
#include "CP0_IPE.h"
#include "Common.h"
#include "Logger.h"


Logger::Logger(Processor_IPE *core) {
    this->core = core;
    this->CP_Flag = 0;
    this->file_no = 1;
    this->count_pc = 1;
}
Logger::Logger(const Logger& orig) {
}
Logger::~Logger() {
}

void Logger::writePcFile(uint64_t pc){
     sprintf(this->filename,"%s_%d.txt",fileCount,this->core->getCP0()->corenum());
     this->fp1 = fopen(this->filename, "a+");
     if( this->fp1 == NULL ){ P::EPRINT(FILE_NOTFOUND_ERROR);}
     fprintf(this->fp1,"0x%016llx\n",pc);//pc);
     fclose(this->fp1);
}
void Logger::writeStateFile(uint64_t pc){
    sprintf(this->filename,"%s_%d.txt",((file_no == 1)?file_1:((file_no==2)?file_2:((file_no==3)?file_3:file_4))),this->core->getCP0()->corenum());
    
    this->fp1 = fopen(this->filename, "a+");
    if( this->fp1 == NULL){ P::EPRINT(FILE_NOTFOUND_ERROR);}
    fprintf(this->fp1,"0x%016llx\n",pc);
    
    for(int i =0; i< 32 ; i++){
        fprintf(this->fp1,"0x%016llx\n",this->core->getGPR()->read(i));
    }
    
    fprintf(this->fp1,"#\n");
    fclose(this->fp1);
    
//    if(this->CP_Flag == 1){
//        sprintf(this->filename,"%s_%d.txt",file_2,(this->core->getCP0()->read(H2_CP0_EBASE_1,1) & CORE_NUM_MASK));
//        
//    this->fp2 = fopen(this->filename, "a+");
//    if( this->fp2 == NULL){ P::EPRINT(FILE_NOTFOUND_ERROR);}
//    
//    fprintf(this->fp2,"PC = 0x%016llx\n",pc);
//    fprintf(this->fp2,"CP0_INDEX        :0x%016llx\n", this->core->getCP0()->read(H2_CP0_INDEX, SEL_ZERO));
//    fprintf(this->fp2,"CP0_RANDOM       :0x%016llx\n", this->core->getCP0()->read(H2_CP0_RANDOM, SEL_ZERO));
//    fprintf(this->fp2,"CP0_ENTRYLO0     :0x%016llx\n", this->core->getCP0()->read(H2_CP0_ENTRYLO0, SEL_ZERO));
//    fprintf(this->fp2,"CP0_ENTRYLO1     :0x%016llx\n", this->core->getCP0()->read(H2_CP0_ENTRYLO1, SEL_ZERO));
//    fprintf(this->fp2,"CP0_CONTEXT      :0x%016llx\n", this->core->getCP0()->read(H2_CP0_CONTEXT, SEL_ZERO));
//    fprintf(this->fp2,"CP0_PAGEMASK     :0x%016llx\n", this->core->getCP0()->read(H2_CP0_PAGEMASK, SEL_ZERO));
//    fprintf(this->fp2,"CP0_PAGEGRAIN_1  :0x%016llx\n", this->core->getCP0()->read(H2_CP0_PAGEGRAIN_1, SEL_ONE));
//    fprintf(this->fp2,"CP0_WIRED        :0x%016llx\n", this->core->getCP0()->read(H2_CP0_WIRED, SEL_ZERO));
//    fprintf(this->fp2,"CP0_HWRENA       :0x%016llx\n", this->core->getCP0()->read(H2_CP0_HWRENA, SEL_ZERO));
//    fprintf(this->fp2,"CP0_BADVADDR     :0x%016llx\n", this->core->getCP0()->read(H2_CP0_BADVADDR , SEL_ZERO));
//    fprintf(this->fp2,"CP0_COUNT        :0x%016llx\n", this->core->getCP0()->read(H2_CP0_COUNT, SEL_ZERO));
//    fprintf(this->fp2,"CP0_ENTRYHI      :0x%016llx\n", this->core->getCP0()->read(H2_CP0_ENTRYHI, SEL_ZERO));
//    fprintf(this->fp2,"CP0_COMPARE      :0x%016llx\n", this->core->getCP0()->read(H2_CP0_COMPARE, SEL_ZERO));
//    fprintf(this->fp2,"CP0_STATUS       :0x%016llx\n", this->core->getCP0()->read(H2_CP0_STATUS, SEL_ZERO));
//    fprintf(this->fp2,"CP0_INTCTL_1     :0x%016llx\n", this->core->getCP0()->read(H2_CP0_INTCTL_1, SEL_ONE));
//    fprintf(this->fp2,"CP0_SRSCTL_      :0x%016llx\n", this->core->getCP0()->read(H2_CP0_SRSCTL_2, SEL_TWO));
//    fprintf(this->fp2,"CP0_CAUSE        :0x%016llx\n", this->core->getCP0()->read(H2_CP0_CAUSE, SEL_ZERO));
//    fprintf(this->fp2,"CP0_EPC          :0x%016llx\n", this->core->getCP0()->read(H2_CP0_EPC, SEL_ZERO));
//    fprintf(this->fp2,"CP0_PRID         :0x%016llx\n", this->core->getCP0()->read(H2_CP0_PRID, SEL_ZERO));
//    fprintf(this->fp2,"CP0_EBASE_1      :0x%016llx\n", this->core->getCP0()->read(H2_CP0_EBASE_1, SEL_ONE));
//    fprintf(this->fp2,"CP0_CONFIG       :0x%016llx\n", this->core->getCP0()->read(H2_CP0_CONFIG, SEL_ZERO));
//    fprintf(this->fp2,"CP0_CONFIG_1     :0x%016llx\n", this->core->getCP0()->read(H2_CP0_CONFIG_1, SEL_ONE));
//    fprintf(this->fp2,"CP0_CONFIG_2     :0x%016llx\n", this->core->getCP0()->read(H2_CP0_CONFIG_2, SEL_TWO));
//    fprintf(this->fp2,"CP0_CONFIG_3     :0x%016llx\n", this->core->getCP0()->read(H2_CP0_CONFIG_3, SEL_THREE));
//    fprintf(this->fp2,"CP0_IWATCHL      :0x%016llx\n", this->core->getCP0()->read(H2_CP0_IWATCHLO, SEL_ZERO));
//    fprintf(this->fp2,"CP0_DWATCHLO_1   :0x%016llx\n", this->core->getCP0()->read(H2_CP0_DWATCHLO_1, SEL_ONE));
//    fprintf(this->fp2,"CP0_IWATCHHI     :0x%016llx\n", this->core->getCP0()->read(H2_CP0_IWATCHHI, SEL_ZERO));
//    fprintf(this->fp2,"CP0_DWATCHHI_1   :0x%016llx\n", this->core->getCP0()->read(H2_CP0_DWATCHHI_1, SEL_ONE));
//    fprintf(this->fp2,"CP0_XCONTEXT     :0x%016llx\n", this->core->getCP0()->read(H2_CP0_XCONTEXT, SEL_ZERO));
//    fprintf(this->fp2,"CP0_DEBUG        :0x%016llx\n", this->core->getCP0()->read(H2_CP0_DEBUG, SEL_ZERO));
//    fprintf(this->fp2,"CP0_DEBUG_EPC    :0x%016llx\n", this->core->getCP0()->read(H2_CP0_DEBUG_EPC, SEL_ZERO));
//    fprintf(this->fp2,"CP0_PERFCNTCTL   :0x%016llx\n", this->core->getCP0()->read(H2_CP0_PERFCNTCTL, SEL_ZERO));
//    fprintf(this->fp2,"CP0_PERFCNTCTL_2 :0x%016llx\n", this->core->getCP0()->read(H2_CP0_PERFCNTCTL_2, SEL_TWO));
//    fprintf(this->fp2,"CP0_PERFCNTCNT_1 :0x%016llx\n", this->core->getCP0()->read(H2_CP0_PERFCNTCNT_1, SEL_ONE));
//    fprintf(this->fp2,"CP0_PERFCNTCNT_3 :0x%016llx\n", this->core->getCP0()->read(H2_CP0_PERFCNTCNT_3, SEL_THREE));
//    fprintf(this->fp2,"CP0_ERROREPC     :0x%016llx\n", this->core->getCP0()->read(H2_CP0_ERROREPC, SEL_ZERO));
//    fprintf(this->fp2,"CP0_DESAVE       :0x%016llx\n", this->core->getCP0()->read(H2_CP0_DESAVE, SEL_ZERO));
//
//    fprintf(this->fp2,"CP0_ICACHEERR    :0x%016llx\n", this->core->getCP0()->read(H2_CP0_ICACHEERR, SEL_ZERO));
//    fprintf(this->fp2,"CP0_DCACHEERR_1  :0x%016llx\n", this->core->getCP0()->read(H2_CP0_DCACHEERR_1, SEL_ONE));
//    fprintf(this->fp2,"CP0_ICACHETAGLO  :0x%016llx\n", this->core->getCP0()->read(H2_CP0_ICACHETAGLO, SEL_ZERO));
//    fprintf(this->fp2,"CP0_DCACHETAGLO_2:0x%016llx\n", this->core->getCP0()->read(H2_CP0_DCACHETAGLO_2, SEL_TWO));
//    fprintf(this->fp2,"CP0_ICACHEDATALO1:0x%016llx\n", this->core->getCP0()->read(H2_CP0_ICACHEDATALO_1, SEL_ONE));
//    fprintf(this->fp2,"CP0_DCACHEDATALO3:0x%016llx\n", this->core->getCP0()->read(H2_CP0_DCACHEDATALO_3, SEL_THREE));
//    fprintf(this->fp2,"CP0_DCACHETAGHI_2:0x%016llx\n", this->core->getCP0()->read(H2_CP0_DCACHETAGHI_2, SEL_TWO));
//    fprintf(this->fp2,"CP0_ICACHEDATAHI1:0x%016llx\n", this->core->getCP0()->read(H2_CP0_ICACHEDATAHI_1, SEL_ONE));
//    fprintf(this->fp2,"CP0_DCACHEDATAHI3:0x%016llx\n", this->core->getCP0()->read(H2_CP0_DCACHEDATAHI_3, SEL_THREE));
//    fprintf(this->fp2,"CP0_CVMCTL_7     :0x%016llx\n", this->core->getCP0()->read(H2_CP0_CVMCTL_7, SEL_SEVEN));
//    fprintf(this->fp2,"CP0_CVMMEMCTL_7  :0x%016llx\n", this->core->getCP0()->read(H2_CP0_CVMMEMCTL_7, SEL_SEVEN));
//    fprintf(this->fp2,"CP0_CVMCOUNT_6   :0x%016llx\n", this->core->getCP0()->read(H2_CP0_CVMCOUNT_6, SEL_SIX));
//    fprintf(this->fp2,"CP0_MULTICORE_DBG_LOG:0x%016llx\n", this->core->getCP0()->read(H2_CP0_MULTICORE_DEBUG, SEL_ZERO));
//
////    this->core->getTLB()->write_tlb_state(this->fp2);
//    fprintf(this->fp2,"#\n");
//    fclose(this->fp2);
//    }
}

void Logger::writeLogFile(uint64_t pc) {
//        if(this->core->getTrapCount() > 14359852){file_no = 3;}
//          if(LOG_GEN){file_no =3; }
//    if (pc == 0xffffffff84a04fc8LL && this->core->getTrapCount() > 85420000) {
////        file_no =3;
//        printf("OK!Trap_count=%ld, Pc_count=%ld\n", this->core->getTrapCount(), this->count_pc++);
////        //0xffffffff84a04fc8
//        this->core->getGPR()->write(0x0000000000000000,0);
//        this->core->getGPR()->write(0x0000000000000000,1);
//        this->core->getGPR()->write(0x0000000000000000,2);
//        this->core->getGPR()->write(0xa800000006888fc8,3);
//        this->core->getGPR()->write(0xfffffffffffffffc,4);
//        this->core->getGPR()->write(0xffffffff85d1bc48,5);
//        this->core->getGPR()->write(0x0000000000000004,6);
//        this->core->getGPR()->write(0x0000000000000000,7);
//        this->core->getGPR()->write(0x0000000000001f02,8);
//        this->core->getGPR()->write(0x0000000000040000,9);
//        this->core->getGPR()->write(0x0000000000000004,10);
//        this->core->getGPR()->write(0x0000000000000004,11);
//        this->core->getGPR()->write(0x0000000000000001,12);
//        this->core->getGPR()->write(0x0000000000dfffc8,13);
//        this->core->getGPR()->write(0x0000000000040000,14);
//        this->core->getGPR()->write(0x000000000002aef3,15);
//        this->core->getGPR()->write(0xffffffff85d1bc00,16);
//        this->core->getGPR()->write(0xffffffff85d34cc0,17);
//        this->core->getGPR()->write(0xffffffff85d30000,18);
//        this->core->getGPR()->write(0xffffffff85d00000,19);
//        this->core->getGPR()->write(0xffffffff84105b30,20);
//        this->core->getGPR()->write(0x0000000000000001,21);
//        this->core->getGPR()->write(0x0000000000000001,22);
//        this->core->getGPR()->write(0xffffffffc00d5b90,23);
//        this->core->getGPR()->write(0x0000000000000001,24);
//        this->core->getGPR()->write(0xffffffffffffffc0,25);
//        this->core->getGPR()->write(0xffffffffc00d9ef8,26);
//        this->core->getGPR()->write(0xfffffffffffffffe,27);
//        this->core->getGPR()->write(0xffffffff849ac000,28);
//        this->core->getGPR()->write(0xffffffff849afdf0,29);
//        this->core->getGPR()->write(0xffffffffc0057828,30);
//        this->core->getGPR()->write(0xffffffff84a04efc,31);
//    } 
////    else if(pc == 0xffffffff84a04714LL)
////    {
////        printf("\nOk!Trap_count=%ld PC=0x%016llx\n",this->core->getTrapCount(),pc);
////        file_no =3;
////        CP_Flag = 1;
////    }
//    else if (pc == 0xffffffff84a04744LL)
//    {
//        printf("\nOk!Trap_count=%ld PC=0x%016llx\n",this->core->getTrapCount(),pc);
//        this->core->getGPR()->write(0x1,2);
//    }
////    else if (pc == 0xffffffff8420ee5cLL)
////    {
////        printf("\nOk!Trap_count=%ld PC=0x%016llx\n",this->core->getTrapCount(),pc);
////        this->core->getGPR()->write(0xffffffff84afa158,2);
////    }
//    else if (pc == 0xffffffff841c8ce0LL)
//    {
//        printf("\nOk!Trap_count=%ld PC=0x%016llx",this->core->getTrapCount(),pc);
//        this->core->getGPR()->write(0x3,2);
//    }
//    else if (pc == 0xffffffff841008d4LL)
//    {
//        printf("\nOk!Trap_count=%ld PC=0x%016llx",this->core->getTrapCount(),pc);
//        this->core->getGPR()->write(0x4,12);
//    }
//    else if (pc == 0xffffffff841009a8LL)
//    {
//        printf("\nOk!Trap_count=%ld PC=0x%016llx",this->core->getTrapCount(),pc);
//        this->core->getGPR()->write(0x4,12);
//    }
  //  if(this->file_no == 3)
        this->writePcFile(pc);
//    if(this->file_no == 3)this->writeStateFile(pc);
    return;
}

void Logger::writeInsLog(std::vector<IInfo>* InsBlock, TranslationBlock *transBlock, uint64_t PC){
     sprintf(this->filename,"%s_%d.txt",fileAsm,this->core->getCP0()->corenum());
     
     this->fp1 = fopen(this->filename, "a+");
     if( this->fp1 == NULL ){ P::EPRINT(FILE_NOTFOUND_ERROR);}
     fprintf(this->fp1,"\n#######################################################\n");
     fprintf(this->fp1,"\nBlock start PC is 0x%016llx\n\n",PC);
     fclose(this->fp1);
     
     if(Guest_Block){
         this->fp1 = fopen(this->filename, "a+");
         fprintf(this->fp1,"\n*********Original Block*********\n");
         fclose(this->fp1);
         this->writeInsBlock(InsBlock);
     }
     
     if(Trans_Block){
         this->fp1 = fopen(this->filename, "a+");
         fprintf(this->fp1,"\n********Translated Block********\n");
         fclose(this->fp1);
         this->writeAsmBlock((void*)transBlock->data(), (uint32_t)(transBlock->size() * 4));
     }
     
     if(GPR_LOG)
     {
         this->fp1 = fopen(this->filename, "a+");
         fprintf(this->fp1,"\n********General Purpose Registers********\n\n");
         for(int i =0; i< 32 ; i++)
            fprintf(this->fp1,"Reg[%d]\t=\t0x%016llx\n",i, this->core->getGPR()->read(i));
         fclose(this->fp1);
     }
     
     if(CPR_LOG)
     {
         this->fp1 = fopen(this->filename, "a+");
         fprintf(this->fp1,"\n*********CoProcessor 0 Registers*********\n\n");
         this->writeCPRegisters();
         fclose(this->fp1);
     }
     
}

void Logger::writeInsBlock(std::vector<IInfo>* InsBlock)
{
    long i = 0;    
    int sz = InsBlock->size();
    uint32_t data[sz];
    for (i = 0; i < sz; i++)        
        data[i] = InsBlock->at(i).encoding;
    this->writeAsmBlock((void*)data, sz * 4);
}

void Logger::writeAsmBlock(void *data, uint32_t size)
{
    sprintf(this->tmpfilename,"%s_%d.bin",fileTmp,this->core->getCP0()->corenum());
    
    this->fp2 = fopen(this->tmpfilename, "w+");
    fwrite(data, size, 1, this->fp2);
    fclose(this->fp2);
    
    sprintf(this->command,"%s %s %s | %s >> %s",objdump_path,objdump_arg,this->tmpfilename,sed_cmd,this->filename);
    system(this->command);
}

void Logger::writeCPRegisters()
{
    fprintf(this->fp1,"CP0_INDEX        :0x%016llx\n", this->core->getCP0()->read(H2_CP0_INDEX, SEL_ZERO));
    fprintf(this->fp1,"CP0_RANDOM       :0x%016llx\n", this->core->getCP0()->read(H2_CP0_RANDOM, SEL_ZERO));
    fprintf(this->fp1,"CP0_ENTRYLO0     :0x%016llx\n", this->core->getCP0()->read(H2_CP0_ENTRYLO0, SEL_ZERO));
    fprintf(this->fp1,"CP0_ENTRYLO1     :0x%016llx\n", this->core->getCP0()->read(H2_CP0_ENTRYLO1, SEL_ZERO));
    fprintf(this->fp1,"CP0_CONTEXT      :0x%016llx\n", this->core->getCP0()->read(H2_CP0_CONTEXT, SEL_ZERO));
    fprintf(this->fp1,"CP0_PAGEMASK     :0x%016llx\n", this->core->getCP0()->read(H2_CP0_PAGEMASK, SEL_ZERO));
    fprintf(this->fp1,"CP0_PAGEGRAIN_1  :0x%016llx\n", this->core->getCP0()->read(H2_CP0_PAGEGRAIN_1, SEL_ONE));
    fprintf(this->fp1,"CP0_WIRED        :0x%016llx\n", this->core->getCP0()->read(H2_CP0_WIRED, SEL_ZERO));
    fprintf(this->fp1,"CP0_HWRENA       :0x%016llx\n", this->core->getCP0()->read(H2_CP0_HWRENA, SEL_ZERO));
    fprintf(this->fp1,"CP0_BADVADDR     :0x%016llx\n", this->core->getCP0()->read(H2_CP0_BADVADDR , SEL_ZERO));
    fprintf(this->fp1,"CP0_COUNT        :0x%016llx\n", this->core->getCP0()->read(H2_CP0_COUNT, SEL_ZERO));
    fprintf(this->fp1,"CP0_ENTRYHI      :0x%016llx\n", this->core->getCP0()->read(H2_CP0_ENTRYHI, SEL_ZERO));
    fprintf(this->fp1,"CP0_COMPARE      :0x%016llx\n", this->core->getCP0()->read(H2_CP0_COMPARE, SEL_ZERO));
    fprintf(this->fp1,"CP0_STATUS       :0x%016llx\n", this->core->getCP0()->read(H2_CP0_STATUS, SEL_ZERO));
    fprintf(this->fp1,"CP0_INTCTL_1     :0x%016llx\n", this->core->getCP0()->read(H2_CP0_INTCTL_1, SEL_ONE));
    fprintf(this->fp1,"CP0_SRSCTL_      :0x%016llx\n", this->core->getCP0()->read(H2_CP0_SRSCTL_2, SEL_TWO));
    fprintf(this->fp1,"CP0_CAUSE        :0x%016llx\n", this->core->getCP0()->read(H2_CP0_CAUSE, SEL_ZERO));
    fprintf(this->fp1,"CP0_EPC          :0x%016llx\n", this->core->getCP0()->read(H2_CP0_EPC, SEL_ZERO));
    fprintf(this->fp1,"CP0_PRID         :0x%016llx\n", this->core->getCP0()->read(H2_CP0_PRID, SEL_ZERO));
    fprintf(this->fp1,"CP0_EBASE_1      :0x%016llx\n", this->core->getCP0()->read(H2_CP0_EBASE_1, SEL_ONE));
    fprintf(this->fp1,"CP0_CONFIG       :0x%016llx\n", this->core->getCP0()->read(H2_CP0_CONFIG, SEL_ZERO));
    fprintf(this->fp1,"CP0_CONFIG_1     :0x%016llx\n", this->core->getCP0()->read(H2_CP0_CONFIG_1, SEL_ONE));
    fprintf(this->fp1,"CP0_CONFIG_2     :0x%016llx\n", this->core->getCP0()->read(H2_CP0_CONFIG_2, SEL_TWO));
    fprintf(this->fp1,"CP0_CONFIG_3     :0x%016llx\n", this->core->getCP0()->read(H2_CP0_CONFIG_3, SEL_THREE));
    fprintf(this->fp1,"CP0_IWATCHL      :0x%016llx\n", this->core->getCP0()->read(H2_CP0_IWATCHLO, SEL_ZERO));
    fprintf(this->fp1,"CP0_DWATCHLO_1   :0x%016llx\n", this->core->getCP0()->read(H2_CP0_DWATCHLO_1, SEL_ONE));
    fprintf(this->fp1,"CP0_IWATCHHI     :0x%016llx\n", this->core->getCP0()->read(H2_CP0_IWATCHHI, SEL_ZERO));
    fprintf(this->fp1,"CP0_DWATCHHI_1   :0x%016llx\n", this->core->getCP0()->read(H2_CP0_DWATCHHI_1, SEL_ONE));
    fprintf(this->fp1,"CP0_XCONTEXT     :0x%016llx\n", this->core->getCP0()->read(H2_CP0_XCONTEXT, SEL_ZERO));
    fprintf(this->fp1,"CP0_DEBUG        :0x%016llx\n", this->core->getCP0()->read(H2_CP0_DEBUG, SEL_ZERO));
    fprintf(this->fp1,"CP0_DEBUG_EPC    :0x%016llx\n", this->core->getCP0()->read(H2_CP0_DEBUG_EPC, SEL_ZERO));
    fprintf(this->fp1,"CP0_PERFCNTCTL   :0x%016llx\n", this->core->getCP0()->read(H2_CP0_PERFCNTCTL, SEL_ZERO));
    fprintf(this->fp1,"CP0_PERFCNTCTL_2 :0x%016llx\n", this->core->getCP0()->read(H2_CP0_PERFCNTCTL_2, SEL_TWO));
    fprintf(this->fp1,"CP0_PERFCNTCNT_1 :0x%016llx\n", this->core->getCP0()->read(H2_CP0_PERFCNTCNT_1, SEL_ONE));
    fprintf(this->fp1,"CP0_PERFCNTCNT_3 :0x%016llx\n", this->core->getCP0()->read(H2_CP0_PERFCNTCNT_3, SEL_THREE));
    fprintf(this->fp1,"CP0_ERROREPC     :0x%016llx\n", this->core->getCP0()->read(H2_CP0_ERROREPC, SEL_ZERO));
    fprintf(this->fp1,"CP0_DESAVE       :0x%016llx\n", this->core->getCP0()->read(H2_CP0_DESAVE, SEL_ZERO));

    fprintf(this->fp1,"CP0_ICACHEERR    :0x%016llx\n", this->core->getCP0()->read(H2_CP0_ICACHEERR, SEL_ZERO));
    fprintf(this->fp1,"CP0_DCACHEERR_1  :0x%016llx\n", this->core->getCP0()->read(H2_CP0_DCACHEERR_1, SEL_ONE));
    fprintf(this->fp1,"CP0_ICACHETAGLO  :0x%016llx\n", this->core->getCP0()->read(H2_CP0_ICACHETAGLO, SEL_ZERO));
    fprintf(this->fp1,"CP0_DCACHETAGLO_2:0x%016llx\n", this->core->getCP0()->read(H2_CP0_DCACHETAGLO_2, SEL_TWO));
    fprintf(this->fp1,"CP0_ICACHEDATALO1:0x%016llx\n", this->core->getCP0()->read(H2_CP0_ICACHEDATALO_1, SEL_ONE));
    fprintf(this->fp1,"CP0_DCACHEDATALO3:0x%016llx\n", this->core->getCP0()->read(H2_CP0_DCACHEDATALO_3, SEL_THREE));
    fprintf(this->fp1,"CP0_DCACHETAGHI_2:0x%016llx\n", this->core->getCP0()->read(H2_CP0_DCACHETAGHI_2, SEL_TWO));
    fprintf(this->fp1,"CP0_ICACHEDATAHI1:0x%016llx\n", this->core->getCP0()->read(H2_CP0_ICACHEDATAHI_1, SEL_ONE));
    fprintf(this->fp1,"CP0_DCACHEDATAHI3:0x%016llx\n", this->core->getCP0()->read(H2_CP0_DCACHEDATAHI_3, SEL_THREE));
    fprintf(this->fp1,"CP0_CVMCTL_7     :0x%016llx\n", this->core->getCP0()->read(H2_CP0_CVMCTL_7, SEL_SEVEN));
    fprintf(this->fp1,"CP0_CVMMEMCTL_7  :0x%016llx\n", this->core->getCP0()->read(H2_CP0_CVMMEMCTL_7, SEL_SEVEN));
    fprintf(this->fp1,"CP0_CVMCOUNT_6   :0x%016llx\n", this->core->getCP0()->read(H2_CP0_CVMCOUNT_6, SEL_SIX));
    fprintf(this->fp1,"CP0_MULTICORE_DBG_LOG:0x%016llx\n", this->core->getCP0()->read(H2_CP0_MULTICORE_DEBUG, SEL_ZERO));
}

/* 
 * File:   FileHandler.h
 * Author: usamanwar
 *
 * Created on September 25, 2013, 12:55 PM
 */

#ifndef FILEHANDLER_H
#define	FILEHANDLER_H

#include <stdint.h>
#include "Processor_IPE.h"

#define Guest_Block       1
#define Trans_Block       0
#define GPR_LOG           0
#define CPR_LOG           0

#define filePC  "./LOG_Folder/LogFile_PC"
#define fileCount "./LOG_Folder/LogFile_count"
#define file_1  "./LOG_Folder/LogFile_GPRegs1"
#define file_2  "./LOG_Folder/LogFile_GPRegs2"
#define file_3  "./LOG_Folder/LogFile_GPRegs3"
#define file_4  "./LOG_Folder/LogFile_GPRegs4"
#define file_5  "./LOG_Folder/LogFile_GPRegs5"
#define file_6  "./LOG_Folder/LogFile_GPRegs6"

#define fileAsm "./LOG_Folder/LogFile_assembly"
#define fileTmp "./LOG_Folder/temp"
#define objdump_path    "/usr/local/Cavium_Networks/OCTEON-SDK/tools/usr/bin/mips64-octeon-linux-gnu-objdump"
#define objdump_arg     "-b binary -m mips -d -D -EB"
#define sed_cmd         "sed -e '2,7d'"

class Logger {
public:
   
    Logger(Processor_IPE *core);
    Logger(const Logger& orig);
    virtual ~Logger();
    
    void writeLogFile(uint64_t pc);
    void writeInsLog(std::vector<IInfo>*, TranslationBlock*, uint64_t);
    
private:
    
    
    inline void writeStateFile(uint64_t pc);
    inline void writePcFile(uint64_t pc); 
    inline void writeInsBlock(std::vector<IInfo>*);
    inline void writeTransBlock(TranslationBlock*);
    inline void writeAsmBlock(void *data, uint32_t size);
    inline void writeCPRegisters(void);
    
private:
    int file_no;
    int CP_Flag;
    int count_pc;
    FILE *fp1;
    FILE *fp2;
    Processor_IPE *core;
    
    char filename[50];
    char tmpfilename[50];
    char command[200];
};

#endif	/* FILEHANDLER_H */


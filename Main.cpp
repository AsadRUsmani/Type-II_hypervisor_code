/* 
 * File:   main.cpp
 * Author: asadusmani
 *
 * Created on February 12, 2014, 11:48 AM
 */


#include "SystemBoard_IPE.h"
#include "CLParser.h"




int main(int argc, char** argv) {
    CLParser cmd_line(argc,argv,true);
    
    printf("Process started with ID %ld\n",(long)getpid());
    SystemBoard_IPE sysBoard(cmd_line.get_specs()); //Pass number of core required in constructor of system board.
    
    return (EXIT_SUCCESS);
}


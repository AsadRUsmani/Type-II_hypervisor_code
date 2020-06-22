/* 
 * File:   CLParser.cpp
 * Author: usamanwar
 * 
 * Created on 18 September 2015, 15:37
 */

#include "CLParser.h"
#include <stdlib.h>

/** Constructor for Command Line Parser
  * This class parses the commandline arguments and give them in form of specifications class
  * @param argc_: Number of arguments
  * @param argv_: Pointer to arguments
  * @param switches_on_: Switch for parsing
  * 
  * @retval none
  */
CLParser::CLParser(int argc_, char * argv_[],bool switches_on_)
{
    argc=argc_;
    argv.resize(argc);
    copy(argv_,argv_+argc,argv.begin());
    switches_on=switches_on_;

    //map the switches to the actual
    //arguments if necessary
    if (switches_on)
    {
        vector<string>::iterator it1,it2;
        it1=argv.begin();
        it2=it1+1;

        while (true)
        {
            if (it1==argv.end()) break;
            if (it2==argv.end()) break;

            if ((*it1)[0]=='-')
                switch_map[*it1]=*(it2);

            it1++;
            it2++;
        }
    }
}

/** Gives argument for specified switch
  * @param s: Argument switch
  * 
  * @retval value for required switch
  */
string CLParser::get_arg(string s)
{
    if (!switches_on) return "";

    if (switch_map.find(s)!=switch_map.end())
        return switch_map[s];

    return "";
}

/** Parse arguments to specifications class
  * @param none
  * 
  * @retval specifications class having required board specifications
  */
Specifications* CLParser::get_specs()
{
    Specifications *specs = new Specifications();
    
    string temp;
    
    temp = this->get_arg("-h");
    if(temp == "help")
    {
        printf("For no of cores type:       -cores <no of cores>\n");
        printf("For uboot path type:        -uboot <path> \n");
        printf("For kernel image path type: -kernel <path>\n");
        return 0;
    }
    
    /*No of Cores CMD Argument*/
    temp = this->get_arg("-cores");
    int core_val = atoi(temp.c_str());
    if (temp!="") 
    {
        specs->setNumCores(core_val);
    }
    else
    {
        printf("Please type \"-h help\" for help \n");
        return 0;
    }
    
     /*Uboot path CMD Argument*/
    temp = this->get_arg("-uboot");
    if (temp!="") 
    {
        specs->setUbootPath(temp.c_str());
    }
    else
    {
        printf("Please type \"-h help\" for help \n");
        return 0;
    }
       
    /*No of Cores CMD Argument*/
    temp = this->get_arg("-kernel");
    if (temp!="") 
    {
        specs->setKernelPath(temp.c_str());
    }
    else
    {
        printf("Please type \"-h help\" for help \n");
        return 0;
    }
    
    return specs;
}

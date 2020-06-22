/* 
 * File:   CLParser.h
 * Author: usamanwar
 *
 * Created on 18 September 2015, 15:37
 */

#ifndef CLPARSER_H
#define	CLPARSER_H

#include "Specifications.h"
#include <vector>
#include <map>
#include <string>

using namespace std;

class CLParser
{
public:

    CLParser(int argc_, char * argv_[],bool switches_on_=false);
    ~CLParser(){}
    Specifications* get_specs();

private:
    string get_arg(int i);
    string get_arg(string s);

private:

    int argc;
    vector<string> argv;

    bool switches_on;
    map<string,string> switch_map;
};

#endif	/* CLPARSER_H */


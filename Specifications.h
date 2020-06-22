/* 
 * File:   Specifications.h
 * Author: usamanwar
 *
 * Created on 18 September 2015, 15:42
 */

#ifndef SPECIFICATIONS_H
#define	SPECIFICATIONS_H


#define MAXCORES                    16

class Specifications {
public:
    Specifications();
    virtual ~Specifications();
    void setNumCores(int);
    void setUbootPath(const char*);
    void setKernelPath(const char*);
    int getNumCores();
    const char* getUbootPath();
    const char* getKernelPath();
private:
    int numcores;
    const char *uboot_path;
    const char *kernel_path;

};

#endif	/* SPECIFICATIONS_H */


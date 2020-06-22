/* 
 * File:   Specifications.cpp
 * Author: usamanwar
 * 
 * Created on 18 September 2015, 15:42
 */

#include "Specifications.h"

/** Constructor for Specifications
  * This class holds all the specifications related to virtual board
  * @param none
  * 
  * @retval none
  */
Specifications::Specifications()
{
}

/** Destructor for Specifications
  * @param none
  * 
  * @retval none
  */
Specifications::~Specifications()
{
}

/** Sets number of cores
  * @param num = number of cores
  * 
  * @retval none
  */
void Specifications::setNumCores(int num)
{
    this->numcores = (num > 0 && num < MAXCORES) ? num: 1;
}

/** Sets Uboot path
  * @param path = path of bootloader
  * 
  * @retval none
  */
void Specifications::setUbootPath(const char* path)
{
    this->uboot_path = path;
}

/** Sets Kernel path
  * @param path = path of kernel
  * 
  * @retval none
  */
void Specifications::setKernelPath(const char* path)
{
    this->kernel_path = path;
}

/** Gives number of cores
  * @param none
  * 
  * @retval number of cores
  */
int Specifications::getNumCores()
{
    return this->numcores;
}

/** Gives Uboot path
  * @param none
  * 
  * @retval path of bootloader
  */
const char* Specifications::getUbootPath()
{
    return this->uboot_path;
}

/** Gives kernel path
  * @param none
  * 
  * @retval path of kernel
  */
const char* Specifications::getKernelPath()
{
    return this->kernel_path;
}


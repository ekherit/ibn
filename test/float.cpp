/*
 * =====================================================================================
 *
 *       Filename:  float.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12.01.2019 13:32:43
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan B. Nikolaev (ekherit), I.B.Nikolaev@inp.nsk.su
 *   Organization:  Budker Insitute of Nuclear Physics
 *
 * =====================================================================================
 */

#include <iostream>
#include "../Float.h"

int main()
{
  Float<3'322'323'632'423'452'353,-6234235236234546235> f;
  std::cout << f << std::endl;
  std::cout << sizeof(long) << std::endl;
  std::cout << sizeof(long) << std::endl;
};

/*
 * =====================================================================================
 *
 *       Filename:  averager.cpp
 *
 *    Description:  Test for averager algorithm
 *
 *        Version:  1.0
 *        Created:  24.12.2018 12:33:14
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan B. Nikolaev (ekherit), I.B.Nikolaev@inp.nsk.su
 *   Organization:  Budker Insitute of Nuclear Physics
 *
 * =====================================================================================
 */

#include <vector>
#include "../averager.h"

using namespace std;
int main(int argc, char ** argv)
{
  ibn::averager<double> av;
  std::cout << "data: ";
  for(int i=0;i<10;i++)
  {
    std::cout << i << " ";
    av.add(i);
  }
  std::cout << std::endl;
  std::cout << "mean = " << av.average() << std::endl;
  std::cout << "RMS = " << av.rms() << std::endl;
  std::cout << "sigma = " << av.sigma() << std::endl;
  std::cout << "sigma_average = " << av.sigma_average() << std::endl;
}

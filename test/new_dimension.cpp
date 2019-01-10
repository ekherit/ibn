/*
 * =====================================================================================
 *
 *       Filename:  new_dimension.cpp
 *
 *    Description:  test for new dimension algorithm
 *
 *        Version:  1.0
 *        Created:  08.01.2019 00:34:07
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan B. Nikolaev (ekherit), I.B.Nikolaev@inp.nsk.su
 *   Organization:  Budker Insitute of Nuclear Physics
 *
 * =====================================================================================
 */

#include "../new_dim.h"

#include <type_traits>
#include <typeinfo>
#include <iostream>
#include <iomanip>
#include <map>
#include <bitset>
using namespace std;

std::map<int, std::string> DMAP;

template <int Id1, typename R1> 
std::ostream & operator<<(std::ostream & os, BaseDimension<Id1, R1> )
{
  os <<DMAP[Id1]; 
  if(R1::den !=1 || R1::num<0)
  {
  os<<"^(" << R1::num;
  if(R1::den !=1) os << "/"<<R1::den;
  os << ")";
  }
  else os << "^" << R1::num;
  return os;
}

template <typename B1> 
std::ostream & operator<<(std::ostream & os,  Dimension<B1>)
{
  return os << B1();
}

template <typename B1, typename ...Bs> 
std::ostream & operator<<(std::ostream & os,  Dimension<B1,Bs...>)
{
  return os << B1() << "·" << Dimension<Bs...>();
}

std::ostream & operator<<(std::ostream & os,  Dimension<>)
{
  return os << "none";
}

int main()
{
  int test_number = 0;
  using D =  Dimension<>;
  using D0 = new_dimension<0>;
  DMAP[0]="D0";
  using D1 = new_dimension<1>;
  DMAP[1]="D1";
  using D2 = new_dimension<2>;
  DMAP[2]="D2";
  std::cout << "D:" << D() << std::endl;
  std::cout << "D0:" << D0() << std::endl;
  std::cout << "D1:" << D1() << std::endl;

  int shift=20;
  using DxD = Multiply<D,D>::type;
  std::cout << "Test # " << ++test_number << " multiplication on dimensionless: " << std::endl;
  std::cout << setw(shift) << "D  * D = " << DxD() << endl;
  using DxD0 = Multiply<D,D0>::type;
  std::cout << setw(shift) << "D  * D0 = " << DxD0() << endl;
  using D0xD = Multiply<D0,D>::type;
  std::cout << setw(shift) << "D0 * D  = " << D0xD() << endl;


  using test = Test<D0,D1>::type;
  std::cout << "Test : " << test() << std::endl;

  using D0xD0 =  Multiply<D0,D0>::type;
  std::cout << "Test square of D0: D0 * D0 =  " << D0xD0() <<std::endl;
  using D0xD0xD0 =  Multiply<D0,D0xD0>::type;
  std::cout << "Test square of D0: D0 * D0 * D0 =  " << D0xD0xD0() <<std::endl;
  //std::cout << "DR = " << Multiply<D0,D0>::DR() << std::endl;
  //std::cout << "D1DR = " << Multiply<D0,D0>::MD1DR() << std::endl;
  //std::cout << "MM = " << Multiply<D0,D1>::MM() << std::endl;
  //std::cout << "id1 = " << Multiply<D1,D0>::id1 << std::endl;
  //std::cout << "id2 = " << Multiply<D1,D0>::id2 << std::endl;
  using D0xD1 =  Multiply<D0,D1>::type;
  std::cout << "                   D0 * D1 =  " << D0xD1() <<std::endl;
  using D1xD0 =  Multiply<D1,D0>::type;
  std::cout << "                   D1 * D0 =  " << D1xD0() <<std::endl;
  using D1xD2 =  Multiply<D1,D2>::type;
  std::cout << "                   D1 * D2 =  " << D1xD2() <<std::endl;

  using A = Dimension<BaseDimension<3, std::ratio<-5,1> >> ;
  std::cout << "Test square of A =  " << A() <<std::endl;

 using D0xD1xD2 = Multiply<D0,D1xD2>::type;
  std::cout << "Test square of D0xD1xD2 =  " << D0xD1xD2() <<std::endl;
  using D2xD0xD1 = Multiply<D2,D0xD1>::type;
  std::cout << "Test square of D2xD0xD1 =  " << D2xD0xD1() <<std::endl;
  //std::cout << "ID(7) = " << hex << int('7') << bitset<8>('7') << std::endl;
}

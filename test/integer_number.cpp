/*
 * =====================================================================================
 *
 *       Filename:  integer_number.cpp
 *
 *    Description:  Test for compile time integer number
 *
 *        Version:  1.0
 *        Created:  20.01.2019 11:55:00
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan B. Nikolaev (ekherit), I.B.Nikolaev@inp.nsk.su
 *   Organization:  Budker Insitute of Nuclear Physics
 *
 * =====================================================================================
 */
#include "../IntegerNumber.h"
#include <iostream>
#include <iomanip>

std::ostream & operator<<(std::ostream & os,  NaturalNumber<>) {return os;}

template<typename D1, typename...Ds>
std::ostream & operator<<(std::ostream & os,  NaturalNumber< D1,Ds...> d)
{
  //std::cout << D1::digit << " " << NaturalNumber< Ds...>();
  std::cout << NaturalNumber< Ds...>() << D1::digit;
  return os;
};

template<typename N, bool p>
std::ostream & operator<<(std::ostream & os,  IntegerNumber<N,p>)
{
  std::cout << ( p ? '+' : '-' ) << N();
  return os;
};

  template<long N,long B=10> using mint = make_integer_number_t<N,B>;


template<long N1, long N2, long Base = 10>
void test(void)
{
  std::cout << std::setfill('#') << std::setw(80) << "" << std::endl;
  std::cout << std::setfill(' ');
  using I1 = mint<N1,Base>;
  using I2 = mint<N2,Base>;
  std::cout << "Test initialization for: " << N1                      << " : " << I1()  << "  using base " << Base  << std::endl;
  std::cout << "Test initialization for: " << N2                      << " : " << I2()  << "  using base " << Base  << std::endl;
  std::cout << std::setw(25)               << "Test addition: "       << std::setw(10) << N1    << " + " << std::setw(10) << N2              << " = " << std::setw(15) << N1+N2      << " = "  << std::setw(15) << add_t<I1,I2>()      << std::endl;
  std::cout << std::setw(25)               << "Test subtraction: "    << std::setw(10) << N1    << " - " << std::setw(10) << N2              << " = " << std::setw(15) << N1 - N2    << " = "  << std::setw(15) << sub_t<I1,I2>()      << std::endl;
  std::cout << std::setw(25)               << "Test multiplication: " << std::setw(10) << N1    << " * " << std::setw(10) << N2              << " = " << std::setw(15) << N1*N2      << " = "  << std::setw(15) << multiply_t<I1,I2>() << std::endl;
};
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
  int
main ( int argc, char *argv[] )
{ 
  test<8,4>();
  test<4,8>();
  test<11,9>();
  test<111,99>();
  test<99,111>();
  test<-999,-1>();
  test<-10,-1000>();
  test<43234,12435,10>();
  test<43234,12435,48>();

}				/* ----------  end of function main  ---------- */


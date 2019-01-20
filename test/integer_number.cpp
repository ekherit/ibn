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

  template<long N> using mint = make_integer_number_t<N>;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
  int
main ( int argc, char *argv[] )
{ 
  using n5 = make_natural_number_t<5>;
  using n10 = make_natural_number_t<10>;
  using n3 = make_natural_number_t<3>;

  using i5 = IntegerNumber<n5,true>;
  using i3 = IntegerNumber<n3,false>;
  using i10 = IntegerNumber<n10,false>;

  using s1 = sub_t<n10,n5>;
  std::cout << s1() << std::endl;

  using i = IntegerNumber<s1,false>;
  std::cout << i::number() << std::endl;
  using sum1 = add_t< i5, i10>;
  std::cout << "5 + -10 = " << sum1::number() << " positivity = " << sum1::positivity << std::endl;

  using sum2 = add_t< i10, i5>;
  std::cout << "-10+5  = " << sum2::number() << " positivity = " << sum2::positivity << std::endl;

  using s2 = sub_t<n5,n3>;
  std::cout << s2() << std::endl;

  using sum3 = add_t<i5,i3>;
  std::cout << "5+ -3  = " << sum3() << " positivity = " << std::endl;

  using I1 = make_integer_number_t<23423523>;
  using I2 = make_integer_number_t<-42353>; 
  using I3 = make_integer_number_t<-423533243>;


  std::cout << I1() << " + " << I2() <<  " = " << add_t<I1,I2>() << std::endl;
  std::cout << I1() << " + " << I3() <<  " = " << add_t<I1,I3>() << std::endl;
  std::cout << sub_t< I3::number, I1::number>() << std::endl;

  std::cout << sub_t< mint<109>::number, mint<89>::number >() << std::endl;
  std::cout << sub_t< mint<110>::number, mint<89>::number >() << std::endl;
  std::cout << sub_t< mint<18>::number, mint<9>::number >() << std::endl;
  std::cout << sub_t< mint<234>, mint<432> >() << std::endl;

  std::cout << multiply_t< mint<124>, mint<-34> >() << std::endl;

}				/* ----------  end of function main  ---------- */


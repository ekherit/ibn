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
#include "../new_unit.h"

#include <type_traits>
#include <typeinfo>
#include <iostream>
#include <iomanip>
#include <map>
#include <bitset>
using namespace std;

#include "../dimension_print.h"

int main()
{
  int test_number = 0;
  using D =  Dimension<>;
  using D0 = make_dimension<0>;
  DMAP[0]="D0";
  using D1 = make_dimension<1>;
  DMAP[1]="D1";
  using D2 = make_dimension<2>;
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

  DMAP[3]="A";
  using A = Dimension<BaseDimension<3, std::ratio<-5,1> >> ;
  std::cout << "Test square of A =  " << A() <<std::endl;

 using D0xD1xD2 = Multiply<D0,D1xD2>::type;
  std::cout << "Test square of D0xD1xD2 =  " << D0xD1xD2() <<std::endl;
  using D2xD0xD1 = Multiply<D2,D0xD1>::type;
  std::cout << "Test square of D2xD0xD1 =  " << D2xD0xD1() <<std::endl;
  //std::cout << "ID(7) = " << hex << int('7') << bitset<8>('7') << std::endl;
  //using D0xD1xD2_2 = Multiply<D0xD1, D2>::type;
  std::cout << "(D0xD1)xD2 =  " << D0xD1xD2() <<std::endl;
  using TEST_DIM2 = Dimension<BaseDimension<1>, BaseDimension<2> >;
  std::cout << "HEAD (1,2) " << typename Tail<TEST_DIM2>::type() << std::endl;
  using TEST_DIM3 = Dimension<BaseDimension<0>, BaseDimension<1>,BaseDimension<2> >;
  std::cout << "TEST_DIM3::size = " << TEST_DIM3::size << std::endl;
  std::cout << "first TEST_DIM3 = " << Dimension< First<TEST_DIM3>::type >() << std::endl;
  std::cout << "HEAD TEST_DIM3 " << typename Tail<TEST_DIM3>::type() << std::endl;

  auto s = make_unit<0>(); DMAP[0]="T";
  auto cm = make_unit<1>(); DMAP[1]="L";
  auto g = make_unit<2>(); DMAP[2]="M";

  auto J = g*cm*cm/s/s;
  using Length_t = decltype(cm)::dimension;
  using Time_t   = decltype(s)::dimension;
  using Mass_t   = decltype(g)::dimension;
  using Energy_t = decltype(J)::dimension;

  auto l = 2e5*cm+1.3e4*cm;
  auto t = 13*s;
  auto v = l/t;
  using Velocity_t = decltype(v)::dimension;

  using test = typename Multiply< Time_t, Velocity_t>::type;
  //using test2 = typename Multiply< Velocity_t, Time_t>::type;
  std::cout << v.data << " " <<Velocity_t() << std::endl;
  std::cout << "V*T : " << test() << std::endl;
  auto rho = 1.0*g/(cm*cm*cm);
  auto v2 = v*v;
  auto P = rho*v*v;
  auto test2 = P*v*t*l/(g*g*g*v2);
  std::cout << test2.data << " " << decltype(test2)::dimension() << std::endl;

  auto keV = 1.6e-12*J;
  double x = keV/J;
  std::cout << "electron rest energy: " << x << " Joules" << std::endl;

  //auto test3 = 1.0*J.pow<3>();
  using test3 = Power<D0,3>::type;
  std::cout << "test 3 = "  << test3() << std::endl;
  //for(auto i : mm)
  //{
  //  std::cout << i.first << "   " << i.second << "  " << bitset<8>(i.first) << std::endl;
  //}
}

/*
 * =====================================================================================
 *
 *       Filename:  benchmark_dimension_unit.cpp
 *
 *    Description:  Flight of the body in atmosphere with friction
 *
 *        Version:  1.0
 *        Created:  11.01.2019 18:31:05
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan B. Nikolaev (ekherit), I.B.Nikolaev@inp.nsk.su
 *   Organization:  Budker Insitute of Nuclear Physics
 *
 * =====================================================================================
 */

#include "../new_unit.h"
int main(int argc,char ** argv)
{
  using Length_t = make_unit<0>;
  using Mass_t  = make_unit<1>;
  using Time_t = make_unit<2>;
  auto m = Length_t(); //meter
  auto s = Time_t(); //second
  auto kg = Mass_t(); //kilogramm

  using Velocity_t = decltype(m/s);
  using Acceleration_t = decltype(Velocity_t()/s);
  using Force_t = decltype(m*Acceleration_t());

  //Environment parametrs
  const Acceleration_t g = 9.80665*(m/s/s);
  const auto rho = 1.2754*kg/(m*m*m);

  //body parameters
  const Mass_t M = 1.0*kg;
  const auto S = 0.1*m*0.1*m; //Front cross section

};

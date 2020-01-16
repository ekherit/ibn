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

#include "../unit.h"
#include "../unit_print.h"
#include <iostream>
using namespace ibn;
using namespace std;

int main(int argc,char ** argv)
{
  using Length_t = double;
  using Mass_t  = double;
  using Time_t = double;
  const auto m = Length_t{1.0}; //meter
  const auto s = Time_t{1.0}; //second
  const auto kg = Mass_t{1.0}; //kilogramm

  /*
  DMAP[0]="m";
  DMAP[1]="kg";
  DMAP[2]="s";
  */
  using Velocity_t     = decltype(m/s);
  using Acceleration_t = decltype(Velocity_t()/s);
  using Force_t        = decltype(m*Acceleration_t());

  //Environment parametrs
  const Acceleration_t g{9.80665*(m/s/s)};
  const auto rho = 1.2754*kg/(m*m*m);

  //body parameters
  const auto M = 1.0*kg;
  const auto S = 0.1*m*0.1*m; //Front cross section
  auto dt = 1e-5*s;
  int idx=0;
  auto y0{0*m};
  auto y{y0};
  auto v0{10*m/s};
  auto v{v0};
  auto t = 0*s;
  auto y_exact{0*m};
  auto x0{0*m};
  auto x{x0};
  auto vx0{30*m/s};
  auto vx{vx0};
  for(; v>0*m/s; t+=dt,++idx ){
    y+= v*dt;
    v+= -g*dt;
    y_exact = y0-g*t*t/2.0;
    auto diff = (y-y_exact);
    auto rel_dif = diff/y_exact;
    x+= vx*dt;
    
    //std::cout << idx << "  " << t << "   " <<  v << "   " << y <<  "  " << y_exact << "  " << diff << "  " << rel_dif << std::endl; 
  }
  std::cout << "Up time: " << t/s << "s" << " ymax = " << y/m << "m " <<  " xmax = " << x/m << "m" <<  std::endl;
};

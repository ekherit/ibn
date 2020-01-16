/*
 * =====================================================================================
 *
 *       Filename:  flight.h
 *
 *    Description:  Body flight in gravity
 *
 *        Version:  1.0
 *        Created:  07.01.2020 18:15:37
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan B. Nikolaev (ekherit), I.B.Nikolaev@inp.nsk.su
 *   Organization:  Budker Insitute of Nuclear Physics
 *
 * =====================================================================================
 */
#pragma once
#include <iostream>

template < typename Length_t, typename Time_t, typename Mass_t> 
void flight(double x0d, double y0d, double vx0d, double vy0d, double dtd) {
  const auto m = Length_t{1.0}; //meter
  const auto s = Time_t{1.0};   //second
  const auto kg = Mass_t{1.0}; //kilogramm

  /*
  DMAP[0]="m";
  DMAP[1]="kg";
  DMAP[2]="s";
  */
  using Velosity_t = decltype(m/s);
  using Acceleration_t = decltype(Velosity_t()/s);
  using Force_t        = decltype(m*Acceleration_t());

  //Environment parametrs
  const Acceleration_t g{9.80665*(m/s/s)};
  //const auto rho = 1.2754*kg/(m*m*m);

  //body parameters
  //const auto M = 1.0*kg;
  const auto S = 0.1*m*0.1*m; //Front cross section
  int idx=0;
  Length_t x = x0d*m;
  Length_t y = y0d*m;

  Velosity_t vx = vx0d*m/s;
  Velosity_t vy = vy0d*m/s;

  Time_t t{0};
  Time_t dt = dtd*s;
  Acceleration_t ax{0};
  Acceleration_t ay{g};
  for(; vy>0*m/s; t+=dt,++idx ){

    y  += vy*dt;
    x  += vx*dt;

    vx += ax*dt;
    vy += ay*dt;

    ax = 0*(-g);
    ay  = -g;
    //std::cout << idx << "  " << t << "   " <<  v << "   " << y <<  "  " << y_exact << "  " << diff << "  " << rel_dif << std::endl; 
  }
  std::cout << "Up time: " << t/s << "s" << " ymax = " << y/m << "m " <<  " xmax = " << x/m << "m" <<  std::endl;
}


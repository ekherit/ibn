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
#include "flight.h"
#include <chrono>
using namespace ibn;
using namespace std;
using namespace std::chrono;

int main(int argc,char ** argv)
{
  const double x0 = 0;
  const double y0 = 0;

  const double vx0 = 20;
  const double vy0 = 20;

  const double dt = 1e-8;

  auto t0{steady_clock::now()};
  flight<make_unit<0>, make_unit<1>, make_unit<2>> (x0,y0,vx0,vy0, dt);
  auto t1{steady_clock::now()};
  std::cout << "unit: " << duration_cast<duration<double>>(t1-t0).count() << "s" << endl;

  t0=steady_clock::now();
  flight<double, double, double> (x0,y0,vx0,vy0, dt);
  t1=steady_clock::now();
  std::cout << "double: " << duration_cast<duration<double>>(t1-t0).count() << "s" << endl;
};

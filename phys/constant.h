/*
 * =====================================================================================
 *
 *       Filename:  constant.h
 *
 *    Description:  Fundamental constants
 *
 *        Version:  1.0
 *        Created:  16.09.2013 16:21:27
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan B. Nikolaev (), I.B.Nikolaev@inp.nsk.su
 *        Company:  Budker Institute of Nuclear Physics, Novosibirsk, Russia
 *
 * =====================================================================================
 */
#ifndef IBN_PHYS_CONSTANT_H
#define IBN_PHYS_CONSTANT_H

#include "../unit.h"

namespace ibn
{

  namespace phys
  {
    static const double pi = M_PI;

    static const auto meter =    make_unit<0>(); 
    static const auto second =   make_unit<1>();
    static const auto kilogram = make_unit<2>();
    static const auto coloumb =  make_unit<3>();
    static const auto kelvin =   make_unit<4>();
    static const auto mole =     make_unit<5>();

    static const auto joule = meter*meter/second/second*kilogram;
    static const auto volt =  joule/coloumb;

    static const auto c  = 299'792'458*meter/second;    //exact 
    static const auto e  = 1.602'176'634e-19*coloumb;     //exact
    static const auto h  = 6.626'070'150e-34*joule*second; //exact 
    static const auto hbar = h/(2*M_PI); //exact*
    static const auto kb = 1.380'649e-23*joule/kelvin; //exact
    static const auto Na = 6.022'140'76e23/mole;  //exact



    static const auto cm = 1e-2*meter;
    static const auto km = 1e3*meter;
    static const auto mm = 1e-3*meter;
    static const auto um = 1e-6*meter;
    static const auto nm = 1e-9*meter;

    decltype(meter) operator "" m(long double x)  { return x*meter; } 
    decltype(meter) operator "" cm(long double x) { return x*cm; } 
    decltype(meter) operator "" mm(long double x) { return x*mm; } 
    decltype(meter) operator "" um(long double x) { return x*um; } 
    decltype(meter) operator "" nm(long double x) { return x*nm; } 

    static const auto eV = e*volt;
    static const auto keV = 1e3*eV;
    static const auto MeV = 1e6*eV;
    static const auto GeV = 1e9*eV;
    static const auto TeV = 1e12*eV;

    static const auto barn = 1e-24*cm*cm;
    static const auto mb = 1e-3*barn;
    static const auto nb = 1e-9*barn;
    static const auto pb = 1e-12*barn;


    //const double m_e = 0.410998918; //MeV
    //const double r_e = 2.817940325e-15; //m
    //const double c  = 299792458; //m/s
    const double hc = 197.326968e-15; //MeV*m
    //const double e_SI = 1.60217653e-19;//C electron charge in SI
    namespace PDG2019 {
      //static const double ALPHA=1/137.035999139(13); 
      static const double ALPHA       = 7.2973525664e-3;  
      static const double ALPHA_ERROR = 0.0000000017e-3;

      static const auto ME       = 0.5109989461*MeV; //2014 CODATA value
      static const auto ME_ERROR = 0.0000000031*MeV;

      static const auto MMU       = 105.6583745*MeV;
      static const auto MMU_ERROR =   0.0000024*MeV;

      static const auto MTAU       = 1776.860*MeV;
      static const auto MTAU_ERROR = 0.12*MeV;


      static const auto MJPSI = 3096.900*MeV;
      static const auto MJPSI_ERROR = 6.0*keV;

      static const auto MPSIP = 3686.097*MeV; 
      static const auto MPSIP_ERROR =10.0*keV;

      static const auto SIGMA_TOMSON = 0.665245854*barn; 
    };

    namespace PDG2020 {
      static const double ALPHA       = 7.297'352'5693e-3; 
      static const double ALPHA_ERROR = 0.000'000'0011e-3;

      static const auto ME       = 0.510'998'950'00*MeV; 
      static const auto ME_ERROR = 0.000'000'000'15*MeV;

      static const auto SIGMA_TOMSON       = 0.665'245'873'21*barn; 
      static const auto SIGMA_TOMSON_ERROR = 0.000'000'000'60*barn; 
    }

    using namespace PDG2019;
  }
}
#endif


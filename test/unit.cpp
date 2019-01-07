/*
 * =====================================================================================
 *
 *       Filename:  unit2.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  17.07.2017 22:22:30
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan B. Nikolaev (ekherit), I.B.Nikolaev@inp.nsk.su
 *   Organization:  Budker Insitute of Nuclear Physics
 *
 * =====================================================================================
 */

#include "../unit.h"
#include "../opt.h"


int main(int argc, char ** argv)
{
  ibn::SimpleOpt opt(argc,argv);
  if(opt.is("-b"))
  {
    using B0 = BaseDimension<0>;
    using B1 = BaseDimension<1>;
    using B2 = BaseDimension<2>;
    std::cout << "Test for printing of the dimensions:\n";
    std::cout << "B0: "   << B0()  << std::endl;
    std::cout << "B1: "   << B1()  << std::endl;
    std::cout << "B2: "   << B2()  << std::endl;

    using B02 = BaseDimension<0, std::ratio<2,1> >;
    using B05 = BaseDimension<0, std::ratio<5,1> >;
    using B0_10 = BaseDimension<0, std::ratio<1,10> >;
    using B0_minus_2_3 = BaseDimension<0, std::ratio<-2,3> >;
    std::cout << "Test for power initialization:\n";
    std::cout << "B0^2: " << B02() << std::endl;
    std::cout << "B0^5: " << B05() << std::endl;
    std::cout << "B0^(1/10): " << B0_10() << std::endl;
    std::cout << "B0^(-2/3): " << B0_minus_2_3() << std::endl;
    std::cout << "Test for dimension reduction:\n";
    using B02_3 = BaseDimension<0, std::ratio<8,12> >;
    std::cout << "B0^(8/12) = B0^(2/3): " << B02_3() << std::endl;

    std::cout << "Test of multiplication with same dimension" << std::endl;
    using B0B0 = typename Multiply<B0,B0>::type;
    std::cout << "B0*B0 = " << B0B0() << std::endl;
    std::cout << "B0*B0*B0 = " << Multiply<B0, B0B0 >::type()  << std::endl;

    std::cout << "Test for multiplication of different dimensions (result is sorted by dimension id)" << std::endl;
    using U01 = Multiply<B0,B1>::type;
    std::cout << "U01 = B0*B1 = " << U01() << std::endl;
    using U001 = Multiply<B0,U01>::type;
    std::cout << "U001 = B0*B0*B1 = " << U001() << std::endl;

    using U0001 = Multiply<B0,U001>::type;
    std::cout << "U0001 = B0*B0*B0*B1 = " << U0001() << std::endl;

    using U10001 = Multiply<B1,U0001>::type;
    std::cout << "U10001 = B1*B0*B0*B0*B1 = " << U10001() << std::endl;

    std::cout << "Проверка перемножения, когда схлопываются размерности" << std::endl;
    using U110001 = Multiply<B1, U10001>::type;
    std::cout << "U110001 = B1 * U10001   = "<< U110001() << std::endl;

    std::cout << "Проверка перемножения, когда схлопываются размерности" << std::endl;
    using U2110001 = Multiply<B2, U110001>::type;
    std::cout << "U2110001 = B2 * U110001   = "<< U2110001() << std::endl;

    std::cout << "Проверка перемножения, добавление степени в середине" << std::endl;
    using U12110001 = Multiply<B1, U2110001>::type;
    std::cout << "U12110001 = B1 * U2110001   = "<< U12110001() << std::endl;

    std::cout << "Проверка перемножения двух составных размерностей" << std::endl;
    using UU = Multiply<U2110001, U12110001>::type;
    std::cout << "U12110001 * U2110001   = "<< UU() << std::endl;

    std::cout << "Проверка перемножения справа" << std::endl;
    using UU = Multiply<U2110001, U12110001>::type;
    std::cout << "U12110001 * U2110001   = "<< UU() << std::endl;
  }


  if(opt.is("-p"))
  {
    using Length = unit<Dimension<BaseDimension<0>>>;
    using Time =   unit<Dimension<BaseDimension<1>>>;
    using Mass =   unit<Dimension<BaseDimension<2>>>;
    using Energy = unit<Dimension<BaseDimension<3>>>;

    //проверка физических величин
    Mass g; //this is the gramm
    Length cm; //this is cm
    Time  s; //second

    using Area = decltype(cm*cm);
    using Volume = decltype(Area()*cm);
    Area theArea = 28*32*cm*cm;
    Volume theVol = theArea*2*cm;
    std::cout << "theArea = " << theArea << " cm^2" << std::endl;
    std::cout << "theVol = " << theVol << " cm^3" << std::endl;

    auto minutes = 60*s;
    auto day = 86400*s;
    auto month = 30*day;
    auto year = 12*month;
    std::cout << "day is " << day << " s" << std::endl;
    std::cout << "month is " << month << " s" << std::endl;
    std::cout << "year approximate is " << year << " seconds" << std::endl;
    //std::cout << "year approximate is " << double(year/day) << " days" << std::endl;
    std::cout << "One minute is " << minutes << std::endl;
    //std::cout << "year approximate is " << year/minutes << " minutes" << std::endl;

    // auto test_sum = cm + g; this is unable to compile

    auto test = cm*s;
    std::cout << "cm has dimention: " << decltype(cm)::dimension() << std::endl;
    std::cout << "g has dimention: " << decltype(g)::dimension() << std::endl;
    std::cout << "s has dimention: " << decltype(s)::dimension() << std::endl;
    std::cout << "area has dimention: " << Area::dimension() << std::endl;
    std::cout << "cm*s = " << test << " dimension = " << decltype(test)::dimension() << std::endl;
    std::cout << "cm*cm*s = " << cm*cm*s << " dimension = " << decltype(cm*cm*s)::dimension() << std::endl;
    std::cout << "cm^3*s = " << cm*cm*cm*s << " dimension = " << decltype(cm*cm*cm*s)::dimension() << std::endl;
    std::cout << "cm^3*s^2 = " << s*cm*cm*cm*s << " dimension = " << decltype(s*cm*cm*cm*s)::dimension() << std::endl;
    std::cout << "cm^3*s^2*g = " << s*cm*cm*cm*s*g << " dimension = " << decltype(s*cm*cm*cm*s*g)::dimension() << std::endl;

    std::cout << "Test for division: " << std::endl;
    std::cout << "velocity: cm/s = " << cm/s << " dimension = " << decltype(cm/s)::dimension() << std::endl;

    //speed of light
    auto c = 2.99792458e10*cm/s;
    std::cout << "Speed of light c = " << c << " cm/s" << std::endl;
    std::cout << "Light travel for year : " << c*year << " cm " << decltype(c*year)::dimension() << std::endl;

    auto nodim_test = c/c;
    std::cout << "Dimentionless c/c = " << nodim_test << std::endl;


    auto some_dim1 = 125*cm*cm*cm/s/g*s*s;
    auto some_dim2 = some_dim1*3*cm;
    std::cout << some_dim2/some_dim1 << " cm " << decltype(some_dim2/some_dim1)::dimension() << std::endl;

    auto minutes_in_the_year = (year/minutes);
    std::cout << "year has " << minutes_in_the_year << " minutes" << " dim = " << decltype(year/minutes)::dimension() << std::endl;

    auto test_for_dimensionless = year/minutes + 20.;
    auto test_for_mimentionless_subtraction = year/day -  5.; 
    std::cout << "Test for demintion less add " << test_for_dimensionless << ", dim = " << std::endl;
    std::cout << "Test for demintion less subtraction " << test_for_mimentionless_subtraction << std::endl;

    unit<BaseDimension<10>> eV;
    auto keV = 1e3*eV;
    auto MeV = 1e6*eV;
    auto GeV = 1e9*eV;

    auto me = 0.511*MeV;
    auto J = 1./1.6e-19*eV;
    auto erg = 1./1.6e-12*eV;

    std::cout << "Electron mass is : "  << me/erg/(c*c) << " g " << std::endl;

    //double a = 1.0*me/J;
    //std::cout << "a = " << a << std::endl;
    //double b = me/J + 0.0;

    //unit<20,3,double> nodim;
    //std::cout << " me/J * month/day: " <<  decltype(me/J*month/day)::dimension() << std::endl;
    double b =me/J+0.0;
    std::cout << "b = " << b << std::endl;
    //double b2 = me*month/J/day+0.0;
    //std::cout << "b2 = " << b2 << std::endl;
    //std::cout << decltype(me/J)::dimension() << std::endl;

    ///std::cout << "Test for power" <<std::endl;
    ///auto keV2 = pow(keV/eV,2.0);
    ///std::cout << keV2 << std::endl;

    //  std::cout << "dimesionless(Dimension<B0>) == " << is_dimensionless<Dimension<BaseDimension<0>>>::value << std::endl;
    //  std::cout << "dimesionless(B0) == " << is_dimensionless<BaseDimension<0>>::value << std::endl;
    //auto candela = make_unit();
    //std::cout << "Test for make_unit: candella dimension: " << decltype(candela)::dimension() << std::endl;

    //auto ampere = make_unit();
    //std::cout << "Test for make_unit: ampere dimension: " << decltype(ampere)::dimension() << std::endl;
  }

  if(opt.is("--SGS"))
  {
    using Time = new_dimension<0>;
    using Length = new_dimension<1>;
    using Mass = new_dimension<2>;

    auto s =  unit<Time>();
    auto cm = unit<Length>();
    auto g =  unit<Mass>();
    std::cout << "Time: " << decltype(s)::dimension() << std::endl;
    std::cout << "Length: " << decltype(cm)::dimension() << std::endl;
    std::cout << "Mass: " << decltype(g)::dimension() << std::endl;
    auto a = cm/s/s;
    std::cout << "Acceleration: " << decltype(a)::dimension() <<std::endl;
    auto F =  a*g;
    std::cout << "Force: " << decltype(F)::dimension() <<std::endl;
    auto J =  a*g*cm;
    std::cout << "Energy: " << decltype(J)::dimension() <<std::endl;
    std::cout << "Energy/Force: " << decltype(J/F)::dimension() <<std::endl;
    auto eV = 1.6e-19*J;
    //double x = eV/J;
    std::cout << "eV = " << eV/J << " J" << std::endl;
  }
}


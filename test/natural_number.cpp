/*
 * =====================================================================================
 *
 *       Filename:  natural_number.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12.01.2019 20:41:39
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan B. Nikolaev (ekherit), I.B.Nikolaev@inp.nsk.su
 *   Organization:  Budker Insitute of Nuclear Physics
 *
 * =====================================================================================
 */

#include "../Float.h"

#include <any>
#include <iostream>
#include <sstream>
#include <string>
#include <bitset>
#include <iomanip>
using namespace std;

std::ostream & operator<<(std::ostream & os,  NaturalNumber<>) {return os;}

template<typename D1, typename...Ds>
std::ostream & operator<<(std::ostream & os,  NaturalNumber< D1,Ds...> d)
{
  std::cout << D1::digit << " " << NaturalNumber< Ds...>();
  return os;
};

//template<typename A,typename B> using add  = typename Add<A,B>::type;

template<typename A, typename B> 
auto add(A a, B b) { return typename Add<A,B>::type(); };

template<typename Number>
struct Str;

template<>
struct Str< NaturalNumber<> >
{
  public:
    Str(int){}
    std::string value="";
};

template<typename Number>
struct Str
{
  private:
  using D1 = typename FirstDigit<Number>::type;
  using Nt = typename TailNumber<Number>::type;
  using str = Str<Nt>;
  public:
  Str(int opt = 2)
  {
    value += str(opt).value;
    switch(opt)
    {
      case 0: value+=to_string(D1::digit); 
        break;
      case 1: value+=std::bitset<sizeof(D1::digit)*8>(D1::digit).to_string(); 
        break;
      case 2: {
                std::stringstream oss;
                oss << std::setw(sizeof(D1::digit)*2) << std::hex << std::setfill('0') << D1::digit;
                value+=oss.str();
              }
              break;
      default:
              break;
    };
  };
  std::string value;
};

template<typename Number>
std::ostream & operator<<(std::ostream & os, const Str<Number> & s)
{
  using D1 = typename FirstDigit<Number>::type;
  //os <<  std::setw(sizeof(D1)*8*Number::size)  << s.value;
  os << s.value;
  //os << std::setw(sizeof(typename D1::type)*2*Number::size) << s.value;
  return os;
};

template<int n, typename N>
struct Nx2
{
  using N2 = typename Add<N,N>::type;
  using N21 = typename Add<NaturalNumber<Digit<1>>, N2>::type;
  static void print(int opt, int width,char c)
  {
    std::cout << std::setw(width) << std::setfill(c) << Str<N>(opt) << std::endl;
    Nx2<n-1,N21>::print(opt,width,c);
  };
};

template<typename N>
struct Nx2<0, N>
{
  static void print(int opt, int width, char c )
  {
    std::cout << std::setw(width) << std::setfill(c) << Str<N>(opt) << std::endl;
  };
};





int main()
{
  using n1 = NaturalNumber<Digit<4'135'146'346>>;
  using n2 = NaturalNumber<Digit<4'105'146'345>>;
  //using n1 = NaturalNumber<Digit<122>>;
  //using n2 = NaturalNumber<Digit<100>>;
  using sum = typename Add<n1,n2>::type;
  using sum2 = typename Add<sum,sum>::type;
  using sum4 = typename Add<sum2,sum2>::type;
  using sum8 = typename Add<sum4,sum4>::type;
  std::cout << sum() << std::endl;
  std::cout << sum2() << std::endl;
  std::cout << sum4() << std::endl;
  std::cout << sum8() << std::endl;
  using n4 = NaturalNumber<Digit<1>,Digit<2>, Digit<3>, Digit<4>>;
  using n6 = NaturalNumber<Digit<5>,Digit<6>, Digit<7>, Digit<8>, Digit<9>, Digit<10>>;
  using sum9 = typename Add<n4,n6>::type;
  std::cout << "sum9=" << sum9() <<std::endl;
  static const int N=512;
  Nx2<N-1, NaturalNumber<Digit<1>>>::print(2,N/4,'0');
  //std::cout << Str<sum9>() << std::endl;
};



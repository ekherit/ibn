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
#include <vector>
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
  Str(int opt = 0)
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
                //oss << std::setw(sizeof(D1::digit)*2) << std::hex << std::setfill('0') << D1::digit;
                oss << std::hex << D1::digit;
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
  os << s.value;
  return os;
};

template<int n, typename N>
struct Nx2
{
  using N2 = typename Add<N,N>::type;
  using N21 = typename Add<NaturalNumber<Digit<1>>, N2>::type;
  static void print(int opt=0, int width=0,char c=' ')
  {
    std::cout << std::setw(width) << std::setfill(c) << Str<N>(opt) << std::endl;
    Nx2<n-1,N2>::print(opt,width,c);
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

template<int n, typename N=NaturalNumber<Digit<1>>>
struct Inc
{
  using N21 = typename Add<NaturalNumber<Digit<1>>, N>::type;
  static void print(int opt=0, int width=0,char c=' ')
  {
    std::cout << std::setw(width) << std::setfill(c) << Str<N>(opt) << std::endl;
    Inc<n-1,N21>::print(opt,width,c);
  };
};

template<typename N>
struct Inc<0, N>
{
  static void print(int opt, int width, char c )
  {
    std::cout << std::setw(width) << std::setfill(c) << Str<N>(opt) << std::endl;
  };
};


//template <int N>
//constexpr vector<int> range(void)
//{
//  std::vector<int> r(N);
//  for (int i{0};i<N;++i) r[i] = i;
//  return r;
//};

int main()
{
  using n1 = NaturalNumber<Digit<1>>;
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
  static const int N=128;
  Nx2<N-1, NaturalNumber<Digit<1>>>::print();
  //std::cout << Str<sum9>() << std::endl;
  using n9 = NaturalNumber<Digit<9>>;
  using n10 = typename Add< n9, n1>::type;
  std::cout << Str<n10>(0) << std::endl;
  using b1 = NaturalNumber<Digit<1,2-1>>;
  using o1 = NaturalNumber<Digit<1,8-1>>;
  Nx2<20, o1>::print();
  //Inc<800>::print();

  using D8 = make_number_t<8>;
  std::cout << Str<D8>() << std::endl;
  using H256 = make_number_t<256, 15>;
  std::cout << "H256=" << Str<H256>() << std::endl;
  using H255 = make_number_t<255, 15>;
  std::cout << "H255=" << Str<make_number_t<65535,15>>(2) << std::endl;
  using O8 = make_number_t<8,7>;
  std::cout << "O8=" << Str<O8>() << std::endl;
  using r = RealNumber<n9, n10>;
  using n = make_integer_t<23>;

  using D35 = make_number_t<35>;
  using D42 = make_number_t<42>;
  std::cout << "35 < 42 = " << Less<D35,D42>::value << std::endl;
  std::cout << "42 < 35 = " << Less<D42,D35>::value << std::endl;
  std::cout << "3534563 < 3534562 = " << Less<make_number_t<3534563>,make_number_t<3534562>>::value << std::endl;
  std::cout << "3534563 < 3534563 = " << Less<make_number_t<3534563>,make_number_t<3534563>>::value << std::endl;
  std::cout << "3534563 < 3534564 = " << Less<make_number_t<3534563>,make_number_t<3534564>>::value << std::endl;
  std::cout << Str<n4>() << " <  " << Str<n6>() << " = " << Less<n4,n6>::value << std::endl;

  //std::bitset<16> a(5);
  //std::bitset<16> b(255);
  //auto ab = multiply_bitset<16>(a,b);
  //std::cout <<"  a = " <<  a << std::endl;
  //std::cout <<"  b = " <<  b << std::endl;
  //std::cout <<" ab = " <<  ab << std::endl;
  //std::cout <<" ab = " <<  ab.to_ulong() << "   " << a.to_ulong()*b.to_ulong() << std::endl;
  //std::bitset<8>  byte(235);
  //std::bitset<16> word(3253);

  using D3 = make_number_t<3>;
  //using D3xD42 = Multiply<D3,D42>::type;
  //using r5 = typename Range<5>::type;
  //auto printer = [](int d) { std::cout << d<< std::endl; return true; };
  //using for1 = For<r5, decltype(printer)>;

  //for(int i{0}; i<100; ++i)
  //{
  //  using a  = make_number_t<i>;
  //  for(int j{0};j<100;++j)
  //  {
  //  }
  //}
};



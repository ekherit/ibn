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

#include "../NaturalNumber.h"

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
  //std::cout << D1::digit << " " << NaturalNumber< Ds...>();
  std::cout << NaturalNumber< Ds...>() << D1::digit;
  return os;
};

//template<typename A,typename B> using add  = typename Add<A,B>::type;

//template<typename A, typename B> 
//auto add(A a, B b) { return typename Add<A,B>::type(); };

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
  //using D1 = typename FirstDigit<Number>::type;
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

//template <typename T>
//std::string Show(const  T  & t);
//
//template <bool>
std::string Show(const bool & t)
{
  return t ? "True" : "False";
};

int main()
{
  using n1 = make_natural_number_t<1>;
  std::cout << "Initialization 1 = " << n1() << std::endl;
  using n2 = make_natural_number_t<2>;
  std::cout << "Initialization 2 = " << n2() << std::endl;
  using n27 = make_natural_number_t<27>;
  std::cout << "Initialization 27 = " << n27() << std::endl;
  using sum_1_2 = add_t<n1,n2>;
  std::cout << "1+2 = " << sum_1_2() << std::endl;
  using n3 = make_natural_number_t<3>;
  std::cout << "n3 = " << n3() << std::endl;
  std::cout << "n1+n2  = " << sum_1_2() << std::endl;
  //static_assert( is_same< n3, sum_1_2>::value );
  using sum_27_3 = add_t<n27,n3>;
  std::cout << "27+3 = " << sum_27_3() << std::endl;
  using n92356 = make_natural_number_t<92356>;
  std::cout << "92356 = " << n92356() << std::endl;
  using n73212 = make_natural_number_t<73212>;
  std::cout << "73212 = " << n73212() << std::endl;
  using sum1 = add_t<n92356, n73212>;
  
  std::cout << "12356 + 73212 = " << sum1() << std::endl;
  std::cout << "Whether 12356 + 73212 = 165568 ?  " << Show(is_equal<make_natural_number_t<165568>, sum1 >) << std::endl;
  std::cout << "Whether 12356 + 73212 = 4626734574 ?  " << Show(is_equal<make_natural_number_t<462673457>, sum1 >) << std::endl;

  std::cout << "Check comparizon:" << std::endl;
  std::cout << "Whether 1 < 2 ? " << Show(is_less<n1,n2>) << std::endl;
  std::cout << "Whether 2 < 1 ? " << Show(is_less<n2,n1>) << std::endl;
  using n6 = multiply_t<n2,n3>;
  std::cout << n6() << std::endl;
  using n18 = multiply_t<n6,n3>;
  using n18x18 = multiply_t<n18, n18>;
  std::cout << n18x18() << std::endl;;
  using n104976 =multiply_t<n18x18,n18x18>;
  using n104976pow2 =multiply_t<n104976,n104976>;
  std::cout << n104976pow2() << std::endl;;
  using n18_b3 = make_natural_number_t<18,3>;
  std::cout << n18_b3() << std::endl;
  //using test_sum = add_t<n18_b3,n18>;
  //std::cout << test_sum() << std::endl;
  constexpr uint32_t max =std::numeric_limits<uint32_t>::max();
  constexpr uint64_t max2 =std::numeric_limits<uint32_t>::max();
  using big_n  = make_natural_number_t< max2,10>;
  std::cout << "uint32_t max = " << max << std::endl;
  std::cout << big_n() << std::endl;
  //std::cout << multiply_t<big_n,big_n>() << std::endl;
  std::cout << to_base_t<n27,3>() << std::endl;;
  std::cout << make_natural_number_t<27,3>() << std::endl;
  using big2 = multiply_t<big_n,big_n>;
  using big3 =multiply_t<big2,big2>;
  //using big4 =multiply_t<big3,big3>;
  //using big5 =multiply_t<big4,big4>;
  //using big6 =multiply_t<big5,big5>;
  std::cout << "big2 = " << big2() << std::endl;
  std::cout << "big3 = " << big3() << std::endl;
  //std::cout << "big4 = " << big4() << std::endl;
  //std::cout << "big4_nb = " << to_base_t<big4, max2>() << std::endl;;
  //std::cout << "big5 = " << big5() << std::endl;
  //std::cout << "big6 = " << big6() << std::endl;

  using sub1 = sub_t<big3,big2>;
  std::cout << sub1() << std::endl;

  using sub2 = sub_t<n2,n1>;
  std::cout << sub2() << std::endl;
  //using sub2= typename Subtract<big2,big3>::type;

  //using big_n2  = make_natural_number_t< std::numeric_limits<uint32_t>::max(), 1>;
};



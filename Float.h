/*
 * =====================================================================================
 *
 *       Filename:  Float.h
 *
 *    Description:  Just playing with template metaprogramming
 *
 *        Version:  1.0
 *        Created:  12.01.2019 13:31:39
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan B. Nikolaev (ekherit), I.B.Nikolaev@inp.nsk.su
 *   Organization:  Budker Insitute of Nuclear Physics
 *
 * =====================================================================================
 */

#ifndef IBN_FLOAT_H
#define IBN_FLOAT_H
#pragma once

#include<type_traits>
#include<limits>
#include<cmath>
#include<bitset>
#include <tuple>

template< unsigned long d , unsigned long num_base=1>
struct Digit
{ 
  using type = decltype(d);
  static const type digit = d;
  static const type max = num_base-1;
  static const type base = num_base;
  using unit = Digit<1,num_base>;
};

template <typename ... Ds>
struct NaturalNumber
{
  static const int size = sizeof...(Ds);
};




template < typename N>
struct FirstDigit;

template <typename D1, typename...Ds>
struct FirstDigit < NaturalNumber<D1,Ds...> >
{
  using type = D1;
};

template < typename N>
struct TailNumber;

template <typename D1, typename...Ds>
struct TailNumber < NaturalNumber<D1,Ds...> >
{
  using type = NaturalNumber<Ds...>;
};

template <typename N1, typename N2>
struct Concatenate;

template<typename A1, typename...Bs>
struct Concatenate<NaturalNumber<A1>, NaturalNumber<Bs...>>
{
  using type = NaturalNumber<A1,Bs...>;
};

template<typename N2>
struct Concatenate<NaturalNumber<>, N2>
{
  using type = N2;
};


template <unsigned long d, unsigned long base=10> 
struct MakeNumber
{
  using type = typename std::conditional< 
                      base==1, 
                      NaturalNumber< Digit<d,base> >,
                      typename Concatenate < 
                                              NaturalNumber< Digit<d % base, base> >, 
                                              typename std::conditional< 
                                                                          (d/base) ==0,
                                                                          NaturalNumber<>,
                                                                          typename MakeNumber<d/base, base>::type
                                                                       >::type
                                >::type
                     >::type ;
};

template <unsigned long base> 
struct MakeNumber<0,base>
{
  using type = NaturalNumber<Digit<0,base>>;
};

template <unsigned long d, unsigned long base=10> 
using make_number_t = typename MakeNumber<d,base>::type;

template<typename NA, typename NB>
struct Add
{
  private:
    using A1 = typename FirstDigit<NA>::type;
    using NAt = typename TailNumber<NA>::type;
    using B1 = typename FirstDigit<NB>::type;
    using NBt = typename TailNumber<NB>::type;

    static const typename A1::type max_digit =  A1::base == 1 ? std::numeric_limits<typename A1::type>::max() : A1::max;
    static const typename A1::type base =  A1::base;

    using sumA1B1  =  NaturalNumber<typename std::conditional<
      base == 1, //full reprezentaion by long 
      Digit< A1::digit + B1::digit, 1>, 
      Digit< (A1::digit + B1::digit) % (A1::max+1), base> //num-base reperezentation
    >::type>;

    static const bool overflow =  max_digit - A1::digit < B1::digit;

  public:
    using type = typename std::conditional< overflow,
      typename Concatenate< sumA1B1, typename Add< NAt, typename Add< NaturalNumber<typename A1::unit>, NBt>::type >::type>::type,
      typename Concatenate< sumA1B1, typename Add< NAt, NBt>::type >::type
      >::type;
};

template<typename...As>
struct Add< NaturalNumber<As...>, NaturalNumber<> >
{
  using type = NaturalNumber<As...>;
};

template<typename B, typename...Bs>
struct Add< NaturalNumber<>, NaturalNumber<B,Bs...> >
{
  using type = NaturalNumber<B,Bs...>;
};

template<typename NA, typename NB>
struct Equal
{
  using A1 = typename FirstDigit<NA>::type;
  using B1 = typename FirstDigit<NB>::type;
  using NAs = typename TailNumber<NA>::type;
  using NBs = typename TailNumber<NB>::type;
  static const bool value = A1::digit == B1::digit && Equal<NAs,NBs>::value;
};

template<typename A1, typename...As>
struct Equal< NaturalNumber<A1,As...>, NaturalNumber<>>
{
  static const bool value = false;
};

template<typename B1, typename...Bs>
struct Equal<NaturalNumber<>, NaturalNumber<B1,Bs...>>
{
  static const bool value = false;
};

template<>
struct Equal<NaturalNumber<>, NaturalNumber<>>
{
  static const bool value = true;
};


template<typename NA, typename NB>
struct Less
{
  private:
    using A1 = typename FirstDigit<NA>::type;
    using B1 = typename FirstDigit<NB>::type;
    using NAs = typename TailNumber<NA>::type;
    using NBs = typename TailNumber<NB>::type;
  public:
    static const bool value = Equal<NAs,NBs>::value ? A1::digit < B1::digit : Less<NAs,NBs>::value;
};

template<typename...As>
struct Less< NaturalNumber<As...>, NaturalNumber<> >
{
  static const bool value = false;
};

template<typename B, typename...Bs>
struct Less< NaturalNumber<>, NaturalNumber<B,Bs...> >
{
  static const bool value = true;
};



template<typename NA, typename NB>
struct Multiply
{
  private:
    using A1 = typename FirstDigit<NA>::type;
    using NAt = typename TailNumber<NA>::type;
    using B1 = typename FirstDigit<NB>::type;
    using NBt = typename TailNumber<NB>::type;
  public:
//    using type = typename Add
};

template<std::size_t N>
constexpr bool operator+=(std::bitset<N>  & R,  const std::bitset<N>  B)
{
  bool carry=false;
  for(size_t i{0}; i<N;++i) 
  {
    constexpr auto sum = (R[i] ^ B[i]) ^ carry;
    carry = (R[i] && B[i]) || (R[i] && carry) || (B[i] && carry);
    R[i] =  sum;
  }
  return carry;
};

template<typename Type>
constexpr std::tuple<Type,Type> operator<<(  std::tuple<Type,Type> b, int n)
{
  const size_t size = sizeof(Type)*8;
  const Type max = std::numeric_limits<unsigned>::max();
  Type mask = ~(max>>n);
  Type carry_data = (mask & std::get<0>(b)) >> (size-n);
  mask>>=(size-n);
  auto low = std::get<0>(b) << n;
  auto high = (std::get<1>(b) << n) | carry_data;
};

template<typename Type>
constexpr std::tuple<Type,Type,bool> long_sum (  std::tuple<Type,Type> b1, std::tuple<Type,Type> b2 )
{
  //const size_t size = sizeof(Type)*8;
  const Type max = std::numeric_limits<unsigned>::max();
  std::tuple<Type,Type,bool> result;
  std::get<0>(result) = std::get<0>(b1) + std::get<0>(b2);
  std::get<1>(result) = std::get<1>(b1) + std::get<1>(b2);
  bool carry1 = max - std::get<0>(b1) < std::get<0>(b2);
  bool carry2 = max - std::get<1>(b1) < std::get<1>(b2);
  if(carry1) 
  {
    if(std::get<1>(result) == max) carry2=true; 
    std::get<1>(result)+=1; 
  }
  std::get<2>(result) = carry2;
  return result;
};


template<typename Type>
constexpr std::tuple<Type,Type,bool> long_multiply(Type d1, Type d2)
{
  std::tuple<Type,Type> b1{d1,0};
  std::tuple<Type,Type> b2{d2,0};
  std::tuple<Type,Type,bool> R; //the result 
  const size_t N = sizeof(Type)*8;
  auto get = [&N](std::tuple<Type,Type> & b1,  int i) -> bool
  {
    if(i<N) return (std::get<0>(b1)>>i) & 0x1;
    else return (std::get<1>(b1) >> (i-N) ) & 0x1;
  };
  for(size_t i{0}; i<N; ++i) if(get(b1,i)) R = long_sum(R, (b2 << i));
  return R;
};

//template< std::size_t N>
//constexpr auto sum_bitset( const std::bitset<N> & a, const std::bitset<N> & b ) -> std::bitset<N> 
//{
//  std::bitset<N> R(0);
//  bool carry=false;
//  for(size_t i{0}; i<N;++i) 
//  {
//    auto constexpr sum = (a[i] ^b[i]) ^ carry;
//    carry = (a[i] && b[i]) || (a[i] && carry) || (b[i] && carry);
//    R[i] =  sum;
//  }
//  return R;
//};

//template< std::size_t N>
//constexpr auto multiply_bitset( const std::bitset<N> & a, const std::bitset<N> & b ) -> std::bitset<N> 
//{
//  //std::bitset<2*N> A(a.to_ulong()); //the result
//  //std::bitset<2*N> B(b.to_ulong()); //the result
//  std::bitset<N> R(0);  //the result
//  for(size_t i{0}; i<N; ++i) if(a[i]) R = sum_bitset(R, (b << i));
//  return R;
//};

template<typename D1,typename D2>
constexpr auto multiply(void)
{
  using base_type = typename D1::type;
  base_type n1 = D1::digit;
  base_type n2 = D2::digit;
};

template<typename D1, typename D2>
struct MultiplicationTable
{
  private:
  static const  std::size_t size = sizeof(typename D1::type)*8;
  static const  auto d1d2 = long_multiply(D1::digit, D2::digit);
  //multiplicate two digits. the result is double sized bitset
  //static constexpr auto mb  = []()
  //{
  //  const std::size_t size{sizeof(typename D1::type)*8};
  //  //constexpr std::bitset<2*size> b1(D1::digit);
  //  //constexpr std::bitset<2*size> b2(D2::digit);
  //  ////multiplicate two bitset
  //  //constexpr std::bitset<2*size> result = multiply_bitset<2*size>(b1,b2);
  //  //constexpr std::bitset<2*size> result;
  //  constexpr 
  //  return result;
  //}();
  ////the mask of lower "byte"
  //static const constexpr std::bitset<2*size> mask{std::bitset<2*size>(std::numeric_limits<typename D1::type>::max())};
  ////calculate lower byte
  //static const constexpr std::bitset<2*size> low{mb & mask};
  ////calculate  upper byte
  //static const constexpr std::bitset<2*size> up = (mb & (mask << size))>> size;
  ////find lower digit
  //static const typename  D1::type d1 = low.to_ulong();
  ////fined upper digit
  //static const typename  D1::type d2 = up.to_ulong();
  //public:
  ////now declare new time with the result of multiplication
  //using type = typename std::conditional< d2==0,
  //      NaturalNumber< Digit<d1, D1::base>>,
  //      NaturalNumber< Digit<d1, D1::base>, Digit<d2, D1::base> >
  //        >::type;
};

template<typename A1, typename B1, typename...Bs>
struct Multiply< NaturalNumber<A1>, NaturalNumber<B1,Bs...> >
{
  private:
    using R1 = typename MultiplicationTable<A1,B1>::type;
    using Zero  = NaturalNumber< Digit<0, A1::base> >;
  public:
    using type = typename Add< R1, typename Concatenate< Zero, typename Multiply< NaturalNumber<A1>, NaturalNumber<Bs...> >::type >::type >::type;
};

template<typename...Ds>
struct Multiply< NaturalNumber<Ds...>, NaturalNumber<> >
{
  using type = NaturalNumber<Ds...>;
};


template<typename NatNum, bool Positivity=true>
struct IntegerNumber
{
  static const bool positivity = Positivity;
  using number = NatNum;
};

template<long N, unsigned long Base=10>
struct MakeIntegerNumber
{
  using type = typename std::conditional< N >= 0,
        IntegerNumber< make_number_t<N,Base>>,
        IntegerNumber< make_number_t<-N,Base>, false>>::type;
};

template<long N, unsigned B=10>
using make_integer_t = typename MakeIntegerNumber<N,B>::type;

template<typename NA, typename NB, bool pA, bool pB>
struct Add<IntegerNumber<NA,pA>,IntegerNumber<NB,pB>>
{
  private:
    using A1 = typename FirstDigit<NA>::type;
    using NAt = typename TailNumber<NA>::type;
    using B1 = typename FirstDigit<NB>::type;
    using NBt = typename TailNumber<NB>::type;

    static const typename A1::type max_digit =  A1::base == 1 ? std::numeric_limits<typename A1::type>::max() : A1::max;
    static const typename A1::type base =  A1::base;

    using sumA1B1  =  NaturalNumber<typename std::conditional<
      base == 1, //full reprezentaion by long 
      Digit< A1::digit + B1::digit, 1>, 
      Digit< (A1::digit + B1::digit) % (A1::max+1), base> //num-base reperezentation
    >::type>;

    static const bool overflow =  max_digit - A1::digit < B1::digit;

  public:
    using type = typename std::conditional< overflow,
      typename Concatenate< sumA1B1, typename Add< NAt, typename Add< NaturalNumber<typename A1::unit>, NBt>::type >::type>::type,
      typename Concatenate< sumA1B1, typename Add< NAt, NBt>::type >::type
      >::type;
};





template<typename Mantissa, typename Power>
struct RealNumber
{
  using mantinssa = Mantissa;
  using power = Power;
};



template<long  Mantisa, long Power, long Base=10>
struct Float 
{
  static const long base = Base;
  static const long mantisa = Mantisa;
  static const long power = Power;
  constexpr inline operator double()
  {
    return Mantisa*pow( double(Base), double(Power) );
  }
};


template <typename F1, typename F2>
struct Add;

template <long M1, long P1, long M2, long P2, long B>
struct Add<Float<M1,P1,B>,Float<M2,P2,B>>
{
    static constexpr long Pmax = P2 > P1 ? P2 : P1;
    static constexpr long Pmin = P2 > P1 ? P1 : P2;
    static constexpr long Mmin = P2 > P1 ? M1 : M2;
    static constexpr long Mmax = P2 > P1 ? M2 : M1;
    static constexpr long M3 = Mmin + Mmax*pow(B,Pmax-Pmin);
    using type = Float< M3, Pmin, B>;
};

template <typename Head, typename Tail> 
struct List 
{
  using head = Head;
  using tail = Tail;
};

struct End 
{
  using head = End;
  using tail = End;
};

template <int N>
struct Range
{
  using type  = List<IntegerNumber<Digit<N>>, typename Range<N-1>::type >;
};
template <>
struct Range<0>
{
  using type = End;
};

//template <typename F>
//struct Action
//{
//  void operator()()
//  {
//  }
//};
//
//template <typename Range, typename Action>
//struct For
//{
//  using I  = typename Range::type::head;
//  using Inext  = typename Range::type::h;
//  static const Action A;
//  static const bool value  = A( FirstDigit<I>::digit ) && For<typename Range::type::tail, Action>::value;
//  using type = List< Action< FirstDigit<I>::digit 
//  void operator()()
//  {
//  }
//};


#endif

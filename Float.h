/*
 * =====================================================================================
 *
 *       Filename:  Float.h
 *
 *    Description:  
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

template< unsigned long d , unsigned long max_digit=0>
struct Digit
{ 
  using type  = unsigned long;
  static const type digit = d;
  static const type max = max_digit;
  using unit = Digit<1,max_digit>;
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


template <unsigned long d, unsigned long max_digit=9> 
struct MakeNumber
{
  using type = typename std::conditional< 
                      max_digit == 0, 
                      NaturalNumber< Digit<d,max_digit> >,
                      typename Concatenate < 
                                              NaturalNumber< Digit<d % (max_digit+1), max_digit> >, 
                                              typename MakeNumber< d/(max_digit+1), max_digit>::type
                                >::type
                     >::type ;
};

template <unsigned long max_digit> 
struct MakeNumber<0,max_digit>
{
  using type = NaturalNumber<>;
};

template <unsigned long d, unsigned long max_digit=9> 
using make_number = typename MakeNumber<d,max_digit>::type;

template<typename NA, typename NB>
struct Add
{
  private:
    using A1 = typename FirstDigit<NA>::type;
    using NAt = typename TailNumber<NA>::type;
    using B1 = typename FirstDigit<NB>::type;
    using NBt = typename TailNumber<NB>::type;
    static const typename A1::type max_digit =  A1::max == 0 ? std::numeric_limits<typename A1::type>::max() : A1::max;
    using sum  =  typename std::conditional<
      A1::max == 0,
      Digit< A1::digit + B1::digit, A1::max>,
      Digit< (A1::digit + B1::digit) % (A1::max+1), A1::max>
    >::type;
    static const bool overflow =  max_digit - A1::digit < B1::digit;
  public:
    using type = typename std::conditional<
      !overflow,
      typename Concatenate< NaturalNumber<sum>, typename Add< NAt, NBt>::type >::type,
      typename Concatenate< NaturalNumber<sum>, typename Add< NAt, typename Add< NaturalNumber<typename A1::unit>, NBt>::type >::type>::type
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

template<typename NatNum, bool Positivity=true>
struct IntegerNumber
{
  static const bool positivity = Positivity;
  using number = NatNum;
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


#endif

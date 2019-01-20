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
#include <vector>

#include <array>

#include "NaturalNumber.h"





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

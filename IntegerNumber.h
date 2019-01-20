/*
 * =====================================================================================
 *
 *       Filename:  IntegerNumber.h
 *
 *    Description:  Compile time integer number
 *
 *        Version:  1.0
 *        Created:  19.01.2019 22:18:50
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan B. Nikolaev (ekherit), I.B.Nikolaev@inp.nsk.su
 *   Organization:  Budker Insitute of Nuclear Physics
 *
 * =====================================================================================
 */
#ifndef IBN_INTEGER_NUBMER_H
#define IBN_INTEGER_NUBMER_H
#pragma once 
#include "NaturalNumber.h"

template<typename N, bool posit>
struct IntegerNumber
{
  using number = N;
  static const bool positivity = posit;
};

template<long N, DigitImplType base=10>
using make_integer_number_t = IntegerNumber <  make_natural_number_t< (N > 0 ? N : -N) , base> , N>=0 >;

template<typename N1, typename N2>
struct Add< IntegerNumber<N1,true>, IntegerNumber<N2,true> >
{
  using type =  IntegerNumber< add_t<N1,N2> , true>;
};

template<typename N1, typename N2>
struct Add< IntegerNumber<N1,false>, IntegerNumber<N2,false> >
{
  using type =  IntegerNumber< add_t<N1,N2> , false>;
};

template <typename N1, typename N2>
struct Max
{
  using type = typename std::conditional< is_less<N1,N2>, N2, N1>::type;
};

template<typename N1, typename N2> 
using max_t = typename Max<N1,N2>::type;

template<typename N1, typename N2> 
using min_t = typename std::conditional< is_less<N1,N2>, N1, N2>::type;

template<typename N1, typename N2, bool b1, bool b2>
struct Add< IntegerNumber<N1,b1>, IntegerNumber<N2,b2> >
{
  using Nmin = min_t<N1,N2>;
  using Nmax = max_t<N1,N2>;
  static constexpr bool cmp = is_less<N1,N2>;
  static constexpr bool pos =  (b1 && !b2 ) ?  !cmp : cmp;  
  using type = IntegerNumber< sub_t<Nmax,Nmin>, pos>;
};

template < typename N>
struct Negate;

template < typename N,  bool b>
struct Negate< IntegerNumber<N,b> >
{
  using type = IntegerNumber<N,!b>;
};

template<typename N1, typename N2, bool b1, bool b2>
struct Subtract< IntegerNumber<N1,b1>, IntegerNumber<N2,b2> >
{
  using type = typename Add< IntegerNumber<N1,b1>,  IntegerNumber<N2,!b2> >::type;
};

template<typename N1, typename N2, bool b1, bool b2>
struct Multiply< IntegerNumber<N1,b1>, IntegerNumber<N2,b2> >
{
  using type = IntegerNumber<   multiply_t<N1,N2>,!b1^b2 >;
};
#endif

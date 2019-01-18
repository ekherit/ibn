/*
 * =====================================================================================
 *
 *       Filename:  NaturalNumber.h
 *
 *    Description:  Compile time natural and integer numbers
 *    Natural numbers are: 0, 1,2, .... 
 *
 *        Version:  1.0
 *        Created:  18.01.2019 11:55:10
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan B. Nikolaev (ekherit), I.B.Nikolaev@inp.nsk.su
 *   Organization:  Budker Insitute of Nuclear Physics
 *
 * =====================================================================================
 */

#ifndef IBN_NATURAL_NUMBER_H
#define IBN_NATURAL_NUMBER_H
#pragma once

#include<type_traits>
#include<limits>

//interface
using DigitImplType = unsigned long; 

template< DigitImplType d, DigitImplType num_base=1>
struct Digit;

//natural number is represented by positional numeric system by number of digit
template <typename ... Ds>
struct NaturalNumber;

template <DigitImplType d, DigitImplType base=10> 
struct MakeNaturalNumber;

template <DigitImplType d, DigitImplType base=10> 
using make_natural_number_t = typename MakeNaturalNumber<d,base>::type;

template<typename NA, typename NB>
struct Add;

template<typename NA, typename NB>
struct Equal;

template<typename NA, typename NB>
struct Less;

template <typename A, typename B> 
using add_t = typename Add<A,B>::type;

template <typename A, typename B> 
using is_less = typename Less<A,B>::value;

template <typename A, typename B> 
using is_equal = typename Equal<A,B>::value;



template< DigitImplType d, DigitImplType num_base>
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


template <DigitImplType d, DigitImplType base> 
struct MakeNaturalNumber
{
  using type = typename std::conditional< 
    base==1, 
    NaturalNumber< Digit<d,base> >,
    typename Concatenate < 
      NaturalNumber< Digit<d % base, base> >, 
    typename std::conditional< 
      (d/base) ==0,
    NaturalNumber<>,
    typename MakeNaturalNumber<d/base, base>::type
      >::type
      >::type
      >::type ;
};

template <DigitImplType base> 
struct MakeNaturalNumber<0,base>
{
  using type = NaturalNumber<Digit<0,base>>;
};


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

#endif

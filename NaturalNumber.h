/*
 * =====================================================================================
 *
 *       Filename:  NaturalNumber.h
 *
 *    Description:  Compile time natural numbers with zero 
 *    Natural numbers are: 1,2, .... 
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
#include<cstdint>


//natural number is represented by positional numeric system by number of digit
template <typename ... Ds>
struct NaturalNumber;

using DigitImplType = uint64_t; 
  static constexpr DigitImplType MAX_BASE = (0x1L<<32);

template< DigitImplType d, DigitImplType num_base=MAX_BASE>
struct Digit;

template <unsigned long d, DigitImplType base=MAX_BASE> 
struct MakeNaturalNumber;


template<typename NA, typename NB>
struct Equal;

template<typename NA, typename NB>
struct Less;

template<typename NA, typename NB>
struct Add;

template<typename NA, typename NB>
struct Multiply;

template<typename NA, typename NB>
struct Subtract;

template<typename N, DigitImplType newBase>
struct ToBase;

/* #####   Some helper functions ################################################ */

template <uint64_t d, DigitImplType base=10> 
using make_natural_number_t = typename MakeNaturalNumber<d,base>::type;

template <typename A, typename B> 
using add_t = typename Add<A,B>::type;

template <typename A, typename B> 
using multiply_t = typename Multiply<A,B>::type;

template <typename A, typename B> 
using sub_t = typename Subtract<A,B>::type;

template <typename A, typename B> 
constexpr bool is_less = Less<A,B>::value;

template <typename A, typename B> 
constexpr bool is_equal = Equal<A,B>::value;


template<typename N, DigitImplType B>
using to_base_t  = typename ToBase<N,B>::type;



/* #####   IMPLEMENTATIONS  ############################################# */

template< DigitImplType d, DigitImplType num_base>
struct Digit
{ 
  using type = decltype(d);
  static const type digit = d;
  static const type max = num_base-1;
  static const type base = num_base;
  using unit = Digit<1,num_base>;
  using zero = Digit<0,num_base>;
  static constexpr type max_base = (0x1L<<32);
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

template <typename N>
struct LastDigit;

template<typename D1>
struct LastDigit< NaturalNumber<D1> >
{
  using type = D1;
};

template<typename D1, typename...Ds>
struct LastDigit< NaturalNumber<D1,Ds...> >
{
  using type = typename LastDigit<NaturalNumber<Ds...>>::type;
};

template<typename N>
struct HeadNumber;

template<typename D1>
struct HeadNumber< NaturalNumber<D1> >
{
  using type = NaturalNumber<>;
};



template <typename N1, typename N2, typename...Ns>
struct Concatenate;

template<typename N1, typename N2>
using conc_t = typename Concatenate<N1,N2>::type;

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

template<typename D1, typename...Ds>
struct HeadNumber< NaturalNumber<D1, Ds...> >
{
  using type = conc_t< NaturalNumber<D1>, typename HeadNumber< NaturalNumber<Ds...> >::type>;
};

template <DigitImplType d, DigitImplType base> 
struct MakeNaturalNumber
{
  static_assert( base <= MAX_BASE, "base must be less or equal MAX_BASE=2^32");
  using type = typename Concatenate < 
      NaturalNumber< Digit<d % base, base> >, 
       typename std::conditional< (d/base) == 0,
       NaturalNumber<>,
      typename MakeNaturalNumber<d/base, base>::type
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
    using A1  = typename FirstDigit<NA>::type;
    using NAt = typename TailNumber<NA>::type;
    using B1  = typename FirstDigit<NB>::type;
    using NBt = typename TailNumber<NB>::type;

    static_assert( A1::base == B1::base, "Digit's base mismatch");
    static const typename A1::type base =  A1::base;
    static const typename A1::type max_digit =  A1::max;  

    static const uint64_t sum  = A1::digit + B1::digit;
    static const uint64_t d1 =  sum % base;
    static const uint64_t d2 =  sum / base;
    using D1 = NaturalNumber< Digit<d1,base> >;
    using D2 = NaturalNumber< Digit<d2,base> >;
    using sumA1B1 = make_natural_number_t<A1::digit + B1::digit, A1::base>;
    using sumTail = add_t<NAt,NBt>;

  public:
    using type = typename std::conditional< d2==0,
          conc_t< D1, sumTail>,
          conc_t< D1, add_t<D2, sumTail>> 
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

/* ################## Multiplication IMPLEMENTATION ################################# */

template<typename A>
struct Multiply< NaturalNumber<A>, NaturalNumber<> > 
{
  using type = NaturalNumber<>;
};

template<typename A>
struct Multiply< NaturalNumber<>, NaturalNumber<A> > 
{
  using type = NaturalNumber<>;
};

template<typename A, typename NB>
struct Multiply< NaturalNumber<A>, NB> 
{
  private:
    using B = typename FirstDigit<NB>::type;
    using NBs = typename TailNumber<NB>::type;
    using digit_type = typename A::type;
    static_assert( A::base == B::base);
    static const uint64_t base = A::base;
    static const uint64_t da = A::digit; 
    static const uint64_t db = B::digit;
    static const uint64_t ab = da*db;
    using N1 = make_natural_number_t<ab,A::base>;
  public:
    using type = add_t < N1, conc_t< NaturalNumber<typename A::zero>,  typename Multiply<NaturalNumber<A>, NBs >::type >>;
};

template<typename A, typename NB, typename...As>
struct Multiply< NaturalNumber<A,As...>, NB> 
{
  using type = add_t < typename Multiply< NaturalNumber<A>, NB>::type,  
                       typename Concatenate< NaturalNumber<typename A::zero>, typename Multiply< NaturalNumber<As...>, NB>::type >::type >;
};
/* ##################### SUBTRACT IMPLEMENTATION ################################################################################# */



template<typename N>
struct Norm //remove zero from higher positions
{
  using D = typename LastDigit<N>::type;
  using H =  typename HeadNumber<N>::type;
  using type = typename std::conditional< D::digit == 0,   typename Norm<H>::type, N >::type;
};

template<typename D>
struct Norm< NaturalNumber<D> >
{
  using type = NaturalNumber<D>;
};

template<typename NA>
struct Subtract<NA,NaturalNumber<>>
{
  using type = NA;
};

template<typename A, typename B>
struct Subtract<NaturalNumber<A>,NaturalNumber<B>>
{
  //static_assert( B::digit <= A::digit,  "Subtrahend NB must less or equal then minuend" );
  using type = make_natural_number_t<A::digit-B::digit, A::base>;
};

template<typename NA, typename NB>
struct Subtract
{
  private:
    static_assert( is_less<NB,NA> || is_equal<NB,NA>,  "Subtrahend NB must less or equal then minuend" );
    using A1 = typename FirstDigit<NA>::type;
    using B1 = typename FirstDigit<NB>::type;
    using NAs = typename TailNumber<NA>::type;
    using NBs = typename TailNumber<NB>::type;
    //using A2 =  typename FirstDigit<NAs>::type;
    using Nsub1 = NaturalNumber<Digit<A1::digit-B1::digit, A1::base>>;
    using Nsub2 = NaturalNumber<Digit<A1::digit + A1::base-B1::digit, A1::base>>;
    using One = NaturalNumber<typename B1::unit>;
  public:
    using type = typename std::conditional<
                     is_equal<NB,NA>,
                     make_natural_number_t<0,A1::base>,
                     typename Norm<
                         typename std::conditional
                           < A1::digit >= B1::digit,
                             conc_t< Nsub1, sub_t<NAs,NBs> >,
                             conc_t< Nsub2, sub_t
                                            < 
                                               sub_t < NAs, One >, 
                                               NBs 
                                            >
                                   >
                           >::type
                     >::type
                 >::type;
};


template<typename A1,typename A2, typename B1>
struct Subtract< NaturalNumber<A1,A2>, NaturalNumber<B1> >
{
  private:
    static_assert( A2::base+ A1::digit >= B1::digit );
  public:
    using type = make_natural_number_t< A2::base*A2::digit - B1::digit, A2::base >;
};

/* ################ TOBASE IMPLEMENTATION ############################################ */

template<typename D1, DigitImplType newBase>
struct ToBase<NaturalNumber<D1>, newBase>
{
  using type = make_natural_number_t<D1::digit, newBase>;
};


template<typename N, DigitImplType newBase>
struct ToBase
{
  using D1 = typename FirstDigit<N>::type;
  using Ns = typename TailNumber<N>::type;
  using N1=typename ToBase<NaturalNumber<D1>, newBase>::type;
  using OldBase = make_natural_number_t<D1::base,newBase>;
  using type = add_t<N1, 
        multiply_t<OldBase, 
        typename ToBase< Ns, newBase>::type>>;
};

#endif


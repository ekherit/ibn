/*
 * =====================================================================================
 *
 *       Filename:  dimension.h
 *
 *    Description:  Dimension realization
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
#ifndef IBN_DIMENSION_H
#define IBN_DIMENSION_H
#pragma once

#include <iostream>
#include <type_traits>
#include <typeinfo>
#include <ratio>

//base dimention with power
//Id is the BaseDimension Id
//R is ratio number from std::ratio
template<int Id, typename R = std::ratio<1,1> >
struct BaseDimension 
{
  static const int  id = Id;
  using ratio = R;
};

//complex dimension is a series of base dimensions
template<typename ...B>
struct Dimension {};


template< typename B1, typename B2>
struct Compare {};

template < int Id1, typename R1, int Id2, typename R2>
struct Compare < BaseDimension<Id1,R1>, BaseDimension<Id2,R2> >
{
  private:
    using zero = std::ratio<0,1>;
    using sum = typename std::ratio_add<R1,R2>::type;
    static const bool sum_is_zero = std::ratio_equal<sum,zero>::value;
  public:
    static const int value = 
      (Id1 == Id2) ? ( sum_is_zero ?  2 : 0 )
      : (   (Id1<Id2) ? -1 : +1) ;
};

//Multiplication of the base dimensions
//
//T = 0   if B1 ~ B2
//T = 2   if B1 ~ B2 и ratio(B1+B2) == 0
//T = -1, if B1 < B2
//T = +1, if B1 > B2
template<int T, typename B1, typename B2>
struct MultiplyBaseImpl;

//B1 = B2
template < int Id1, typename R1, typename R2>
struct MultiplyBaseImpl<0,  BaseDimension<Id1, R1 >, BaseDimension<Id1,R2> >
{
  private:
    using sum = typename std::ratio_add<R1,R2>::type;
  public:
    using type = BaseDimension<Id1, sum>;
};

template < int Id1, typename R1, typename R2>
struct MultiplyBaseImpl<2,  BaseDimension<Id1, R1 >, BaseDimension<Id1,R2> >
{
  private:
    using sum = typename std::ratio_add<R1,R2>::type;
  public:
    using type = Dimension<BaseDimension<Id1, sum>>;
};

//B1 less then B2
template < int Id1, typename R1, int Id2, typename R2>
struct MultiplyBaseImpl<-1,  BaseDimension<Id1,R1>, BaseDimension<Id2,R2> >
{
  private:
    using B1 = BaseDimension<Id1,R1>;
    using B2 = BaseDimension<Id2,R2>;
  public:
    using type = Dimension<B2,B1>;
};

//B2 less B1
template < int Id1, typename R1, int Id2, typename R2>
struct MultiplyBaseImpl<+1,  BaseDimension<Id1,R1>, BaseDimension<Id2,R2> >
{
  private:
    using B1 = BaseDimension<Id1,R1>;
    using B2 = BaseDimension<Id2,R2>;
  public:
    using type = Dimension<B1,B2>;
};

template<typename B1, typename B2>
struct MultiplyBase
{ 
  static const int cmp = Compare<B1,B2>::value;
  public:
  using type = typename MultiplyBaseImpl<cmp, B1,B2>::type;
};




//multiplication of the complex dimensions
template<typename D1, typename D2>
struct Multiply{};

template<int cmp, typename D1, typename D2> struct MultiplyImpl{};

//multiplication in the midle of the recursion, 
//B1=B2  turn of the recursion, but dimension remain not equal zero
template< typename B1, typename B2,  typename ... Bases>
struct MultiplyImpl<0,  Dimension<B1>, Dimension<B2,  Bases...> >
{
  private:
    using B1B2  = typename MultiplyBaseImpl<0, B1,B2>::type;
  public:
    using type = Dimension<B1B2, Bases...>;
};

//multiplication in the midle of the recursion, 
//B1=B2  turn of the recursion, but dimension disappear
template< typename B1, typename B2,  typename ... Bases>
struct MultiplyImpl<2,  Dimension<B1>, Dimension<B2,  Bases...> >
{
  using type = Dimension<Bases...>;
};

///template< typename B1, typename B2>
///struct MultiplyImpl<2,  B1, Dimension<B2> >
///{
///    using type = Dimension<>;
///};

//B2<B1 turn of the recursion (concatenation)
template< typename B1, typename B2, typename ... Bases>
struct MultiplyImpl<+1,  Dimension<B1>, Dimension<B2, Bases...> >
{
  public:
    using type = Dimension<B1,B2, Bases...>;
};

//B1<B2  make recursion deeper
template< typename B1, typename B2, typename ... Bases>
struct MultiplyImpl<-1,  Dimension<B1>, Dimension<B2, Bases...> >
{
  private:
    using D = typename Multiply<Dimension<B1>, Dimension<Bases...> >::type;
  public:
    using type = typename Multiply<B2, D>::type;
};


//multiplication on dimensionless
template< typename B1>
struct Multiply<Dimension<B1>, Dimension<> >
{
  using type = Dimension<B1>;
};


/*  
//start recursion of the multiplication
//let swap base dimentions even they are the same
//template < int Id1, int N1, int Id2, int N2, int Id3, int N3, typename ... Bases>
//template < typename B1, typename B2, typename ... Bases>
template < int Id1, typename R1, typename B2, typename ... Bases>
struct Multiply< Dimension<BaseDimension<Id1,R1>>, Dimension<B2,Bases...> >
{
  using B1 = BaseDimension<Id1,R1>;
  private:
  static const int cmp = Compare<B1,B2>::value;
  public:
  using type = typename MultiplyImpl<cmp, B1, Dimension<B2, Bases...>>::type;
};
*/


//start recursion of the multiplication
//let swap base dimentions even they are the same
//template < int Id1, int N1, int Id2, int N2, int Id3, int N3, typename ... Bases>
//template < typename B1, typename B2, typename ... Bases>
template < typename B1, typename B2, typename ... Bases>
struct Multiply< Dimension<B1>, Dimension<B2,Bases...> >
{
  private:
  static const int cmp = Compare<B1,B2>::value;
  public:
  using type = typename MultiplyImpl<cmp, B1, Dimension<B2, Bases...>>::type;
};



//right multiplication in term off left multiplication
template < int Id1, typename R1, typename ... Bases>
struct Multiply<Dimension<Bases...>,  BaseDimension<Id1,R1>>
{
  private:
    using B1 = BaseDimension<Id1,R1>;
  public:
    using type = typename Multiply<B1, Dimension<Bases...> >::type;
};

//multiplication of the two complex dimensions
template < typename B1, typename... Bases1, typename ... Bases2>
struct Multiply< Dimension<B1, Bases1...>, Dimension<Bases2...> >
{
  private:
    using RU = Dimension<Bases1...>;
    using U = typename Multiply<B1, Dimension<Bases2...> >::type;
  public:
    using type = typename Multiply<RU, U>::type;
};

//end of the recursion of multilication of two complex dimensions
template < typename ... Bases2>
struct Multiply< Dimension<>, Dimension<Bases2...> >
{
  using type = Dimension<Bases2...>;
};

//multiplication of two dimensionless values
template<>
struct Multiply< Dimension<>, Dimension<> >
{
  using type = Dimension<>;
};


//template<typename B, typename D> using Multiply  = typename Multiply< Dimension<B>, D>::type;

//inverse element
template<typename B>
struct Inverse { };

//inverse element of base dimension
template <int Id, typename R>
struct Inverse<BaseDimension<Id,R>>
{
  private:
    using sub = typename std::ratio_subtract< std::ratio<0ll,1ll>, R >::type; 
  public:
    using type = BaseDimension<Id,sub>;
};

//inverse element of complex dimension
template <typename B1, typename ... Bs>
struct Inverse< Dimension<B1,Bs...> >
{
  private:
    using invB1 = typename Inverse<B1>::type;
    using invU  = typename Inverse<Dimension<Bs...>>::type;
  public:
    using type = typename Multiply<invB1, invU>::type;
};

//inverse element of dimensionless type
template<>
struct Inverse< Dimension<> >
{
  using type = Dimension<>;
};

//devision 
template<typename  T1, typename T2>
struct Divide
{
  private:
    using invT2 = typename Inverse<T2>::type;
  public:
    using type = typename Multiply< T1, invT2>::type;
};


//power operator
template<typename B, typename P>
struct Power
{
};

template<int Id, typename R, typename P>
struct Power< BaseDimension<Id,R>, P>
{
  using type = BaseDimension<Id, std::ratio_multiply<R,P>>;
};


template<typename B1, typename ...Bs, typename P>
struct Power< Dimension<B1, Bs...>, P> 
{
  private:
    using Bp = typename Power<B1,P>::type;
    using Up = typename Power<Dimension<Bs...>,P>::type;
  public:
    using type = typename Multiply<Bp, Up>::type;
};

template<typename  P> 
struct Power< Dimension<>, P> 
{
  using type = Dimension<>;
};

  template <int Id1, typename R1> 
std::ostream & operator<<(std::ostream & os, BaseDimension<Id1, R1> )
{
  os << Id1 <<"^(" << R1::num;
  if(R1::den != 1 ) os << "/"<<R1::den;
  os << ")";
  return os;
}

  template <typename B1> 
std::ostream & operator<<(std::ostream & os,  Dimension<B1>)
{
  return os << B1();
}

  template <typename B1, typename ...Bs> 
std::ostream & operator<<(std::ostream & os,  Dimension<B1,Bs...>)
{
  return os << B1() << "*" << Dimension<Bs...>();
}


//template <> 
//struct is_dimensionless
//{
//  constexpr int value = true;
//};

template <typename U> 
struct is_dimensionless;
//{
//};

template <> 
struct is_dimensionless<Dimension<>>
{
  static const bool value = true;
};

//template < typename B> 
//struct is_dimensionless< Dimension<B> >
//{
//  static const bool value = (B::ratio::num == 0);
//};

template < typename B, typename ...Bs > 
struct is_dimensionless< Dimension<B, Bs...> >
{
  static const bool value = is_dimensionless<Dimension<Bs...> >::value && (B::ratio::num == 0);
};



template < typename U >
struct TotalDimension
{
};


template < typename B, typename ... Bs>
struct TotalDimension< Dimension<B, Bs...> >
{
  static const int value = TotalDimension<Bs ...>::value + fabs(B::ratio::num);
};

template<int Id, int Denom>
struct Dimension< BaseDimension<Id, std::ratio<0,Denom> > >
{ 
  operator  Dimension<>(){return Dimension<>();}
};

//template< typename B>
//struct TotalDimension< BaseDimension<id, std::ratio

//template<typename B, typename ... Bs>
//struct total_dimension
//{
//  static constexpr int value = total_dimesion<Bs ...>::value + fabs( B::ratio::num);
//};

//template<>
//struct total_dimension
//{
//  static constexpr int  value = 0;
//}

template<int Id>  using new_dimension = Dimension< BaseDimension<Id> >;

#endif

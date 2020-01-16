/*
 * =====================================================================================
 *
 *       Filename:  unit.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05.01.2019 19:22:20
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan B. Nikolaev (ekherit), I.B.Nikolaev@inp.nsk.su
 *   Organization:  Budker Insitute of Nuclear Physics
 *
 * =====================================================================================
 */

#ifndef IBN_UNIT_H
#define IBN_UNIT_H
#pragma once

#include<ratio>
#include <cmath>
//#include<type_traits>

namespace ibn {
//base dimention with power
//Id is the BaseDimension Id
//R is ratio number from std::ratio

template<int Id, typename Power = std::ratio<1,1> >
struct BaseDimension : public Power
{
  static const int  id = Id;
  //using power = Power;
};

//complex dimension is a series of base dimensions
template<typename ...Bs>
struct Dimension
{
  static const int size = sizeof...(Bs); //number of base dimensions in complex dimension
};

// ************* OPERATION WITH DIMENSIONS *************************************** 

//concatenation dimensions without manipulaing with them
template<typename D1, typename D2>
struct Concatenate;

template<typename B, typename... Bs>
struct Concatenate< Dimension<B>, Dimension<Bs...> >
{
  using type = Dimension<B,Bs...>;
};


//extracting base dimensions from complex dimension
//First base dimension in dimension list
template<typename D>
struct First;

template <typename B>
struct First<Dimension<B>> { using type = B; };

template <typename B, typename... Bs>
struct First<Dimension<B,Bs...>> { using type = B; };


//Remain dimension list ( first base dimension is removed)
template <typename D>
struct Tail;

template <typename B>
struct Tail<Dimension<B>> { using type = Dimension<>; };

template <typename B, typename... Bs>
struct Tail<Dimension<B,Bs...>> { using type = Dimension<Bs...>; };


//multiplication of the complex dimensions.
//Implementaion detail
template<typename D1, typename D2>
struct MultiplyLeft;

template<typename B> 
struct MultiplyLeft<Dimension<B>, Dimension<>> { using type = Dimension<B>; };


//Multiplication on dimensionless on left
template<typename... Bs>
struct MultiplyLeft<Dimension<Bs...>, Dimension<> > { using type = Dimension<Bs...>; };

//Implementation of multiplication of complex dimension
template<typename B1, typename B2, typename ...Bs2>
struct MultiplyLeft< Dimension<B1>, Dimension<B2,Bs2...> >
{ 
  private: 
    //just define some usefull aliases
    template<typename ... Bs> using D = Dimension<Bs...>;
    using power_sum = typename std::ratio_add<B1,B2>::type;
    static const bool is_power_sum_zero = std::ratio_equal<power_sum,std::ratio<0,1>>::value;
    template<typename D1, typename D2>  using Mult =  typename MultiplyLeft<D1,D2>::type;
    template<typename D1, typename D2>  using  Con =  typename  Concatenate<D1,D2>::type;
    template< bool C, typename A, typename B> using if_then_else  = typename std::conditional<C,A,B>::type;
    using Bsum = BaseDimension<B1::id, power_sum>;
    using DR = Dimension<Bs2...>;
  public:
    using type =  
      if_then_else <  (B1::id  < B2::id),  D<B1,B2,Bs2...>,     //the dimension id is ordered in a right way this is the end of the recursion
      if_then_else <  (B2::id  < B1::id),  Con<D<B2>, Mult<D<B1>, DR>>, //wrong id order swap dimensions and go deeper into recursion
      if_then_else <   is_power_sum_zero,  D<Bs2...>,            //same dimensions and sum of the powers is zero remove this base dimension from result Dimension list (end of the recursion)
                                           D<Bsum,Bs2...> //multiply same dimensions (end of the recursion)
        >>>;
};

template<typename D1, typename D2>
struct Multiply;

//Multiplication on dimensionless on right
template<typename B, typename...Bs> 
struct Multiply< Dimension<>, Dimension<B,Bs...> > { using type = Dimension<B,Bs...>; };

//Multiplication on dimensionless on right
template<typename...Bs> 
struct Multiply< Dimension<Bs...>, Dimension<> > { using type = Dimension<Bs...>; };

template <typename D1, typename D2>
struct Multiply
{
  private:
    using DL = typename Tail<D1>::type;
    using B1 = typename First<D1>::type;
  public:
    using type = typename Multiply< DL, typename MultiplyLeft< Dimension<B1>, D2>::type >::type;
};

//inverse element
template<typename B>
struct Inverse;

template<>
struct Inverse<Dimension<>> 
{
  using type = Dimension<>;
};

//inverse element of base dimension
template <int Id, typename R>
struct Inverse<Dimension<BaseDimension<Id,R>>>
{
  private:
    using sub = typename std::ratio_subtract< std::ratio<0,1>, R >::type; 
  public:
    using type = Dimension<BaseDimension<Id,sub>>;
};

//inverse element of complex dimension
template <typename B1, typename ... Bs>
struct Inverse< Dimension<B1,Bs...> >
{
  private:
    using DinvB1 = typename Inverse<Dimension<B1>>::type;
    using DinvBs  = typename Inverse<Dimension<Bs...>>::type;
  public:
    using type = typename Concatenate<DinvB1, DinvBs>::type;
};

//power operator
template<typename B, int n, int m=1>
struct Power;

template<int n, int m> 
struct Power< Dimension<>, n, m> 
{
  using type = Dimension<>;
};

template<typename B1, int n, int m, typename...Bs>
struct Power< Dimension<B1, Bs...>, n, m> 
{
  private:
    using B1n = Dimension< BaseDimension<B1::id, std::ratio_multiply<B1,std::ratio<n,m>>> >;
    using Bsn = typename Power<Dimension<Bs...>,n, m>::type;
  public:
    using type = typename Multiply<B1n, Bsn>::type;
};


//some usefull functions
template <typename U> 
struct is_dimensionless;

template <> 
struct is_dimensionless<Dimension<>>
{
  static const bool value = true;
};

template < typename B, typename ...Bs > 
struct is_dimensionless< Dimension<B, Bs...> >
{
  static const bool value = is_dimensionless<Dimension<Bs...> >::value && (B::num == 0);
};

template<int Id>  using make_dimension = Dimension< BaseDimension<Id> >;


template< typename D, typename Field = double >
struct unit
{
  private:
  public:
  using dimension = D;
  using field=Field;
  using type = unit<dimension, Field>;
  unit(Field f) noexcept: data(f){} 
  //unit(unit  & t)       noexcept : data(t.data) {}
  //unit(unit  && t)      noexcept : data(t.data) {}
  unit(const unit  & t) = default;
  unit(void) : data(1.0){};

  //type & operator=(unit && t) = default;
  //type & operator=(unit & t) = default;
  //type & operator=(const unit && t) = default;
  type & operator=(const unit & t) = default;

  constexpr friend type operator*(const field & f, const type  & t) noexcept { return {f*t.data}; }
  constexpr friend type operator*(const  type & t, const field & f) noexcept { return {t.data*f}; }

  constexpr friend type operator+(const type & t1, const type & t2) noexcept { return {t1.data + t2.data}; }
  constexpr friend type operator-(const type & t1, const type & t2) noexcept { return {t1.data - t2.data}; }

  constexpr friend type operator/(const type & t, const field & f) { return {t.data/f}; }
  constexpr friend auto operator/(const field & f, const type & t) -> typename Inverse<dimension>::type { return {f/t.data}; }

  constexpr type & operator+=(const type & t) { data+=t.data; return *this; }


  constexpr friend bool operator<(const type & t1, const type &t2) {
    return t1.data < t2.data;
  }
  constexpr friend bool operator>(const type & t1, const type &t2) {
    return t1.data > t2.data;
  }

  constexpr friend type operator-(const type & t1) { return {-t1.data}; }


  

  constexpr operator typename std::conditional< is_dimensionless<dimension>::value, field, type>::type () { return data; };

  template<int Num,int Den=1>
  constexpr auto pow(void) -> unit<typename Power<dimension, Num, Den>::type,field>
  { 
    return static_cast<field>(::pow(static_cast<double>(data),static_cast<double>(Num)/Den));
  };

  private:


  public:
  field data;
};


template < typename T1, typename T2>
inline auto operator*(const T1 & t1 , const T2 & t2 ) -> unit<typename Multiply<typename T1::dimension, typename T2::dimension>::type, typename T1::field>
{
  return  {t1.data*t2.data};
}

template < typename D1, typename D2, typename Field>
inline auto operator/(const unit<D1, Field> & t1 , const unit<D2, Field> & t2 ) -> unit<typename Multiply<D1, typename Inverse<D2>::type >::type, Field>
{
  return  {t1.data/t2.data};
}


//struct force_constexpr
//{
//  int n;
//  constexpr force_constexpr(const int m): n(m){};
//};

//static constexpr int make_const(const int i){ return i; }

//template < typename D1, typename Field>
//constexpr auto operator^(unit<D1, Field> t1, const int n)//  -> decltype( unit< typename Power<D1,n>::type, Field>)
//{
//  //return  unit<typename Power<D1,make_const(n)>::type, Field>(pow(t1.data,n));
//  constexpr auto f = force_constexpr(n);
//  constexpr int N = f.n;
//  return  unit<typename Power<D1,N>::type, Field>();
//};


template<int Id, typename Field=double>
using make_unit = unit< Dimension<BaseDimension<Id> >, Field>; 
}



#endif

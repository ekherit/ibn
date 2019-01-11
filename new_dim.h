/*
 * =====================================================================================
 *
 *       Filename:  new_dim.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07.01.2019 23:32:50
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

#include<ratio>
//#include<type_traits>

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
template<typename B, int n>
struct Power;

template<typename B, int n>
struct Power< Dimension<B>, n>
{
  using type = Dimension< BaseDimension<B::id, std::ratio_multiply<B,std::ratio<n,1>>> >;
};


template<typename B1, int n, typename...Bs>
struct Power< Dimension<B1, Bs...>, n> 
{
  private:
    using B1n = typename Power<B1,n>::type;
    using Bsn = typename Power<Dimension<Bs...>,n>::type;
  public:
    using type = typename Multiply<B1n, Bsn>::type;
};

template<int n> 
struct Power< Dimension<>, n> 
{
  using type = Dimension<>;
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
#endif

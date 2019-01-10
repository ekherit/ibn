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

#include<ratio>
#include<type_traits>

//base dimention with power
//Id is the BaseDimension Id
//R is ratio number from std::ratio
template<int Id, typename Power = std::ratio<1,1> >
struct BaseDimension 
{
  static const int  id = Id;
  using power = Power;
};

//complex dimension is a series of base dimensions
template<typename ...B>
struct Dimension{};

//multiplication of the complex dimensions
template<typename D1, typename D2>
struct Multiply;


//template<>
//struct Multiply<Dimension<>, Dimension<> > { using type = Dimension<>; };

//define the end of the recursion (Multiplication on dimensionless on left)
template<typename... Bs>
struct Multiply<Dimension<Bs...>, Dimension<> > { using type = Dimension<Bs...>; };

//Multiplication on dimensionless on right
template<typename B, typename...Bs> 
struct Multiply< Dimension<>, Dimension<B,Bs...> > { using type = Dimension<B,Bs...>; };

template<typename D1, typename D2>
struct Concatenate;

template<typename B, typename... Bs>
struct Concatenate< Dimension<B>, Dimension<Bs...> >
{
  using type = Dimension<B,Bs...>;
};

template<typename B1, typename D2, typename... Bs>
struct Concatenate< Dimension<Bs...,B1>, D2>
{
  using type = typename Concatenate< Dimension<Bs...>,  typename Concatenate<Dimension<B1>, D2>::type>::type;
};

template<typename B1, typename B2, typename ...Bs2>
struct Multiply< Dimension<B1>, Dimension<B2,Bs2...> >
{ 
  private: 
    //just define some usefull aliases
    template<typename ... Bs> using D = Dimension<Bs...>;
    using power_sum = typename std::ratio_add<typename B1::power,typename B2::power >::type;
    static const bool is_power_sum_zero = std::ratio_equal<power_sum,std::ratio<0,1>>::value;
    template<typename A, typename B>  using Mult =  typename Multiply<A,B>::type;
    template<typename A, typename B>  using Con =  typename Concatenate<A,B>::type;
    template< bool C, typename A, typename B> using IF  = typename std::conditional<C,A,B>::type;
    using DR = Dimension<Bs2...>;
  public:
    using type =  
      IF<  (B1::id  < B2::id),  Dimension<B1, B2,Bs2...>,     //the dimension id is ordered in a right way this is the end of the recursion
      IF<  (B2::id  < B1::id),  Con< D<B2>, Mult<D<B1>, DR>>, //wrong id order swap dimensions and go deeper into recursion
      IF<   is_power_sum_zero,  Dimension<Bs2...>,            //same dimensions and sum of the powers is zero remove this base dimension from result Dimension list (end of the recursion)
                                D<BaseDimension<B1::id, power_sum>, Bs2...> //multiply same dimensions (end of the recursion)
        >>>;
};

//multiplication of the two complex dimensions
template <typename B1, typename D2,typename ... Bs1 >
struct Multiply< Dimension<Bs1...,B1>, D2>
{
    using type = typename Multiply< Dimension<Bs1...> , typename Multiply<Dimension<B1>, D2>::type >::type;
};

template<int Id>  using new_dimension = Dimension< BaseDimension<Id> >;

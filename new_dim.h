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
template<typename B, typename...Bs> struct Multiply< Dimension<>, Dimension<B,Bs...> > { using type = Dimension<B,Bs...>; };

template< typename A, typename B>
struct Test;

template<typename B1, typename B2, typename... Bs2>
struct Test< Dimension<B1>, Dimension<B2,Bs2...> >
{
  using type = Dimension<B1,B2>;
  using multiple = Dimension<Bs2...>;
};

//Multiplication implementaiont of the base dimensions
//
//cmp  is used for comparizon of base dimensin id's and ratio
//template<int CMP, typename D1, typename D2>
//struct MultiplyImpl{};
//
////same dimension and zero power sum -> collapsing dimension
//template<typename B1, typename B2>
//struct MultiplyImpl<0, Dimension<B1>, Dimension<B2>>
//{
//  using type = Dimension<>;
//};
//
////same dimension and nonzero sum -> make dimension with sum of powers
//template<typename B1, typename B2>
//struct MultiplyImpl<1, Dimension<B1>, Dimension<B2>>
//{
//  private:
//    using power_sum = typename std::ratio_add<typename B1::power,typename B2::power >::type;
//  public:
//  using type = Dimension< BaseDimension < B1::id, power_sum > >;
//};
//
////B1::id < B2::id
//template<typename B1, typename B2>
//struct MultiplyImpl<2, Dimension<B1>, Dimension<B2>>
//{
//  using type = Dimension<B1,B2>;
//};
//
//template<typename B1, typename B2>
//struct MultiplyImpl<3, Dimension<B1>, Dimension<B2>>
//{
//  using type = Dimension<B2,B1>;
//};

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

template<typename D1, typename D2>
struct MultiplyImpl;
template<typename... Bs>
struct MultiplyImpl<Dimension<Bs...>, Dimension<> > { using type = Dimension<Bs...>; };

//Multiplication on dimensionless on right
template<typename B, typename...Bs> 
struct MultiplyImpl< Dimension<>, Dimension<B,Bs...> > { using type = Dimension<B,Bs...>; };


template<typename B1,typename B2>
struct MultiplyBase;

//Multiplication of same Base dimensions with different power
template<int Id, typename P1, typename P2>
struct MultiplyBase< BaseDimension<Id,P1>, BaseDimension<Id,P2> >
{
  private:
    using B1 = BaseDimension<Id,P1>;
    using B2 = BaseDimension<Id,P2>;
    using power_sum = typename std::ratio_add<typename B1::power,typename B2::power >::type;
public:
    using type = typename std::conditional<  std::ratio_equal<power_sum,std::ratio<0,1>>::value,
          Dimension<>,
          Dimension<BaseDimension<Id, power_sum>> 
            >::type;
};

template<int Id1, int Id2, typename P1, typename P2>
struct MultiplyBase< BaseDimension<Id1,P1>, BaseDimension<Id2,P2> >
{
  using type = Dimension<BaseDimension<Id1,P1>, BaseDimension<Id2,P2> >;
};


template<typename B1, typename B2, typename ...Bs2>
struct Multiply< Dimension<B1>, Dimension<B2,Bs2...> >
{ private: //just define some usefull aliases
    template<typename ... Bs> using D = Dimension<Bs...>;
    using D1 = Dimension<B1>;
    using D2 = Dimension<B2>;
    using power_sum = typename std::ratio_add<typename B1::power,typename B2::power >::type;
    static const bool is_power_sum_zero = std::ratio_equal<power_sum,std::ratio<0,1>>::value;
    template<typename A, typename B>  using M =  typename Multiply<A,B>::type;
    template< bool C, typename A, typename B> using IF  = typename std::conditional<C,A,B>::type;
    static const bool ordered = B1::id < B2::id;
    static const bool not_ordered = B2::id < B1::id;
    static const bool collapse = B1::id == B2::id && is_power_sum_zero;
  public:
    using DR = Dimension<Bs2...>;
    using MD1DR = M<D1,DR>;
    static const int id1 = B1::id;
    static const int id2 = B2::id;
    //using MM =  typename std::enable_if<true, D1, M<D2,MD1DR>>::type;
    //using MM =  IF<true,D2, M<D2,MD1DR>>;
  using type =  
        IF<  (B1::id  < B2::id),  Dimension<B1, B2,Bs2...>,  //the dimension id is ordered in a right way this is the end of the recursion
        IF<  (B2::id  < B1::id),  typename Concatenate< D<B2>, M<D<B1>, DR>>::type,    //wrong id order swap dimensions and go deeper into recursion
        IF<   is_power_sum_zero,  Dimension<Bs2...>,         //same dimensions and sum of the powers is zero remove this base dimension from result Dimension list (end of the recursion)
                                  Dimension<BaseDimension<B1::id, power_sum>, Bs2...> //multiply same dimensions
        >>>;
  //using type =  
  //      IF<  (B1::id<B2::id), Dimension<B1, B2, Bs2...>,  //the dimension id is ordered in a right way this is the end of the recursion
  //      IF<  (B2::id<B1::id), typename Concatenate< D<B2>, M<D<B1>, DR>>::type,    //wrong id order swap dimensions and go deeper into recursion
  //      typename MultiplyBase<B1,B2>::type
  //      >>;
};

//multiplication of the two complex dimensions
template <typename B1, typename D2,typename ... Bs1 >
struct Multiply< Dimension<Bs1...,B1>, D2>
{
    using type = typename Multiply< Dimension<Bs1...> , typename Multiply<Dimension<B1>, D2>::type >::type;
};

template<int Id>  using new_dimension = Dimension< BaseDimension<Id> >;

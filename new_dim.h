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
template<int Id, typename R = std::ratio<1,1> >
struct BaseDimension 
{
  static const int  id = Id;
  using ratio = R;
};

//complex dimension is a series of base dimensions
template<typename ...B>
struct Dimension{};

//multiplication of the complex dimensions
template<typename D1, typename D2>
struct Multiply;


template<>
struct Multiply<Dimension<>, Dimension<> >
{
  using type = Dimension<>;
};

//define the end of the recursion (Multiplication on dimensionless on left)
template<typename B>
struct Multiply<Dimension<B>, Dimension<> >
{
  using type = Dimension<B>;
};

//Multiplication on dimensionless on right
template<typename B>
struct Multiply< Dimension<>, Dimension<B> >
{
  using type = Dimension<B>;
};

template<typename B1, typename B2>
struct Multiply< Dimension<B1>, Dimension<B2> >
{
  private:
    using rsum = typename std::ratio_add<typename B1::ratio,typename B2::ratio >::type;
    using rzero = std::ratio<0,1>;
    static const bool is_rsum_zero = std::ratio_equal<rsum,rzero>::value;
    //using MULT = typename Multiply<Dimension<B1>, Dimension<Bs...>>::type;
    template<typename A, typename B>  using M =  typename Multiply<A,B>::type;
    template<typename ... Bs> using D = Dimension<Bs...>;
  public:
  using type =  
    //the dimension id is ordered in a right way this is the end of the recursion
    typename std::conditional< !is_rsum_zero && B1::id==B2::id,  
             D<BaseDimension< B1::id, rsum>>,     
             typename std::conditional< is_rsum_zero && B1::id==B2::id,
                      D<>,
                      typename std::conditional< (B1::id < B2::id),
                                D<B1,B2>,
                                D<B2,B1>
                      >::type
            >::type 
    >::type;
};

template<typename B1, typename B2, typename ...Bs2>
struct Multiply< Dimension<B1>, Dimension<B2,Bs2...> >
{
  private:
    using rsum = typename std::ratio_add<typename B1::ratio,typename B2::ratio >::type;
    using rzero = std::ratio<0,1>;
    static const bool is_rsum_zero = std::ratio_equal<rsum,rzero>::value;
    template<typename A, typename B>  using M =  typename Multiply<A,B>::type;
    template<typename ... Bs> using D = Dimension<Bs...>;
    using Deeper = typename Multiply<Dimension<B1>, Dimension<Bs2...> >::type;
    template< bool C, typename A, typename B> using If  = typename std::conditional<C,A,B>::type;
  public:
  //using type =  
  //  //the dimension id is ordered in a right way this is the end of the recursion
  //  typename std::conditional<  (B1::id  < B2::id),   D<B1,B2,Bs2...>,   
  //  //wrong id order go deeper into recursion
  //  typename std::conditional<  (B2::id  < B1::id),  M< D<B2>, M< D<B1>, D<Bs2...>>> ,  
  //  //same dimension id (B1::id==B2::id), 
  //  //if sum of the ratios is zero remove this base dimension from Dimension list
  //  typename std::conditional< !is_rsum_zero,   D<Bs2...>,  
  //  //multiply dimensions
  //  D<BaseDimension< B1::id, rsum>, Bs2...>
  //  >::type >::type >::type;
  //using type = D<BaseDimension< B1::id, rsum>, Bs2...>;
  using type =  
    //the dimension id is ordered in a right way this is the end of the recursion
    If < (!is_rsum_zero && B1::id==B2::id),  
             D<BaseDimension< B1::id, rsum>, Bs2...>,     
             If < (is_rsum_zero && B1::id==B2::id) ,
                      D<Bs2...>,
                      If < (B1::id < B2::id),
                                D<B1,B2,Bs2...>,
                                //D<B1,B2>,
                                M< D<B2>, typename Multiply< D<B1>, D<Bs2...> >::type >
                                //D<B2,B1,Bs2...>
                                //M < D<B2>, D<B1> >
                                //M < D<B2>, Deeper >
                      >
            > 
    >;
};

//multiplication of the two complex dimensions
template <typename B1, typename B2, typename D2,typename ... Bs1 >
struct Multiply< Dimension<Bs1...,B2, B1>, D2>
{
    using type = typename Multiply< Dimension<Bs1...,B2> , typename Multiply<Dimension<B1>, D2>::type >::type;
    //using type = typename Multiply<Dimension<B1>, D2>::type;
};

template<int Id>  using new_dimension = Dimension< BaseDimension<Id> >;

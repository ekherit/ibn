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

#include "dimension.h"

template< typename D, typename Field = double >
struct unit
{
  //using dimension = typename std::conditional< std::is_same<int, decltype(D::id)>::value,  Dimension<D>, D>::type;
  using dimension = D;
  unit() : data(1.0){};
  unit(unit  & t) noexcept : data(t.data) {}
  unit(const unit  & t) noexcept : data(t.data) {}

  private:
  using Type = unit<dimension, Field>;
  public:
  explicit unit(Field f) noexcept: data(f){} 

  friend Type operator*(Field f,  Type t) noexcept { return Type(f*t.data); }
  friend Type operator*(Type  t, Field f) noexcept { return Type(t.data*f); }

  //template < typename T>
  //friend typename std::enable_if< std::is_same<T, Type >::value, Type>::type()
  //{
  //  return Type(data);
  //}
  
  friend Type operator+(Type t1, Type t2) noexcept { return Type(t1.data + t2.data); }
  friend Type operator-(Type t1, Type t2) noexcept { return Type(t1.data - t2.data); }

  friend Type operator/(Type t, Field f) { return Type(t.data/f); }

  friend std::ostream & operator<<(std::ostream & os, const Type & t) { os << t.data; return os; } 

  operator typename std::conditional< is_dimensionless<dimension>::value,  Field, Type>::type ()
  {
    return data;
  };

  private:
  public:
  Field data;
};

template<int Id, typename Field=double>
using make_unit = unit< Dimension<BaseDimension<Id> >, Field>; 


template < typename D1, typename D2, typename Field>
inline auto operator*(unit<D1, Field> t1 , unit<D2, Field> t2 )
{
  using result_type = typename Multiply<D1, D2>::type;
  return  unit<result_type,Field>(t1.data*t2.data);
}

template < typename Unit1, typename Unit2, typename Field>
inline auto operator/(unit<Unit1, Field> t1 , unit<Unit2, Field> t2 )
{
  using result_type = typename Divide<Unit1, Unit2>::type;
  return  unit<result_type,Field>(t1.data/t2.data);
}


#include <math.h>

template <int id, typename F>
F pow(unit<BaseDimension<id,std::ratio<0ll,1ll>>, F> u, F f)
{
  return pow(u.data,f);
};


//result of operation with dimension less units will convert to Files
template <int Id, typename F>
inline F operator+(F f,  unit< Dimension<> , F> u)                                   { return f+u.data; };

template <int Id, typename F>
inline F operator+(unit< Dimension<> , F> u, F f)                                    { return f+u.data; };

template <int Id, typename F>
inline F operator+(F f,  unit< BaseDimension<Id, std::ratio<0,1>> , F> u)            { return f+u.data; };

template <int Id, typename F>
inline F operator+(unit< BaseDimension<Id, std::ratio<0,1>> , F> u, F f)             { return f+u.data; };

template <int Id, typename F>
inline F operator+(F f,  unit< Dimension<BaseDimension<Id, std::ratio<0,1>>> , F> u) { return f+u.data; };

template <int Id, typename F>
inline F operator+(unit< Dimension<BaseDimension<Id, std::ratio<0,1>>> , F> u, F f)  { return f+u.data; };

//subtraction for dimentionless
template <int Id, typename F>
inline F operator-(F f,  unit< BaseDimension<Id, std::ratio<0,1>> , F> u)            { return f-u.data; };

template <int Id, typename F>
inline F operator-(unit< BaseDimension<Id, std::ratio<0,1>> , F> u, F f)             { return u.data-f; };

template <int Id, typename F>
inline F operator-(F f,  unit< Dimension<BaseDimension<Id, std::ratio<0,1>>> , F> u) { return f - u.data; };

template <int Id, typename F>
inline F operator-(unit< Dimension<BaseDimension<Id, std::ratio<0,1>>> , F> u, F f)  { return u.data - f; };

//template <int Id, typename F>
//inline F operator*(F f,  unit< Dimension<> , F> u)                                   { return f*u.data; };
//
//template <int Id,typename F>
//inline F operator*(unit< Dimension<> , F> u, F f)                                    { return f*u.data; };
//
//template <int Id,typename F>
//inline F operator*(F f,  unit< BaseDimension<Id, std::ratio<0,1>> , F> u)            { return f*u.data; };
//
//template <int Id,typename F>
//inline F operator*(unit< BaseDimension<Id, std::ratio<0,1>> , F> u, F f)             { return f*u.data; };
//
//template <int Id,typename F>
//inline F operator*(F f,  unit< Dimension<BaseDimension<Id, std::ratio<0,1>>> , F> u) { return f*u.data; };
//
//template <int Id,typename F>
//inline F operator*(unit< Dimension<BaseDimension<Id, std::ratio<0,1>>> , F> u, F f)  { return f*u.data; };



//template<int Id, typename R, typename R2, typename F>
//auto operator^(unit<BaseUnit<Id,R>,F> u, R2 p)
//{
//  F d = pow(u.data,p);
//  using type = BaseUnit<Id,std::ratio_multiply<R,R2>>;
//  return type(d);
//}


//template<int Id, typename R, typename F>
//constexpr auto operator^(unit<BaseUnit<Id,R>,F> u, long long p)
//{
//  using P = std::ratio<p,1ll>; 
//  F d = pow(u.data,P);
//  using type = BaseUnit<Id,std::ratio_multiply<R,P>>;
//  return type(d);
//}

//template<typename B1, typename ... Bs,  typename F, long P>
//auto operator^(unit<Unit<B1,Bs...>,F> u, std::ratio<P,1ll> p)
//{
//  using PowerType = std::ratio<P,1ll>; 
//  using Bp = typename Power<B1,PowerType>::type;
//  F d = pow(u.data,p);
//  using type = Multiply<Bp
//  return type(d);
//}

/*
namespace counter
{
  template<int N>
    struct flag {
      friend constexpr int adl_flag (flag<N>);
    };

  template<int N>
    struct writer {
      friend constexpr int adl_flag (flag<N>) {
        return N;
      }

      static constexpr int value = N;
    };

  template<int N, int = adl_flag (flag<N> {})>
    int constexpr reader (int, flag<N>) {
      return N;
    }

  template<int N>
    int constexpr reader (float, flag<N>, int R = reader (0, flag<N-1> {})) {
      return R;
    }

  int constexpr reader (float, flag<0>) {
    return 0;
  }

  template<int N = 1>
    int constexpr next (int R = writer<reader (0, flag<32> {}) + N>::value) {
      return R;
    }
}
*/

//constexpr int DIMENSION_ID=0;

//template<typename Field=double>
//auto make_unit(void)
//{
//  constexpr static int id = 0;
//  id++;
//  return unit< Dimension<BaseDimension<id>>, Field>();
//};

#endif

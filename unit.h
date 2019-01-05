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
template< typename U, typename Field = double >
struct unit
{
  unit() : data(1.0){};
  unit(unit  & t) noexcept : data(t.data) {}
  unit(const unit  & t) noexcept : data(t.data) {}
  //operator Field()
  //{
  //  //static_assert(TotalDimension<U>::value == 0);
  //  //if constexpr(TotalDimension<U>::value == 0) return data;
  //  return data;
  //};

  using dimention = U;
  private:
  using Type = unit<U, Field>;
  explicit unit(Field f) noexcept: data(f){} 


  inline friend Type operator*(Field f, Type t)
  {
    return Type(f*t.data);
  }

  friend Type operator+(Type t1, Type t2)
  {
    return Type(t1.data + t2.data);
  }

  friend Type operator-(Type t1, Type t2)
  {
    return Type(t1.data - t2.data);
  }

  friend Type operator*(Type t, Field f )
  {
    return Type(t.data*f);
  }

  friend Type operator/(Type t, Field f)
  {
    return Type(t.data/f);
  }

  friend std::ostream & operator<<(std::ostream & os, const Type & t)
  {
    os << t.data;
    return os;
  }


  public:

  Field data;
  private:
};

#include <math.h>

template <int id, typename F>
F pow(unit<BaseDimension<id,std::ratio<0ll,1ll>>, F> u, F f)
{
  return pow(u.data,f);
};

template<typename Field>
struct dimention_less_unit 
{
  template < int id >
    dimention_less_unit(const unit< BaseDimension<id,std::ratio<0,1> >, Field> d )
    {
      data = d.data;
    }

  operator Field()
  {
    return  data;
  }
  Field data;
};

//type cast to Field (double) for dimentionless units
//template<typename Field>
//struct dimention_less_unit : unit< Unit<>, Field> 
//{
//  operator Field()
//  {
//    return unit<Unit<>,Field>::data;
//  };
//};

//template<int id, typename Field>
//struct base_dimention_less_unit : unit< BaseUnit<id, std::ratio<0ll,1ll>> , Field> 
//{
//  using type = unit< BaseUnit<id, std::ratio<0ll,1ll>> , Field>;
//  private:
//  operator Field()
//  {
//    return type::data;
//  };
//};


template <int Id, typename F>
inline F operator+(F f,  unit< BaseDimension<Id, std::ratio<0ll,1ll>> , F> u)
{
  return f+u.data;
}; 

template <int Id, typename F>
inline F operator+(unit< BaseDimension<Id, std::ratio<0ll,1ll>> , F> u, F f)
{
  return f+u.data;
}; 

template <int Id, typename F>
inline F operator+(F f,  unit< Dimension<BaseDimension<Id, std::ratio<0ll,1ll>>> , F> u)
{
  return f+u.data;
}; 

template <int Id, typename F>
inline F operator+(unit< Dimension<BaseDimension<Id, std::ratio<0ll,1ll>>> , F> u, F f)
{
  return f+u.data;
}; 

//subtraction for dimentionless
template <int Id, typename F>
inline F operator-(F f,  unit< BaseDimension<Id, std::ratio<0ll,1ll>> , F> u)
{
  return f-u.data;
}; 

template <int Id, typename F>
inline F operator-(unit< BaseDimension<Id, std::ratio<0ll,1ll>> , F> u, F f)
{
  return u.data-f;
}; 

template <int Id, typename F>
inline F operator-(F f,  unit< Dimension<BaseDimension<Id, std::ratio<0ll,1ll>>> , F> u)
{
  return f - u.data;
}; 

template <int Id, typename F>
inline F operator-(unit< Dimension<BaseDimension<Id, std::ratio<0ll,1ll>>> , F> u, F f)
{
  return u.data - f;
}; 

template < typename Unit1, typename Unit2, typename Field>
inline auto operator*(unit<Unit1, Field> t1 , unit<Unit2, Field> t2 )
{
  using result_type = typename Multiply<Unit1, Unit2>::type;
  return  unit<result_type,Field>()*(t1.data*t2.data);
}

template < typename Unit1, typename Unit2, typename Field>
inline auto operator/(unit<Unit1, Field> t1 , unit<Unit2, Field> t2 )
{
  using result_type = typename Divide<Unit1, Unit2>::type;
  return  unit<result_type,Field>()*(t1.data/t2.data);
}



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

#endif

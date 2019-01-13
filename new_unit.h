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

#include "new_dim.h"
#include <cmath>

template< typename D, typename Field = double >
struct unit
{
  private:
  public:
  using dimension = D;
  using field=Field;
  using type = unit<dimension, Field>;
  unit(Field f) noexcept: data(f){} 
  unit(unit  & t)       noexcept : data(t.data) {}
  unit(unit  && t)      noexcept : data(t.data) {}
  unit(const unit  & t) noexcept : data(t.data) {}
  unit(void) : data(1.0){};

  friend type operator*(const field & f, const type  & t) noexcept { return {f*t.data}; }
  friend type operator*(const  type & t, const field & f) noexcept { return {t.data*f}; }

  
  friend type operator+(const type & t1, const type & t2) noexcept { return {t1.data + t2.data}; }
  friend type operator-(const type & t1, const type & t2) noexcept { return {t1.data - t2.data}; }

  friend type operator/(const type & t, const field & f) { return {t.data/f}; }
  friend auto operator/(const field & f, const type & t) -> typename Inverse<dimension>::type { return {f/t.data}; }


  

  operator typename std::conditional< is_dimensionless<dimension>::value, field, type>::type () { return data; };

  template<int Num,int Den=1>
  auto pow(void) -> unit<typename Power<dimension, Num, Den>::type,field>
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



#endif

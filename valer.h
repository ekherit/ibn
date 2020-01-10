/*
 * =====================================================================================
 *
 *       Filename:  valer.h
 *
 *    Description:  Value and error pair
 *
 *        Version:  1.0
 *        Created:  07/12/2010 11:12:46 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan B. Nikolaev (), I.B.Nikolaev@inp.nsk.su
 *        Company:  Budker Institute of Nuclear Physics, Novosibirsk, Russia
 *
 * =====================================================================================
 */
#pragma once
#ifndef IBN_VALER_H
#define IBN_VALER_H

#include <cmath>

namespace ibn
{
  template <typename T>
  struct valer
  { 
    using type = typename std::remove_const<typename std::remove_reference<T>::type>::type;
    using referenece = type &;
    //using pointer = T*;
    T value; //the value or reference to value
    T error; //and the error or reference to error

    constexpr valer(void)  = default; // I dont know should i init valer or not
    constexpr valer(const type & v)                  noexcept : value(v),       error(0)       {}
    constexpr valer(const type & v, const type & e)  noexcept : value(v),       error(e)       {}
    constexpr valer(      type & v,       type & e)  noexcept : value(v),       error(e)       {}

    constexpr valer(const valer<type> & v)           noexcept : value(v.value), error(v.error) {}
    constexpr valer(valer<type> & v)                 noexcept : value(v.value), error(v.error) {}


    //dont know why it doest work with const valer<T&> &
    constexpr valer(valer<type&> & v)            noexcept : value(v.value), error(v.error) {}
    constexpr valer(valer<const type&> & v)      noexcept : value(v.value), error(v.error) {}
    //constexpr valer(const valer<type&> & v)            noexcept : value(v.value), error(v.error) {}
    //constexpr valer(const valer<const type&> & v)      noexcept : value(v.value), error(v.error) {}

    //assignment operators
    constexpr valer<T> & operator=(const type & v) noexcept { 
      value=v; 
      error=0; 
      return *this; 
    };

    //constexpr valer<T> & operator=(type && v) noexcept { 
    //  value=v; 
    //  error=0; 
    //  return *this; 
    //};

    constexpr valer<T> & operator=(const valer<type> & v) noexcept { 
      value=v.value; 
      error=v.error;
      return *this;
    }

    constexpr valer<T> & operator=(valer<type> & v) noexcept { 
      value=v.value; 
      error=v.error;
      return *this;
    }

    //implicit (narrowing) conversion to base type
    constexpr operator T (void) const noexcept { return value; };

    inline valer <T>& operator+=(const T & x) {
      value+=x;
      return *this;
    }

    inline valer <T>& operator+=(const valer <T> & x) {
      error=sqrt(error*error + x.error*x.error);
      value+=x.value;
      return *this;
    }

    inline valer <T>& operator-=(const T & x) {
      value-=x;
      return *this;
    }

    inline valer <T>& operator-=(const valer <T> & x) {
      error=sqrt(error*error + x.error*x.error);
      value-=x.value;
      return *this;
    }

    inline valer <T>& operator*=(const T & x) {
      value*=x;
      error*=x;
      return *this;
    }

    inline valer <T> & operator*=(const valer <T> & x) {
      error=sqrt(error*error*x.value*x.value + value*value*x.error*x.error);
      value*=x.value;
      return *this;
    }

    inline valer <T>& operator/=(const T & x) {
      value/=x;
      error/=x;
      return *this;
    }

    inline valer <T> & operator/=(const valer <T> & x) {
      T x2=x.value*x.value;
      error=sqrt(error*error/x2 + value*value/(x2*x2)*x.error*x.error);
      value/=x.value;
      return *this;
    }

    inline valer<T> & operator-(void) noexcept {
      value = - value;
    }



    valer<type> operator + (const type & y)         noexcept { return {valer<type>(*this) += y}; };
    valer<type> operator + (const valer<type> & y)  noexcept { return {valer<type>(*this) += y}; };
    valer<type> operator + (const valer<type&> & y) noexcept { return {valer<type>(*this) += y}; };

    valer<type> operator - (const type & y)         noexcept { return {valer<type>(*this) -= y}; };
    valer<type> operator - (const valer<type> & y)  noexcept { return {valer<type>(*this) -= y}; };
    valer<type> operator - (const valer<type&> & y) noexcept { return {valer<type>(*this) -= y}; };

    valer<type> operator * (const type & y)         noexcept { return {valer<type>(*this) *= y}; };
    valer<type> operator * (const valer<type> & y)  noexcept { return {valer<type>(*this) *= y}; };
    valer<type> operator * (const valer<type&> & y) noexcept { return {valer<type>(*this) *= y}; };

    valer<type> operator / (const type & y)         noexcept { return {valer<type>(*this) /= y}; };
    valer<type> operator / (const valer<type> & y)  noexcept { return {valer<type>(*this) /= y}; };
    valer<type> operator / (const valer<type&> & y) noexcept { return {valer<type>(*this) /= y}; };

    friend valer<type> operator ^ (const valer<T> & x, const valer<T> & y) noexcept {
      return { pow(x.value, y.value), 
        sqrt(  pow( y.value * pow(x.value, y.value-1) * x.error, 2.0)  +
               pow( log(x.value)*pow(x.value,y.value) ,2.0) ) };
    }
  };

  /* Template class argument deduction guide */
  template<class T>  valer(T&)  -> valer<T>;  
  template<class T>  valer(T&&)  -> valer<T>;  
  template<class T>  valer(T&,T&)  -> valer<T>;  
  template<class T>  valer(T&&,T&&)  -> valer<T>;  

}

#endif //IBN_VALER_H

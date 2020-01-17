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
#include <type_traits>
#include <utility>

#include <iostream>
#include <typeinfo>
namespace ibn
{
  template <typename T>
  struct valer
  { 
    using type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
    using referenece = type &;
    //using pointer = T*;
    T value; //the value or reference to value
    T error; //and the error or reference to error

    constexpr void print(std::string_view s) const {
//      //std::cout << "call  " << s << " : "  << "type = " << typeid(type).name() << std::endl;
    }

    constexpr valer(void)  = default; 
    constexpr valer(const type &  v)                   noexcept : value(v),       error(0)       {print("const type & v"); }
    constexpr valer(const type &  v, const type &  e)  noexcept : value(v),       error(e)       {print("const type &v, const type &e");}
    constexpr valer(      type &  v,       type &  e)  noexcept : value(v),       error(e)       {print("type &v, type &e");}
    constexpr valer(      type && v,       type && e)  noexcept : value(v),       error(e)       {print("type&&v, type&&e");}

    /*
    template<typename T2>
    constexpr valer(const T2 & v, const T2 &e)  noexcept : value(v), error(e) {}

    template<typename T2>
    constexpr valer(T2 && v, T2 &&e)  noexcept : value(v), error(e) {}
    */

    constexpr valer(const valer<type> & v)             noexcept : value(v.value), error(v.error) {print("const valer<type> &v");}
    constexpr valer(valer<type> & v)                   noexcept : value(v.value), error(v.error) {print("valer<type> &v");}
    constexpr valer(valer<type> && v)                  noexcept : value(v.value), error(v.error) {print("valer<type> &&v");}


    //dont know why it doest work with const valer<T&> &
    constexpr valer(valer<type&> & v)                  noexcept : value(v.value), error(v.error) {print("valer<type&> &v");}
    constexpr valer(const valer<const type&> & v)      noexcept : value(v.value), error(v.error) {print("const valer<const type&>& v");}
    constexpr valer(const valer<type&> & v)            noexcept : value(v.value), error(v.error) {print("const valer<type&>& v");}
    //constexpr valer(const valer<type&> & v)            noexcept : value(v.value), error(v.error) {}
    //constexpr valer(const valer<const type&> & v)      noexcept : value(v.value), error(v.error) {}

    //assignment operators
    constexpr valer<T> & operator=(const type & v) noexcept { 
//      std::cout << " operator = (const type & v)" << std::endl;
      value=v; 
      error=0; 
      return *this; 
    };

    constexpr valer<T> & operator=(type && v) noexcept { 
//      std::cout << " operator = (type && v)" << std::endl;
      value=v; 
      error=0; 
      return *this; 
    };

    template<typename T2>
    constexpr valer<T> & operator=(const valer<T2> & v) noexcept { 
//      std::cout << " operator = (const veler<type> &v)" << std::endl;
      value=v.value; 
      error=v.error;
      return *this;
    }

    constexpr valer<T> & operator=(const valer<type> & v) noexcept { 
//      std::cout << " operator = (const veler<type> &v)" << std::endl;
      value=v.value; 
      error=v.error;
      return *this;
    }

    constexpr valer<T> & operator=(const valer<type&> & v) noexcept { 
//      std::cout << " operator = (const veler<type&> &v)" << std::endl;
      value=v.value; 
      error=v.error;
      return *this;
    }
    template<std::size_t N>
    constexpr valer<T> & operator=(const type(&v)[N]) noexcept { 
      static_assert(N == 2, "Must be value and error");
      std::cout << " operator = (const std::initializer_list<type> &v)" << std::endl;
      value = v[0];
      error = v[1];
      return *this;
    }

    template<typename T2>
    constexpr valer<T> & operator=(valer<T2> & v) noexcept { 
      value=v.value; 
      error=v.error;
      return *this;
    }

    template<typename T2>
    constexpr valer<T> & operator=(valer<T2> && v) noexcept { 
      value=v.value; 
      error=v.error;
      return *this;
    }

/*
    constexpr valer<T> & operator=(valer<type> & v) noexcept { 
//      std::cout << "operator=(  veler<type> &v)" << std::endl;
      value=v.value; 
      error=v.error;
      return *this;
    }

    constexpr valer<T> & operator=(valer<type&> & v) noexcept { 
//      std::cout << " operator=( veler<type&> &v)" << std::endl;
      value=v.value; 
      error=v.error;
      return *this;
    }
    */


    //constexpr valer<T> & operator=(valer<type> && v) noexcept { 
//    //  std::cout << " operator = (const veler<type> &v)" << std::endl;
    //  value=v.value; 
    //  error=v.error;
    //  return *this;
    //}

    //constexpr valer<T> & operator=(const std::initializer_list<type> & l) noexcept { 
    //  assert(l.size() == 2, "Must be value and error in brace initializer list");
//    //  std::cout << " operator = (const std::initializer_list<type> &v)" << std::endl;
    //  auto it = l.begin();
    //  value = *it;
    //  ++it;
    //  error = *it;
    //  //value=v.value; 
    //  //error=v.error;
    //  return *this;
    //}

    //constexpr valer<T> & operator=(const valer<const type&> & v) noexcept { 
    //  value=v.value; 
    //  error=v.error;
    //  return *this;
    //}


    //dont know why it doest work with const valer<T&> &
    //constexpr valer(valer<type&> & v)            noexcept : value(v.value), error(v.error) {}
    //constexpr valer(valer<const type&> & v)      noexcept : value(v.value), error(v.error) {}

    //constexpr valer<T> & operator= (const std::initializer_list<type> &l) noexcept {
    //  auto it = l.cbegin();
    //  value = it;
    //  error = ++it;
    //  return *this;
    //}

    //constexpr valer<T> & operator=(const valer<const type&> & v) noexcept { 
    //  value=v.value; 
    //  error=v.error;
    //  return *this;
    //}

    //implicit (narrowing) conversion to base type
    constexpr operator type (void) const noexcept { return value; };

    inline valer <T>& operator+=(const T & x) {
      value+=x;
      return *this;
    }

    template <typename T2>
    inline valer <T>& operator+=(const valer <T2> & x) {
      error=sqrt(error*error + x.error*x.error);
      value+=x.value;
      return *this;
    }
    /*

    inline valer <T>& operator+=(const valer <type &> & x) {
      error=sqrt(error*error + x.error*x.error);
      value+=x.value;
      return *this;
    }

    inline valer <T>& operator+=(const valer <type const &> & x) {
      error=sqrt(error*error + x.error*x.error);
      value+=x.value;
      return *this;
    }
    */

    inline valer <T>& operator-=(const T & x) {
      value-=x;
      return *this;
    }

    template <typename T2>
    inline valer <T>& operator-=(const valer <T2> & x) {
      error=sqrt(error*error + x.error*x.error);
      value-=x.value;
      return *this;
    }

    /*
    inline valer <T>& operator-=(const valer <type &> & x) {
      error=sqrt(error*error + x.error*x.error);
      value-=x.value;
      return *this;
    }

    inline valer <T>& operator-=(const valer <type const &> & x) {
      error=sqrt(error*error + x.error*x.error);
      value-=x.value;
      return *this;
    }
    */

    inline valer <T>& operator*=(const T & x) {
      value*=x;
      error*=x;
      return *this;
    }

    template <typename T2>
    inline valer <T> & operator*=(const valer <T2> & x) {
      error=sqrt(error*error*x.value*x.value + value*value*x.error*x.error);
      value*=x.value;
      return *this;
    }
    /*
    inline valer <T> & operator*=(const valer <type> & x) {
      error=sqrt(error*error*x.value*x.value + value*value*x.error*x.error);
      value*=x.value;
      return *this;
    }

    inline valer <T> & operator*=(const valer <type&> & x) {
      error=sqrt(error*error*x.value*x.value + value*value*x.error*x.error);
      value*=x.value;
      return *this;
    }

    inline valer <T> & operator*=(const valer <type const &> & x) {
      error=sqrt(error*error*x.value*x.value + value*value*x.error*x.error);
      value*=x.value;
      return *this;
    }
    */

    inline valer <T>& operator/=(const T & x) {
      value/=x;
      error/=x;
      return *this;
    }

    template<typename T2>
    inline valer <T> & operator/=(const valer <T2> & x) {
      T x2=x.value*x.value;
      error=sqrt(error*error/x2 + value*value/(x2*x2)*x.error*x.error);
      value/=x.value;
      return *this;
    }
    /*

    inline valer <T> & operator/=(const valer <type> & x) {
      T x2=x.value*x.value;
      error=sqrt(error*error/x2 + value*value/(x2*x2)*x.error*x.error);
      value/=x.value;
      return *this;
    }

    inline valer <T> & operator/=(const valer <type&> & x) {
      T x2=x.value*x.value;
      error=sqrt(error*error/x2 + value*value/(x2*x2)*x.error*x.error);
      value/=x.value;
      return *this;
    }

    inline valer <T> & operator/=(const valer <type const&> & x) {
      T x2=x.value*x.value;
      error=sqrt(error*error/x2 + value*value/(x2*x2)*x.error*x.error);
      value/=x.value;
      return *this;
    }
    */

    inline valer<T> & operator-(void) noexcept {
      value = - value;
    }



    valer<type> operator + (const type & y)               noexcept { return {valer<type>(*this) += y}; };
    valer<type> operator + (const valer<type> & y)        noexcept { return {valer<type>(*this) += y}; };
    valer<type> operator + (const valer<type&> & y)       noexcept { return {valer<type>(*this) += y}; };
    valer<type> operator + (const valer<const type&> & y) noexcept { return {valer<type>(*this) += y}; };

    valer<type> operator - (const type & y)               noexcept { return {valer<type>(*this) -= y}; };
    valer<type> operator - (const valer<type> & y)        noexcept { return {valer<type>(*this) -= y}; };
    valer<type> operator - (const valer<type&> & y)       noexcept { return {valer<type>(*this) -= y}; };
    valer<type> operator - (const valer<type const&> & y) noexcept { return {valer<type>(*this) -= y}; };

    template<typename T2, typename R = typename std::common_type<type,T2>::type >
    valer<R> operator * (const T2 & y)                    noexcept { return {valer<R>(value,error) *= y}; };
    valer<type> operator * (const valer<type> & y)        noexcept { return {valer<type>(*this) *= y}; };
    valer<type> operator * (const valer<type&> & y)       noexcept { return {valer<type>(*this) *= y}; };
    valer<type> operator * (const valer<type const&> & y) noexcept { return {valer<type>(*this) *= y}; };

    valer<type> operator / (const type & y)               noexcept { return {valer<type>(*this) /= y}; };
    valer<type> operator / (const valer<type> & y)        noexcept { return {valer<type>(*this) /= y}; };
    valer<type> operator / (const valer<type&> & y)       noexcept { return {valer<type>(*this) /= y}; };
    valer<type> operator / (const valer<type const&> & y) noexcept { return {valer<type>(*this) /= y}; };

    /*
    friend valer<type> operator ^ (const valer<T> & x, const valer<T> & y) noexcept {
      return { pow(x.value, y.value), 
        sqrt(  pow( y.value * pow(x.value, y.value-1) * x.error, 2.0)  +
               pow( log(x.value)*pow(x.value,y.value) ,2.0) ) };
    }
    */

    constexpr valer<T> & operator++(void) noexcept { 
      ++value;
      return *this;
    }
    constexpr valer<T> & operator--(void) noexcept { 
      --value;
      return *this;
    }

    constexpr valer<T>  operator++(int) noexcept { 
      valer<type> tmp(*this);
      ++value;
      return std::move(tmp);
    }

    constexpr valer<T>  operator--(int) noexcept { 
      valer<type> tmp(*this);
      --value;
      return std::move(tmp);
    }

  };

  /* Template class argument deduction guide */
  template<class T>  valer(T&)  -> valer<T>;  
  template<class T>  valer(T&&)  -> valer<T>;  
  template<class T>  valer(T&,T&)  -> valer<T>;  
  template<class T>  valer(T&&,T&&)  -> valer<T>;  

  /*
  template<typename T, typename = std::enable_if< std::is_same<typename std::remove_cv<T>::type,T>::value>>  valer(T&)  -> valer<T>;  
  template<typename T, typename = std::enable_if< std::is_same<typename std::remove_cv<T>::type,T>::value>>  valer(T&&)  -> valer<T>;  
  template<typename T, typename = std::enable_if< std::is_same<typename std::remove_cv<T>::type,T>::value>>  valer(T&,T&)  -> valer<T>;  
  template<typename T, typename = std::enable_if< std::is_same<typename std::remove_cv<T>::type,T>::value>>  valer(T&&,T&&)  -> valer<T>;  
  */
  /*
  template<class T>  valer( valer<T&> & ) -> valer<T>;
  template<class T>  valer( valer<T&> && ) -> valer<T>;
  template<class T>  valer( valer<const T&>  ) -> valer<T>;
  */

  template<typename T1,typename T2>
  bool operator<( const valer<T1> & v1, const valer<T2> & v2) {
    return v1.value < v2.value;
  }

  template<typename T1,typename T2>
  bool operator<=( const valer<T1> & v1, const valer<T2> & v2) {
    return v1.value <= v2.value;
  }

  template<typename T1,typename T2>
  bool operator>( const valer<T1> & v1, const valer<T2> & v2) {
    return v1.value > v2.value;
  }

  template<typename T1,typename T2>
  bool operator>=( const valer<T1> & v1, const valer<T2> & v2) {
    return v1.value >= v2.value;
  }

  template<typename T1,typename T2>
  bool operator==( const valer<T1> & v1, const valer<T2> & v2) {
    return v1.value == v2.value;
  }

  template<typename T1,typename T2>
  bool operator!=( const valer<T1> & v1, const valer<T2> & v2) {
    return !(v1.value == v2.value);
  }

  template<typename T1, typename T2, typename R = typename std::common_type<T1,T2>::type >
  valer<R> operator + (const T1 & y, const ibn::valer<T2> & v)             noexcept  { return {valer<R>(y) += v}; };

  //template<typename U>
  template<typename T1, typename T2, typename R = typename std::common_type<T1,T2>::type >
  valer<R> operator - (const T1 & y, const ibn::valer<T2> & v)               noexcept { return {valer<R>(y) -= v}; };

  template<typename T1, typename T2, typename R = typename std::common_type<T1,T2>::type >
  valer<R> operator * (const T1 & y, const ibn::valer<T2> & v)               noexcept { return valer<R>(y) *= v; };
  //valer<R> operator * (const T1 & y, const ibn::valer<T2> & v)               noexcept { std::cout << "*" << std::endl; return valer<R>(y) *= v; };

  //template<typename U>
  template<typename T1, typename T2, typename R = typename std::common_type<T1,T2>::type >
  valer<R> operator / (const T1 & y, const ibn::valer<T2> & v)               noexcept { return {valer<R>(y) /= v}; };

//  //template<typename U>
//  template<typename T1, typename T2, typename R = typename std::common_type<T1,T2>::type >
//  valer<R> operator + (const T1 & y, const ibn::valer<T2> & v)               noexcept { return {valer<R>(y) += v}; };


}

#endif //IBN_VALER_H

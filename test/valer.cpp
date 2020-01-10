#include "../valer.h"

#include "../opt.h"

#include <iostream>
#include <type_traits>
#include <iomanip>
#include <typeinfo>
#include <limits>
#include <complex>

template<typename T>
std::ostream & operator << (std:: ostream & os,  const ibn::valer<T> & d) {
  os << d.value << " +- " << d.error;
  return os;
};

template<typename T> using lim = std::numeric_limits<T>;


int main(int argc, char ** argv) {
  using namespace ibn;
  using namespace std;

  cout << "Testing constructor from simple type " << endl;
  {
    cout << " from int: ";
    using type_t  = int;
    constexpr type_t t = M_PI;
    constexpr valer<type_t> v(t);
    static_assert( 3 == v.value &&  v.error == 0 );
    cout << setprecision(lim<type_t>::digits10) << v << ":  OK " << endl;
  }
  {
    cout << " from long int: ";
    using type_t  = long int;
    constexpr type_t t = std::numeric_limits<long int>::max();
    constexpr valer<type_t> v(t);
    static_assert( t == v.value &&  v.error == 0 );
    cout << setprecision(lim<type_t>::digits10) << v << ":  OK " << endl;
  }
  {
    cout << " from float: ";
    using type_t  = float;
    constexpr type_t t = M_PI;
    constexpr valer<type_t> v(t);
    static_assert( type_t(t) == v.value &&  v.error == 0 );
    cout << setprecision(lim<type_t>::digits10) << v << ":  OK " << endl;
  }
  {
    cout << " from double: ";
    using type_t  = double;
    constexpr type_t t = M_PI;
    constexpr valer<type_t> v(t);
    static_assert( type_t(t) == v.value &&  v.error == 0 );
    cout << setprecision(lim<type_t>::digits10) << v << ":  OK " << endl;
  }
  {
    cout << " from long double: ";
    using type_t  = long double;
    constexpr type_t t = M_PIl;
    constexpr valer<type_t> v(t);
    static_assert( type_t(t) == v.value &&  v.error == 0 );
    cout << setprecision(lim<type_t>::digits10) << v << ":  OK " << endl;
  }

  cout << "Testing aggregate initializer" << endl;
  {
    cout << " from double: ";
    using type_t  = double;
    constexpr type_t t = M_PI;
    constexpr valer<type_t> v{t,t};
    static_assert( type_t(t) == v.value &&  v.error == type_t(t) );
    cout << setprecision(lim<type_t>::digits10) << v << ":  OK " << endl;
  }

  valer v(5.3);
  cout << v << endl;
  valer v2 = 23.3;
  valer v3 = 3;
  cout << v2 << endl;
  cout << typeid(v2.value).name() << endl;
  std::cout << "Test implicit conversion: " << std::endl;
  {
    cout << " to double: ";
    using type_t  = double;
    constexpr type_t t = M_PI;
    constexpr valer<type_t> v{t,t};
    //static_assert( type_t(t) == v.value &&  v.error == type_t(t) );
    //cout << setprecision(lim<type_t>::digits10) << v << ":  OK " << endl;
    constexpr type_t result = v;
    static_assert( t == result );
    cout << result << " OK " << endl;
  }

  std::cout << "Test implicit conversion: " << std::endl;
  {
    cout << " to complex<float>: ";
    using type_t  = complex<float>;
    cout << setprecision(lim<float>::digits10);
    constexpr type_t t = {M_PI, M_E};
    constexpr valer<type_t> v{t,t};
    constexpr type_t result = v;
    static_assert( t == result );
    cout << v << "  -> " << result << " OK " << endl;
  }

  std::cout << "Test reference: " << endl;
  {
    std::cout << "  double, double " << std::endl;
    double value=3.14;
    double error=2.72;
    valer <double &>  v{value,error};
    //valer <double &>  v(value,error);
    v.value *= 2;
    v.error *= 3;
    if( 3.14*2 == value && 2.72*3 == error) {
      cout << "value: 3.14  -> " << value << "  error: 2.72 -> " << error <<  " : OK " << endl ;
    }
  }
  {
    std::cout << " from valer<double,double> " << std::endl;
    valer<double> v(3.1,1.4);
    valer <double &>  ref(v);
    cout << ref << "  OK " << endl;
  }
  {
    std::cout << " from const double, const double " << std::endl;
    double value=3.14;
    double error=2.72;
    valer <const double &>  v{value,error};
    value *= 2;
    error *= 3;
    if( 3.14*2 == v.value && 2.72*3 == v.error) {
      cout << "v.value: 3.14  -> " << value << "  v.error: 2.72 -> " << error <<  " : OK " << endl ;
    }
  }
  {
    std::cout << "Test initialization from valer reference:  ";
    double value=3.14;
    double error=2.72;
    valer <double &>  v(value,error);
    valer <double> r(v); //impicit conversion to double and then to valer  lost error
    if( 3.14 == r.value && 2.72 == r.error) {
      cout << "v.value: 3.14  -> " << value << "  v.error: 2.72 -> " << error <<  " : OK " << endl ;
    }
    else { 
      cout << " ERROR " << endl;
      return 1;
    }
    r/=2.3;
  }
  {
    std::cout << "Check operations " << endl;
    valer  x{2.1, 3.1};
    valer  y{4.2, 5.2};
    cout << x+y << endl;
    cout << x-y << endl;
    cout << x*y << endl;
    cout << x/y << endl;
    //cout << x/2.0 << endl;
    //cout << x*2.0 << endl;
    cout << 2.0*x << endl;
    cout << 3.*x << endl;
    cout << x+2. << endl;
    cout << 2.+x << endl;
    cout << x-2. << endl;
    cout << 2.-x << endl;
    x={2,3};
    y={1,1};
    cout << (x*=3.0) << endl;
    cout << (x/=3.0) << endl;
    cout << (x-=1.0) << endl;
    cout << (x+=1.0) << endl;
    cout <<  (valer{1.3,0.3}^valer{1.1,0.5}) << endl;
    cout <<  (valer{0.,0.3}^valer{2.1,0.5}) << endl;

    valer a{2.0};
    valer b{3.0};
    valer c{1.0};
    //auto z  = ~y;
    valer<double>::type  zz;
    cout << "zz = " << zz << endl;

    valer a1 = {2.3,5.3};
    cout << valer{5.6,8.3} << endl;
    cout << valer{6,8} << endl;
    valer<const double &> a2(a1);
    valer<double> a3(a2);
  }
  {
    std::vector< valer<double> > v;
    for(int i=0;i<100;++i) {
      v.push_back({sqrt(i),sqrt(sqrt(i))});
    }
    for(auto & x : v) cout << x << endl;
  }
  {
    cout << "Test for referece and values" << endl;
    double value=2.0;
    double error = 0.5;
    valer<double & > v(value,error);
    valer<double> v2(1.5,0.3);
    cout << v+v2 << endl;
    cout << v2+v << endl;
    cout << v*v2 << endl;
    cout << v2*v << endl;
    cout << v*v << endl;
    cout << v/v2 << endl;
    cout << v2/v << endl;
  }
  {
    auto  fun_const_ref = [](const ibn::valer<double> & v) {
      std::cout << "fun_const_ref " << v << std::endl;
    };
    auto fun_const_ref_ref = [] (const ibn::valer<double> && v) {
      std::cout << "fun_const_ref_ref " << v << std::endl;
    };
    auto fun_const_cref = [] (const ibn::valer<const double> & v) {
      std::cout << "fun_const_cref " << v << std::endl;
    };
    valer<double> v(1.5,0.3);
    double value=2.0;
    double error = 0.5;
    valer<double & > vref(value,error);
    valer<const double & > vcref(value,error);
    fun_const_ref(v);
    fun_const_ref(vref);
    fun_const_ref(vcref);
    //fun_const_ref_ref(v);
    fun_const_ref_ref(vref);
    fun_const_ref_ref(vcref);
    fun_const_cref(v);
    fun_const_cref(vref);
    fun_const_cref(vcref);
  }

  return 0;
};

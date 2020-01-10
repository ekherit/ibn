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

  valer v(5);
  cout << v << endl;
  valer v2 = 23.3;
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

  {
    std::cout << "Test reference " << std::endl;
    double value=3.14;
    double error=2.72;
    valer <double &>  v{value,error};
    v.value *= 2;
    v.error *= 3;
    if( 3.14*2 == value && 2.72*3 == error) {
      cout << "value: 3.14  -> " << value << "  error: 2.72 -> " << error <<  " : OK " << endl ;
    }
  }
  {
    std::cout << "Test const reference " << std::endl;
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
    std::cout << "Test initialization from valer reference " << std::endl;
    double value=3.14;
    double error=2.72;
    valer <double &>  v(value,error);
    valer <double> r(v); //impicit conversion to double and then to valer  lost error
    cout << r << endl;
    if( 3.14 == r.value && 2.72 == r.error) {
      cout << "v.value: 3.14  -> " << value << "  v.error: 2.72 -> " << error <<  " : OK " << endl ;
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
    x={2,3};
    y={1,1};
    cout << (x*=3.0) << endl;
    cout << (x/=3.0) << endl;
    cout << (x-=1.0) << endl;
    cout << (x+=1.0) << endl;
    cout <<  (valer{1.3,0.3}^valer{1.1,0.5}) << endl;
    cout <<  (valer{0.,0.3}^valer{1.1,0.5}) << endl;
  }

  return 0;
};

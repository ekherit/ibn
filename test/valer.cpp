#include "../valer.h"

#include "../opt.h"

#include <iostream>
#include <type_traits>
#include <iomanip>
#include <typeinfo>
#include <limits>
#include <complex>
#include <cassert>
#include <sstream>

template<typename T>
std::ostream & operator << (std:: ostream & os,  const ibn::valer<T> & d) {
  os << d.value << " +- " << d.error;
  return os;
};

template<typename T>
std::istream & operator >> (std:: istream & is,  ibn::valer<T> & d) {
  is >> d.value >> d.error;
  return is;
};

template<typename T>
std::istream & operator >> (std:: istream & is,  ibn::valer<T&> && d) {
  is >> d.value >> d.error;
  return is;
};

template<typename T> using lim = std::numeric_limits<T>;

static int width=50;
static int check_number=0;


template<typename T1, typename T2>
bool is_equal(const T1 & t1, const T2 & t2) {
  using type = typename std::remove_cv< typename std::remove_reference<T2>::type >::type;
  return std::fabs(t1-t2) <= std::numeric_limits<type>::epsilon();
};

template <typename type_t>
void check_equality_and_print(std::string_view title, const ibn::valer<type_t> & v, typename std::remove_reference<type_t>::type const  & value, typename std::remove_reference<type_t>::type const    & error, std::string_view comment="") {
  using type = typename std::remove_reference<type_t>::type;
  //bool ok = (value == v.value) && (v.error == error);
  //bool ok = fabs(value - v.value) <= std::numeric_limits<type>::epsilon()  && fabs(value - v.value) <= std::numeric_limits<type>::epsilon()
  bool ok = is_equal(value, v.value) && is_equal(error, v.error);
  std::cout << std::setprecision(std::numeric_limits<type>::digits10+3);
  std::cout << std::setw(5) << check_number << std::setw(width) << title <<":"<<  std::setw(10) << ( ok ? "OK" : "FAIL" ) << std::setw(width)  << v <<  "  " << comment << std::endl;
  ++check_number;
}

//template <typename type_t, typename type= typename std::remove_reference<type_t>::type >
//void check_equality_and_print(std::string_view title, const ibn::valer<type_t> & v, const type  & value, const type  & error, std::string_view comment="") {
//  bool ok = (std::numeric_limits<type_t>::max() == v.value) && (v.error == error);
//  if constexpr ( std::is_reference<type_t>::value)  {
//    v.value=v.value/type(2);
//    v.error=v.error*type(2);
//    ok = ok && v.value == value && v.error == error;
//    std::cout << "reference" << std::endl;
//  }
//  std::cout << std::setprecision(std::numeric_limits<type_t>::digits10+3);
//  std::cout << std::setw(5) << check_number << std::setw(width) << title <<":"<<  std::setw(10) << ( ok ? "OK" : "FAIL" ) << std::setw(width)  << v <<  "  " << comment << std::endl;
//  ++check_number;
//}

template <typename type_t>
void check_constructor_from_sigle_literal(std::string_view title, std::string comment="") {}

template<>
void check_constructor_from_sigle_literal<char>(std::string_view title, std::string comment) {
  using type_t = char;
  ibn::valer <type_t> v(0x7f);
  check_equality_and_print(title, v, std::numeric_limits<type_t>::max(), type_t(0),comment);
}

template<>
void check_constructor_from_sigle_literal<int>(std::string_view title, std::string comment) {
  using type_t = int;
  ibn::valer <type_t> v(2147483647);
  check_equality_and_print(title, v, std::numeric_limits<type_t>::max(), type_t(0),comment);
}

template<>
void check_constructor_from_sigle_literal<long int>(std::string_view title, std::string comment) {
  using type_t = long int;
  ibn::valer <type_t> v(9223372036854775807);
  check_equality_and_print(title, v, std::numeric_limits<type_t>::max(), type_t(0),comment);
}

template<>
void check_constructor_from_sigle_literal<unsigned char>(std::string_view title, std::string comment) {
  using type_t = unsigned char;
  ibn::valer <type_t> v(0xff);
  check_equality_and_print(title, v, std::numeric_limits<type_t>::max(), type_t(0),comment);
}

template<>
void check_constructor_from_sigle_literal<unsigned int>(std::string_view title, std::string comment) {
  using type_t = unsigned int;
  ibn::valer <type_t> v(4294967295);
  check_equality_and_print(title, v, std::numeric_limits<type_t>::max(), type_t(0),comment);
}

template<>
void check_constructor_from_sigle_literal<unsigned long>(std::string_view title, std::string comment) {
  using type_t = unsigned long;
  ibn::valer <type_t> v(18446744073709551615U);
  check_equality_and_print(title, v, std::numeric_limits<type_t>::max(), type_t(0),comment);
}
template<>
void check_constructor_from_sigle_literal<float>(std::string_view title, std::string comment) {
  using type_t = float;
  ibn::valer <type_t> v(3.4028235e+38);
  check_equality_and_print(title, v, std::numeric_limits<type_t>::max(), type_t(0), comment);
}

template<>
void check_constructor_from_sigle_literal<double>(std::string_view title, std::string comment) {
  using type_t = double;
  ibn::valer <type_t> v(1.79769313486231571e+308);
  check_equality_and_print(title, v, std::numeric_limits<type_t>::max(), type_t(0), comment);
}

template<>
void check_constructor_from_sigle_literal<long double>(std::string_view title, std::string comment) {
  using type_t = long double;
  ibn::valer <type_t> v(1.18973149535723176502e+4932L);
  check_equality_and_print(title, v, std::numeric_limits<type_t>::max(), type_t(0), comment);
}


template <typename type_t>
void check_constructor_from_sigle_type(std::string_view title, std::string comment="") {
  type_t t{std::numeric_limits<type_t>::max()};
  ibn::valer <type_t> v(t);
  check_equality_and_print(title, v, std::numeric_limits<type_t>::max(), type_t(0), comment);
}

template <typename type_t>
void check_aggregate_from_sigle_type(std::string_view title, std::string comment="") {
  type_t t{std::numeric_limits<type_t>::max()};
  ibn::valer <type_t> v{t};
  check_equality_and_print(title, v, std::numeric_limits<type_t>::max(), type_t(0), comment);
}

/*
template <>
void check_constructor_from_sigle_type<std::complex<double>>(std::string_view title, std::string comment) {
  using type_t = std::complex<double>;
  type_t t(M_PI,M_E);
  ibn::valer <type_t> v(t);
  check_equality_and_print(title, v, {M_PI, M_E}, type_t(0), comment);
}
*/

template<typename type_t>
void check_aggregate_from_sigle_literal(std::string_view title, std::string comment="") {
  //using type_t = char;
  ibn::valer <type_t> v{0};
  check_equality_and_print(title, v, std::numeric_limits<type_t>::max(), type_t(0), comment);
}

template<>
void check_aggregate_from_sigle_literal<char>(std::string_view title, std::string comment) {
  using type_t = char;
  ibn::valer <type_t> v{0x7f};
  check_equality_and_print(title, v, std::numeric_limits<type_t>::max(), type_t(0), comment);
}

template<>
void check_aggregate_from_sigle_literal<int>(std::string_view title, std::string comment) {
  using type_t = int;
  ibn::valer <type_t> v{2147483647};
  check_equality_and_print(title, v, std::numeric_limits<type_t>::max(), type_t(0), comment);
}

template<>
void check_aggregate_from_sigle_literal<long int>(std::string_view title, std::string comment) {
  using type_t = long int;
  ibn::valer <type_t> v{9223372036854775807};
  check_equality_and_print(title, v, std::numeric_limits<type_t>::max(), type_t(0), comment);
}

template<>
void check_aggregate_from_sigle_literal<unsigned char>(std::string_view title, std::string comment) {
  using type_t = unsigned char;
  ibn::valer <type_t> v(0xff);
  check_equality_and_print(title, v, std::numeric_limits<type_t>::max(), type_t(0), comment);
}

template<>
void check_aggregate_from_sigle_literal<unsigned int>(std::string_view title, std::string comment) {
  using type_t = unsigned int;
  ibn::valer <type_t> v{4294967295};
  check_equality_and_print(title, v, std::numeric_limits<type_t>::max(), type_t(0), comment);
}

template<>
void check_aggregate_from_sigle_literal<unsigned long>(std::string_view title, std::string comment) {
  using type_t = unsigned long;
  ibn::valer <type_t> v{18446744073709551615U};
  check_equality_and_print(title, v, std::numeric_limits<type_t>::max(), type_t(0), comment);
}
template<>
void check_aggregate_from_sigle_literal<float>(std::string_view title, std::string comment) {
  using type_t = float;
  ibn::valer <type_t> v{3.4028235e+38};
  check_equality_and_print(title, v, std::numeric_limits<type_t>::max(), type_t(0), comment);
}

template<>
void check_aggregate_from_sigle_literal<double>(std::string_view title, std::string comment) {
  using type_t = double;
  ibn::valer <type_t> v{1.79769313486231571e+308};
  check_equality_and_print(title, v, std::numeric_limits<type_t>::max(), type_t(0), comment);
}

template<>
void check_aggregate_from_sigle_literal<long double>(std::string_view title, std::string comment) {
  using type_t = long double;
  ibn::valer <type_t> v{1.18973149535723176502e+4932L};
  check_equality_and_print(title, v, std::numeric_limits<type_t>::max(), type_t(0), comment);
}

template <typename type_t>
void check_constructor_from_type_type(std::string_view title, std::string comment="") {
  using type = typename std::remove_cv<typename std::remove_reference<type_t>::type>::type;
  type value{std::numeric_limits<type>::max()};
  type error{std::numeric_limits<type>::min()};
  ibn::valer <type_t> v(value,error);
  check_equality_and_print<type_t>(title, v, std::numeric_limits<type>::max(), std::numeric_limits<type>::min(), comment);
}

template <typename type_t>
void check_ref_constructor_from_type_type(std::string_view title, std::string comment="") {
  using type = typename std::remove_cv<typename std::remove_reference<type_t>::type>::type;
  type value{std::numeric_limits<type>::max()};
  type error{std::numeric_limits<type>::min()};
  ibn::valer <type_t> v(value,error);
  v.value/=1024.;
  v.error*=1024.;
  check_equality_and_print<type_t>(title, v, value, error, comment);
}


template <typename type_t>
void check_const_ref_constructor_from_type_type(std::string_view title, std::string comment="") {
  using type = typename std::remove_cv<typename std::remove_reference<type_t>::type>::type;
  type value{std::numeric_limits<type>::max()};
  type error{std::numeric_limits<type>::min()};
  ibn::valer <type_t> v(value,error);
  value/=1024.;
  error*=1024.;
  check_equality_and_print<type_t>(title, v, value, error, comment);
}


//template <typename type_t>
//void check_constructor_from_ref_ref(std::string_view title, std::string comment="") {
//  using type = typename std::remove_cv<typename std::remove_reference<type_t>::type>::type;
//  type value{std::numeric_limits<type>::max()};
//  type error{std::numeric_limits<type>::min()};
//  ibn::valer <type & > v(value,error);
//  check_equality_and_print<type_t>(title, v, std::numeric_limits<type>::max(), std::numeric_limits<type>::min(), comment);
//}


template<typename T1, typename T2> 
void check_sum(std::string_view title1, std::string_view title2, std::string comment="") {
  using type1 = typename ibn::valer<T1>::base_type;
  using type2 = typename ibn::valer<T2>::base_type;
  type1 val1 = 10;
  type1 err1 = 1;
  type1 val2 = 5;
  type1 err2 = 2;

  ibn::valer<T1> v1(val1,err1);
  ibn::valer<T2> v2(val2,err2);
  //std::cout << v1 << " " << v2 << std::endl;

  auto value = val1+val2;
  auto error = std::hypot(err1,err2);

  std::ostringstream iss;
  iss << std::setw(23) << std::right << title1;
  iss << " + ";
  iss << std::setw(23) << std::left  << title2;

  auto result = v1+v2;
  check_equality_and_print(iss.str(), result, value, error, comment);
}

template<typename T1, typename T2> 
void check_sub(std::string_view title1, std::string_view title2, std::string comment="") {
  using type1 = typename ibn::valer<T1>::base_type;
  using type2 = typename ibn::valer<T2>::base_type;
  type1 val1 = 10;
  type1 err1 = 1;
  type1 val2 = 5;
  type1 err2 = 2;

  ibn::valer<T1> v1(val1,err1);
  ibn::valer<T2> v2(val2,err2);
  //std::cout << v1 << " " << v2 << std::endl;

  auto value = val1-val2;
  auto error = std::hypot(err1,err2);

  std::ostringstream iss;
  iss  << std::setw(23) << std::right << title1;
  iss << " - ";
  iss  << std::setw(23) << std::left<< title2;

  auto result = v1-v2;
  check_equality_and_print(iss.str(), result, value, error, comment);
}

template<typename T1, typename T2> 
void check_mult(std::string_view title1, std::string_view title2, std::string comment="") {
  using type1 = typename ibn::valer<T1>::base_type;
  using type2 = typename ibn::valer<T2>::base_type;
  type1 val1 = 10;
  type1 err1 = 1;
  type1 val2 = 5;
  type1 err2 = 2;

  ibn::valer<T1> v1(val1,err1);
  ibn::valer<T2> v2(val2,err2);
  //std::cout << v1 << " " << v2 << std::endl;

  auto value = val1*val2;
  auto error = std::hypot(val1*err2, val2*err1);

  std::ostringstream iss;
  iss  << std::setw(23) << std::right << title1;
  iss << " * ";
  iss  << std::setw(23) << std::left<< title2;

  auto result = v1*v2;
  check_equality_and_print(iss.str(), result, value, error, comment);
}

template<typename T1, typename T2> 
void check_div(std::string_view title1, std::string_view title2, std::string comment="") {
  using type1 = typename ibn::valer<T1>::base_type;
  using type2 = typename ibn::valer<T2>::base_type;
  type1 val1 = 10;
  type1 err1 = 1;
  type1 val2 = 5;
  type1 err2 = 2;

  ibn::valer<T1> v1(val1,err1);
  ibn::valer<T2> v2(val2,err2);
  //std::cout << v1 << " " << v2 << std::endl;

  auto value = val1/val2;
  auto error = value*std::hypot( err1/val1, err2/val2);

  std::ostringstream iss;
  iss  << std::setw(23) << std::right << title1;
  iss << " / ";
  iss  << std::setw(23) << std::left<< title2;

  auto result = v1/v2;
  check_equality_and_print(iss.str(), result, value, error, comment);
}

template<typename T> 
void check_sum_with_literal(std::string_view title1, std::string_view title2, std::string comment="") {
  using type1 = typename ibn::valer<T>::base_type;
  type1 val1 = 10;
  type1 err1 = 1;
  ibn::valer<T> v1(val1,err1);

  auto value = val1+3.1415;
  auto error = err1;

  std::ostringstream iss;
  iss << std::setw(23) << std::right << title1;
  iss << " + ";
  iss << std::setw(23) << std::left  << title2;

  auto result = v1 + 3.1415;
  check_equality_and_print(iss.str(), result, value, error, comment);
  //iss.clear();

  std::ostringstream iss2;
  iss2 << std::setw(23) << std::right << title2;
  iss2 << " + ";
  iss2 << std::setw(23) << std::left  << title1;
  auto result2 = 3.1415 + v1;
  check_equality_and_print(iss2.str(), result2, value, error, comment);
}

template<typename T> 
void check_sub_with_literal(std::string_view title1, std::string_view title2, std::string comment="") {
  using type1 = typename ibn::valer<T>::base_type;
  type1 val1 = 10;
  type1 err1 = 1;
  ibn::valer<T> v1(val1,err1);

  auto value = val1-3.1415;
  auto error = err1;

  std::ostringstream iss;
  iss << std::setw(23) << std::right << title1;
  iss << " - ";
  iss << std::setw(23) << std::left  << title2;

  auto result = v1 - 3.1415;
  check_equality_and_print(iss.str(), result, value, error, comment);
  //iss.clear();

  std::ostringstream iss2;
  iss2 << std::setw(23) << std::right << title2;
  iss2 << " - ";
  iss2 << std::setw(23) << std::left  << title1;
  value = 3.1415  - val1;
  auto result2 = 3.1415 - v1;
  check_equality_and_print(iss2.str(), result2, value, error, comment);
}

template<typename T> 
void check_mult_with_literal(std::string_view title1, std::string_view title2, std::string comment="") {
  using type1 = typename ibn::valer<T>::base_type;
  type1 val1 = 10;
  type1 err1 = 1;
  ibn::valer<T> v1(val1,err1);

  auto value = val1*3.1415;
  auto error = err1*3.1415;

  std::ostringstream iss;
  iss << std::setw(23) << std::right << title1;
  iss << " * ";
  iss << std::setw(23) << std::left  << title2;

  auto result = v1 * 3.1415;
  check_equality_and_print(iss.str(), result, value, error, comment);
  //iss.clear();

  std::ostringstream iss2;
  iss2 << std::setw(23) << std::right << title2;
  iss2 << " * ";
  iss2 << std::setw(23) << std::left  << title1;
  auto result2 = 3.1415 * v1;
  check_equality_and_print(iss2.str(), result2, value, error, comment);
}



//template <typename type_t>
//void check_cmp(std::string_view title1, std::string_view title2, std::string_view oper,   std::string_view comment="") {
//  using type = typename std::remove_reference<type_t>::type;
//  //bool ok = (value == v.value) && (v.error == error);
//  //bool ok = fabs(value - v.value) <= std::numeric_limits<type>::epsilon()  && fabs(value - v.value) <= std::numeric_limits<type>::epsilon()
//  bool ok = is_equal(value, v.value) && is_equal(error, v.error);
//  std::cout << std::setprecision(std::numeric_limits<type>::digits10+3);
//  std::cout << std::setw(5) << check_number << std::setw(width) << title <<":"<<  std::setw(10) << ( ok ? "OK" : "FAIL" ) << std::setw(width)  << v <<  "  " << comment << std::endl;
//  ++check_number;
//}

//template<typename T1, typename T2> 
//void check_comparison(const ibn::valer<T1> &v1, const ibn::valer<T2> &v2, std::string_view title1, std::string_view title2, std::string comment="") {
//  std::
//}




template<typename T> 
void check_div_with_literal(std::string_view title1, std::string_view title2, std::string comment="") {
  using type1 = typename ibn::valer<T>::base_type;
  type1 val1 = 10;
  type1 err1 = 6.3;
  ibn::valer<T> v1(val1,err1);

  auto value = val1/3.1415;
  auto error = err1/3.1415;

  std::ostringstream iss;
  iss << std::setw(23) << std::right << title1;
  iss << " / ";
  iss << std::setw(23) << std::left  << title2;

  auto result = v1 / 3.1415;
  check_equality_and_print(iss.str(), result, value, error, comment);
  //iss.clear();

  std::ostringstream iss2;
  iss2 << std::setw(23) << std::right << title2;
  iss2 << " / ";
  iss2 << std::setw(23) << std::left  << title1;
  value = 3.1415/val1;
  error = value*err1/val1;
  //std::cout << error << std::endl;
  auto result2 = 3.1415/v1;
  check_equality_and_print(iss2.str(), result2, value, error, comment);
}

void check_increment_decrement(std::string_view title, std::string_view comment="") {
  double value = 1.1;
  double error = 2.2;
  ibn::valer<double> v{value,error};

  ++v;
  std::string s = "++" + std::string(title);
  check_equality_and_print(s, v, value+1,error,comment);

  --v;
  s = "--" + std::string(title);
  check_equality_and_print(s, v, value,error,comment);

  v++;
  s = std::string(title)+"++";
  check_equality_and_print(s, v, value+1,error,comment);

  v--;
  s = std::string(title)+"--";
  check_equality_and_print(s, v, value,error,comment);
}

void check_operator_minus(std::string_view title, std::string_view comment) {
  double value = 1.1;
  double error = 2.2;
  ibn::valer<double> v{value,error};
  check_equality_and_print("-valer<dobule>", -v, -value,error,comment);

  ibn::valer<double&> v2{value,error};
  check_equality_and_print("-valer<dobule&>", -v, -value,error,comment);

  ibn::valer<const double&> v3{value,error};
  check_equality_and_print("-valer<const dobule&>", -v, -value, error,comment);
}

int main(int argc, char ** argv) {
  using namespace ibn;
  using namespace std;

  //cout << "Testing constructor from simple type " << endl;
  check_constructor_from_sigle_literal<char>                  ("valer<char>(literal char)");
  check_constructor_from_sigle_literal<int>                   ("valer<int>(literal int)");
  check_constructor_from_sigle_literal<long>                  ("valer<long>(literal long)");
  check_constructor_from_sigle_literal<unsigned>              ("valer<unsinged>(literal unsigned)");
  check_constructor_from_sigle_literal<unsigned long>         ("valer<unsinged long>(literal unsigned long)");
  check_constructor_from_sigle_literal<float>                 ("valer<float>(literal float)");
  check_constructor_from_sigle_literal<double>                ("valer<double>(literal double)");
  check_constructor_from_sigle_literal<long double>           ("valer<long double>(literal long double)");
  //check_constructor_from_sigle_literal<std::complex<double>>  ("valer<complex<double>>(complex<double>)");

  check_constructor_from_sigle_type<char>                  ("valer<char>(char)");
  check_constructor_from_sigle_type<int>                   ("valer<int>(int)");
  check_constructor_from_sigle_type<long>                  ("valer<long>(long)");
  check_constructor_from_sigle_type<unsigned>              ("valer<unsinged>(unsigned)");
  check_constructor_from_sigle_type<unsigned long>         ("valer<unsinged long>(unsigned long)");
  check_constructor_from_sigle_type<float>                 ("valer<float>(float)");
  check_constructor_from_sigle_type<double>                ("valer<double>(double)");
  check_constructor_from_sigle_type<long double>           ("valer<long double>(long double)");
  //check_constructor_from_sigle_type<std::complex<double>>  ("valer<complex<double>>(complex<double>)");


  cout << "Testing aggregate initializer from literal" << endl;

  check_aggregate_from_sigle_literal<char>                  ("valer<char>{literal char}");
  check_aggregate_from_sigle_literal<int>                   ("valer<int>{literal int}");
  check_aggregate_from_sigle_literal<long>                  ("valer<long>{literal long}");
  check_aggregate_from_sigle_literal<unsigned>              ("valer<unsinged>{literal unsigned}");
  check_aggregate_from_sigle_literal<unsigned long>         ("valer<unsinged long>{literal unsigned long}");
  check_aggregate_from_sigle_literal<float>                 ("valer<float>{literal float}");
  check_aggregate_from_sigle_literal<double>                ("valer<double>{literal double}");
  check_aggregate_from_sigle_literal<long double>           ("valer<long double>{literal long double}");
  //check_aggregate_from_sigle_literal<std::complex<double>>  ("valer<complex<double>>{complex<double>}");

  check_aggregate_from_sigle_type<char>                  ("valer<char>(char)");
  check_aggregate_from_sigle_type<int>                   ("valer<int>(int)");
  check_aggregate_from_sigle_type<long>                  ("valer<long>(long)");
  check_aggregate_from_sigle_type<unsigned>              ("valer<unsinged>(unsigned)");
  check_aggregate_from_sigle_type<unsigned long>         ("valer<unsinged long>(unsigned long)");
  check_aggregate_from_sigle_type<float>                 ("valer<float>(float)");
  check_aggregate_from_sigle_type<double>                ("valer<double>(double)");
  check_aggregate_from_sigle_type<long double>           ("valer<long double>(long double)");
  //check_aggregate_from_sigle_type<std::complex<double>>  ("valer<complex<double>>(complex<double>)");

  //cout << "Testing constructor from (double, double)" << std::endl;
  check_constructor_from_type_type<double>  ("valer<double>(double,double)");
  check_ref_constructor_from_type_type<double&> ("valer<double&>(double,double)");
  check_const_ref_constructor_from_type_type<const double&> ("valer<const double&>(double,double)");

  check_sum<double,double>                         ("valer <dobule>", "valer <double>");
  check_sum<double,double&>                        ("valer <dobule>", "valer <double &>");
  check_sum<double&,double>                      ("valer <dobule &>", "valer <double>");
  check_sum<double&,double&>                     ("valer <dobule &>", "valer <double &>");
  check_sum<double,double const&>                  ("valer <dobule>", "valer <double const &>");
  check_sum<double const &,double>         ("valer <dobule const &>", "valer <double>");
  check_sum<double const &,double const &> ("valer <dobule const &>", "valer <double const &>");
  check_sum<double&,double const&>               ("valer <dobule &>", "valer <double const &>");
  check_sum<double const &,double&>        ("valer <dobule const &>", "valer <double &>");


  check_sub<double,double>                         ("valer <dobule>", "valer<double>");
  check_sub<double,double&>                        ("valer <dobule>", "valer<double &>");
  check_sub<double&,double>                      ("valer <dobule &>", "valer<double>");
  check_sub<double&,double&>                     ("valer <dobule &>", "valer<double &>");
  check_sub<double,double const&>                  ("valer <dobule>", "valer<double const &>");
  check_sub<double const &,double>         ("valer <dobule const &>", "valer<double>");
  check_sub<double const &,double const &> ("valer <dobule const &>", "valer<double const &>");
  check_sub<double&,double const&>               ("valer <dobule &>", "valer<double const &>");
  check_sub<double const &,double&>        ("valer <dobule const &>", "valer<double &>");


  check_mult<double,double>                        ("valer <dobule>", "valer <double>");
  check_mult<double,double&>                       ("valer <dobule>", "valer <double &>");
  check_mult<double&,double>                     ("valer <dobule &>", "valer <double>");
  check_mult<double&,double&>                    ("valer <dobule &>", "valer <double &>");
  check_mult<double,double const&>                 ("valer <dobule>", "valer <double const &>");
  check_mult<double const &,double>        ("valer <dobule const &>", "valer <double>");
  check_mult<double const &,double const &>("valer <dobule const &>", "valer <double const &>");
  check_mult<double&,double const&>              ("valer <dobule &>", "valer <double const &>");
  check_mult<double const &,double&>       ("valer <dobule const &>", "valer <double &>");


  check_div<double,double>                         ("valer <dobule>","valer <double>");
  check_div<double,double&>                        ("valer <dobule>","valer <double &>");
  check_div<double&,double>                      ("valer <dobule &>","valer <double>");
  check_div<double&,double&>                     ("valer <dobule &>","valer <double &>");
  check_div<double,double const&>                  ("valer <dobule>","valer <double const &>");
  check_div<double const &,double>         ("valer <dobule const &>","valer <double>");
  check_div<double const &,double const &> ("valer <dobule const &>","valer <double const &>");
  check_div<double&,double const&>               ("valer <dobule &>","valer <double const &>");
  check_div<double const &,double&>        ("valer <dobule const &>","valer <double &>");


  check_sum_with_literal<double>                   ("valer <dobule>", "literal double");
  check_sub_with_literal<double>                   ("valer <dobule>", "literal double");
  check_mult_with_literal<double>                  ("valer <dobule>", "literal double");
  check_div_with_literal<double>                   ("valer <dobule>", "literal double");

  check_increment_decrement               ("valer<double>");
  check_operator_minus("","");


  std::cout << "Test implicit conversion: " << std::endl;
  {
    cout << " to double: ";
    using type_t  = double;
    constexpr type_t t = M_PI;
    valer<type_t> v{t,t};
    //static_assert( type_t(t) == v.value &&  v.error == type_t(t) );
    //cout << setprecision(lim<type_t>::digits10) << v << ":  OK " << endl;
    type_t result = v;
    assert( t == result );
    cout << result << " OK " << endl;
  }

  std::cout << "Test implicit conversion: " << std::endl;
  {
    cout << " to complex<float>: ";
    using type_t  = complex<float>;
    cout << setprecision(lim<float>::digits10);
    constexpr type_t t = {M_PI, M_E};
    valer<type_t> v{t,t};
    type_t result = v;
    assert( t == result );
    cout << v << "  -> " << result << " OK " << endl;
  }

  std::cout << "Test reference initialization: " << endl;
  {
    std::cout << "  from {double, double} " << std::endl;
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
    std::cout << "  from {double&, double&} " << std::endl;
    double value=3.14;
    double error=2.72;
    double &val =value;
    double &err = error;
    valer <double &>  v{val,err};
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
    cout << x*3.0 << endl;
    cout << x+2. << endl;
    cout << 2.+x << endl;
    cout << x-2. << endl;
    cout << 2.-x << endl;
    x={2.,13.3};
    y={1,1};
    cout << (x*=3.0) << endl;
    cout << (x/=3.0) << endl;
    cout << (x-=1.0) << endl;
    cout << (x+=1.0) << endl;
    //cout <<  (valer{1.3,0.3}^valer{1.1,0.5}) << endl;
    //cout <<  (valer{0.,0.3}^valer{2.1,0.5}) << endl;

    valer a{2.0};
    valer b{3.0};
    valer c{1.0};
    //auto z  = ~y;
    valer<double>::base_type  zz;
    cout << "zz = " << zz << endl;

    valer a1 = {2.3,5.3};
    cout << valer{5.6,8.3} << endl;
    cout << valer{6,8} << endl;
    valer<const double &> a2(a1);
    valer<double> a3(a2);
    std::cout <<  (a<b) << std::endl;
    std::cout <<  (b<a) << std::endl;
    std::cout <<  (a<=b) << std::endl;
    std::cout <<  (b<=a) << std::endl;
    std::cout <<  (a==b) << std::endl;
    std::cout <<  (a>b) << std::endl;
    std::cout <<  (b>a) << std::endl;
    std::cout <<  (a>=b) << std::endl;
    std::cout <<  (b>=a) << std::endl;
  }
  //{
  //  std::vector< valer<double> > v;
  //  for(int i=0;i<100;++i) {
  //    v.push_back({sqrt(i),sqrt(sqrt(i))});
  //  }
  //  for(auto & x : v) cout << x << endl;
  //}
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
    const valer<double &> vv(value,error);
    valer<double> zz;
    //zz = vv;
    //cout << zz << endl;
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

  valer v0 = {2,3};
  double x,y;
  valer<double> v1 = {x, y};
  valer<double&> v2 = {x, y};
  valer<const double&> v3 = {x, y};
  const valer<double> v4 = {x, y};
  const valer<double&> v5 = {x, y};
  const valer<const double&> v6 = {x, y};
  valer<double> v7{x,y};

  cout << "Check constructors: " << endl;
  {
    cout << " from (number leteral)" << endl;
    valer<double> v1(3.14);
    valer<const double> v2(3.14);
    const valer<double> v3(3.14);
    const valer<const double> v4(3.14);
  }
  {
    cout << " from (number leteral , number literal)" << endl;
    valer<double> v1(3.14,2.72);
    valer<const double> v2(3.14,2.72);
  }
  {
    cout << " from (value variable, value variable)" << endl;
    double x = 3.1;
    double y = 2.7;
    valer<double> v1(x,y);
    valer<const double> v2(x,y);
    const valer<double> v3(x,y);
    valer<double&> v4(x,y);
    valer<const double&> v5(x,y);
    const valer<double&> v6(x,y);
    const valer<const double&> v7(x,y);
  }
  {
    cout << " from valer<double> " << endl;
    valer<double> v0(3.1,2.7);
    valer<double> v1(v0);
    valer<const double> v2(v0);
    const valer<double> v3(v0);
    valer<double&> v4(v0);
    valer<const double&> v5(v0);
    const valer<double&> v6(v0);
    const valer<const double&> v7(v0);
  }

  {
    cout << " from valer<double&> " << endl;
    double x = 3.1;
    double y = 2.7;
    valer<double&> v0(x,y);
    valer<double> v1(v0);
    valer<const double> v2(v0);
    const valer<double> v3(v0);
    valer<double&> v4(v0);
    valer<const double&> v5(v0);
    const valer<double&> v6(v0);
    const valer<const double&> v7(v0);
  }

  {
    cout << " from valer<const double&> " << endl;
    double x = 3.1;
    double y = 2.7;
    valer<const double&> v0(3.1,2.7);
    valer<double> v1(v0);
    valer<const double> v2(v0);
    const valer<double> v3(v0);
    //valer<double&> v4(v0);
    valer<const double&> v5(v0);
    //const valer<double&> v6(v0);
    const valer<const double&> v7(v0);

  }
  //{
  //  valer<double> v;
  //  cin >> v ;
  //  cout << v << endl;

  //  double x{3.14},y{2.14};
  //  valer<double &> v2(x,y);
  //  cin >> v2;
  //  cout << v2 << endl;
  //  cout << " x = " <<  x << " y=" << y << endl;

  //  std::istringstream iss("128.6 128.2");
  //  iss >> v2;
  //  cout << v2 << endl;
  //  auto f = [](double &x, double &y) { return ibn::valer<double&>(x,y); };
  //  iss >> f(x,y) ;
  //}
  //{
  //  ibn::valer v = {23.,42.};
  //  double x=1;
  //  double y =2;
  //  auto f = [](double &x, double &y) ->  ibn::valer<double&> { return ibn::valer<double&>(x,y); };
  //  auto v2 = v - f(x,y);
  //  cout << v2 << endl;
  //  auto g = [](const ibn::valer<double> & v1, const ibn::valer<double> & v2) ->ibn::valer<double> {
  //    return v1+v2;;
  //  };

  //  double &u = x;
  //  auto z = g(u,f(x,y));
  //  cout << z << endl;

  //}
  {
    valer v1{2.3,1.0};
    valer v2{3,1};
    cout << (v1 <= valer{3.0,2.3} ) << endl;
    cout << 2+v1 << endl;
    cout << 2*v1 << endl;
    cout << v1*3 << endl;
    cout << v2*3 << endl;
    cout << 3*v2 << endl;
    cout << v2*3. << endl;
    cout << 3.3*v2 << endl;
    cout << v1 << endl;
  }
  {
    double v=3.14;
    double e=0.1; 
    //valer v1{&v,&e};
  }

  return 0;
};

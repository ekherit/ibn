#pragma once
#include <memory>

namespace ibn
{

template <typename T> 
struct raw_pointer {
  using type = T*;
  type pointer;
  constexpr raw_pointer(T * ptr) : pointer(ptr) {}
  constexpr type operator()(T * ptr) const { return ptr; }
  constexpr T & operator*() { return *pointer; }
  constexpr const T & operator*() const { return *pointer; }
};

template<typename T>
struct raw_pointer< std::shared_ptr<T> > : public raw_pointer<T> {
  constexpr raw_pointer(const std::shared_ptr<T>  & ptr) : raw_pointer<T>(ptr.get()) {}
  constexpr raw_pointer(std::shared_ptr<T>  && ptr) : raw_pointer<T>(ptr.get()) {}
};

template<typename T>
struct raw_pointer< std::unique_ptr<T> > : public raw_pointer<T> {
  constexpr raw_pointer(const std::unique_ptr<T>  & ptr) : raw_pointer<T>(ptr.get()) {}
  constexpr raw_pointer(std::unique_ptr<T>  && ptr)      : raw_pointer<T>(ptr.get()) {}
};

//template<typename T>
//struct raw_pointer< std::weak_ptr<T> > : public raw_pointer<T> {
//  constexpr raw_pointer(const std::weak_ptr<T>  & ptr) : raw_pointer<T>(ptr.get()) {}
//  constexpr raw_pointer(std::weak_ptr<T>  && ptr)      : raw_pointer<T>(ptr.get()) {}
//};

template<typename T> raw_pointer(std::shared_ptr<T>) -> raw_pointer<std::shared_ptr<T>>;
template<typename T> raw_pointer(const std::unique_ptr<T> &) -> raw_pointer<std::unique_ptr<T>>;
//template<typename T> raw_pointer(std::weak_ptr<T>) -> raw_pointer<std::weak_ptr<T>>;

template<typename T>
using raw_pointer_t = typename raw_pointer<T>::type;

}

#include "../pointer.h"
#include <iostream>
#include <string>
#include <iomanip>

using namespace std;
template<typename RawPointer>
bool test(std::string message, RawPointer && ptr, const int n) {
  cout << setw(30) <<  message;
  if(n == *ptr) {
    cout << setw(5) << *ptr << setw(10) << "OK\n";
    return true;
  } else { cout << setw(15) << "FAIL\n"; }
  return false;
}

int main() {
    cout << "Testing pointer.h\n";
    bool ok = true;


    int * rptr = new int(42);
    ok = ok &&  test("int* -> int*: ", ibn::raw_pointer(rptr), 42);


    auto sptr = make_shared<int>(23);
    ok = ok && test("shared_ptr<int> -> int*: ", ibn::raw_pointer(sptr), 23);

    auto uptr = make_unique<int>(17);
    ok = ok && test("unique_ptr<int> -> int*: ", ibn::raw_pointer(uptr), 17);

   // weak_ptr<int> wptr(sptr);
   // ok = ok && test("weak_ptr<int> -> int*: ", ibn::raw_pointer(wptr), 23);
    return ok;
}

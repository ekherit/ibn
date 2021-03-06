#include "../opt.h"
#include <iostream>

int main(int argc, char ** argv) {
  ibn::opt2 opt(argc, argv);
  opt.print();
  if ( opt("--help") ) {
    std::cout << "--help option: " << std::endl;
  }
  if ( opt["--help"] ) {
    std::cout << "brased --help option: " << std::endl;
  }
  std::cout << opt.get<double>("--double",3.14) << std::endl;
  std::cout << opt.get<int>("--int",3) << std::endl;
  std::cout << opt.get<double>("--double2") << std::endl;
  int i;
  double d;
  opt.get("-i",&i);
  opt.get("-d",&d);
  std::cout <<  i << "  " << d << std::endl;
  for(int i=0;i<argc;++i) {
    std::cout << opt[i] << std::endl;
  }
  //double e;
  //if(opt("-e", e) ){
  //  std::cout << "is = " << e << std::endl;
  //}
}

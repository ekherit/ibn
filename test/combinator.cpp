/*
 * =====================================================================================
 *
 *       Filename:  combinator
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  15.12.2018 19:01:40
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan B. Nikolaev (ekherit), I.B.Nikolaev@inp.nsk.su
 *   Organization:  Budker Insitute of Nuclear Physics
 *
 * =====================================================================================
 */
#include <iostream>
#include <cmath>
#include "../combinator.h"

#include <cassert>

#include <algorithm>


#include <map>
inline void test_make_unique_pairs(int NA, int NB)
{
  std::vector<int>  A={1};
  std::vector<char> B={'a'};
  for( int i=1;i<NA;++i) A.push_back(i+1);
  for( int i=1;i<NB;++i) B.push_back('a'+i);
  ibn::print_array("A",A);
  ibn::print_array("B",B);
  std::vector< std::vector < std::pair<int, char> > > R;
  ibn::make_unique_pairs(A.begin(), A.end(), B.begin(), B.end(), R);
  std::map< decltype(R)::value_type, long > M;
  for(auto & r : R) {
    M[r]++;
  }
  std::cout << "The result of algorithm: " << R.size() << " combinations:" << std::endl;
  ibn::print(R);
  auto dublet=find_if(M.begin(),M.end(), [](auto & m){return m.second>1;});
  while(dublet!=M.end())
  {
    std::cout << "Find dubpicated combination: ";  
    ibn::print_pairs(dublet->first);
    std::cout << " with " << dublet->second << " entries\n";
    dublet=find_if(dublet,M.end(), [](auto & m){return m.second>1;});
  }
}

template<class Integer>
auto fac(Integer N)  -> Integer
{
  if(N == 0 || N == 1) return Integer{1};
  return N*fac(N-1);
};

inline void test_make_unique_pairs(int N=2)
{
  using namespace std;
  std::vector<int>  A={1};
  for( int i=1;i<N;++i) A.push_back(i+1);
  ibn::print_array("array = ",A);
  auto Aorig = A;
  std::vector< std::vector < std::pair<int, int> > > R;
  ibn::make_unique_pairs(A.begin(),A.end(), R);
  std::map< decltype(R)::value_type, long > M;
  for(auto & r : R)
  {
    M[r]++;
  }
  std::cout << "The result of algorithm: " << R.size() << " combinations:" << std::endl;
  ibn::print(R);
  auto dublet=find_if(M.begin(),M.end(), [](auto & m){return m.second>1;});
  while(dublet!=M.end())
  {
    std::cout << "Find dubpicated combination: ";  
    ibn::print_pairs(dublet->first);
    std::cout << " with " << dublet->second << " entries\n";
    dublet=find_if(dublet,M.end(), [](auto & m){return m.second>1;});
  }
  int Npairs = A.size()/2;
  std::cout << "number of pairs: " << Npairs <<  " - ";
  for( auto & combination : R)
    assert(Npairs ==  combination.size() );
  std::cout << "OK\n";
  long n = (A.size() % 2 == 0) ? Npairs : Npairs+1;
  //std::cout << "n = " << n;
  //std::cout << "  fac(2n) = " << fac(2*n);
  //std::cout << "  pow(2,n) = " << pow(2,n);
  //std::cout << "   fac(n) = " << fac(n);
  //std::cout << "\n";
  long Nexp =  fac(2*n)/(pow(2,n)*fac(n));
  std::cout << "number of combinations: " << Nexp << " - " ;
  assert(Nexp==R.size());
  std::cout << "OK\n";
  std::cout << "Modification of initial array: ";
  auto it = A.begin();
  auto it_orig = Aorig.begin();
  bool same=Aorig.size() == A.size();
  while(it!=A.end() && it_orig!=Aorig.end() ) same&=*it++ == *it_orig++;
  std::cout << "NONE\n";
  std::cout<< std::flush;
}


using namespace std;
int main(int argc, char ** argv  )
{
  int NA=5, NB=3;
  if(argc==2)
  {
    NA = std::stoi(argv[1]);
  }
  if(argc==3)
  {
    NA = std::stoi(argv[1]);
    NB = std::stoi(argv[2]);
  }
  std::cout << "Test make_unique_pairs from two arrays with size: " << NA << " and " << NB << ": \n";
  test_make_unique_pairs(NA, NB);
  std::cout << "Test make_unique_pairs from one array with size: " << NA << ": \n";
  test_make_unique_pairs(NA);
}

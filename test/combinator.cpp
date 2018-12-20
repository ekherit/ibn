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
template<class Integer>
auto fac(Integer N)  -> Integer
{
  if(N == 0 || N == 1) return Integer{1};
  return N*fac(N-1);
};

auto check_modif = [](const auto & A1, const auto &A2) -> bool
{
  auto it1 = A1.begin();
  auto it2 = A2.begin();
  bool same = A1.size() == A2.size();
  if(!same) return false;
  while(it1 !=A1.end() && it2 != A2.end()) 
  {
    same &= *it1++ == *it2++;
    if(!same) return false;
  }
  return same;
};

template<typename CombinationList>
auto check_duplets(const  CombinationList & R) ->   std::map< typename CombinationList::value_type, long >
{
  std::map< typename CombinationList::value_type, long > M;
  //count combinations
  for(const auto  & r : R) M[r]++;
  //remove single, keep only dulicates
  for(auto it = M.begin();it!=M.end();)
  {
    if(it->second == 1) 
      it = M.erase(it);
    else
      ++it;
  }
  return M;
};

inline void test_make_unique_pairs(int NA, int NB)
{
  if(NA<NB) 
  {
    std::cerr << "It must be NA >= NB, but now have: " << NA << " < " << NB << "\n";
    std::cerr << "Do nothing";
    return;
  };
  std::vector<int>  A={1};
  std::vector<char> B={'a'};
  for( int i=1;i<NA;++i) A.push_back(i+1);
  for( int i=1;i<NB;++i) B.push_back('a'+i);
  ibn::print_array("A",A);
  ibn::print_array("B",B);
  auto Aorig = A;
  auto Borig = B;
  std::vector< std::vector < std::pair<int, char> > > R;
  ibn::make_unique_pairs(A.begin(), A.end(), B.begin(), B.end(), R);

  std::cout << "The result of algorithm: " << R.size() << " combinations:" << std::endl;
  ibn::print(R);

  /* Check number of pairs */
  long Npairs = NB;
  std::cout << "number of pairs: " << Npairs <<  " - ";
  for( auto & combination : R) assert(Npairs ==  combination.size() );
  std::cout << "OK\n";

  /* Check number of combinations */
  long Ncomb = fac(NA)/fac(NA-NB);
  std::cout << "number of combinations: " << Ncomb << " - " ;
  assert(Ncomb==R.size());
  std::cout << "OK\n";


  /* Check duplicates */
  std::cout << "Looking for duplicates: ";
  auto M = check_duplets(R);
  if(M.size() != 0) 
  {
    std::cout << "... find duplicates: \n" ;
    for(auto & m : M)
    {
      ibn::print_pairs(m.first);
      std::cout << " : count = " <<  m.second  << "\n";
    }
  }
  assert(M.size() == 0);
  std::cout << "NONE\n";

  std::cout << "Modification of initial array A: ";
  check_modif(Aorig,A);
  std::cout << "NONE\n";

  std::cout << "Modification of initial array B: ";
  check_modif(Borig,B);
  std::cout << "NONE\n";
  std::cout<< std::flush;

}


inline void test_make_unique_pairs(int N=2)
{
  using namespace std;
  std::vector<int>  A={1};
  for( int i=1;i<N;++i) A.push_back(i+1);
  ibn::print_array("array = ",A);
  auto Aorig = A;
  std::vector< std::vector < std::pair<int, int> > > R;
  ibn::make_unique_pairs(A.begin(),A.end(), R);
  std::cout << "The result of algorithm: " << R.size() << " combinations:" << std::endl;
  ibn::print(R);

  /* check number of pairs */
  int Npairs = A.size()/2;
  std::cout << "number of pairs: " << Npairs <<  " - ";
  for( auto & combination : R)
    assert(Npairs ==  combination.size() );
  std::cout << "OK\n";
  long n = (A.size() % 2 == 0) ? Npairs : Npairs+1;

  /* check number of combinations */
  long Nexp =  fac(2*n)/(pow(2,n)*fac(n));
  std::cout << "number of combinations: " << Nexp << " - " ;
  assert(Nexp==R.size());
  std::cout << "OK\n";

  /* Check duplicates */
  std::cout << "Looking for duplicates: ";
  auto M = check_duplets(R);
  if(M.size() != 0) 
  {
    std::cout << "... find duplicates: \n" ;
    for(auto & m : M)
    {
      ibn::print_pairs(m.first);
      std::cout << " : count = " <<  m.second  << "\n";
    }
  }
  assert(M.size() == 0);
  std::cout << "NONE\n";

  /* Check array modification */
  std::cout << "Modification of initial array: ";
  check_modif(Aorig,A);
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

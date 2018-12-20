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
#include "../combinator.h"



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

inline void test_make_unique_pairs_old(int N=2)
{
  std::vector<int>  A={1};
  for( int i=1;i<N;++i) A.push_back(i+1);
  ibn::print_array("array = ",A);
  std::vector< std::vector < std::pair<int, int> > > R;
  ibn::make_unique_pairs(A.begin(), A.end(), R);

  std::map< decltype(R)::value_type, long > M;
  for(auto & r : R) {
    M[r]++;
  }
  std::cout << "The result of algorithm: " << R.size() << " combinations:" << std::endl;
  ibn::print(R);
  //auto dublet=find_if(M.begin(),M.end(), [](auto & m){return m.second>1;});
  //while(dublet!=M.end())
  //{
  //  std::cout << "Find dubpicated combination: ";  
  //  ibn::print_pairs(dublet->first);
  //  std::cout << " with " << dublet->second << " entries\n";
  //  dublet=find_if(dublet,M.end(), [](auto & m){return m.second>1;});
  //}
}

inline void test_make_unique_pairs2(int N=2)
{
  //auto R = ibn::test <std::vector<double> > (24);
  //for(auto r: R) std::cout <<r << " ";
  //std::cout << std::endl;
  std::vector<int>  A={1};
  for( int i=1;i<N;++i) A.push_back(i+1);
  ibn::print_array("array = ",A);
  auto R = ibn::make_unique_pairs2
    <
    decltype(A)::iterator,
    std::vector< std::vector < std::pair<int, int> > >
    >(A.begin(), A.end());

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
  //test_make_unique_pairs(NA, NB);

  //test_make_unique_pairs_old(NA);
  test_make_unique_pairs2(NA);
}

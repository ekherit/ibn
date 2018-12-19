/*
 * =====================================================================================
 *
 *       Filename:  combinator.h
 *
 *    Description: Make all compinations of unique pairs from two item lists
 *
 *        Version:  1.0
 *        Created:  13.12.2018 15:28:30
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan B. Nikolaev (ekherit), I.B.Nikolaev@inp.nsk.su
 *   Organization:  Budker Insitute of Nuclear Physics
 *
 * =====================================================================================
 */

#include <vector>
//#include<map>
#include <iostream>
//#include <algorithm>
#include <utility>
#include <algorithm>
#include <iomanip>

namespace ibn
{
  template< class Ps>
    void print_pairs( const Ps & P)
    {
      for( const auto & p: P)
      {
        std::cout << p.first<<p.second<< " ";
      }
    };
  template< class Cs>
    void print( const Cs & C)
    {
      int idx=1;
      for(const auto & P: C)
      {
        std::cout << std::setw(20) << idx << "  : ";
        print_pairs(P);
        //for( const auto & p: P)
        //{
        //  std::cout << p.first<<p.second<< " ";
        //}
        std::cout << std::endl;
        idx++;
      }
    };

  template<class Cont>
    void print_array(std::string name, const Cont & A)
    {
      std::cout << name << " = {";
      for(auto it = A.begin(); it!=A.end(); ++it)
      {
        std::cout << *it;
        if( std::next(it)!=A.end() ) std::cout << ',';
      }
      std::cout << "}\n";
    };

  template<class It>
    void print_array(std::string name, const It b, const It e)
    {
      std::cout << name << "{";
      for(auto it = b; it!=e; ++it)
      {
        std::cout << *it;
        if( std::next(it)!=e ) std::cout << ',';
      }
      std::cout << "}";
    };

  template < 
    class ItA,   //iterator in the first container of objects type A
    class ItB,   //iterator in the second container of object type B
    class R>     //result contains all combinations of pairs:  
                 //   { (a1,b1) (a2,b2) (a3,b3) }
                 //   { (a1,b2) (a2,b1) (a3,b3) }
                 //   ...
      void  make_unique_pairs
      (
       ItA a_begin, ItA a_end, 
       ItB b_begin, ItB b_end, 
       R & result
      )
      {
        //this is the type of pair's items (A,B)
        using A = typename std::iterator_traits<ItA>::value_type; 
        using B = typename std::iterator_traits<ItB>::value_type;
        //the end of recursion
        if( a_begin==a_end || b_begin==b_end) return;
        if( result.empty() ) result.push_back( typename R::value_type() ); //initial start
        typename R::value_type begin_combination; 
        std::copy(result.back().begin(),result.back().end(), std::back_inserter(begin_combination));
        static int depth=-1;
        depth++;
        static auto BEGIN_A = a_begin;
        auto shift = [](int d){for(int i=0;i<d;++i) std::cout << "     ";};
        shift(depth);
        std::cout << "begin combination: depth=" << depth << " "; print_pairs(begin_combination); std::cout << std::endl;
        shift(depth);
        print_array("Total array: ", BEGIN_A, a_end);
        std::cout << std::endl;
        int a_idx=0;
        for(auto a = a_begin; a!=a_end; ++a)
        {
          shift(depth);
          std::cout << "   a_idx = " << a_idx << " ";
          print_array("swap ", a_begin, a_end);
          if(a != a_begin) std::swap(*a, *a_begin);
          print_array(" -> ", a_begin, a_end);
          std::cout << std::endl;
          R tmp{typename R::value_type({std::pair<A,B> (*a_begin, *b_begin)})};
          shift(depth);
          std::cout << "   depth = " << depth <<" a_idx=" << a_idx << " b_idx = " << " pair = " << *a_begin <<  *b_begin<< std::endl;
          make_unique_pairs(next(a_begin), a_end, next(b_begin), b_end, tmp);
          for(auto it = tmp.begin(); it!=tmp.end(); ++it)
          {
            std::copy(it->begin(), it->end(),std::back_inserter(result.back()));
            if(std::next(a) != a_end  || std::next(it)!=tmp.end()) result.push_back(begin_combination); 
          }
          shift(depth);
          print_array("   back swap ", a_begin, a_end);
          if(a!=a_begin) std::swap(*a, *a_begin); //swap back
          print_array(" -> ", a_begin, a_end);
          std::cout << std::endl;
          a_idx++;
        }
        depth--;
      }
}

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
#pragma once
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

  template< class Cs>
    void remove_duplicates(Cs & C)
    {
      for(auto & c : C) sort(c.begin(), c.end());
    }

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
      inline void  make_unique_pairs
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
  template< class It, class R> 
    inline void make_unique_pairs_old(It it_begin, It it_end, R & result)
    {
      using A = typename std::iterator_traits<It>::value_type; 
      using Pairs_t = typename R::value_type; 
      std::sort(it_begin, it_end);
      do
      {
        result.push_back(Pairs_t());
        for( auto it = it_begin; it!=it_end;++it)
        {
          auto & first = *it;
          it++;
          if(it==it_end) break;
          auto & second = *it;
          result.back().push_back(std::pair<A,A>(std::min(first,second), std::max(first,second)));
        }
      } while(std::next_permutation(it_begin,it_end));
      typedef typename R::iterator ItR;
      for(ItR r=result.begin(); r!=result.end(); ++r) std::sort(r->begin(),r->end());
      //sort final result
      std::sort(result.begin(),result.end());
      //remove duplicated combinations
      result.erase(std::unique(result.begin(),result.end()), result.end());
    }

  template< class It, class R> 
    inline void make_unique_pairs_adopt(It it_begin, It it_end, R & result)
    {
      static int depth=-1;
      depth++;
      static auto BEGIN_A = it_begin;
      auto shift = [](int d){for(int i=0;i<d;++i) std::cout << "    ";};
      using A = typename std::iterator_traits<It>::value_type; 
      if( std::next(it_begin) == it_end ||  it_begin == it_end ) return; //the end of recursion
      //if( it_begin == it_end ) return; //the end of recursion
      if( result.empty() ) result.push_back( typename R::value_type() ); //initial start
      typename R::value_type begin_combination; 
      std::copy(result.back().begin(),result.back().end(), std::back_inserter(begin_combination));

      shift(depth);
      std::cout << "depth = " << depth << ", begin combination: "; print_pairs(begin_combination);
      print_array("  Total array: ", BEGIN_A, it_end);
      std::cout << std::endl;

      auto & first = *it_begin;
      auto it_second = std::next(it_begin);
      //std::cout << "begin combination = ";
      //print_pairs(begin_combination);
      //std::cout << std::endl;
      int idx=0;
      for(auto it = it_second; it!=it_end; ++it)
      {
        shift(depth);
        std::cout <<"  depth = " << depth << " idx = " << idx << " *it=" <<  *it << " ";
        std::swap(*it_second, *it);
        //R tmp{typename R::value_type({std::pair<A,A> (first, *it_second)})};
        R tmp{typename R::value_type({std::pair<A,A>(std::min(first,*it_second), std::max(first,*it_second))})};
        std::cout << " new pair: " << first << " " << *it_second << "  ";
        //print(tmp);
        std::cout << std::endl;

        make_unique_pairs_adopt(std::next(it_second), it_end, tmp);
        //print(tmp);
        for(auto x = tmp.begin(); x!=tmp.end(); ++x)
        {
          std::copy(x->begin(), x->end(),std::back_inserter(result.back()));
          if(std::next(it) != it_end || std::next(x)!=tmp.end()) result.push_back(begin_combination); 
        }
        std::swap(*it_second, *it);//swap back
        depth--;
        idx++;
      }
      shift(depth);
      std::cout << " total loop: " << idx << std::endl;
      //print(result);
    }

  template< class It, class CombinationList> 
    inline auto make_unique_pairs_adopt2(It it_begin, It it_end) -> CombinationList
    {
      //some code for debug
      static int depth=-1;
      depth++;
      auto shift = [](int d){for(int i=0;i<d;++i) std::cout << "    ";};
      static auto BEGIN_A = it_begin;
      shift(depth);
      print_array("  Total array: ", BEGIN_A, it_end);
      std::cout << std::endl;

      //definitions
      using A = typename std::iterator_traits<It>::value_type; 
      // using PairList = typename  CombinationList::value_type();

      //result.clear();

      CombinationList result; 

      //nothing to do with this case
      if( std::next(it_begin) == it_end ||  it_begin == it_end ) return result; 


      //pointers to pair candidates
      auto  first = it_begin;
      auto  second = std::next(it_begin);

      int idx=0;
      for(auto it = second; it!=it_end; ++it)
      {
        shift(depth);
        std::cout <<"  depth = " << depth << " idx = " << idx << " *it=" <<  *it << " ";
        std::cout << std::endl;
        //prepare for next iteration
        std::swap(*second, *it);
        CombinationList local_result =  make_unique_pairs_adopt2<It,CombinationList>(std::next(second), it_end);
        std::cout << "local_result = " << std::endl;;
        print(local_result);
        if(local_result.empty())  local_result.push_back(typename CombinationList::value_type()); 
        //now save the local_result to total result
        for(const auto & pair_list: local_result)
        {
          //start new combination
          result.push_back(typename CombinationList::value_type());
          result.back().push_back(std::pair<A,A>(*first,*second));
          std::copy(pair_list.begin(), pair_list.end(),std::back_inserter(result.back()));
        }
        std::swap(*second, *it);//swap back
        depth--;
        idx++;
      }
      shift(depth);
      std::cout << " total loop: " << idx << std::endl;
      //print(result);
      return result;
    }

  template< class It, class R> 
    inline void make_unique_pairs(It it_begin, It it_end, R & result)
    {
      using A = typename std::iterator_traits<It>::value_type; 
      //separate algorithm for odd and even array size
      auto distance = std::distance(it_begin,it_end);
      std::cout << "distance = " << distance << std::endl;
      if( distance % 2 == 0) { 
        //even size: just call algorithm
        //std::cout << *it_begin << std::endl;
        make_unique_pairs_adopt(it_begin,it_end,result);
      }
      else { //odd size:  convert array to vector of pointers and add nullptr pointer to
        //get even number and apply common algorithm
        std::vector<A*> V;
        V.resize(distance+1); //allocate a
        int idx=0;
        for(It it = it_begin; it!= it_end; ++it,++idx) V[idx] = &(*it);
        V[distance]=nullptr;
        std::vector< std::vector< std::pair<A*, A*> > > tmp;
        //make unique pairs with nullptr pairs
        make_unique_pairs_adopt(V.begin(),V.end(), tmp);
        //copy to result and remove nullptr pair
        for(auto & pair_list: tmp)
        {
          result.push_back(typename R::value_type());
          for(const auto & p : pair_list) 
            if(p.first!=nullptr && p.second!=nullptr) 
              result.back().push_back({*p.first,*p.second}); 
        }
      }
    }

  template< class It, class CombinationList> 
    inline auto make_unique_pairs2(It it_begin, It it_end) -> CombinationList
    {
      //definitions
      using A = typename std::iterator_traits<It>::value_type; 
      //using PairList = typename  CombinationList::value_type();

      //separate algorithm for odd and even array size
      auto distance = std::distance(it_begin,it_end);
      std::cout << "distance = " << distance << std::endl;
      if( distance % 2 == 0) { //even size: just call algorithm
        return make_unique_pairs_adopt2<It,CombinationList>(it_begin,it_end);
      }
      else { //odd size:  convert array to vector of pointers and add nullptr pointer to
        //get even number and apply common algorithm
        std::vector<A*> V;
        V.resize(distance+1); //allocate a
        int idx=0;
        for(It it = it_begin; it!= it_end; ++it,++idx) V[idx] = &(*it);
        V[distance]=nullptr;
        using ItPtr = typename std::vector<A*>::iterator;
        using CombPtr = typename std::vector< std::vector< std::pair<A*, A*> > >;
        auto tmp_result_with_nullptr_pairs = make_unique_pairs_adopt2<ItPtr, CombPtr> (V.begin(),V.end());
        //copy to result and remove nullptr pair
        CombinationList result;
        for(auto & ptr_pair_list: tmp_result_with_nullptr_pairs)
        {
          //PairList res_lst;
          result.push_back(typename CombinationList::value_type());
          for(const auto & p : ptr_pair_list) 
            if(p.first!=nullptr && p.second!=nullptr) 
              result.back().push_back({*p.first,*p.second}); 
        }
        return result;
      }
    }

  template<class A, class R> 
    auto test(A a)  -> R
    { 
      R result;
      for(int i=0;i<a; ++i) result.push_back(i*i);
      return result;
    }
}

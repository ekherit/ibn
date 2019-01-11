/*
 * =====================================================================================
 *
 *       Filename:  dimension_print.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11.01.2019 20:34:15
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan B. Nikolaev (ekherit), I.B.Nikolaev@inp.nsk.su
 *   Organization:  Budker Insitute of Nuclear Physics
 *
 * =====================================================================================
 */
#ifndef IBN_DIMESION_PRINT
#define IBN_DIMESION_PRINT
#pragma once

#include <map>
#include <iostream>
#include <sstream>

#include "new_dim.h"

std::map<int, std::string> DMAP;
std::map<char,std::string> super_map
{
  {'0',"⁰"},
  {'1',"¹"}, 
  {'2',"²"},
  {'3',"³"},
  {'4',"⁴"},
  {'5',"⁵"},
  {'6',"⁶"},
  {'7',"⁷"},
  {'8',"⁸"},
  {'9',"⁹"},
  {'+',"⁺"},
  {'-',"⁻"},
  {'=',"⁼"},
  {'(',"⁽"},
  {')',"⁾"},
  {'n',"ⁿ"},
  {'i',"ⁱ"}
};

std::ostream & operator<<(std::ostream & os,  Dimension<>)
{
  return os << "none";
}

template <typename B, typename...Bs> 
std::ostream & operator<<(std::ostream & os,  Dimension<B,Bs...>)
{
  const int num = B::num;
  const int den = B::den;
  const int id = B::id;
  os <<DMAP[id]; 
  std::ostringstream oss;
  bool super=true;
  if(den !=1 || num<0)
  {
    if(!super) oss<<"^(";
    oss << num;
    if(den !=1) oss << "/"<<den;
    if(!super)oss << ")";
  }
  else 
  {
    if(!super) oss << "^";
    oss  << num;
  }
  //std::string power;
  for(const int c : oss.str()) os << super_map[c];
  if ( Dimension<Bs...>::size == 0) return os;
  else return os << "·" << Dimension<Bs...>();
};

#endif

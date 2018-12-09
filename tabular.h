/*
 * =====================================================================================
 *
 *       Filename:  tabular.h
 *
 *    Description:  Tabular printing
 *
 *        Version:  1.0
 *        Created:  07.12.2018 13:10:23
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan B. Nikolaev (ekherit), I.B.Nikolaev@inp.nsk.su
 *   Organization:  Budker Insitute of Nuclear Physics
 *
 * =====================================================================================
 */

#pragma once 

#include <string>
#include <vector>
#include <regex>
#include <iostream>
#include <typeinfo>
#include <sstream>

#define IBN_TABULAR_DEBUG

struct hline {}; //intermediate hline
struct fhline {}; //final hline
class tabular
{
  public:
    tabular(std::string format) : os(&buffer)
    {
      //format like %10.2f*10
      static const std::regex cell_re(R"(([^%]*)?(%(\d+)(?:\.\d+)?[[:alpha:]])(?:\*(\d+))?([^%]*)?)");
      for (std::sregex_iterator it(format.begin(), format.end(),cell_re); it != std::sregex_iterator(); ++it)
      {
        auto & match = *it;
        // match[0] - full match
        // match[1] - left border
        // match[2] - format for printf
        // match[3] - cell width
        // match[4] - repetition number
        // match[5] - right border
#ifdef IBN_TABULAR_DEBUG
        int idx=0;
        for(auto & s : match ) std::cout << idx++ << ":" << s  << " len = " << s.length() << std::endl; ;
        std::cout << "match[1][0] = " << match.str()[0] << std::endl;
#endif
        std::string  left_border = match[1];
        std::string right_border = match[5]; 
        int repeat = match[4].length()==0 ?  1 : std::stoi(match[4]);
        int  width = std::stoi(match[3]);
        std::string fmt = match[2];
        for(int i=0;i<repeat;++i) 
        {
          if(i!=0) left_border=""; 
          cells.push_back({width,fmt,left_border, right_border});
        }
      }

    }
    template<class D>
    tabular & operator%(const D & data)
    {
      //std::ostream os (&buffer);
      char buf[1024];
      auto & cell = cells[cell_cursor];
      sprintf(buf,cell.format.c_str(),data);
      if     ( cell.left_border == "│" || cell.left_border=="|" ) os << "│";
      else if( cell.left_border == "║" || cell.left_border=="||") os << "║";
      else os << cell.left_border;
      os << buf;
      if( cell.right_border == "|") os << "│";
      else if( cell.right_border == "║" || cell.right_border=="||") os << "║";
      else os << cell.right_border;
      if(++cell_cursor == cells.size()) 
      {
        cell_cursor=0;
        os << std::endl;
        last_hline=false;
      }
      return *this;
    }

    tabular & operator%(fhline h)
    {
      std::cout << "In fhline" << std::endl;
      int idx = 0;
      for( auto & c : cells)
      {
        std::string line="─";
        if(c.left_border == "│" || c.left_border == "|") 
        {
          if( idx==0) os << "└";
          else  os << "┴";
        } 
        else 
        if(c.left_border == "║" || c.left_border == "||") 
        {
          if( idx==0) os << "╙";
          else  os << "╨";
        } 
        else for( int i=0;i<c.left_border.size(); ++i)  os << line;

        for( int i=0;i<c.width; ++i)  os << line;

        if(c.right_border == "│" || c.right_border == "|") 
        {
          if( idx==(cells.size()-1)) os << "┘";
          else  os << "┴";
        } 
        else if(c.right_border == "║" || c.right_border == "||") 
        {
          if( idx==(cells.size()-1)) os << "╜";
          else  os << "╨";
        } 
        else for( int i=0;i<c.right_border.size(); ++i)  os << line;
        idx++;
      }
      os << std::endl;
      last_hline=true;
      return *this;
    }

    tabular & operator%(hline h)
    {
      std::cout << "In hline" << std::endl;
      int idx = 0;
      for( auto & c : cells)
      {
        std::string line="─";
        if(c.left_border == "│" || c.left_border == "|") 
        {
          if( idx==0) os << "├";
          else  os << "┼";
        } 
        else if(c.left_border == "║" || c.left_border == "||") 
        {
          if( idx==0) os << "╟";
          else  os << "╫";
        } 
        else for( int i=0;i<c.left_border.size(); ++i)  os << line;

        for( int i=0;i<c.width; ++i)  os << line;

        if(c.right_border == "│" || c.right_border == "|") 
        {
          if( idx==(cells.size()-1)) os << "┤";
          else  os << "┼";
        } 
        else if(c.right_border == "║" || c.right_border == "||") 
        {
          if( idx==(cells.size()-1)) os << "╢";
          else  os << "╫";
        } 
        else for( int i=0;i<c.right_border.size(); ++i)  os << line;
        idx++;
      }
      os << std::endl;
      last_hline=true;
      return *this;
    }
    void print_info(void) const
    {
      std::cout << setw(20) << "Cell widthes:";
      for( auto & c : cells)
      {
        std::cout << c.left_border << setw(10) << c.width << c.right_border;
      }
      std::cout << std::endl;
      std::cout << setw(20) << "format:";
      for( auto & c : cells)
      {
        std::cout << c.left_border << setw(10) << c.format<< c.right_border;
      }
      std::cout << std::endl;
    }
    void flush(void) const
    {
      std::cout << buffer.str();
      std::cout.flush();
      //buffer.seekpos(0);
    }
  private:
    struct cell_t
    {
      int width; //width of the cell without borders
      std::string format;
      std::string left_border;
      std::string right_border;
    };
    std::vector< cell_t>  cells;
    std::stringbuf buffer;
    int cell_cursor=0;
    std::ostream os;
    bool last_hline=false;
};

//tabular & hline2( tabular & t )
//{
//  std::cout << "In hline2" << std::endl;
//  for( auto & c : t.cells)
//  {
//    std::cout << c.format << std::endl;
//    std::string line="─";
//    if(c.left_border == "│") t.os << "┴";
//    else for( int i=0;i<c.left_border.size(); ++i)  t.os << line;
//    for( int i=0;i<c.width; ++i)  t.os << line;
//    if(c.right_border == "│") t.os << "┴";
//    else for( int i=0;i<c.left_border.size(); ++i)  t.os << line;
//  }
//  t.os << std::endl;
//  return t;
//
//}




//auto t = new tabular("|%10.3d*3|%20.5f*3|");

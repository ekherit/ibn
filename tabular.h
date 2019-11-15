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
#include <iostream>
#include <iomanip>

#define IBN_TABULAR_DEBUG

//struct line
//{
//  line(int t) { type = t; };
//  int type;
//};
//
//struct hline : public line
//{
//  hline(int t) : line(t) {}
//}
//
//struct vline : public line
//{
//  hline(int t) : line(t) {}
//}

template < class Item>
using matrix = std::vector< std::vector<Item> >;
using fances_t = matrix<std::string>;
//typedef  fances_t;

fances_t C001 = {{"╭","┬","╮"}, 
                 {"├","┼","┤"}, 
                 {"╰","┴","╯"}};

fances_t C00 = {{"┌","┬","┐"},
                {"├","┼","┤"},
                {"└","┴","┘"}};

fances_t C01 = {{"╒","╤","╕"},
                {"╞","╪","╡"},
                {"╘","╧","╛"}};

fances_t C02 = {{"┍","┯","┑"},
                {"┝","┿","┥"},
                {"┕","┷","┙"}};

fances_t C10 = {{"╓","╥","╖"},
                {"╟","╫","╢"},
                {"╙","╨","╜"}};

fances_t C11 = {{"╔","╦","╗"},
                {"╠","╬","╣"},
                {"╚","╩","╝"}};

fances_t C12 = {{"━","━","━"},
                {"━","━","━"},
                {"━","━","━"}};

fances_t C20 = {{"┎","┰","┒"},
                {"┠","╂","┨"},
                {"┖","┸","┚"}};

fances_t C21 = {{"═","═","═"},
                {"═","═","═"},
                {"═","═","═"}};

fances_t C22 = {{"┏","┳","┓"},
                {"┣","╋","┫"},
                {"┗","┻","┛"}};

matrix<fances_t> F = { {C00, C01, C02},
                       {C10, C11, C12},
                       {C20, C21, C22}};

struct hline {}; //intermediate hline
struct fhline {}; //final hline
class tabular
{
  public:
    tabular(std::string format) : os(&buffer)
    {
      //format like %10.2f*10
      static const std::regex cell_re(R"(([^%]*)?(%(\d+)(?:\.\d+)?[[:alpha:]])(?:\*(\d+))?([^%]*)?)");
      int cell_idx=0;
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
        //for(auto & s : match ) std::cout << idx++ << ":" << s  << " len = " << s.length() << std::endl; ;
        //std::cout << "match[1][0] = " << match.str()[0] << std::endl;
#endif
        std::string  left_border = match[1];
        std::string right_border = match[5]; 
        int repeat = match[4].length()==0 ?  1 : std::stoi(match[4]);
        int  width = std::stoi(match[3]);
        std::string fmt = match[2];
        border_t left(match[1]);
        border_t right(match[5]);
        left_border = left.border;
        right_border = right.border;
        for(int i=0;i<repeat;++i) 
        {
          if(i!=0) 
          {
            left = border_t("");
            left_border=""; 
          }
          cells.push_back({width,fmt,left_border, right_border, left,right});
        }
      }
      if(cells.front().left.position == 0) cells.front().left.orientation = border_t::LEFT;
      if(cells.back().right.position == cells.back().right.border.size()-1) cells.back().right.orientation = border_t::RIGHT;
    }
    template<class D>
    tabular & operator%(const D & data)
    {
      //std::ostream os (&buffer);
      char buf[1024];
      auto & cell = cells[cell_cursor];
      sprintf(buf,cell.format.c_str(),data);
      cell.left.print(os);
      os << buf;
      cell.right.print(os);
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
      int idx = 0;
      std::string line_segment("─");
      for( auto & c : cells)
      {
        c.left.hline(os,0,border_t::BOTTOM);
        //line below data
        for( int i=0;i<c.width; ++i)  os << line_segment;
        c.right.hline(os,0,border_t::BOTTOM);
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
        std::regex  l_e(R"(│|[^\]*\|[^\|]*)");
        std::smatch sm;
        //if(std::regex_search(c.left_border, sm, l_e))
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
      std::cout << std::setw(20) << "Cell widthes:";
      for( auto & c : cells)
      {
        std::cout << c.left_border << std::setw(10) << c.width << c.right_border;
      }
      std::cout << std::endl;
      std::cout << std::setw(20) << "format:";
      for( auto & c : cells)
      {
        std::cout << c.left_border << std::setw(10) << c.format << c.right_border;
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
    struct border_t //border configuration
    {
      std::string border;
      int type=-1; //type of the vertical line
      int position=0; //position in the border string
      enum {LEFT, CENTER, RIGHT} orientation; //border is left ,right or between cells
      enum {TOP, MIDDLE, BOTTOM};
      border_t(void){}
      border_t(const std::string & b)
      {
        border = b;
        orientation = CENTER;
        //std::regex l_re(R"([^\|]*(\|)[^\|]*)"); //thin signle vertical line
        //std::regex ll_re(R"([^\|]*(\|\|)[^\|]*)"); //thin double vertical line
        std::smatch sm;
        std::vector < std::regex > line_re =
        {
          std::regex{R"([^\|]*(\|)[^\|]*)"},   //detect "│"
          std::regex{R"([^\|]*(\|\|)[^\|]*)"}, //detect "║"
          std::regex{R"([^\|]*(I)[^\|]*)"}     //detect "┃"
        };
        for ( int t = 0; t<line_re.size(); ++t)
        {
          if(std::regex_match(border, sm, line_re[t]))
          {
            type = t;
            position = sm.position(1);
            std::cout << "Found type " << t << " in positon: " << position << " in border string " << border << std::endl;
            return;
          }
        }
      }
      std::ostream & hline(std::ostream & os, int hline_type, int vert_orient)
      {
        std::cout << "In new hline:  hline_type=" <<  hline_type << " vert_orient= " << vert_orient << " orient = " << orientation << std::endl;
        std::vector<std::string> line_segment = {"─","═","━"};
        if(type < 0 || type>2 )  //no vertical line in border simple print
        {
          for( int i=0;i<border.size(); ++i) os << line_segment[hline_type];
        }
        else 
        {
          int i=0;
          for(;i<position; ++i) os << line_segment[hline_type];
          os << F[type][hline_type][vert_orient][orientation];
          for(;i<border.size()-2; ++i) os << line_segment[hline_type];
        }
        return os;
      }
      void print(std::ostream & os)
      {
        std::cout << "Print vertical line";
        std::vector<std::string> line_segment = {"│","║","┃"};
        if(type < 0 || type>2 )  //no vertical line in border simple print
        {
          os << border;
        }
        else 
        {
          int i=0;
          for(;i<position; ++i) os << border[i];
          os << line_segment[type];
          for(;i<border.size()-2; ++i) os << border[i];
        }
        return os;
      }
    };
    struct cell_t
    {
      int width; //width of the cell without borders
      std::string format;
      std::string left_border;
      std::string right_border;
      border_t  left, right;
      //int left_vline=-1;
      //int right_vline=-1;
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



auto test_regex(std::string s, std::string regexpr)
{
  std::regex  re(regexpr);
  std::smatch sm;
  if(std::regex_match(s, sm, re))
  {
    std::cout << "Match: " << std::endl;
    int idx=0;
    for(auto & m : sm )
    {
      std::cout << idx++ << " : " <<  m << std::endl;
    }
  }
}

void test(std::string s = "|%10d|%20.4f*2|")
{
(tabular(s) % 123 % 3.23452 % 2.72345234 % hline() % 3 % 24.23 % 234.6 % fhline() ).flush();
}

//auto t = new tabular("|%10.3d*3|%20.5f*3|");

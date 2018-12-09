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

#include <string>
#include <vector>
#include <regex>
#include <iostream>


class tabular
{
  public:
    tabular(std::string format)
    {
      //format like %10.2fx10
      std::regex re(R"(%(\d+)(\.\d+f)?(\*\d+)?)");
      std::regex times_re(R"(\*(\d+))");
      std::regex float_re(R"(\.\d+f)");
      //calculate numbe of sell
      //
      for (std::sregex_iterator it(format.begin(), format.end(),re); it != std::sregex_iterator(); ++it)
      {
        std::cout << "total match string" << it->str() << std::endl;
        if(it->size()>1)  //ok found
        {
          int repeat=1;
          std::string float_str;
          if(it->size()>2)  //looking for multiplication
          {
            for(int i=2; i<it->size();++i)
            {
              std::smatch sm;
              std::string s((*it)[i]);
              std::cout << "proceed s: " << s << std::endl;
              if(std::regex_search(s, sm, times_re)) repeat    = stoi(sm[1]);
              if(std::regex_search(s, sm, float_re)) float_str = sm[0];
            }
          }
          for(int i=0;i<repeat;++i) 
          {
            cell_widths.push_back(std::stod((*it)[1]));
            cell_formats.push_back( (*it)[1].str()+float_str);
          }
        }
        std::cout << std::endl;
      }
      std::cout << setw(20) << "Cell widthes:";
      for(auto  w : cell_widths)
      {
        std::cout << setw(10) << w;
      }
      std::cout << std::endl;
      std::cout << setw(20) << "format:";
      for(auto  w : cell_formats)
      {
        std::cout << setw(10) << w;
      }
      std::cout << std::endl;

    }

  private:
    std::vector<std::string> cell_formats;
    std::vector<int> cell_widths;
};

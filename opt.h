//#=====================================================================================
//#
//#       Filename:  Opt.h
//#
//#    Description:  Getting options from command line
//#
//#        Version:  1.0
//#        Created:  12.12.2002
//#       Revision:  none
//#       Compiler:  GNU C/C++
//#
//#         Author:  Ivan B. Nikolaev
//#        Company:  BINP,Russia
//#          Email:  I.B.Nikolaev@inp.nsk.su
//#
//#=====================================================================================
#ifndef IBN_OPT_H
#define IBN_OPT_H
#include <map>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <iostream>

#include <utility>
#include <sstream>

namespace ibn
{

  class opt2 {
    public:
    opt2(int argc, char ** argv) {
      ARGV = argv;
      pars.reserve(argc);
      for(int i=1;i<argc;++i) {
        std::string_view s(argv[i]);
        auto ptr = std::find(s.begin(),s.end(),'=');
        std::string_view key(s.begin(), std::distance(s.begin(), ptr));
        if(ptr!=s.end()) ++ptr;
        std::string_view value(ptr, std::distance(ptr, s.end()));
        pars.push_back({i,key,value});
      }
    }

    bool operator()(std::string_view key) const {
      return count(key);
    }

    template<typename T>
    bool operator()(std::string_view key, T & t) const {
      return  get(key,t);
    }

    bool is ( std::string_view key) {
      return count(key);
    }

    bool count(std::string_view key) const {
      auto it = std::find_if(pars.begin(),pars.end(), 
          [key](auto  s) {
            return key==s.key;
          } );
      if(it==pars.end() ) return false;
      return true;
    };

    template<typename T>
      T get(std::string_view key, T && t) const {
        auto it = find(key);
        if(it==pars.end() ) return T{t};
        return it->get(std::forward<T>(t));
      }

    template<typename T>
      T get(std::string_view key) const {
        return get<T>(key,T{});
      }

    template <typename T> 
      bool get ( std::string_view key, T * data ) const {
        auto it = find(key);
        if(it==pars.end() ) return false;
        *data = it->get(T{});
        return true;
      }

      void print(void)  {
        for(const auto & p : pars) {
          std::cout << p.idx << " " << p.key << "  " << p.value << std::endl; 
        }
      }

      std::string_view operator[](int idx) {
        return ARGV[idx];
      };
      bool operator[](std::string_view key) {
        return count(key);
      };
    private:
      struct option_t {
        int idx;
        std::string_view key;
        std::string_view value;

        template<typename T>
          T get(T && t) const {
            T data;
            if(parse(&data)) return data;
            return T{t};
          }

        template<typename T>
          T get(void) const {
            return get(T{});
          }
          
        template<typename T> 
          bool parse(T * t) const {
            std::string s{value};
            std::istringstream is(s);
            T data;
            if(! (is >> data)) { 
              return false;
            }
            *t = data;
            return true;
          }
      };
      std::vector<option_t>::const_iterator find(std::string_view key) const {
          return std::find_if(
              pars.begin(), pars.end(), 
              [&key](auto & p) {
                return p.key == key;
              } );
        }
      std::vector<option_t> pars;
      char ** ARGV;
  };

  typedef opt2 SimpleOpt;

  class opt	
  {
    public:
      class Bad	
      {
        public:
          std::string opt;
          std::string error;
          Bad(std::string o, std::string e="")	
          {
            opt = o; 
            error = e; 
          }
      };
      /* Information about options */
      std::map<std::string,unsigned> pmap;
      /* here is filled optins which come from argc and argv of main function */
      std::map<std::string,std::vector<std::string> > opts;
      /* here is list of common parameters of command line */
      std::list<std::string> par;
      opt(void);
      /* create opt with std::string "a:b:cdef" a la getopt.h */
      opt(std::string s,char par=':');
      /* add option */
      void add(std::string str, unsigned n=0);
      /* remove option */
      void del(std::string str);
      /* fill option pmapameters */
      void fill(int argc, char **argv);	
      bool isopt(const std::string &f);
      template <class T> bool getopt(const std::string &f, T* data);
      template <class T> bool getopt(const std::string &f, std::vector<T> * vec);
  };

  inline opt::opt(void){}

  /* create opt with std::string "a:b:cdef" a la getopt.h */
  inline opt::opt(std::string s,char par)	
  {
    std::string op;
    unsigned npar;
    char buf[]={0,0};
    for(unsigned int i = 0; i<s.size(); i++)	
    {
      buf[0]=s[i];
      std::string op(buf);
      std::string op2='-'+op;
      npar=0;
      if( i<(s.size()-1) && par==s[i+1]) 
      {
        npar=1;
        i++;
      }
      pmap[op2]=npar;
    }
  } //end of opt::opt


  /* add option */
  inline void opt::add(std::string str, unsigned n)	
  { 
    pmap[str]=n; 
  }

  /* remove option */
  inline void opt::del(std::string str)	
  { 
    pmap.erase(pmap.find(str)); 
  }

  /* fill option pmapameters */
  inline void opt::fill(int argc, char **argv)	
  {
    for(int i = 1; i<argc; i++)	
    {
      std::string o(argv[i]);
      std::map<std::string,unsigned >::iterator p = pmap.find(o);
      if(p==pmap.end()) 
        par.push_back(o);
      else	
      {
        std::vector<std::string> * vec = &opts[o];
        vec->resize(p->second);
        for(unsigned np = 0; np < p->second; np++)	
        {
          if(++i==argc)	
            throw Bad(o,"must has more values");
          std::string data(argv[i]);
          (*vec)[np]=data;
        }
      }
    }
  }

  inline bool opt::isopt(const std::string &f)	
  {
    if(opts.find(f)==opts.end()) 
      return false;
    else 
      return true;
  }

  template <class T> 
  inline  bool opt::getopt(const std::string &f, T* data)	
    {
      std::map<std::string,std::vector<std::string> >::const_iterator p = opts.find(f);
      if(p==opts.end()) 
        return false;
      else 
      {
        std::istringstream is(((p->second)[0]).c_str());
        is >> *data;
      }
      return true;
    }

  template <class T> 
    inline   bool opt::getopt(const std::string &f, std::vector<T> * vec)	
    {
      std::map<std::string,std::vector<std::string> >::const_iterator p = opts.find(f);
      if(p==opts.end()) 
        return false;
      else 
      {
        vec->resize((p->second).size());
        for(unsigned i =0; i<vec->size(); i++)	
        {
          std::istringstream is(((p->second)[i]).c_str());
          is >> (*vec)[i];
        }
      }
      return true;
    }

}
#endif //IBN_OPT_H

#ifndef IBN_TIMER_H
#define IBN_TIMER_H
/*
 * =====================================================================================
 *
 *       Filename:  timer.h
 *
 *    Description:  Simple timer for measuring time
 *
 *        Version:  1.0
 *        Created:  02/06/2008 07:06:14 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Nikolaev (ekherit), I.B.Nikolaev@inp.nsk.su
 *        Company:  Budker Institute of Nuclear Physics
 *
 * =====================================================================================
 */

//#include <unistd.h>
#include <chrono>
#include <thread>
//#include <sys/time.h>


namespace ibn
{
  class timer
  {
    using clock = std::chrono::steady_clock;
    public:
      timer(void)  {restart();};

      void restart(void) {
        //gettimeofday(&begin_time,0);
        begin_time = clock::now();
      }


      double elapsed(void) {
        /*
        timeval current_time;
        gettimeofday(&current_time,0);
        return double(current_time.tv_sec - begin_time.tv_sec)+ double(current_time.tv_usec-begin_time.tv_usec)*1e-6;
        */
        return std::chrono::duration_cast<std::chrono::duration<double> >(clock::now()-begin_time).count();
      }

      void set_begin_time(std::chrono::system_clock::time_point t) 
      {
        system_start_time=t;
      }

      std::chrono::system_clock::time_point get_begin_time(void) const 
      { 
        return system_start_time; 
      }

      //void set_begin_time(timeval t) 
      //{
      //  begin_time=t;
      //}

    private:
      //timeval begin_time;	
      clock::time_point  begin_time;
      std::chrono::system_clock::time_point system_start_time;
  };

  class sleep
  {
    using clock = std::chrono::steady_clock;
    public:
      sleep(double time, bool volatile * flag, double precision=1e-3 /* sec */) {
        using namespace std::literals;
        //timeval begin_time;
        //gettimeofday(&begin_time,0);
        //double dt=0;
        //do
        //{
        //  usleep(useconds_t(precision*1e6));

        //  timeval current_time;
        //  gettimeofday(&current_time,0);
        //  dt=double(current_time.tv_sec - begin_time.tv_sec)+ double(current_time.tv_usec-begin_time.tv_usec)*1e-6;
        //} while ( dt <= time && flag);
        auto begin_time = clock::now();
        std::chrono::duration<double> sd(time);
        clock::duration wait_time ( std::chrono::duration_cast<clock::duration>(sd));
        //auto sleep_time = std::chrono::milliseconds(10);
        while( *flag && (clock::now() - begin_time) < wait_time ) {
          std::this_thread::sleep_for(10ms);
        }
      }
      ~sleep(void) = default;
  };

};
#endif

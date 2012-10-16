/*
 * =====================================================================================
 *
 *       Filename:  math.h
 *
 *    Description:  My own math functions
 *
 *        Version:  1.0
 *        Created:  25.02.2012 20:25:52
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan B. Nikolaev (), I.B.Nikolaev@inp.nsk.su
 *        Company:  Budker Institute of Nuclear Physics, Novosibirsk, Russia
 *
 * =====================================================================================
 */

#ifndef IBN_MATH_H
#define IBN_MATH_H
namespace ibn
{
  inline double sq(double x) { return x*x;}
  inline double cb(double x) { return x*x*x;}
}
#endif

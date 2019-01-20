/*
 * =====================================================================================
 *
 *       Filename:  RealNumber.h
 *
 *    Description:  Compile time real number
 *
 *        Version:  1.0
 *        Created:  20.01.2019 14:39:14
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan B. Nikolaev (ekherit), I.B.Nikolaev@inp.nsk.su
 *   Organization:  Budker Insitute of Nuclear Physics
 *
 * =====================================================================================
 */
#ifndef IBN_REAL_NUMBER_H
#define IBN_REAL_NUMBER_H
#pragma once

#include "IntegerNumber.h"

template<typename Mantissa, typename Power>
struct RealNumber
{
  using mantinssa = Mantissa;
  using power = Power;
};


template< typename M1, typename P1, typename  M2, typename P2>
struct Multiply< RealNumber<M1,P1>, RealNumber<M2,P2> >
{
  using type = RealNumber< multiply_t<M1,M2>, add_t<P1,P2> >;
};

#endif

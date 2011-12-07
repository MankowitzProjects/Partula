/*
 *  Copyright (c) 2011 Yanan Wen <madwyn@gmail.com>
 *
 *  This program is for reviewing only, any other purposes are not granted.
 */

#ifndef MATH_EXT_H_INCLUDED
#define MATH_EXT_H_INCLUDED

#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX(a, b) ((a>b)?a:b)

#define MIN(a, b) ((a<b)?a:b)

#define Rad2Ang(rad) (rad*(180/M_PI))
#define Ang2Rad(ang) (ang*(M_PI/180))

/** \brief Swap two integer values
 *
 * \param a int&
 * \param b int&
 * \return void
 *
 */
static inline void swapInt(int &a, int &b)
{
    a = a^b;
    b = a^b;
    a = a^b;
}

static inline void swapDouble(double &a, double &b)
{
    double temp = a;
    a = b;
    b = temp;
}

/** \brief Calculate the square of deviation
 *
 * \param x1 double*
 * \param x2 double*
 * \return double
 *
 */
static inline double __devSqrt(double *x1, double *x2)
{
    *x1 -= *x2;
    return ((*x1) * (*x1));
}

/** \brief Calculate the distance between to points
 *
 * \param x1 double
 * \param y1 double
 * \param x2 double
 * \param y2 double
 * \return double
 *
 */
static inline double dist(double x1, double y1, double x2, double y2)
{
    return sqrt(__devSqrt(&x1, &x2) + __devSqrt(&y1, &y2));
}

/** \brief Resolve a quadratic equation by given the three values
 *
 * \param x1 double*
 * \param x2 double*
 * \param a double
 * \param b double
 * \param c double
 * \return void
 *
 */
static inline void quadratic(double *x1, double *x2, double a, double b, double c)
{
    c = b*b - 4*a*c;
    a *= 2;
    *x1 = (-b + sqrt(c)) / a;
    *x2 = (-b - sqrt(c)) / a;
}

/** \brief Get a random value of given range
 *
 * \param min int
 * \param max int
 * \return int
 *
 */
static inline int rngRand(int min, int max)
{
    int ret = 0;
    srand(time(NULL));
    ret = rand();
    ret %= (max-min);
    return ret += min;
}

/** \brief Convert radian to [-pi, pi]
 *
 * \param rad double
 * \return double
 *
 */
static inline double normRad(double rad)
{
    rad -= (floor(rad / (2*M_PI))) * (2*M_PI);
    return ((rad > M_PI) ? (rad - 2*M_PI) : rad);
}

#endif // MATH_EXT_H_INCLUDED

/**
    \file
    \brief Solve inputed equation
*/

#ifndef SOLVERH

#define SOLVERH

#include "Colors.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/**
    \brief Contains possible numbers of roots
 */
enum nOfRoots         
{
    InfSol = -1,      
    NoRoots = 0, 
    OneRoot = 1,
    TwoRoots = 2
}; 

/**
    \brief Accyracy with which doubles are compared 
 */
static const double ACCURACY = 0.0001;

/**
    \brief Solve square equation 

    Solve square equation with known coefficients. 
    \param[in] a is leader coefficient of inputed square equation
    \param[in] b is first degree coefficient of inputed square equation
    \param[in] c is constant term of inputed square equation
    \param[out] x1 is a pointer to bigger root of solving equation. If there are no roots or their number is not finit has value NAN
    \param[out] x2 is a pointer to smaller root of solving equation. If there are no roots, only one root or their number is not finit
    has value NAN
    \return Return number of roots. All possible values are conteined in enum nOfRoots.
    \warning In case discriminant in range from -ACCURACY to ACCURACY program inteptret it like discriminant is zero
*/
enum nOfRoots SolveQuadratic (double a, double b, double c, double *x1, double *x2); 

/**
    \brief Solve linear equation 

    \param[in] b is first degree coefficient of inputed square equation
    \param[in] c is constant term of inputed square equation
    \param[out] x1 is a pointer to root of solving equation. If theer are no root or their number is not finit has value NAN
*/
enum nOfRoots SolveLinear (double b, double c, double *x1);

/**
    \brief Compare the double value with zero

    Compare the double absolute value with ACCYRACY
    \param[in] isZero a double which value you need to compare
    \return 1 if double value is in range from -ACCYRACY to ACCYRACY, 0 if it's not
*/
int IsDoubleZero (double isZero);                

#endif
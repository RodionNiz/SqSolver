/**
    \file
    \brief Define struct polynomial
 */

#ifndef STRUCT_POL

#define STRUCT_POL

/**
    \brief Contains coeffitients, number of roots and their value of square polynomial
 */
struct Polynomial //cringenaming
{
    double aP, bP, cP;
    double x1, x2;
    int nOfSol;
};

#endif
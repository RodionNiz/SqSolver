/**
    \file 
    \brief Run solver tests
 */

#ifndef SqTestsH
#define SqTestsH

#include "Solver.h"
#include "StructPoly.h"
#include <assert.h>
#include <time.h>

enum modeOfTestSolve
{
    AutoTests = -2,
    SolveMenuErr = -1,
    ManualTests = 0,
    SqNoRoots = 1,
    SqOneRoot = 2,
    SqTwoRoots = 3,
    LnOneRoot = 4,
    LnNoRoot = 5,
    _Max_mode
};

/**
    \brief Make one solver test
    \param [in] testPolRef contain reference coefficients and number of roots in square equation 
    \return 0 if the test was passed, 1 if the test was failed
 */
int TestSolve (struct Polynomial testPolRef);

/**
    \brief Chech is the x a root of square equation with coeffitients a, b, c
    \param [in] x is a possible root of equation
    \param [in] a is a reference leader coefficient
    \param [in] b is a reference first degree coefficient
    \param [in] c is a reference constant term
    \return 0 if x is a root, 1 if it's not
 */
int CheckX (double x, double a, double b, double c);

/**
    \brief Run auto solver tests with different scenaries
    \param [in] repeats is a number of tests
    \param [in] kindOTests is a mode of tests. All possible values are contained in enum modeOfTestSolve
    \return number of failed tests
 */
int RunTestSolveAuto (int repeats, int kindOfTest);

/**
    \brief Run manual solver tests
    \return number of failed tests
 */
int RunTestSolveManual ();

/**
    \brief Generate a random double in a range from -5 to 15
    \return random double 
 */
double RandDouble ();

/**
    \brief Generate a random coefficients of square equation with two roots
 */
void GenerateTwoRootsTest (double *a, double *b, double *c);

/**
    \brief Generate a random coefficients of square equation with one root
 */
void GenerateOneRootTest (double *a, double *b, double *c);

/**
    \brief Generate a random coefficients of square equation with no roots
 */
void GenerateNoRootsTest (double *a, double *b, double *c);

/**
    \brief Generate a random coefficients of linear equation with one root
 */
void GenerateOneRootsTestLinear (double *a, double *b, double *c);

/**
    \brief Generate a random coefficients of linear equation with no roots
 */
void GenerateNoRootsTestLinear (double *a, double *b, double *c);
#endif
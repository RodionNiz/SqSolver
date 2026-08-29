/**
    \file
    \brief Run parser tests
 */

#ifndef TESTPARSER

#define TESTPARSER

#include "Solver.h"
#include "Parser.h"
#include <assert.h>

/**
    \brief Contains string for parser tests and reference values of coeffitients
 */
struct ParseTestString 
{
    const char *testString;
    double aRef, bRef, cRef;
};

/**
    \brief Make one parser test
    \param [in] aRef is a reference value of the leader coeffirient
    \param [in] bRef is a reference value of the first degree coeffirient
    \param [in] cRef is a reference value of the constant term
    \param [in] testString is a string which will be parsed
    \return 0 if the test was passed, 1 if the test was failed
 */
int ParseTest (double aRef, double bRef, double cRef, const char testString []);

/**
    \brief Run manual parser tests
    \return number of failed tests
 */
int RunParseTestsManual ();

#endif
#ifndef TESTPARSE

#define TESTPARSE

#include "Solver.h"
#include <assert.h>

struct ParseTestString 
{
    char *testString;
    double aRef, bRef, cRef;
};

int ParseTest               (double aRef, double bRef, double cRef, char testString []);    //один тест парсера
int RunParseTestsManual     ();                                                             //запуск ручного теста парсера

#endif
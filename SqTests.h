#ifndef SqTestsH

#define SqTestsH

#include "ConstValues.h"
#include "StructPol.h"
#include <assert.h>

//функции для теста
int TestSolve                   (struct Polinomial testPolRef);                          //1 тест решения
int CheckX                      (double x, double a, double b, double c);                //проверяет корень уравнения
int RunTestSolveAuto            (int repeats, int kindOfTest);                           //запускает авто тесты решений
int RunTestSolveManual         ();                                                      //запускает ручные тесты решений
double RandDouble               ();                                                      //создает рандомнфй double из диапазона
void GenerateTwoRootsTest       (double *a, double *b, double *c);                       //генерация квадратного уравнения с 2 корнями
void GenerateOneRootTest        (double *a, double *b, double *c);                       //генерация квадратного уравнения с 1 корнмем
void GenerateNoRootsTest        (double *a, double *b, double *c);                       //генерация квадратного уравнения без корней
void GenerateOneRootsTestLinear (double *a, double *b, double *c);                       //генерация линейного уравнения c 1 корнем
void GenerateNoRootsTestLinear  (double *a, double *b, double *c);                       //генерация линейного уравнения без корней
int IsDoubleZero                (double iszero);                                         //необходим для компиляции SqTests
int SolveKv                     (double a, double b, double c, double *x1, double *x2);  //необходим для компиляции SqTests
int SolveLinear                 (double b, double c, double *x1);                        //необходим для компиляции SqTests

#endif
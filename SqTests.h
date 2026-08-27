#ifndef SqTestsH

#define SqTestsH

#include "DefinesConst.h"
#include "StructPol.h"
#include <assert.h>

//функции для теста
int SolverTest                 (struct Polinomial testPolRef);
int CheckX                     (double x, double a, double b, double c);
void RunSolveTestsAuto         (int repeats, int kindOfTest);
void RunSolveTestsManual       ();
double RandDouble              ();
void BufferClean               ();
void GenerateTwoRootsTest      (double *a, double *b, double *c);
void GenerateOneRootTest       (double *a, double *b, double *c);
void GenerateNoRootsTest       (double *a, double *b, double *c);
void GenerateOneRootsTestLinear(double *a, double *b, double *c);
void GenerateNoRootsTestLinear (double *a, double *b, double *c);
int IsDoubleZero               (double iszero);
int SolveKv                    (double a, double b, double c, double *x1, double *x2);
int SolveLinear                (double b, double c, double *x1);

#endif
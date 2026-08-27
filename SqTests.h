#ifndef RED RESET GREEN ACCURACY
#define RED     "\033[31m"
#define RESET   "\033[0m"
#define GREEN   "\033[32m"

#define ACCURACY 0.001

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "StructPol.h"



//функции для теста
int SolverTest                 (struct Polinomial testPolRef);
int CheckX                     (double x, double a, double b, double c);
void RunSolveTestsAuto         (int repeats, int kindOfTest);
void RunSolveTestsManual       ();
double RandDouble              ();
void menu                      ();
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
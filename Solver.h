#ifndef SOLVERH

#define SOLVERH

#include "ConstValues.h"

int SolveLinear          (double b, double c, double *x1);                          //поиск корня в случае нулевого старшего коэффициента квадратного уравнения
int SolveKv              (double a, double b, double c, double *x1, double *x2);    //функция поиска корней квадратного уравнения  с ненулевым старшим коэффициентом
int IsDoubleZero         (double iszero);                                           //сравнивает double с 0 с точностью ACCURACY                     

#endif
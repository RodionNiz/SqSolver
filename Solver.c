#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "Solver.h"

//поиск корня в случае нулевого старшего коэффициента квадратного уравнения
int SolveLinear (double b, double c, double *x1)
{
    assert (isfinite (b));
    assert (isfinite (c));
    assert (x1 != NULL);

    if (b == 0)        //проверка конечности количества решений
    {
        if (c == 0)
        {
            printf (RED "\nInput error! Try again!\n" RESET);
            return -1;
        }
        return 0;
    }
    *x1 = -c / b;
    return 1;
}


//функция поиска корней квадратного уравнения с ненулевым старшим коэффициентом
int SolveKv (double a, double b, double c, double *x1, double *x2)
{
    assert (x1 != NULL);
    assert (x2 != NULL);
    assert (isfinite (a));
    assert (isfinite (b));
    assert (isfinite (c));


    if (IsDoubleZero (a))
    {
        return SolveLinear (b, c, x1);
    }

    double discriminant = b * b - 4 * a * c;
    double a2 = a * 2;

    if (IsDoubleZero (discriminant))
    {
        *x1 = -b / a2;                    
        return 1;
    }

    if (discriminant > ACCURACY)
    {
        *x1 = (-b - sqrt(discriminant)) / a2;    //корни уравнения с положительным ненулевым D
        *x2 = (-b + sqrt(discriminant)) / a2;
        return 2;
    }
    
    return 0;                   //случай с отсутствием действительных корней 
}
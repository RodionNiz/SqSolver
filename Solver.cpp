#include "Solver.h"


enum nOfRoots SolveLinear (double b, double c, double *x1)
{
    assert (isfinite (b));
    assert (isfinite (c));
    assert (x1 != NULL);

    if (b == 0)
    {
        *x1 = NAN;

        if (c == 0)
        {
            return InfSol;
        }
        return NoRoots;
    }

    *x1 = -c / b;

    return OneRoot;
}


enum nOfRoots SolveQuadratic (double a, double b, double c, double *x1, double *x2)
{
    assert (x1 != NULL);
    assert (x2 != NULL);
    assert (isfinite (a));
    assert (isfinite (b));
    assert (isfinite (c));


    if (IsDoubleZero (a))
    {
        *x2 = NAN;
        return SolveLinear (b, c, x1);
    }

    double discriminant = b * b - 4 * a * c;
    double a2 = a * 2;

    if (IsDoubleZero (discriminant))
    {
        *x1 = -b / a2;
        *x2 = NAN;               
        return OneRoot;
    }

    if (discriminant > 0)
    {
        *x1 = (-b - sqrt(discriminant)) / a2;
        *x2 = (-b + sqrt(discriminant)) / a2;
        return TwoRoots;
    }

    *x1 = NAN;
    *x2 = NAN;

    return NoRoots;
}


int IsDoubleZero (double isZero)
{
    return (fabs (isZero) < ACCURACY);
}
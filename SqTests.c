#include "SqTests.h"
#include "StructPol.h"

#include <stdio.h>
#include <stdlib.h>


//
int SolverTest (struct Polinomial testPolRef)
{
    struct Polinomial testPol = {.aP = testPolRef.aP, .bP = testPolRef.bP, .cP = testPolRef.cP};
    testPol.nOfSol = SolveKv (testPol.aP, testPol.bP, testPol.cP, &testPol.x1, &testPol.x2);
    if (testPolRef.nOfSol == 2 &&                                                                   //проверка правильности 2 корней
        (CheckX (testPol.x1, testPolRef.aP, testPolRef.bP, testPolRef.cP) ||
        CheckX (testPol.x2, testPolRef.aP, testPolRef.bP, testPolRef.cP)) &&
        testPolRef.nOfSol == testPol.nOfSol)
    {
        printf (RED "Test FAILED! Wrong answer!:\n" RESET
                "a = " RED "%lg, " RESET "b = " RED "%lg, " RESET "c = " RED "%lg" RESET "\n"
                "got: numOfSol = " RED "%d, " RESET "x1 = " RED "%lg, " RESET "x2 = " RED "%lg\n" RESET
                , testPolRef.aP, testPolRef.bP, testPolRef.cP, testPol.nOfSol, testPol.x1, testPol.x2);
        return 1;
    }
    if (CheckX (testPol.x1, testPolRef.aP, testPolRef.bP, testPolRef.cP) && testPolRef.nOfSol == 1 && testPolRef.nOfSol == testPol.nOfSol)
    {                                                                                              //проверка правильности одного корня
        printf (RED "Test FAILED! Wrong answer!:\n" RESET
                "a = " RED "%lg, " RESET "b = " RED "%lg, " RESET "c = " RED "%lg" RESET "\n"
                "got: numOfSol = " RED "%d, " RESET "x1 = " RED "%lg, " RESET "x2 = " RED "%lg\n" RESET
                , testPolRef.aP, testPolRef.bP, testPolRef.cP, testPol.nOfSol, testPol.x1, testPol.x2);
        return 1;
    }
    else if (testPolRef.nOfSol == 0 && testPolRef.nOfSol == testPol.nOfSol)                       //случай без корней
    {
        printf (GREEN"Test PASSED\n" RESET);
        return 0;
    }
    else if (testPolRef.nOfSol != testPol.nOfSol)                                                 //ошибка при несовпадении колва корней
    {
        printf (RED "Test FAILED! Wrong nOfSol:\n" RESET
                "a = " RED "%lg, " RESET "b = " RED "%lg, " RESET "c = " RED "%lg" RESET "\n"
                "got: numOfSol = " RED "%d, " RESET "expected: numOfSOl = " RED "%d\n"  RESET
                , testPolRef.aP, testPolRef.bP, testPolRef.cP, testPol.nOfSol, testPolRef.nOfSol);
        return 1;
    }
    printf (GREEN"Test PASSED\n" RESET);
    return 0;
}


//проверка правильности корня, 0 если корень верный, 1 если неверный
int CheckX (double x, double a, double b, double c)
{
    return !IsDoubleZero (a * x * x + b * x + c);
}

//запуск автоматических тестов решения
void RunSolveTestsAuto (int repeats, int kindOfTest)
{   
    int countFail = 0;
    struct Polinomial testPolRef;
    switch (kindOfTest)
    {
    case 0:
        while (repeats)
        {
            testPolRef.nOfSol = 0;
            GenerateNoRootsTest (&testPolRef.aP, &testPolRef.bP, &testPolRef.cP);
            countFail += SolverTest (testPolRef);
            --repeats;
        }
        printf ("%d tests had been failed\n", countFail);
        abort ();
    case 1:
        while (repeats)
        {
            testPolRef.nOfSol = 1;
            GenerateOneRootTest (&testPolRef.aP, &testPolRef.bP, &testPolRef.cP);
            countFail += SolverTest (testPolRef);
            --repeats;
        }
        printf ("%d tests had been failed\n", countFail);
        abort ();
    case 2:
        while (repeats)
        {
            testPolRef.nOfSol = 2;
            GenerateTwoRootsTest (&testPolRef.aP, &testPolRef.bP, &testPolRef.cP);
            countFail += SolverTest (testPolRef);
            --repeats;
        }
        printf ("%d tests had been failed\n", countFail);
        abort ();
    case 3:
        while (repeats)
        {
            testPolRef.nOfSol = 1;
            GenerateOneRootsTestLinear (&testPolRef.aP, &testPolRef.bP, &testPolRef.cP);
            countFail += SolverTest (testPolRef);
            --repeats;
        }
        printf ("%d tests had been failed\n", countFail);
        abort ();
    case 4:
        while (repeats)
        {
            testPolRef.nOfSol = 0;
            GenerateNoRootsTestLinear (&testPolRef.aP, &testPolRef.bP, &testPolRef.cP);
            countFail += SolverTest (testPolRef);
            --repeats;
        }
        printf ("%d tests had been failed\n", countFail);
        abort ();
    }
}


//запуск ручных тестов решения
void RunSolveTestsManual ()
{
    struct Polinomial testsPolinomsRef [3] = 
    {
        {.aP = 1, .bP = 2.2, .cP = 1.21, .nOfSol = 1},
        {.aP = 0, .bP = 0, .cP = 0, .nOfSol = -1}, //////////InfSol
        {.aP = 0, .bP = 0, .cP = 1, .nOfSol = 0}
    };
    unsigned int size = sizeof (testsPolinomsRef) / sizeof (struct Polinomial);
    for (unsigned int i = 0; i < size; i++)
    {
        SolverTest (testsPolinomsRef [0]);
    }
    abort ();
}

//создание рандомного double в диапазоне от maxDoubleRange до minDoubleRange
double RandDouble ()
{
    int maxDoubleRange = 10; 
    int minDoubleRange = -5;
    double randD = (double)rand () / RAND_MAX * (maxDoubleRange - minDoubleRange) + minDoubleRange;
    return randD;
}


//генерация уравнения с 2 корнями
void GenerateTwoRootsTest (double *a, double *b, double *c)
{
    assert (a != NULL);
    assert (b != NULL);
    assert (c != NULL);

    double x1Ref = RandDouble ();
    double x2Ref = RandDouble ();
    double aRef = RandDouble ();

    *a = aRef;
    *b = -aRef * (x1Ref+x2Ref);
    *c = aRef * x1Ref * x2Ref;
}


//генерация уравнения с 1 корнем и а != 0
void GenerateOneRootTest (double *a, double *b, double *c)
{
    assert (a != NULL);
    assert (b != NULL);
    assert (c != NULL);

    double x1Ref = RandDouble ();
    double aRef = RandDouble ();

    *a = aRef;
    *b = -aRef * 2 * x1Ref;
    *c = aRef * x1Ref * x1Ref;
}

//генерация трехчлена без корней
void GenerateNoRootsTest (double *a, double *b, double *c)
{
    assert (a != NULL);
    assert (b != NULL);
    assert (c != NULL);

    double x1Ref = RandDouble ();
    double aRef = RandDouble ();
    double cRef = RandDouble ();

    if ((cRef <= 0 && aRef > 0) || (cRef >= 0 && aRef < 0))
    {
        cRef  *= -1;
    }

    *a = aRef;
    *b = -aRef * 2 * x1Ref;
    *c = aRef * x1Ref * x1Ref + cRef;
}


//генерация линейного уравнения с 1 корнем
void GenerateNoRootsTestLinear (double *a, double *b, double *c)
{
    assert (a != NULL);
    assert (b != NULL);
    assert (c != NULL);

    *a = 0;
    *b = 0;
    *c = RandDouble ();
}

//генерация линейного уравнения без корней
void GenerateOneRootsTestLinear (double *a, double *b, double *c)
{
    assert (a != NULL);
    assert (b != NULL);
    assert (c != NULL);

    *a = 0;
    *b = RandDouble ();
    *c = RandDouble ();
}
#include "TestSolver.h"
#include "StructPoly.h"

#include <stdio.h>
#include <stdlib.h>

//
int TestSolve (struct Polynomial testPolyRef)
{
    struct Polynomial testPoly = {.aP = testPolyRef.aP, .bP = testPolyRef.bP, .cP = testPolyRef.cP};

    testPoly.nOfSol = SolveQuadratic (testPoly.aP, testPoly.bP, testPoly.cP, &testPoly.x1, &testPoly.x2);

    if (testPolyRef.nOfSol == 2 &&
       (CheckX (testPoly.x1, testPolyRef.aP, testPolyRef.bP, testPolyRef.cP) ||
        CheckX (testPoly.x2, testPolyRef.aP, testPolyRef.bP, testPolyRef.cP)) &&
        testPolyRef.nOfSol == testPoly.nOfSol)
    {
        printf (RED "Test FAILED! Wrong answer!:\n" RESET
                "a = " RED "%lg, " RESET "b = " RED "%lg, " RESET "c = " RED "%lg" RESET "\n"
                "got: numOfSol = " RED "%d, " RESET "x1 = " RED "%lg, " RESET "x2 = " RED "%lg\n" RESET
                , testPolyRef.aP, testPolyRef.bP, testPolyRef.cP, testPoly.nOfSol, testPoly.x1, testPoly.x2);
        return 1;
    }

    if (CheckX (testPoly.x1, testPolyRef.aP, testPolyRef.bP, testPolyRef.cP) &&
                testPolyRef.nOfSol == 1 && testPolyRef.nOfSol == testPoly.nOfSol)
    {
        printf (RED "Test FAILED! Wrong answer!:\n" RESET
                "a = " RED "%lg, " RESET "b = " RED "%lg, " RESET "c = " RED "%lg" RESET "\n"
                "got: numOfSol = " RED "%d, " RESET "x1 = " RED "%lg, " RESET "x2 = " RED "%lg\n" RESET
                , testPolyRef.aP, testPolyRef.bP, testPolyRef.cP, testPoly.nOfSol, testPoly.x1, testPoly.x2);
        return 1;
    }

    else if (testPolyRef.nOfSol == 0 && testPolyRef.nOfSol == testPoly.nOfSol)
    {
        printf (GREEN"Test PASSED\n" RESET);
        return 0;
    }

    else if (testPolyRef.nOfSol != testPoly.nOfSol)
    {
        printf (RED "Test FAILED! Wrong nOfSol:\n" RESET
                "a = " RED "%lg, " RESET "b = " RED "%lg, " RESET "c = " RED "%lg" RESET "\n"
                "got: numOfSol = " RED "%d, " RESET "expected: numOfSOl = " RED "%d\n"  RESET
                , testPolyRef.aP, testPolyRef.bP, testPolyRef.cP, testPoly.nOfSol, testPolyRef.nOfSol);
        return 1;
    }

    printf (GREEN"Test PASSED\n" RESET);
    return 0;
}


int CheckX (double x, double a, double b, double c)
{
    return !IsDoubleZero (a * x * x + b * x + c);
}


int RunTestSolveAuto (int repeats, int kindOfTest)
{   
    int countFail = 0;
    struct Polynomial testPolyRef;

    srand (time (NULL));

    switch (kindOfTest)
    {
    case SqNoRoots:
        while (repeats)
        {
            testPolyRef.nOfSol = 0;
            GenerateNoRootsTest (&testPolyRef.aP, &testPolyRef.bP, &testPolyRef.cP);
            countFail += TestSolve (testPolyRef);
            --repeats;
        }
        break;

    case SqOneRoot:
        while (repeats)
        {
            testPolyRef.nOfSol = 1;
            GenerateOneRootTest (&testPolyRef.aP, &testPolyRef.bP, &testPolyRef.cP);
            countFail += TestSolve (testPolyRef);
            --repeats;
        }
        break;

    case SqTwoRoots:
        while (repeats)
        {
            testPolyRef.nOfSol = 2;
            GenerateTwoRootsTest (&testPolyRef.aP, &testPolyRef.bP, &testPolyRef.cP);
            countFail += TestSolve (testPolyRef);
            --repeats;
        }
        break;

    case LnOneRoot:
        while (repeats)
        {
            testPolyRef.nOfSol = 1;
            GenerateOneRootsTestLinear (&testPolyRef.aP, &testPolyRef.bP, &testPolyRef.cP);
            countFail += TestSolve (testPolyRef);
            --repeats;
        }
        break;

    case LnNoRoot:
        while (repeats)
        {
            testPolyRef.nOfSol = 0;
            GenerateNoRootsTestLinear (&testPolyRef.aP, &testPolyRef.bP, &testPolyRef.cP);
            countFail += TestSolve (testPolyRef);
            --repeats;
        }
        break;
    }
    printf ("%d tests had been failed\n", countFail);
    return countFail;
}


int RunTestSolveManual ()
{
    int countFail = 0;

    struct Polynomial testsPolynomsRef [] = 
    {
        {.aP = 1,   .bP = 2.2,    .cP = 1.21, .nOfSol = 1},
        {.aP = 0,   .bP = 0,      .cP = 0,    .nOfSol = InfSol},
        {.aP = 0,   .bP = 0,      .cP = 1,    .nOfSol = 0},
        {.aP = 0.1, .bP = -1.2,   .cP = 3.5,  .nOfSol = 2},
        {.aP = 2.3, .bP = 0,      .cP = 0,    .nOfSol = 1},
        {.aP = 1,   .bP = -4.001, .cP = 0,    .nOfSol = 2},
        {.aP = 1,   .bP = -2.001, .cP = 1,    .nOfSol = 2}
    };

    unsigned int size = sizeof (testsPolynomsRef) / sizeof (struct Polynomial);
    for (unsigned int i = 0; i < size; i++)
    {
        countFail += TestSolve (testsPolynomsRef [i]);
    }

    return countFail;
}


double RandDouble ()
{
    int maxDoubleRange = 10; 
    int minDoubleRange = -5;
    double randD = (double)rand () / RAND_MAX * (maxDoubleRange - minDoubleRange) + minDoubleRange;
    return randD;
}


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


void GenerateNoRootsTestLinear (double *a, double *b, double *c)
{
    assert (a != NULL);
    assert (b != NULL);
    assert (c != NULL);

    *a = 0;
    *b = 0;
    *c = RandDouble ();
}


void GenerateOneRootsTestLinear (double *a, double *b, double *c)
{
    assert (a != NULL);
    assert (b != NULL);
    assert (c != NULL);

    *a = 0;
    *b = RandDouble ();
    *c = RandDouble ();
}
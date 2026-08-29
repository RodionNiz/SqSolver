#include "TestParser.h"
#include "Parser.h"

int ParseTest (double aRef, double bRef, double cRef, const char testString [])
{
    assert (testString != NULL);

    double a = NAN, b = NAN, c = NAN;
    char outString [InputStrLen]; 
    int size = strlen (testString);

    for (int i = 0; i < size; i++)
    {
        outString [i] = testString [i];
    }

    ParseMain (&a, &b, &c, outString);

    if (!IsDoubleZero (a - aRef) || !IsDoubleZero (b - bRef) || !IsDoubleZero (c - cRef))
    {
        printf ("Parsing test FAILED:\n"
                "Expected: aRef = %lg, bRef = %lg, cRef = %lg\n"
                "Got: a = %lg, b = %lg, c = %lg",
                aRef, bRef, cRef, a, b, c);
        return 1;
    }
    
    return 0;
}


int RunParseTestsManual ()
{
    int errorCount = 0;
    struct ParseTestString test1 = {.testString = "1.4x^2 + 0.0x + 1 = 0.0x^2 + 7.5x", .aRef = 1.4, .bRef = -7.5, .cRef = 1};
    errorCount += ParseTest (test1.aRef, test1.bRef, test1.cRef, test1.testString);
    printf ("%d test was failed", errorCount);
    return errorCount;
}
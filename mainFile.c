#include <assert.h>
#include <ctype.h>
#include <string.h>

#include "StructPol.h"      //структура Polinomial
#include "ConstValues.h" 
#include "Parser.h"         
#include "Solver.h"         
#include "SqTests.h"        //проверка решений квадратного уравнения


//структура для тестов парсера
struct ParseTestString 
{
    char *testString;
    double aRef, bRef, cRef;
};


enum Sol         
{
    InfSol = -1,     //количество решений -1 соответствует бесконечному числу решений
}; 


void PrintAns           (struct Polinomial);
void PrintStructPolinom (struct Polinomial);
void BufferClean        ();
int Menu                ();


int ParseTest            (double aRef, double bRef, double cRef, char testString []);    //один тест парсера
int RunParseTestsManual ();                                                             //запуск ручного теста парсера

int main ()
{  
    struct Polinomial mainPol = {.aP = 0, .bP = 0, .cP = 0, .nOfSol = 0, .x1 = NAN, .x2 = NAN};

    int menuRes = Menu ();

    if (menuRes != SolveEqMod)   //выбор режима работы
    {
        return 0;
    }
    else  
    {
        printf ("Error code is %d", menuRes);
    }  

    BufferClean ();     //очистка буффера ввода после функций scanf

    if (ReadInputBuffer (&mainPol.aP, &mainPol.bP, &mainPol.cP) == 0)    //возвращаемое значение - количество считаных коэффициентов
    {
        return 0;
    }

    mainPol.nOfSol = SolveKv (mainPol.aP, mainPol.bP, mainPol.cP, &mainPol.x1, &mainPol.x2);    
    PrintAns (mainPol);                                                                         
}


int Menu ()
{
    printf ("enter 0 if you want to solve equation\n"
            "enter 1 if you want to start solver tests\n"
            "enter 2 if you want to start parse tests\n");

    int mode = -1;
    scanf ("%d", &mode);

    if (mode < SolveEqMod || mode > TestParseMod)
    {
        printf (RED "INPUT ERROR!\n" RESET);
        return InputErr;
    }

    switch (mode)
    {
    case SolveEqMod:
        return 0;
        break;

    case TestSolveMod:
        printf ("enter 0 if you want to start auto tests with no roots\n"
                "enter 1 if you want to start auto tests with one root\n"
                "enter 2 if you want to start auto tests with two roots\n"
                "enter 3 if you want to start auto test with one linear root\n"
                "enter 4 if you want to start auto test with no linear root\n"
                "enter 5 if you want to start manual test\n");
        
        if (!(scanf ("%d", &mode)))
        {
            printf (RED "INPUT ERROR!\n" RESET);
            return InputErr;
        }

        int repeats = 0;
        if (mode < ManualTests) 
        {
            printf ("how many tests you want to do?\n");
            
            if (!(scanf ("%d", &repeats)))
            {
                printf (RED "INPUT ERROR!\n" RESET);
                return InputErr;
            }
            if (!RunTestSolveAuto (repeats, mode))
            {
                return ErrInTests;
            }
            return 1;
        }
        else if (mode == ManualTests)
        {
            if (!RunTestSolveManual ())
            {
                return ErrInTests;
            }
            return 1;
        }
        else 
        {
            printf (RED "INPUT ERROR!\n" RESET);
            return InputErr;
        }
        break;
    case TestParseMod:
        if (!RunParseTestsManual ())
        {
            return ErrInTests;
        }
        return 1;
        break;
    default:
        printf (RED "INPUT ERROR!\n" RESET);
        return InputErr;
        break;
    }
}

//при необходимости печатает содержащиеся в структуре коэффициенты
void PrintStructPolinom (struct Polinomial printingStruc)
{
    printf ("a = %lg, b = %lg, c = %lg\n", printingStruc.aP, printingStruc.bP, printingStruc.cP);
    printf ("x1 = %lg, x2 = %lg\n", printingStruc.x1, printingStruc.x2);
}


int IsDoubleZero (double iszero)
{
    return (fabs (iszero) < ACCURACY);
}


void PrintAns (struct Polinomial mainPol)
{   
    printf ("\nYour equation is %lgx^2 %+lgx %+lg = 0\n\n", mainPol.aP, mainPol.bP, mainPol.cP);

    switch (mainPol.nOfSol)
    {
    case 0:
        printf ("You have no answers \n");                            //discriminant  <0
        break;
    case 1:
        printf ("Your answer is " GREEN "%lg \n" RESET, mainPol.x1);  //линейное или квадратное с одним корнем
        break;
    case 2:
        printf ("Your first answer is " GREEN "%lg," RESET" and your second answer is" GREEN " %lg" RESET, mainPol.x1, mainPol.x2);  //квадратное
        break;
    case InfSol:
        printf ("Every x is an answer \n");                           //два старших коэффициента нулевые
        break;
    default:
        break;
    }
}

//отчистка буффера ввода
void BufferClean ()
{
    while (1)
    {
        int tempChar = getchar ();                                  
        if (tempChar == '\n' || tempChar == EOF)
        {
            break;
        }
    }
}


int ParseTest (double aRef, double bRef, double cRef, char testString [])
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

    if (a != aRef || b != bRef || c != cRef)
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



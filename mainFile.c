#include <assert.h>
#include <ctype.h>
#include <string.h>

#include "StructPol.h"      //структура Polinomial
#include "ConstValues.h" 
#include "Parser.h"         
#include "Solver.h"         
#include "SqTests.h"        //проверка решений квадратного уравнения
#include "TestParse.h"





enum Sol         
{
    InfSol = -1,     //количество решений -1 соответствует бесконечному числу решений
}; 


void PrintAns                       (struct Polynomial);
void PrintStructPolynomial          (struct Polynomial StructPoly);
void BufferClean                    ();
enum modeOfTestSolve mainMenu       ();
enum modeOfTestSolve TestSolveMenu  ();


int main ()
{  
    struct Polynomial mainPoly = {.aP = 0, .bP = 0, .cP = 0, .nOfSol = 0, .x1 = NAN, .x2 = NAN};

    int menuRes = mainMenu ();

    if (menuRes != SolveEqMod)   //выбор режима работы
    {
        return 0;
    }
    else  
    {
        printf ("Error code is %d", menuRes);
    }  

    BufferClean ();     //очистка буффера ввода после функций scanf

    if (ReadInputBuffer (&mainPoly.aP, &mainPoly.bP, &mainPoly.cP) == 0)    //возвращаемое значение - количество считаных коэффициентов
    {
        return 0;
    }

    mainPoly.nOfSol = SolveKv (mainPoly.aP, mainPoly.bP, mainPoly.cP, &mainPoly.x1, &mainPoly.x2);    
    PrintAns (mainPoly);                                                                         
}


enum modeOfTestSolve mainMenu ()
{
    printf ("enter 0 if you want to solve equation\n"
            "enter 1 if you want to start solver tests\n"
            "enter 2 if you want to start parse tests\n");

    int mode = -1;
    if (scanf ("%d", &mode) != 1)
    {
        printf (RED "INPUT ERROR! Error code is %d\n" RESET, InputErr);
        return ERROR;
    }

    if (mode < SolveEqMod || mode > TestParseMod)
    {
        printf (RED "INPUT ERROR! Error code is %d\n" RESET, InputErr);
        return ERROR;
    }

    switch (mode)
    {
    case SolveEqMod:
        return SolveEqMod;

    case TestSolveMod:
        TestSolveMenu ();    
        return TestSolveMod;
    case TestParseMod:
        RunParseTestsManual ();
        return TestParseMod;
    default:
        printf (RED "INPUT ERROR! Error code is %d\n" RESET, InputErr);
        return ERROR;
    }
}

enum modeOfTestSolve TestSolveMenu ()
{
    int mode = -1;
    printf ("enter 0 if you want to start auto tests with no roots\n"
                "enter 1 if you want to start auto tests with one root\n"
                "enter 2 if you want to start auto tests with two roots\n"
                "enter 3 if you want to start auto test with one linear root\n"
                "enter 4 if you want to start auto test with no linear root\n"
                "enter 5 if you want to start manual test\n");
        
    if (scanf ("%d", &mode) != 1)
    {
        printf (RED "INPUT ERROR! Error code is %d\n" RESET, InputErr);
        return ERROR;
    }

    int repeats = 0;
    int failCount = 0;
    if (mode < ManualTests) 
    {
        printf ("how many tests you want to do?\n");
        
        if (scanf ("%d", &repeats) != 1)
        {
            printf (RED "INPUT ERROR! Error code is %d\n" RESET, InputErr);
            return ERROR;
        }

        failCount = RunTestSolveAuto (repeats, mode);
        if (failCount != 0)
        {
            printf ("%d fail(s) in tests", failCount);
        }
        return AutoTests;
    }
    else if (mode == ManualTests)
    {
        failCount = RunTestSolveManual ();
        if (failCount != 0)
        {
            printf ("%d fail(s) in tests", failCount);
        }
        return ManualTests;
    }
    else 
    {
        printf (RED "INPUT ERROR! Error code is %d\n" RESET, InputErr);
        return ERROR;
    }
}

//при необходимости печатает содержащиеся в структуре коэффициенты
void PrintStructPolynomial (struct Polynomial StructPoly)
{
    printf ("a = %lg, b = %lg, c = %lg\n", StructPoly.aP, StructPoly.bP, StructPoly.cP);
    printf ("x1 = %lg, x2 = %lg\n", StructPoly.x1, StructPoly.x2);
}


void PrintAns (struct Polynomial mainPoly)
{   
    printf ("\nYour equation is %lgx^2 %+lgx %+lg = 0\n\n", mainPoly.aP, mainPoly.bP, mainPoly.cP);

    switch (mainPoly.nOfSol)
    {
    case 0:
        printf ("You have no answers \n");                            //discriminant  <0
        break;
    case 1:
        printf ("Your answer is " GREEN "%lg \n" RESET, mainPoly.x1);  //линейное или квадратное с одним корнем
        break;
    case 2:
        printf ("Your first answer is " GREEN "%lg," RESET" and your second answer is" GREEN " %lg" RESET, mainPoly.x1, mainPoly.x2);  //квадратное
        break;
    case InfSol:
        printf ("Every x is an answer \n");                           //все коэффициенты нулевые
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




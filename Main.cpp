/**
    \file
    \brief Program solve square equation

    This program solve square equation inputed like "1.2x^2 + 3.4x + 5 = 6.7x^2 + 8.9x + 0"
*/

#include "StructPoly.h"
#include "Colors.h"  
#include "Parser.cpp"
#include "Solver.cpp"   
#include "TestSolver.cpp"
#include "TestParser.cpp"

/**
    \brief Enum contain available modes of program work
 */
enum modeOfWork
{
    MainMenuErr = -1,
    SolveEqMod = 0,
    TestSolveMod = 1,
    TestParseMod = 2
};

/**
    \brief Enum contain error codes
 */
enum ErrorCodes
{
    InputErr = 100
};

/**
    \brief Print anwer(s) which got from solved equation

    Print inputed equation. Print roots depending on number of roots in parametre (possible values are contein in enum nOfRoots).
    If there are no real roots or their number not finit print it.
    \param[in] SolvedPoly Parameter conteins coeffitions, number of roots and their value of solved equation
*/
void PrintAns (struct Polynomial SolvedPoly);

/**
    \brief Print variables in struct Polynomial

    Print coeffitions aP, bP, cP, roots x1, x2 from polynomial in parameter.
    \param[in] StructPoly Polinom wich variables you need to print
*/
void PrintStructPolynomial (struct Polynomial StructPoly);

/**
    \brief Clean buffer from stdin
*/
void BufferClean ();

/**
    \brief Select mode of work

    Sekect mode of work depending on user's command. Can launche solve equation, tests for solver or parser 
    \return Return selected mode of work. All supported returning values are conteined in enum modeOfWork
*/
enum modeOfWork MainMenu ();

/**
    \brief Select solver test mode

    Sekect mode of work depending on user's command. Can launche manual testes or auto tests with different scenarios
    \return Return selected mode of tests. All supported returning values are conteined in enum modeOfTestSolve
 */
enum modeOfTestSolve TestSolveMenu ();


int main ()
{  
    struct Polynomial mainPoly = {.aP = 0, .bP = 0, .cP = 0, .x1 = NAN, .x2 = NAN, .nOfSol = 0};

    int menuRes = MainMenu ();

    if (menuRes != SolveEqMod)
    {
        return 0;
    }

    else  
    {
        printf ("Error code is %d", menuRes);
    }  

    BufferClean ();

    if (ParseInputCoef (&mainPoly.aP, &mainPoly.bP, &mainPoly.cP) == 0)
    {
        return 0;
    }

    mainPoly.nOfSol = SolveQuadratic (mainPoly.aP, mainPoly.bP, mainPoly.cP, &mainPoly.x1, &mainPoly.x2);    
    PrintAns (mainPoly);                                                                         
}


enum modeOfWork MainMenu ()
{
    printf ("enter 0 if you want to solve equation\n"
            "enter 1 if you want to start solver tests\n"
            "enter 2 if you want to start parse tests\n");

    int mode = -1;

    if (scanf ("%d", &mode) != 1)
    {
        printf (RED "INPUT ERROR! Error code is %d\n" RESET, InputErr);
        return MainMenuErr;
    }

    if (mode < SolveEqMod || mode > TestParseMod)
    {
        printf (RED "INPUT ERROR! Error code is %d\n" RESET, InputErr);
        return MainMenuErr;
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
        return MainMenuErr;
    }
}

enum modeOfTestSolve TestSolveMenu ()
{
    int mode = -1;

    printf ("enter 0 if you want to start manual test\n"
            "enter 1 if you want to start auto tests with no roots\n"
            "enter 2 if you want to start auto tests with one root\n"
            "enter 3 if you want to start auto tests with two roots\n"
            "enter 4 if you want to start auto test with one linear root\n"
            "enter 5 if you want to start auto test with no linear root\n");
        
    if (scanf ("%d", &mode) != 1)
    {
        printf (RED "INPUT ERROR! Error code is %d\n" RESET, InputErr);
        return SolveMenuErr;
    }

    int repeats = 0;
    int failCount = 0;

    if (mode == ManualTests) 
    {
        failCount = RunTestSolveManual ();

        if (failCount != 0)
        {
            printf ("%d fail(s) in tests", failCount);
        }

        return ManualTests;
    }

    // TODO: СДЕЛАТЬ ФУНКЦИИ ДЛЯ ИФОВ И ПОДОБНЫХ БЛОКОВ 
    
    else if (mode < _Max_mode)
    {
        printf ("how many tests you want to do?\n");
        
        if (scanf ("%d", &repeats) != 1)
        {
            printf (RED "INPUT ERROR! Error code is %d\n" RESET, InputErr);
            return SolveMenuErr;
        }

        if (repeats > 5000)
        {
            printf (RED "Repeats value is too big" RESET);
            return SolveMenuErr;
        }

        failCount = RunTestSolveAuto (repeats, mode);

        if (failCount != 0)
        {
            printf ("%d fail(s) in tests", failCount);
        }

        return AutoTests;
    }

    else 
    {
        printf (RED "INPUT ERROR! Error code is %d\n" RESET, InputErr);
        return SolveMenuErr;
    }
}


void PrintStructPolynomial (struct Polynomial StructPoly)
{
    printf ("a = %lg, b = %lg, c = %lg\n", StructPoly.aP, StructPoly.bP, StructPoly.cP);
    printf ("x1 = %lg, x2 = %lg\n", StructPoly.x1, StructPoly.x2);
}


void PrintAns (struct Polynomial SolvedPoly)
{   
    printf ("\nYour equation is %lgx^2 %+lgx %+lg = 0\n\n", SolvedPoly.aP, SolvedPoly.bP, SolvedPoly.cP);

    switch (SolvedPoly.nOfSol)
    {
    case NoRoots:
        printf ("You have no answers \n");
        break;

    case OneRoot:
        printf ("Your answer is " GREEN "%lg \n" RESET, SolvedPoly.x1); 
        break;

    case TwoRoots:
        printf ("Your first answer is " GREEN "%lg," RESET" and your second answer is" GREEN " %lg" RESET, SolvedPoly.x1, SolvedPoly.x2);
        break;

    case InfSol:
        printf ("Every x is an answer \n"); 
        break;

    default:
        break;
    }
}


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
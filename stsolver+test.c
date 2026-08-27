#include <assert.h>
#include <ctype.h>
#include <string.h>

#include "StructPol.h"      //структура Polinomial
#include "DefinesConst.h"   //дефайны констант
#include "Parser.h"         //парсинг строки с квадратными трехчленами
#include "Solver.h"         //решение квадратного уравнения с известными коэффициентами
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


void PrintAns            (struct Polinomial);       //функция выводит ответы
void PrintStructPolinom  (struct Polinomial);       
void BufferClean         ();
void menu                ();


int main ()
{  
    struct Polinomial mainPol = {.aP = 0, .bP = 0, .cP = 0, .nOfSol = 0, .Sign = 1, .x1 = NAN, .x2 = NAN};

    menu ();                                                                                    //выбар режима работы

    BufferClean ();                                                                             //очистка буффера ввода после функций scanf

    if(!ParseMain (&mainPol.aP, &mainPol.bP, &mainPol.cP))                                      //возвращаемое значение - количество считаных коэффициентов
    {
        return 0;
    }

    mainPol.nOfSol = SolveKv (mainPol.aP, mainPol.bP, mainPol.cP, &mainPol.x1, &mainPol.x2);    //запуск решения
    PrintAns (mainPol);                                                                         //запуск вывода ответа
}

//при необходимости печатает содержащиеся в структуре коэффициенты
void PrintStructPolinom (struct Polinomial printingStruc)
{
    printf ("a = %lg, b = %lg, c = %lg\n", printingStruc.aP, printingStruc.bP, printingStruc.cP);
    printf ("x1 = %lg, x2 = %lg\n", printingStruc.x1, printingStruc.x2);
}


//сравнение double с 0
int IsDoubleZero (double iszero)
{
    return (fabs (iszero) < ACCURACY);
}


//Функция выводит ответы
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







int ParseTest (double aRef, double bRef, double cRef)
{
    double a = NAN, b = NAN, c = NAN;
    

    ParseMain (&a, &b, &c);
    if (a != aRef || b != bRef || c != cRef)
    {
        printf ("Parsing test FAILED:\n"
                "Expected: aRef = %lg, bRef = %lg, cRef = %lg\n"
                "Got: a = %lg, b = %lg, c = %lg",
                aRef, bRef, cRef, a, b, c);
    }
}

/*

int RunParsTests ()
{
    struct ParseTestString test1 = {.testString = "1.4x^2 + 0.0x + 1 = 0.0x^2 + 7.5x\n", .aRef = 1.4, .bRef = 7.5, .cRef = 1};

    EmulateInputBuffer (test1.testString);
    Parsetest (test1.aRef, test1.bRef, test1.cRef);
}


void EmulateInputBuffer (const char *testingString)
{
    int len = strlen (testingString);

    for (int i = len - 1; i >= 0 ; i--)
    {
        ungetc(testingString[i], stdin);
    }
}*/
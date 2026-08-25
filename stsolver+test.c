#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>

struct Polinomial //cringenaming
{
    double aP, bP, cP;
    double x1, x2;
    int nOfSol;
    int Sign;
};


enum Tech
{
    EntReq = -2,      //запрос ввода при парсинге
    EntErr = -3,      //сообщение об ошибке формата ввода
    NoSignErr = -4    //забыт знак
};

enum Sol         
{
    InfSol = -1,     //количество решений -1 соответствует бесконечному числу решений
}; 


int SolveLinear     (double b, double c, double *x1);                              //поиск корня в случае нулевого старшего коэффициента квадратного уравнения
int SolveKv        (struct Polinomial*);                                          //функция поиска корней квадратного уравнения  с ненулевым старшим коэффициентом
void PrintAns      (struct Polinomial);                                           //функция выводит ответы
int ParsMain       (struct Polinomial*);                                          //парсинг пробелов и букв
void PrintTech     (int techCase);                                                //вывод технических сообщений
double ParsNum     (char Part [], int count, int *shift);                         //парсинг цифр
void SignChange    (double *a, int minus);                                        //при необходимости делает коэффициент отрицательным
int DeliteSpase    (char inString[]);                                             //удаление пробелов
void SeparatePol   (char inString [], char lEntPart [], char rEntPart []);
double* ChooseCoef (char selCoef [], double *a, double *b, double *c, int *isAC); //выбирает нужный коэф для последующей обработки
int ParsToCoef     (char EntPart [], struct Polinomial *parsPol);
void PrintStruc    (struct Polinomial);
int isDoubleZero   (double iszero);

//функции для теста
int SolverTest                 (struct Polinomial testPolRef);
int CheckX                     (double x, double a, double b, double c);
void RunSolveTestsAuto         (int repeats, int kindOfTest);
void RunSolveTestsManual       ();
double RandDouble              ();
void menu                      ();
void BufferClean               ();
void GenerateTwoRootsTest      (double *a, double *b, double *c);
void GenerateOneRootTest       (double *a, double *b, double *c);
void GenerateNoRootsTest       (double *a, double *b, double *c);
void GenerateOneRootsTestLinear(double *a, double *b, double *c);
void GenerateNoRootsTestLinear (double *a, double *b, double *c);


//цвета для вывода
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define RESET   "\033[0m"

#define ACCURACY 0.0001



//1.4x^2 + 0.0x + 1 = 0.0x^2 + 7.5x 
//12x + x4 - 16 = 0

int main ()
{  
    struct Polinomial mainPol = {.aP = 0, .bP = 0, .cP = 0, .nOfSol = 0, .Sign = 1, .x1 = NAN, .x2 = NAN};

    menu ();

    BufferClean ();

    if(!ParsMain (&mainPol))
    {
        return 0;
    }

    mainPol.nOfSol = SolveKv (&mainPol);           //запуск решения
    PrintAns (mainPol);            //запуск вывода ответа
}

void PrintStructPolinm (struct Polinomial printingStruc)
{
    printf ("a = %lg, b = %lg, c = %lg\n", printingStruc.aP, printingStruc.bP, printingStruc.cP);
    printf ("x1 = %lg, x2 = %lg\n", printingStruc.x1, printingStruc.x2);
}

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
            PrintTech (EntErr);
            return InfSol;
        }
        return 0;
    }
    *x1 = -c / b;
    return 1;
}


//функция поиска корней квадратного уравнения с ненулевым старшим коэффициентом
int SolveKv (struct Polinomial *mainPol)
{
    assert (mainPol != NULL);
    assert (isfinite ((*mainPol).aP));
    assert (isfinite ((*mainPol).bP));
    assert (isfinite ((*mainPol).cP));


    if (fabs ((*mainPol).aP) < ACCURACY)
    {
        return SolveLinear ((*mainPol).bP, (*mainPol).cP, &(*mainPol).x1);
    }

    double discriminant = (*mainPol).bP * (*mainPol).bP - 4 * (*mainPol).aP * (*mainPol).cP;
    double a2 = (*mainPol).aP * 2;

    if (discriminant > ACCURACY)
    {
        (*mainPol).x1 = (-(*mainPol).bP - sqrt(discriminant)) / a2;    //корни уравнения с положительным ненулевым D
        (*mainPol).x2 = (-(*mainPol).bP + sqrt(discriminant)) / a2;
        return 2;
    }
    else if (discriminant < -ACCURACY)
    {
        return 0;                   //случай с отсутствием действительных корней
    }

    (*mainPol).x1 = -(*mainPol).bP / a2;                    //discriminant = 0
    return 1;
    
}


int isDoubleZero (double iszero)
{
    if (fabs (iszero) < ACCURACY)
    {
        return 1;
    } 
    return 0;
}


//Функция выводит ответы
void PrintAns (struct Polinomial mainPol)
{   
    printf ("\nYour equation is %lgx^2 %+lgx %+lg = 0\n\n", mainPol.aP, mainPol.bP, mainPol.cP);

    switch (mainPol.nOfSol)
    {
    case 0:
        printf ("You have no answers \n");                        //discriminant  <0
        break;
    case 1:
        printf ("Your answer is " GREEN "%lg \n" RESET, mainPol.x1);                    //линейное или квадратное с одним корнем
        break;
    case 2:
        printf ("Your first answer is " GREEN "%lg," RESET" and your second answer is" GREEN " %lg" RESET, mainPol.x1, mainPol.x2);  //квадратное
        break;
    case InfSol:
        printf ("Every x is an answer \n");                      //два старших коэффициента нулевые
        break;
    default:
        break;
    }
}


//функция выводит технические сообщения
void PrintTech (int techCase)
{ 
    switch (techCase)
    {
    case EntReq:
        printf ("Enter equation like\n0.0x^2 + 0.0x + 0 = 0.0x^2 + 0.0x + 0\n\n");
        break;
    case EntErr:
        printf (RED "\nInput error! Try again!\n" RESET);
        break;
    case NoSignErr:
        printf (RED "\nYou forgot a sign! Try again!\n" RESET);
        break;
    default:
        break;
    }
}


//парсинг коэффициентов
int ParsMain (struct Polinomial *mainPol)
{
    assert (mainPol != NULL);

    PrintTech (EntReq); //запрос ввода

    char inString [40];
    char lEntPart [20];
    char rEntPart [20];
    int nParsedCoef = 0;
    
    if (DeliteSpase (inString) == 0)
    {
        return 0;
    } 


    SeparatePol (inString, lEntPart, rEntPart);


    struct Polinomial leftPol = {.aP = 0, .bP = 0, .cP = 0, .Sign = 1};
    struct Polinomial rightPol = {.aP = 0, .bP = 0, .cP = 0, .Sign = -1};

    nParsedCoef += ParsToCoef (lEntPart, &leftPol);
    nParsedCoef += ParsToCoef (rEntPart, &rightPol);
    
    (*mainPol).aP = leftPol.aP + rightPol.aP;
    (*mainPol).bP = leftPol.bP + rightPol.bP;
    (*mainPol).cP = leftPol.cP + rightPol.cP;


    return nParsedCoef;
}


int ParsToCoef (char EntPart [], struct Polinomial *parsPol)
{
    assert (parsPol != NULL);

    int nParsedCoef = 0;
    int shift = 0;
    int isA = 0; //1 - коэф а, 2 - коэф с

    int count = 0;
    
    char selCoef [2] = {' ', ' '};

    while (EntPart [count] != 0)     
    { 

        if (!isdigit(EntPart [count]) && EntPart [count] != 'x')
        {
            count++;
        }
        else if (isdigit(EntPart [count]))
        {
            double difference = ParsNum (EntPart, count, &shift);
            count += shift;
            selCoef [0] = EntPart [(count)]; //х или не х
            selCoef [1] = EntPart [(count + 1)]; //^ или не ^
            double *coefAdress = ChooseCoef (selCoef, &(*parsPol).aP, &(*parsPol).bP, &(*parsPol).cP, &isA);

            if (isA == 1) //смена x^2 на x^w для избежания ошибок парсинга
            {
                EntPart [(count + 2)] = 'w';
            }
            count -= shift;
            if (count > 0)
            {
                //сохранение знака
                if (EntPart [(count - 1)] == '-')
                {   
                    *coefAdress -= difference*(*parsPol).Sign;
                }
                else
                {
                    
                    *coefAdress +=  difference*(*parsPol).Sign;
                    
                }
            }
            else
            {
                *coefAdress += difference*(*parsPol).Sign;
            }
            count += (shift+1);
            shift = 0;
            nParsedCoef++;
            isA = 0;
        }
        else //x или x^2 без коэффициента
        {
            selCoef [0] = EntPart [(count)]; //х или не х
            selCoef [1] = EntPart [(count + 1)]; //^ или не ^
            double *coefAdress = ChooseCoef (selCoef, &(*parsPol).aP, &(*parsPol).bP, &(*parsPol).cP, &isA);

            if (isA)
            {
                EntPart [(count + 2)] = 'w';
            }

            *coefAdress += (*parsPol).Sign;
            count++;
            nParsedCoef++;
            isA = 0;
        }
    }
    return nParsedCoef;
}


//удаление пробелов
int DeliteSpase (char inString[])
{
    int countString = 0;
    char takenChar = ' '; //последний взятый чар

    while (takenChar != EOF && takenChar != '\n')
    { 
        if (takenChar == ' ')
        {
            if (countString > 0) //проверка на ошибку отсутствия знака
            {
                char charBefSpc = inString [(countString - 1)]; //символ перед пробелом
                while (takenChar == ' ')
                {
                    takenChar = getchar();
                }
                inString[countString] = takenChar; //сохранение символа после пробела
                char charAftSpc = inString[countString];
                
                if ((charBefSpc != '-' && charBefSpc != '+' && charBefSpc != '=') && (charAftSpc != '-' && charAftSpc != '+' && charAftSpc != '=' && charAftSpc != 0))
                {
                    PrintTech(NoSignErr);
                    return 0;
                }
            }
            else  //удаление ведущих пробелов
            {
                takenChar = getchar();
            }
        }
        else  //сохранение всего кроме ' '
        {
            inString [countString] = takenChar;
            inString [(countString + 1)] = 0;
            countString++;
            takenChar = getchar();
        }
    }
}


//разделение на левую и правую часть
void SeparatePol (char inString [], char lEntPart [], char rEntPart [])
{
    int partFlag = 0;
    int countIn = 0;
    int countOut = 0;

    //printf ("instring is %s.\n", inString);
    
    while (inString [countIn] != 0)
    {
        if (!partFlag)
        {   if (inString [countIn] != '=')
            {
                lEntPart [countOut] = inString [countIn];
                lEntPart [(countOut + 1)] = 0;
                countIn++;
                countOut++;
            }
            else
            {
                partFlag = 1;
                countOut = 0;
            }
        }
        else
        {
            rEntPart [countOut] = inString [countIn + 1];
            rEntPart [(countOut + 1)] = 0;
            countIn++;
            countOut++;
        }
    }
    //printf ("right part after while %s\n", rEntPart);
}


//парсинг цифр
double ParsNum ( char Part [], int count, int *shift)
{
    assert (shift != NULL);

    double coef = 0;
    int duCount = count;
    int place = -1;

    while (isdigit(Part [duCount]))
    {
        place++;
        duCount++;
    }

    int multiplier = pow (10, place);

    while ((isdigit(Part [count])  || Part [count] == ',' || Part [count] == '.') && Part [count] != 0)
    {
        if (Part [count] == ',' || Part [count] == '.')
        {
            ++count;
            ++*shift; 
        }
        else
        {
            coef += (Part [count]  - '0') * multiplier;
            ++count;
            ++*shift;
            multiplier /= 10;
        }
        
    }
    return coef;
}


double* ChooseCoef (char selCoef[], double *a, double *b, double *c, int *isA)
{
    assert (a != NULL);
    assert (b != NULL);
    assert (c != NULL);
    assert (isA != NULL);

    if (selCoef [0] == 'x')
    {
        if (selCoef [1] == '^')
        {
            *isA = 1;
            return a;
        }
        return b;
    }
    else
    {
        return c;
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

int SolverTest (struct Polinomial testPolRef)
{
    struct Polinomial testPol = {.aP = testPolRef.aP, .bP = testPolRef.bP, .cP = testPolRef.cP};
    testPol.nOfSol = SolveKv (&testPol);
    if (testPolRef.nOfSol == 2 && (CheckX (testPol.x1, testPolRef.aP, testPolRef.bP, testPolRef.cP) || CheckX (testPol.x2, testPolRef.aP, testPolRef.bP, testPolRef.cP)) && testPolRef.nOfSol == testPol.nOfSol)
    {
        printf (RED "Test FAILED! Wrong answer!:\n" RESET
                "a = " RED "%lg, " RESET "b = " RED "%lg, " RESET "c = " RED "%lg" RESET "\n"
                "got: numOfSol = " RED "%d, " RESET "x1 = " RED "%lg, " RESET "x2 = " RED "%lg\n" RESET
                , testPolRef.aP, testPolRef.bP, testPolRef.cP, testPol.nOfSol, testPol.x1, testPol.x2);
        return 1;
    }
    if (CheckX (testPol.x1, testPolRef.aP, testPolRef.bP, testPolRef.cP) && testPolRef.nOfSol == 1 && testPolRef.nOfSol == testPol.nOfSol)
    {
        printf (RED "Test FAILED! Wrong answer!:\n" RESET
                "a = " RED "%lg, " RESET "b = " RED "%lg, " RESET "c = " RED "%lg" RESET "\n"
                "got: numOfSol = " RED "%d, " RESET "x1 = " RED "%lg, " RESET "x2 = " RED "%lg\n" RESET
                , testPolRef.aP, testPolRef.bP, testPolRef.cP, testPol.nOfSol, testPol.x1, testPol.x2);
        return 1;
    }
    else if (testPolRef.nOfSol == 0 && testPolRef.nOfSol == testPol.nOfSol)
    {
        printf(GREEN"Test PASSED\n" RESET);
        return 0;
    }
    else if (testPolRef.nOfSol != testPol.nOfSol)
    {
        printf (RED "Test FAILED! Wrong nOfSol:\n" RESET
                "a = " RED "%lg, " RESET "b = " RED "%lg, " RESET "c = " RED "%lg" RESET "\n"
                "got: numOfSol = " RED "%d, " RESET "expected: numOfSOl = " RED "%d\n"  RESET
                , testPolRef.aP, testPolRef.bP, testPolRef.cP, testPol.nOfSol, testPolRef.nOfSol);
        return 1;
    }
    printf(GREEN"Test PASSED\n" RESET);
    return 0;
}


int CheckX (double x, double a, double b, double c)
{
    if (fabs(x*x*a + x*b +c) <= ACCURACY)
    {
        return 0;
    }
    return 1;
}


void RunSolveTestsAuto (int repeats, int kindOfTest)
{   
    int countFail = 0;
    switch (kindOfTest)
    {
    case 0:
        while (repeats)
        {
            struct Polinomial testPolRef = {.nOfSol = 0};
            GenerateNoRootsTest(&testPolRef.aP, &testPolRef.bP, &testPolRef.cP);
            countFail += SolverTest (testPolRef);
            --repeats;
        }
        printf ("%d tests had been failed\n", countFail);
        abort();
    case 1:
        while (repeats)
        {
            struct Polinomial testPolRef = {.nOfSol = 1};
            GenerateOneRootTest(&testPolRef.aP, &testPolRef.bP, &testPolRef.cP);
            countFail += SolverTest (testPolRef);
            --repeats;
        }
        printf ("%d tests had been failed\n", countFail);
        abort();
    case 2:
        while (repeats)
        {
            struct Polinomial testPolRef = {.nOfSol = 2};
            GenerateTwoRootsTest(&testPolRef.aP, &testPolRef.bP, &testPolRef.cP);
            countFail += SolverTest (testPolRef);
            --repeats;
        }
        printf ("%d tests had been failed\n", countFail);
        abort();
    case 3:
        while (repeats)
        {
            struct Polinomial testPolRef = {.nOfSol = 1};
            GenerateOneRootsTestLinear(&testPolRef.aP, &testPolRef.bP, &testPolRef.cP);
            countFail += SolverTest (testPolRef);
            --repeats;
        }
        printf ("%d tests had been failed\n", countFail);
        abort();
    case 4:
        while (repeats)
        {
            struct Polinomial testPolRef = {.nOfSol = 0};
            GenerateNoRootsTestLinear(&testPolRef.aP, &testPolRef.bP, &testPolRef.cP);
            countFail += SolverTest (testPolRef);
            --repeats;
        }
        printf ("%d tests had been failed\n", countFail);
        abort();
    }
}

void RunSolveTestsManual ()
{
    struct Polinomial testPolRef = {.aP = 1, .bP = 2.2, .cP = 10, .nOfSol = 0};
    SolverTest (testPolRef);
    abort();
}


double RandDouble ()
{
    int maxDoubleRange = 10;
    double randD = (double)rand() / RAND_MAX * maxDoubleRange;
    int randI = rand();
    if (randI > RAND_MAX/2)
    {
        randD *= -1;
    }
    return randD;
}


void menu ()
{
    printf ("enter 0 if you want to solve equation\n"
            "enter 1 if you want to start tests\n");
    int switcher = 0;
    if (scanf ("%d", &switcher) == 0)
        {
            printf (RED"INPUT ERROR!\n"RESET);
            abort();
        }
    switch (switcher)
    {
    case 0:
        break;
    case 1:
        printf ("enter 0 if you want to start auto tests with no roots\n"
                "enter 1 if you want to start auto tests with one root\n"
                "enter 2 if you want to start auto tests with two roots\n"
                "enter 3 if you want to start auto test with one linear root\n"
                "enter 4 if you want to start auto test with no linear root\n"
                "enter 5 if you want to start manual test\n");
        
        if (!(scanf ("%d", &switcher)))
        {
            printf (RED"INPUT ERROR!\n"RESET);
            abort();
        }

        if (isdigit(switcher) && switcher < 5) 
        {
            printf ("how many tests you want to do?\n");
        }
        int repeats = 0;
        if (!(scanf ("%d", &repeats)))
        {
            printf (RED"INPUT ERROR!\n"RESET);
            abort();
        }

        switch (switcher)
        {
        case 0:
            RunSolveTestsAuto (repeats, 0);
            break;
        case 1:
            RunSolveTestsAuto (repeats, 1);
        case 2:
            RunSolveTestsAuto (repeats, 2);
        case 3:
            RunSolveTestsAuto (repeats, 3);
        case 4:
            RunSolveTestsAuto (repeats, 4);
        case 5:
            RunSolveTestsManual ();
        default:
            printf ("INPUT ERROR!\n"RESET);
            abort();
        }

        break;
    default:
        printf(RED"INPUT ERROR!\n"RESET);
        break;
    }
}


void GenerateTwoRootsTest (double *a, double *b, double *c)
{
    double x1Ref = RandDouble();
    double x2Ref = RandDouble();
    double aRef = RandDouble();

    *a = aRef;
    *b = -aRef * (x1Ref+x2Ref);
    *c = aRef * x1Ref * x2Ref;
}


void GenerateOneRootTest (double *a, double *b, double *c)
{
    double x1Ref = RandDouble();
    double aRef = RandDouble();

    *a = aRef;
    *b = -aRef * 2 * x1Ref;
    *c = aRef * x1Ref * x1Ref;
}


void GenerateNoRootsTest (double *a, double *b, double *c)
{
    double x1Ref = RandDouble();
    double aRef = RandDouble();
    double cRef = RandDouble();

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
    *a = 0;
    *b = 0;
    *c = RandDouble();
}


void GenerateOneRootsTestLinear (double *a, double *b, double *c)
{
    *a = 0;
    *b = RandDouble();
    *c = RandDouble();
}


/*int ParsTest (char coefOrderRef[4], double aRef, double bRef, double cRef)
{
    double a = NAN, b = NAN, c = NAN;
    ParsMain (&a, &b, &c, testString);
    if (a != aRef || b != bRef || c != cRef)
    {
        printf ("Parsing test FAILED:\n"
                "Expected: orderRef - %s, aRef = %lg, bRef = %lg, cRef = %lg\n"
                "Got: order - %s, a = %lg, b = %lg, c = %lg",
                coefOrderRef, aRef, bRef, cRef, coefOrder, a, b, c);
    }
}



int RunParsTests ()
{

}*/

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

struct Polinomial //cringenaming
{
    double aP, bP, cP;
    double x1, x2;
    int nOfSol;
    int Sign;
};


struct ParseTestString 
{
    char *testString;
    double aRef, bRef, cRef;
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


int SolveLinear          (double b, double c, double *x1);                              //поиск корня в случае нулевого старшего коэффициента квадратного уравнения
int SolveKv              (double a, double b, double c, double *x1, double *x2);        //функция поиска корней квадратного уравнения  с ненулевым старшим коэффициентом
void PrintAns            (struct Polinomial);                                           //функция выводит ответы
int ParseMain            (double *a, double *b, double *c);                             //парсинг пробелов и букв
void PrintTech           (int techCase);                                                //вывод технических сообщений
double ParseNum          (char Part [], int count, int *shift);                         //парсинг цифр
int DeliteSpase          (char inString[]);                                             //удаление пробелов
void SeparatePol         (char inString [], char lEntPart [], char rEntPart []);
double* ChooseCoef       (char isX, char isExp, double *a, double *b, double *c, int *isA);     //выбирает нужный коэф для последующей обработки
int ParseToCoef          (char EntPart [], struct Polinomial *parsPol);
void PrintStructPolinom  (struct Polinomial);
int IsDoubleZero         (double iszero);

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

    if(!ParseMain (&mainPol.aP, &mainPol.bP, &mainPol.cP))
    {
        return 0;
    }

    mainPol.nOfSol = SolveKv (mainPol.aP, mainPol.bP, mainPol.cP, &mainPol.x1, &mainPol.x2);           //запуск решения
    PrintAns (mainPol);            //запуск вывода ответа
}

void PrintStructPolinom (struct Polinomial printingStruc)
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
            PrintTech (EntErr); //переделать сообщение
            return InfSol;
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
int ParseMain (double *a, double *b, double *c)
{
    assert (a != NULL);
    assert (b != NULL);
    assert (c != NULL);

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

    nParsedCoef += ParseToCoef (lEntPart, &leftPol);
    nParsedCoef += ParseToCoef (rEntPart, &rightPol);
    
    *a = leftPol.aP + rightPol.aP;
    *b = leftPol.bP + rightPol.bP;
    *c = leftPol.cP + rightPol.cP;


    return nParsedCoef;
}


int ParseToCoef (char EntPart [], struct Polinomial *parsPol)
{
    assert (parsPol != NULL);

    int nParsedCoef = 0;
    int shift = 0;
    int isA = 0; //1 - коэф а, 2 - коэф с

    int count = 0;
    
    char isX = ' ';
    char isExp = ' ';

    while (EntPart [count] != 0)     
    { 

        if (!isdigit(EntPart [count]) && EntPart [count] != 'x')
        {
            count++;
        }
        else if (isdigit(EntPart [count]))
        {
            double parsedDouble = ParseNum (EntPart, count, &shift);
            count += shift;
            isX = EntPart [(count)]; //х или не х
            isExp = EntPart [(count + 1)]; //^ или не ^
            double *coefAdress = ChooseCoef (isX, isExp, &(*parsPol).aP, &(*parsPol).bP, &(*parsPol).cP, &isA);

            double difference = parsedDouble * (*parsPol).Sign;
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
                    *coefAdress -= difference;
                }
                else
                {
                    *coefAdress +=  difference;
                }
            }
            else
            {
                *coefAdress += difference;
            }
            count += (shift + 1);
            shift = 0;
            nParsedCoef++;
            isA = 0;
        }
        else //x или x^2 без коэффициента
        {
            isX = EntPart [(count)]; //х или не х
            isExp = EntPart [(count + 1)]; //^ или не ^
            double *coefAdress = ChooseCoef (isX, isExp, &(*parsPol).aP, &(*parsPol).bP, &(*parsPol).cP, &isA);

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

    const char *allowed = "0123456789.,x^=+- ";

    while (takenChar != EOF && takenChar != '\n' && takenChar != 0)
    { 
        if (strchr (allowed, takenChar) == NULL)
        {
            if (countString != 0)
            {
                printf (RED"ERROR! Unsupported symbol \'%c\' after %c"RESET, takenChar, inString [countString - 1]);
                abort ();
            }
            else
            {
                printf (RED"ERROR! First symbol is unsupported"RESET);
            }
        }

        if (takenChar == ' ')
        {   
            if (countString > 0) //проверка на ошибку отсутствия знака
            {
                char charBefSpc = inString [(countString - 1)]; //символ перед пробелом
                while (takenChar == ' ')
                {
                    takenChar = getchar ();
                }
                if (takenChar == '\n')
                {
                    return 1;
                }
                inString [countString] = takenChar; //сохранение символа после пробела
                inString [(countString + 1)] = 0;
                char charAftSpc = inString[countString];
                countString++;
                takenChar = getchar (); //не было, но как будто нужно. стоит разобраться

                if ((charBefSpc != '-' && charBefSpc != '+' && charBefSpc != '=') &&
                   (charAftSpc != '-' && charAftSpc != '+' && charAftSpc != '=' && charAftSpc != 0))
                {
                    printf (RED"ERROR! No sign after \'%c\', before \'%c\'"RESET, inString [countString - 1], charAftSpc);
                    abort ();
                }
            }
            else  //удаление ведущих пробелов
            {
                takenChar = getchar ();
            }
        }
        else  //сохранение всего кроме ' '
        {
            inString [countString] = takenChar;
            inString [(countString + 1)] = 0;
            countString++;
            takenChar = getchar ();
        }
    }
    return 1;
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
double ParseNum ( char Part [], int count, int *shift)
{
    assert (shift != NULL);

    double coef = 0;
    int duCount = count;
    int place = -1;

    while (isdigit (Part [duCount]))
    {
        place++;
        duCount++;
    }

    double multiplier = pow (10, place);

    while ((isdigit (Part [count])  || Part [count] == ',' || Part [count] == '.') && Part [count] != 0)
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


double* ChooseCoef (char isX, char isExp, double *a, double *b, double *c, int *isA)
{
    assert (a != NULL);
    assert (b != NULL);
    assert (c != NULL);
    assert (isA != NULL);
    assert (*isA == 0);

    if (isX == 'x')
    {
        if (isExp == '^')
        {
            *isA = 1;
            return a;
        }
        if (isdigit (isExp))
        {
            printf (RED"ERROR! Wrong symbol after x"RESET);
            abort ();
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
    testPol.nOfSol = SolveKv (testPol.aP, testPol.bP, testPol.cP, &testPol.x1, &testPol.x2);
    if (testPolRef.nOfSol == 2 && 
        (CheckX (testPol.x1, testPolRef.aP, testPolRef.bP, testPolRef.cP) ||
        CheckX (testPol.x2, testPolRef.aP, testPolRef.bP, testPolRef.cP)) 
        && testPolRef.nOfSol == testPol.nOfSol)
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
        printf (GREEN"Test PASSED\n" RESET);
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
    printf (GREEN"Test PASSED\n" RESET);
    return 0;
}


int CheckX (double x, double a, double b, double c)
{
    return IsDoubleZero (a * x * x + b * x + c);
}


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
            GenerateNoRootsTest(&testPolRef.aP, &testPolRef.bP, &testPolRef.cP);
            countFail += SolverTest (testPolRef);
            --repeats;
        }
        printf ("%d tests had been failed\n", countFail);
        abort ();
    case 1:
        while (repeats)
        {
            testPolRef.nOfSol = 1;
            GenerateOneRootTest(&testPolRef.aP, &testPolRef.bP, &testPolRef.cP);
            countFail += SolverTest (testPolRef);
            --repeats;
        }
        printf ("%d tests had been failed\n", countFail);
        abort ();
    case 2:
        while (repeats)
        {
            testPolRef.nOfSol = 2;
            GenerateTwoRootsTest(&testPolRef.aP, &testPolRef.bP, &testPolRef.cP);
            countFail += SolverTest (testPolRef);
            --repeats;
        }
        printf ("%d tests had been failed\n", countFail);
        abort ();
    case 3:
        while (repeats)
        {
            testPolRef.nOfSol = 1;
            GenerateOneRootsTestLinear(&testPolRef.aP, &testPolRef.bP, &testPolRef.cP);
            countFail += SolverTest (testPolRef);
            --repeats;
        }
        printf ("%d tests had been failed\n", countFail);
        abort ();
    case 4:
        while (repeats)
        {
            testPolRef.nOfSol = 0;
            GenerateNoRootsTestLinear(&testPolRef.aP, &testPolRef.bP, &testPolRef.cP);
            countFail += SolverTest (testPolRef);
            --repeats;
        }
        printf ("%d tests had been failed\n", countFail);
        abort ();
    }
}

void RunSolveTestsManual ()
{
    struct Polinomial testsPolinomsRef [3] = 
    {
        {.aP = 1, .bP = 2.2, .cP = 1.21, .nOfSol = 1},
        {.aP = 0, .bP = 0, .cP = 0, .nOfSol = InfSol},
        {.aP = 0, .bP = 0, .cP = 1, .nOfSol = 0}
    };
    unsigned int size = sizeof (testsPolinomsRef)/sizeof (struct Polinomial);
    for (unsigned int i = 0; i < size; i++)
    {
        SolverTest (testsPolinomsRef[0]);
    }
    abort ();
}


double RandDouble ()
{
    int maxDoubleRange = 10; //реальное максимальное значение меньше на minDoubleRange
    int minDoubleRange = -5;
    double randD = (double)rand () / RAND_MAX * maxDoubleRange + minDoubleRange;
    return randD;
}


void menu ()
{
    printf ("enter 0 if you want to solve equation\n"
            "enter 1 if you want to start tests\n");

    int switcher = 10;
    scanf ("%d", &switcher);

    if (switcher != 0 && switcher != 1)
    {
        printf (RED"INPUT ERROR!\n"RESET);
        abort ();
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
            abort ();
        }

        int repeats = 0;
        if (switcher < 5) 
        {
            printf ("how many tests you want to do?\n");
            
            if (!(scanf ("%d", &repeats)))
            {
                printf (RED"INPUT ERROR!\n"RESET);
                abort ();
            }
        }

        if (switcher < 5)
        {
            RunSolveTestsAuto (repeats, switcher);
        }
        else if (switcher == 5)
        {
            RunSolveTestsManual ();
        }
        else 
        {
            printf ("INPUT ERROR!\n"RESET);
            abort ();
        }

        break;
    default:
        printf(RED"INPUT ERROR!\n"RESET);
        break;
    }
}


void GenerateTwoRootsTest (double *a, double *b, double *c)
{
    double x1Ref = RandDouble ();
    double x2Ref = RandDouble ();
    double aRef = RandDouble ();

    *a = aRef;
    *b = -aRef * (x1Ref+x2Ref);
    *c = aRef * x1Ref * x2Ref;
}


void GenerateOneRootTest (double *a, double *b, double *c)
{
    double x1Ref = RandDouble ();
    double aRef = RandDouble ();

    *a = aRef;
    *b = -aRef * 2 * x1Ref;
    *c = aRef * x1Ref * x1Ref;
}


void GenerateNoRootsTest (double *a, double *b, double *c)
{
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
    *a = 0;
    *b = 0;
    *c = RandDouble ();
}


void GenerateOneRootsTestLinear (double *a, double *b, double *c)
{
    *a = 0;
    *b = RandDouble ();
    *c = RandDouble ();
}


int ParseTest (double aRef, double bRef, double cRef)
{
    double a = NAN, b = NAN, c = NAN;
    

    ParsMain (&a, &b, &c);
    if (a != aRef || b != bRef || c != cRef)
    {
        printf ("Parsing test FAILED:\n"
                "Expected: aRef = %lg, bRef = %lg, cRef = %lg\n"
                "Got: a = %lg, b = %lg, c = %lg",
                aRef, bRef, cRef, a, b, c);
    }
}



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
}
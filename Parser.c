#include "StructPol.h"
#include "Parser.h"

#include <assert.h>
#include <string.h>
#include <ctype.h>


int ReadInputBuffer (double *a, double *b, double *c)
{
    printf ("Enter equation like\n0.0x^2 + 0.0x + 0 = 0.0x^2 + 0.0x + 0\n\n"); //запрос ввода

    char inString [40];

    assert (a != NULL);
    assert (b != NULL);
    assert (c != NULL);

    int tempChar = getchar ();
    int count = 0;
    int isFirst = 0;
    const char *allowed = "0123456789.,x^=+- ";
    while (tempChar != EOF && tempChar != '\n' && tempChar != 0)
    {
        if (strchr (allowed, tempChar) == NULL)
        {
            if (isFirst != 0)
            {
                printf (RED"ERROR! Unsupported symbol \'%c\' after %c"RESET, tempChar, inString [count]);
                abort ();
            }
            else
            {
                printf (RED"ERROR! First symbol is unsupported"RESET);
            }
        }
        inString [count] = tempChar;
        inString [count + 1] = 0;
        isFirst++;
        count++;
        tempChar = getchar ();
    }
    return ParseMain (a, b, c, inString);
}


//Вызов всех функций парсинга
int ParseMain (double *a, double *b, double *c, char inString [])
{
    assert (a != NULL);
    assert (b != NULL);
    assert (c != NULL);
    
    char lEntPart [20];
    char rEntPart [20];
    int nParsedCoef = 0;
    
    //ReadInputBuffer (inString);  //считывание строки из буффера ввода

    if (DeliteSpase (inString) == 0)    //удаление пробелов, проверка корректности ввода
    {
        printf (RED"err"RESET);
        return 0;
    } 

    SeparatePol (inString, lEntPart, rEntPart); //разделение выражения на части до и после '='


    struct Polinomial leftPol = {.aP = 0, .bP = 0, .cP = 0, .Sign = 1};
    struct Polinomial rightPol = {.aP = 0, .bP = 0, .cP = 0, .Sign = -1};

    nParsedCoef += ParseToCoef (lEntPart, &leftPol);
    nParsedCoef += ParseToCoef (rEntPart, &rightPol);
    
    *a = leftPol.aP + rightPol.aP;
    *b = leftPol.bP + rightPol.bP;
    *c = leftPol.cP + rightPol.cP;


    return nParsedCoef;
}

//удаление пробелов, проверка ошибок ввода
int DeliteSpase (char String [])
{
    assert (String != NULL);

    int countInString = 0;
    int countOutString = 0;
    char cBeforeSpace = '0';
    char cAfterSpace = '0';

    while (String [countInString] != 0)
    {
        if (String [countInString] == ' ')
        {
            if (countOutString != 0) //если пробел не ведущий
            {
                cBeforeSpace = String [countOutString - 1];
                while (String [countInString] == ' ') //удаление идущих подряд пробелов
                {
                    countInString++;
                }
                cAfterSpace = String [countInString];
                if ((cBeforeSpace != '-' && cBeforeSpace != '+' && cBeforeSpace != '=') &&                  //проверка на отсутствие знака
                   (cAfterSpace != '-' && cAfterSpace != '+' && cAfterSpace != '=' && cAfterSpace != 0))
                {
                    printf (RED"ERROR! No sign after \'%c\', before \'%c\'"RESET, cBeforeSpace, cAfterSpace);
                    abort ();
                }
                else
                {
                    String [countOutString] = String [countInString];
                    countInString++;
                    countOutString++;
                }
            } 
            else //удаление ведущих пробелов
            {
                countInString++;
            }
        }
        else //сохранение всего кроме символа ' '
        {
            String [countOutString] = String [countInString];
            countInString++;
            countOutString++;
           
        }
    }
    String [countOutString] = 0;
    return 1;
}



//разделение на левую и правую часть
void SeparatePol (char inString [], char lEntPart [], char rEntPart [])
{
    assert (inString != NULL);
    assert (lEntPart != NULL);
    assert (rEntPart != NULL);

    int partFlag = 0;
    int countIn = 0;
    int countOut = 0;
    
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
}


//парсинг строки до появления цифры
int ParseToCoef (char EntPart [], struct Polinomial *parsPol)
{
    assert (parsPol != NULL);
    assert (EntPart != NULL);

    int nParsedCoef = 0;
    int shift = 0;
    int isA = 0; //1 - коэф а, 2 - коэф с

    int count = 0;
    
    char isX = ' ';
    char isExp = ' ';

    while (EntPart [count] != 0)     
    { 
        if (!isdigit (EntPart [count]) && EntPart [count] != 'x')
        {
            count++;
        }
        else if (isdigit (EntPart [count]))
        {
            double parsedDouble = ParseNum (EntPart, count, &shift);
            count += shift;
            isX = EntPart [(count)];        //х или не х
            isExp = EntPart [(count + 1)];  //^ или не ^
            double *coefAdress = ChooseCoef (isX, isExp, &(*parsPol).aP, &(*parsPol).bP, &(*parsPol).cP, &isA);

            double difference = parsedDouble * (*parsPol).Sign;
            if (isA == 1)                   //смена x^2 на x^w для избежания ошибок парсинга
            {
                EntPart [(count + 2)] = 'w';
            }
            count -= shift;

            if (count > 0)
            {
                //выбор знака
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
        else //x или x^2 без коэффициента (+x = +1x)
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


//парсинг цифр, запись значения в коэффициенты
double ParseNum (char Part [], int count, int *shift)
{
    assert (shift != NULL);
    assert (Part != 0);

    double coef = 0;
    int duCount = count;
    int place = -1;

    while (isdigit (Part [duCount])) //определение разряда первой цифры коэффициента
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


//выбор считываемого коэфиициента
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
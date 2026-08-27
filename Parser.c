#include "StructPol.h"
#include "Parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>


//парсинг коэффициентов
int ParseMain (double *a, double *b, double *c)
{
    assert (a != NULL);
    assert (b != NULL);
    assert (c != NULL);

    printf ("Enter equation like\n0.0x^2 + 0.0x + 0 = 0.0x^2 + 0.0x + 0\n\n"); //запрос ввода

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
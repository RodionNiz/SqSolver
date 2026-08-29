#include "StructPoly.h"
#include "Parser.h"

#include <assert.h>
#include <string.h>
#include <ctype.h>


int ParseInputCoef (double *a, double *b, double *c)
{
    printf ("Enter equation like\n1.2x^2 + 3.4x + 5 = 6.7x^2 + 8.9x + 0\n\n");

    char inString [InputStrLen];

    assert (a != NULL);
    assert (b != NULL);
    assert (c != NULL);

    int tempChar = getchar ();
    unsigned int count = 0;
    int isFirst = 0;
    const char *allowed = "0123456789.,x^=+- ";

    while (tempChar != EOF && tempChar != '\n' && tempChar != 0)
    {
        if (strchr (allowed, tempChar) == NULL)
        {
            if (isFirst != 0)
            {
                printf (RED "ERROR! Unsupported symbol \'%c\' after %c" RESET, tempChar, inString [count]);
            }

            else
            {
                printf (RED "ERROR! First symbol is unsupported" RESET);
            }

            return -1;
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
    
    char leftPart [PolyStrLen];
    char rightPart [PolyStrLen];
    int nParsedCoef = 0;

    if (DeleteSpace (inString) == 0)
    {
        abort ();
    } 

    SeparateEq (inString, leftPart, rightPart);

    struct Polynomial leftPoly = {.aP = 0, .bP = 0, .cP = 0};
    struct Polynomial rightPoly = {.aP = 0, .bP = 0, .cP = 0};

    nParsedCoef += ParseToCoef (leftPart,  &leftPoly.aP,  &leftPoly.bP,  &leftPoly.cP);
    nParsedCoef += ParseToCoef (rightPart, &rightPoly.aP, &rightPoly.bP, &rightPoly.cP);
    
    *a = leftPoly.aP - rightPoly.aP;
    *b = leftPoly.bP - rightPoly.bP;
    *c = leftPoly.cP - rightPoly.cP;

    return nParsedCoef;
}


int DeleteSpace (char String [])
{
    assert (String != NULL);

    unsigned int countIn = 0;
    unsigned int countOut = 0;
    char cBeforeSpace = '0';
    char cAfterSpace = '0';

    while (String [countIn] != 0)
    {
        if (String [countIn] == ' ')
        {
            if (countOut != 0) //если пробел не ведущий
            {
                cBeforeSpace = String [countOut - 1];
                
                while (String [countIn] == ' ') //удаление идущих подряд пробелов
                {
                    countIn++;
                }

                cAfterSpace = String [countIn];

                if ((cBeforeSpace != '-' && cBeforeSpace != '+' && cBeforeSpace != '=') &&    //проверка на отсутствие знака
                    (cAfterSpace  != '-' && cAfterSpace  != '+' && cAfterSpace  != '='  && cAfterSpace != 0))
                {
                    printf (RED "ERROR! No sign after \'%c\', before \'%c\'" RESET, cBeforeSpace, cAfterSpace);
                    return 0;
                }
                
                else
                {
                    String [countOut] = String [countIn];
                    countIn++;
                    countOut++;
                }
            } 

            else //удаление ведущих пробелов
            {
                countIn++;
            }
        }

        else
        {
            String [countOut] = String [countIn];
            countIn++;
            countOut++;
        }
    }
    
    String [countOut] = 0;
    return 1;
}


void SeparateEq (char inString [], char leftPart [], char rightPart [])
{
    assert (inString != NULL);
    assert (leftPart != NULL);
    assert (rightPart != NULL);

    int isLeft = 1;
    unsigned int countIn = 0;
    unsigned int countOut = 0;
    
    while (inString [countIn] != 0)
    {
        if (isLeft)
        {   
            if (inString [countIn] != '=')
            {
                leftPart [countOut] = inString [countIn];
                rightPart [(countOut + 1)] = 0;
                countIn++;
                countOut++;
            }

            else
            {
                isLeft = 0;
                countOut = 0;
            }
        }
        else
        {
            rightPart [countOut] = inString [countIn + 1];
            rightPart [(countOut + 1)] = 0;
            countIn++;
            countOut++;
        }
    }
}


int ParseToCoef (char Part [], double *a, double *b, double *c)
{
    assert (a != NULL);
    assert (b != NULL);
    assert (c != NULL);
    assert (Part != NULL);

    int nParsedCoef = 0;
    int shift = 0;
    int isA = 0; 

    unsigned int count = 0;
    
    char isX = 0;
    char isPow = 0;

    while (Part [count] != 0)     
    { 
        if (!isdigit (Part [count]) && Part [count] != 'x')
        {
            count++;
        }

        else if (isdigit (Part [count]))
        {
            double difference = ParseNum (Part, count, &shift);

            count += shift;
            isX = Part [(count)];
            isPow = Part [(count + 1)];

            double *coefPtr = ChooseCoef (isX, isPow, a, b, c, &isA);

            if (isA == 1)  //смена x^2 на x^w для избежания ошибок парсинга
            {
                Part [(count + 2)] = 'w';
            }

            count -= shift;

            if (count > 0)
            {
                //выбор знака
                if (Part [(count - 1)] == '-')
                {   
                    *coefPtr -= difference;
                }

                else
                {
                    *coefPtr +=  difference;
                }
            }

            else
            {
                *coefPtr += difference;
            }

            count += (shift + 1);
            shift = 0;
            nParsedCoef++;
            isA = 0;
        }

        else //x или x^2 без коэффициента (+x = +1x)
        {
            isX = Part [(count)];
            isPow = Part [(count + 1)];

            double *coefPtr = ChooseCoef (isX, isPow, a, b, c, &isA);

            if (isA)
            {
                Part [(count + 2)] = 'w';
            }

            *coefPtr += 1;
            count++;
            nParsedCoef++;
            isA = 0;
        }
    }
    return nParsedCoef;
}


double ParseNum (char Part [], int count, int *shift)
{
    assert (shift != NULL);
    assert (Part != 0);

    double coef = 0;
    unsigned int countCopy = count;
    int place = -1;

    while (isdigit (Part [countCopy]))
    {
        place++;
        countCopy++;
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


double* ChooseCoef (char isX, char isPow, double *a, double *b, double *c, int *isA)
{
    assert (a != NULL);
    assert (b != NULL);
    assert (c != NULL);
    assert (isA != NULL);
    assert (*isA == 0);

    if (isX == 'x')
    {
        assert (!isdigit (isPow));

        if (isPow == '^')
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
#ifndef PARSEH

#define PARSEH

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "DefinesConst.h"

int ParseMain            (double *a, double *b, double *c);                             //парсинг пробелов и букв
double ParseNum          (char Part [], int count, int *shift);                         //парсинг цифр
int DeliteSpase          (char inString[]);                                             //удаление пробелов
void SeparatePol         (char inString [], char lEntPart [], char rEntPart []);
double* ChooseCoef       (char isX, char isExp, double *a, double *b, double *c, int *isA);     //выбирает нужный коэф для последующей обработки
int ParseToCoef          (char EntPart [], struct Polinomial *parsPol);

#endif
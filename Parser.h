#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "StructPol.h"

#define RED     "\033[31m"
#define RESET   "\033[0m"
#define GREEN   "\033[32m"

int ParseMain            (double *a, double *b, double *c);                             //парсинг пробелов и букв
double ParseNum          (char Part [], int count, int *shift);                         //парсинг цифр
int DeliteSpase          (char inString[]);                                             //удаление пробелов
void SeparatePol         (char inString [], char lEntPart [], char rEntPart []);
double* ChooseCoef       (char isX, char isExp, double *a, double *b, double *c, int *isA);     //выбирает нужный коэф для последующей обработки
int ParseToCoef          (char EntPart [], struct Polinomial *parsPol);

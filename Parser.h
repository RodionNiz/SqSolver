#ifndef PARSEH

#define PARSEH

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "ConstValues.h"


int ParseMain           (double *a, double *b, double *c, char inString []);                //парсинг пробелов и букв
double ParseNum         (char Part [], int count, int *shift);                              //парсинг цифр
int DeleteSpace         (char inString[]);                                                  //удаление пробелов
void SeparatePol        (char inString [], char lEntPart [], char rEntPart []);             //разделение строки на части до и после =
double* ChooseCoef      (char isX, char isExp, double *a, double *b, double *c, int *isA);  //выбирает нужный коэф для последующей обработки
int ParseToCoef         (char EntPart [], struct Polinomial *parsPol);                      //парсинг вплоть до коэффициента
int ReadInputBuffer    (double *a, double *b, double *c);                                   //считывает буффер в строку, проверяет корректность символов

#endif
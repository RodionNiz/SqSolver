/**
    \file
    \brief File parse input string
    
    File contain parser which takes string similar "1.2x^2 + 3.4x + 5 = 6.7x^2 + 8.9x + 0",
    parse coeffitients and write them to mainPoly.aP, mainPoly.bP, mainPoly.cP in main
 */

#ifndef PARSEH

#define PARSEH

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "Colors.h"

/**
    \brief Contain max len of input sring and max len of parts before and after equal sign
 */
enum StringsLen
{
    InputStrLen = 40,
    PolyStrLen = 20
};

/**
    \brief Parse coefficients from stdin buffer

    Parse stdin buffer, check is every symbol supported.
    \param [out] a is a pointer to the leader coefficient of solving equation
    \param [out] b is a pointer to the first degree coefficient of solving equation
    \param [out] c is a pointer to the constant term of solving equation
    \return number of coeffitients parsed from inputed string
 */
int ParseInputCoef (double *a, double *b, double *c);

/**
    \brief Parse coefficients from string

    write parsed values to the coeffitients by pointers
    \param [out] a is a pointer to the leader coefficient of solving equation
    \param [out] b is a pointer to the first degree coefficient of solving equation
    \param [out] c is a pointer to the constant term of solving equation
    \param [out] inString is a inputed string
    \return number of coeffitients parsed from inputed string
 */
int ParseMain (double *a, double *b, double *c, char inString []);

/**
    \brief Delite space from inputed string, check availability of signs
    \param [in, out] inString is a inputed string
    \return 0 in case missed sign, 1 if string is correct
 */
int DeleteSpace (char inString[]);

/**
    \brief Separate inputed string

    \param [in] inString is inputed string without spaces
    \param [out] leftPart is a part of inString before equal sign
    \param [out] rightPart is a part of inString after equal sign
 */
void SeparateEq (char inString [], char leftPart [], char rightPart []);

/**
    \brief Parse string until find a digit
    \param [in] Part is a parsing at the moment part of inputed string 
    \param [out] a is a pointer to the leader coefficient of polinom before or after equal sign
    \param [out] b is a pointer to the first degree coefficient of polinom before or after equal sign
    \param [out] c is a pointer to the constant term of polinom before or after equal sign
 */
int ParseToCoef (char Part [], double *a, double *b, double *c);                    //парсинг вплоть до коэффициента

/**
    \brief Parse one number from the string

    Parse and save one number to the coefficient from string Part
    \param [in] Part is a parsing at the moment part of inputed string 
    \param [in] count is first digit of parsing number position in string Part
    \param [out] shift is a number of symbols in parsed number
    \return Parsed number 
 */
double ParseNum (char Part [], int count, int *shift);

/**
    \brief Select changing coefficient 

    Select changing coefficient depending on two symbols after number or symbols without number (x or x^2)
    \param [in] isX contain first symbol after number
    \param [in] isExp contain second symbol after number
    \param [in] a is a pointer to the leader coefficient of polinom before or after equal sign
    \param [in] b is a pointer to the first degree coefficient of polinom before or after equal sign
    \param [in] c is a pointer to the constant term of polinom before or after equal sign 
    \param [out] isA signals is changing coefficient a leader coefficient or not
    \return pointer to changing coefficient of polinom before or after equal sign
 */
double* ChooseCoef (char isX, char isExp, double *a, double *b, double *c, int *isA);

#endif
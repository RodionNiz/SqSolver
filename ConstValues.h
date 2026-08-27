#ifndef DEFS

#define DEFS

#define RED     "\033[31m"
#define RESET   "\033[0m"
#define GREEN   "\033[32m"

#define ACCURACY 0.0001

enum modeOfTestSolve
{
    SqNoRoots = 0,
    SqOneRoot = 1,
    SqTwoRoots = 2,
    LnOneRoot = 3,
    LnNoRoot = 4,
    ManualTests = 5
};

enum modeOfWork
{
    SolveEqMod = 0,
    TestSolveMod = 1,
    TestParseMod = 2
};

enum StringsLen
{
    InputStrLen = 40,
    PoliStrLen = 20
};

enum CodesOfErrors
{
    InputErr = 100,
    ErrInTests = 101
};

#endif
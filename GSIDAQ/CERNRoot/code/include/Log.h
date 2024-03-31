#ifndef Log_HH
#define Log_HH

#include "iostream"
#include "fstream"
#include "TString.h"

class Log
{
    public:
        Log();
        ~Log();
        static void Output(TString flag, TString content);
    private:
};

#endif

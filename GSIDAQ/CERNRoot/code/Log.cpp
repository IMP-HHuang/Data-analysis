#include "Log.h"

Log::Log()
{
}

Log::~Log()
{
}

void Log::Output(TString flag, TString content)
{
    std::ofstream LogOpf;
    LogOpf.open("RunLog.txt", std::ios::app);
    if(flag == "Error" || flag == "error" || flag == "Err")
        LogOpf << "\033[31mError\033[0m : " << content << std::endl;  //Red
    else if(flag == "Inf" || flag == "inf")
        LogOpf << "\033[32mLog\033[0m : " << content << std::endl; //Green
    else 
        LogOpf << "\033[33mPrompt\033[0m : " << content << std::endl; //Yello
    LogOpf.close();
}


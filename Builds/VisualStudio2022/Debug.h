#pragma once
#include <iostream>
#include <fstream>

class Debug
{
private:
    std::string msg;
    bool isFileOpen = false;
    std::ofstream outFile;

public:

    Debug();
    ~Debug();
    void initDebugFile();
    void addLog(std::string msg);
    void closeDebugFile();
};

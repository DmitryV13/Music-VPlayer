#include "Debug.h"

Debug::Debug()
{
    initDebugFile();
}

Debug::~Debug()
{
    closeDebugFile();
}

void Debug::initDebugFile()
{
    outFile.open("D:\\Applications\\Desktop\\MV\\FFTv1.0\\Source\\logs.txt");
    if (!outFile.is_open()) {
        std::cerr << "Не удалось открыть файл для записи." << std::endl;
        isFileOpen = false;
    }
    outFile << "********************  NEW DEBUG OUTPUT  ********************" << std::endl;
    isFileOpen = true;
}

void Debug::addLog(std::string msg)
{
    if (isFileOpen)
        outFile << "---**---" << msg << std::endl;
}

void Debug::closeDebugFile()
{
    if (isFileOpen)
        outFile.close();
}

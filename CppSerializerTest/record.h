#pragma once
#include <string>

struct Record 
{
    enum class DataType { INT, DBL, STR, BOOL };

    DataType type;
    void *pData;
    std::string name;
    Record(std::string s, DataType t, void* d)
        : name(s), type(t), pData(d)
    {}
};



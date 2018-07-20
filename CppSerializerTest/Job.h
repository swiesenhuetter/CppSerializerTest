#pragma once

#include <string>
#include <vector>
#include <ostream>
#include <istream>
#include "JsonSerialization.h"
#include "record.h"

class Job
{
public:
    Job() 
        : Job(0,0.0,L"",false)
    {
    };

    Job(int i, double d, std::wstring ws, bool r)
        : inum(i), myValue(d), name(ws), isRelative(r) 
    {
        init();
    }

    void to_json(nlohmann::json& jsn) const
    {
        jsn = nlohmann::json{  KEY_VAL(inum) , KEY_VAL(myValue), KEY_VAL(name), KEY_VAL(isRelative) };
    }


    std::ostream& toJson(std::ostream& os)
    {
        using namespace atjson;

        JsonObject j(os, "Job");

        return atjson::write(os,
            KEY_VAL(inum),
            KEY_VAL(myValue),
            KEY_VAL(name),
            KEY_VAL(isRelative));
    }

    std::istream& fromJson(std::istream& is)
    {
        return atjson::read(is,
            KEY_VALPTR(inum),
            KEY_VALPTR(myValue),
            KEY_VALPTR(name),
            KEY_VALPTR(isRelative));
    }


    void init()
    {
        records = std::vector<Record>{
            { "inum",       Record::DataType::INT, &inum },
            { "myValue",    Record::DataType::DBL, &myValue },
            { "name",       Record::DataType::STR, &name },
            { "isRelative", Record::DataType::BOOL, &isRelative }
        };
    }

    bool operator== (const Job& j) const;

private:
    int inum{};
    double myValue{};
    std::wstring name;
    bool isRelative{};

    friend std::ostream& operator<<(std::ostream& os, const Job& j);
    friend std::istream& operator>>(std::istream& is, Job& j);
    
    std::vector<Record> records;
    };


bool Job::operator== (const Job& j) const
{
    return (inum == j.inum && myValue == j.myValue && name == j.name && isRelative == j.isRelative);
}

std::istream& operator>>(std::istream& is, Job& j)
{
    is >> j.records;
    return is;
}

std::ostream& operator<< (std::ostream& os, const Job& j)
{
    os << j.records;
    return os;
}

void to_json(nlohmann::json& jsn, const Job& job)
{
    job.to_json(jsn);
}



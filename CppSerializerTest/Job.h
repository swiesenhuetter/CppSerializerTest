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
        : Job(0,0.0,"",false)
    {
    };

    Job(int i, double d, std::string ws, bool r)
        : inum(i), myValue(d), name(ws), isRelative(r) 
    {
        init();
    }

    std::ostream& toJson(std::ostream& os)
    {
        using namespace atjson;

        JsonObject j(os, "Job");

        return atjson::write(os,
            key_val("inum", inum),
            key_val("myValue", myValue),
            key_val("name", name),
            key_val("isRelative", isRelative));
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
    std::string name;
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




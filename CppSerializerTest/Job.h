#pragma once

#include "JsonSerialization.h"
#include "json.hpp"
#include <string>
#include <vector>
#include <ostream>
#include <istream>
#include <codecvt>


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
    }

    void to_json(nlohmann::json& jsn) const
    {
        jsn["inum"] = inum;
        jsn["myValue"] = myValue;
        jsn["name"] = name;
        jsn["isRelative"] = isRelative;
    }

    void from_json(const nlohmann::json& jsn)
    {
        inum = jsn.at("inum").get<int>();
        myValue = jsn.at("myValue").get<double>();
        name = jsn.at("name").get<std::string>();
        isRelative = jsn.at("isRelative").get<bool>();
    }

    bool operator== (const Job& j) const;

private:
    int inum{};
    double myValue{};
    std::string name;
    bool isRelative{};
    
    };


bool Job::operator== (const Job& j) const
{
    return (inum == j.inum && myValue == j.myValue && name == j.name && isRelative == j.isRelative);
}

void to_json(nlohmann::json& jsn, const Job& job)
{
    job.to_json(jsn);
}

void from_json(const nlohmann::json& jsn, Job& job)
{
    job.from_json(jsn);
}


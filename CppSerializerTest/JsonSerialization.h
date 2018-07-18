#pragma once
#include <string>
#include <vector>
#include "record.h"

std::ostream& operator<<(std::ostream& os, const Record& rec)
{
    os << '\"' << rec.name << "\": ";
    switch (rec.type)
    {
    case Record::DataType::BOOL:
    {
        os << *(static_cast<bool*>(rec.pData)) ? "True":"False";
        break;
    }
    case Record::DataType::INT:
    {
        os << *static_cast<int*>(rec.pData);
        break;
    }
    case Record::DataType::DBL:
    {
        os << *static_cast<double*>(rec.pData);
        break;
    }
    case Record::DataType::STR:
    {
        //std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
        //os << myconv.to_bytes(*static_cast<std::wstring*>(rec.pData));
        os << '\"' << (*static_cast<std::string*>(rec.pData)) << '\"';
        break;
    }
    default:
        break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<Record>& records)
{
    os << "{\n\t";
    std::string sep;
    for (const auto& rec : records)
    {
        os << sep << rec;
        sep = ",\n\t";
    }
    os << "\n}\n";
    return os;
}

std::istream& operator>>(std::istream& is, Record& rec)
{
    char c{};
    is >> c; // {
    std::string name;
    std::getline(is, name, ':');
    switch (rec.type)
    {
    case Record::DataType::INT:
        is >> *(static_cast<int*>(rec.pData));
        break;
    case Record::DataType::DBL:
        is >> *(static_cast<double*>(rec.pData));
        break;
    case Record::DataType::BOOL:
        is >> *(static_cast<bool*>(rec.pData));
        break;
    case Record::DataType::STR:
    {
        is >> c; // " 
        auto pstr = static_cast<std::string*>(rec.pData);
        std::getline(is, *pstr, '\"');
        break;
    }
    }
    is >> c;
    return is;
}

std::istream& operator>>(std::istream& is, std::vector<Record>& records)
{
    for (auto& rec : records)
    {
        is >> rec;
    }
    return is;
}





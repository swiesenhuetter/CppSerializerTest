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

// this macro creates a pair<const char*, ValType> first : name of a value second : value
#define KEY_VAL(NAME)  std::make_pair (#NAME, NAME) 

namespace atjson
{
    /// first element of a key-value pair: "key":
    /// tab because values are inside an { object }
    template <typename ValType>
    std::ostream& write_key(std::ostream& os, std::pair<const char*, ValType>& kv)
    {
        os << "\t\"" << kv.first << "\": ";
        return os;
    }

    /// generic output function : key followed by ostream inserted value
    template <typename ValType>
    std::ostream& write(std::ostream& os, std::pair<const char*, ValType> kv)
    {
        write_key(os, kv);
        return (os << kv.second);
    }

    /// specialization: bool format instead of 0/1 numeric
    std::ostream& write(std::ostream& os, std::pair<const char*, bool> b)
    {
        write_key(os, b);
        os << (b.second ? "true" : "false");
        return os;
    }

    /// specialization: json string values are in double quotes
    std::ostream& write(std::ostream& os, std::pair<const char*, std::string> str)
    {
        write_key(os, str);
        os << '\"' << str.second << '\"';
        return os;
    }

    // not actually called, needed for compiler recursion
    std::ostream& write(std::ostream& os)
    {
        return os;
    }

    // recursive variadic template to match any argument list of values to serialize
    template <typename Head, typename... Tail>
    std::ostream& write(std::ostream& os, const Head& head, const Tail&... tail)
    {
        write(os, head);
        os << ",\n";
        return write(os, tail...);
    }

    struct JsonObject
    {
        JsonObject(std::ostream& os, const char* objName) 
            :os_(os)
        {
            os_ << objName << ": {\n";
        }
        ~JsonObject()
        {
            os_ << "\n}";
        }
        std::ostream& os_;
    };

    template <typename Container>
    std::ostream& toJson(std::ostream& os, Container c)
    {
        os << "[";
        for (auto& elem : c)
        {
            elem.toJson(os);
            os << ",\n";
        }
        os << "]\n";
        return os;
    }

}


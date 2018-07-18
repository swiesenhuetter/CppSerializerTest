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

namespace atjson
{
    template <typename KeyType, typename ValType>
    std::pair<KeyType, ValType> key_val(KeyType k, ValType v) { return std::make_pair(k, v); }

    template <typename ValType>
    std::ostream& jsonkey(std::ostream& os, std::pair<const char*, ValType>& kv)
    {
        os << '\"' << kv.first << "\": ";
        return os;
    }

    template <typename ValType>
    std::ostream& write(std::ostream& os, std::pair<const char*, ValType> kv)
    {
        jsonkey(os, kv);
        return (os << kv.second);
    }

    std::ostream& write(std::ostream& os, std::pair<const char*, bool> b)
    {
        jsonkey(os, b);
        os << (b.second ? "true" : "false");
        return os;
    }

    std::ostream& write(std::ostream& os, std::pair<const char*, std::string> str)
    {
        jsonkey(os, str);
        os << '\"' << str.second << '\"';
        return os;
    }

    std::ostream& write(std::ostream& os)
    {
        return os;
    }

    template <typename Head, typename... Tail>
    std::ostream& write(std::ostream& os, const Head& head, const Tail&... tail)
    {
        write(os, head);
        if (sizeof... (Tail))
        {
            os << ",\n";
            return write(os, tail...);
        }
        else
        {
            return os << '\n';
        }
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
            os_ << "\n}\n";
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
        }
        os << "]\n";
        return os;
    }

}


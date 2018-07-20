#pragma once
#include "record.h"
#include "json.hpp"
#include <codecvt>
#include <string>
#include <vector>

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
        std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
        os << '\"' << myconv.to_bytes(*static_cast<std::wstring*>(rec.pData)) << '\"';
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
        std::string txt;
        std::getline(is, txt, '\"');
        std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
        auto pwstr = static_cast<std::wstring*>(rec.pData);
        *pwstr = myconv.from_bytes(txt);
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
#define KEY_VAL(NAME)  std::make_pair(#NAME, NAME) 
#define KEY_VALPTR(NAME) std::make_pair(#NAME, &NAME) 

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
    std::ostream& write(std::ostream& os, std::pair<const char*, std::wstring> str)
    {
        write_key(os, str);
        std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
        os << '\"' << myconv.to_bytes(str.second) << '\"';
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


    /// generic deserialize function : key followed by ostream inserted value
    template <typename ValPtr>
    std::istream& read(std::istream& is, std::pair<const char*, ValPtr>& kv)
    {
        std::string name;
        is >> name;
        return (is >> *kv.second);
    }

    /// special string deserialize function
    std::istream& read(std::istream& is, std::pair<const char*, std::wstring*>& str)
    {
        std::string text;
        is >> text;
        char c;
        is >> c;
        std::getline(is, text, '\"');
        std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
        *str.second = myconv.from_bytes(text);
        return is;
    }



    // not actually called, needed for compiler recursion
    std::istream& read(std::istream& is)
    {
        return is;
    }
    // reverse recursive variadic template to match any sequence of key/value pairs to deserialize
    template <typename Head, typename... Tail>
    std::istream& read(std::istream& is, Head&& head, Tail&&... tail) // head&tail rvalues of key/value pairs
    {
        read(is, head);
        char c;
        is >> c; // ,
        return read(is, tail...);
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

    template <typename Container>
    Container fromJson(std::istream& is)
    {
        char c{};
        is >> c; // [
        std::string name;
        is >> name;
        typename Container::value_type element;
        is >> c; // {
        element.fromJson(is);
        is >> c; // }
        //is >> element;
        Container deserialized;
        deserialized.push_back(element);
        return deserialized;
    }


}


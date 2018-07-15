#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <string>
#include <ostream>
#include <istream>
#include <codecvt>

struct Record 
{
    enum class DataType { INT, DBL, WSTR, BOOL };
    DataType type;
    void *pData;
    std::string name;
    Record(std::string s, DataType t, void* d)
        : name(s), type(t), pData(d)
    {}
};

std::ostream& operator<<(std::ostream& os, const Record::DataType& type)
{
    switch (type)
    {
    case Record::DataType::BOOL:
    {
        os << "BOOL";
        break;
    }
    case Record::DataType::INT:
    {
        os << "INT";
        break;
    }
    case Record::DataType::DBL:
    {
        os << "DBL";
        break;
    }
    case Record::DataType::WSTR:
    {
        os << "WSTR";
        break;
    }
    default:
        break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Record& rec)
{
    os << '{' << rec.type << ':' << rec.name << ':';
    switch (rec.type)
    {
    case Record::DataType::BOOL:
    {
        os << *static_cast<bool*>(rec.pData);
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
    case Record::DataType::WSTR:
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
        os << myconv.to_bytes(*static_cast<std::wstring*>(rec.pData));
        break;
    }
    default:
        break;
    }
    os << "}\n";
    return os;
}


std::istream& operator>>(std::istream& is, Record& rec)
{
    char c{};
    is >> c; // {
    std::string typeId;
    std::getline(is, typeId, ':');
    std::getline(is, rec.name, ':');
    is >> c;
    return is;
}


class Job
{
public:
    Job() {};
    Job(int i, double d, std::wstring ws)
        : inum(i), myValue(d), name(ws) {}

private:
    int inum{};
    double myValue{};
    std::wstring name;

    std::vector<Record> properties{
        { "inum",  Record::DataType::INT, &inum },
        { "dbl",  Record::DataType::DBL, &myValue },
        { "name", Record::DataType::WSTR, &name}
    };

    friend std::ostream& operator<<(std::ostream& os, const Job& j);
    friend std::istream& operator>>(std::istream& is, Job& j);
};

std::istream& operator>>(std::istream& is, Job& j)
{
    for (auto& prop : j.properties)
    {
        is >> prop;
    }
    return is;
}

std::ostream& operator<< (std::ostream& os, const Job& j)
{
    for (const auto& prop : j.properties)
    {
        os << prop;
    }
    return os;
}


TEST_CASE("Serializer: object to string") {
    Job j1(1, 2.2, L"tamping");
    Job j2(3, 4.4, L"s -labtrack");
    Job j3(5, 6.6, L"üהציאט");

    std::ostringstream os;
    os << j1 << j2 << j3;
    std::cout << os.str();
    std::istringstream is(os.str());
    Job jc1, jc2, jc3;
    is >> jc1 >> jc2 >> jc3;

}

TEST_CASE("Serializer: string to object") {

}


#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <string>
#include <ostream>
#include <istream>
#include <codecvt>

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

std::ostream& operator<<(std::ostream& os, const Record& rec)
{
    os << '{' << rec.name << ':';
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
    os << "}\n";
    return os;
}


std::istream& operator>>(std::istream& is, Record& rec)
{
    char c{};
    is >> c; // {
     std::getline(is, rec.name, ':');
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


class Job
{
public:
    Job() {};
    Job(int i, double d, std::string ws)
        : inum(i), myValue(d), name(ws) {}

    bool operator== (const Job& j) const;

private:
    int inum{};
    double myValue{};
    std::string name;

    std::vector<Record> properties{
        { "inum",  Record::DataType::INT, &inum },
        { "dbl",  Record::DataType::DBL, &myValue },
        { "name", Record::DataType::STR, &name}
    };

    friend std::ostream& operator<<(std::ostream& os, const Job& j);
    friend std::istream& operator>>(std::istream& is, Job& j);
};

bool Job::operator== (const Job& j) const
{
    return (inum == j.inum && myValue == j.myValue && name == j.name);
}

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
    Job j1(1, 2.2, "tamping");
    Job j2(3, 4.4, "s -labtrack");
    Job j3(5, 6.6, "üהציאט");

    std::ostringstream os;
    os << j1 << j2 << j3;
    std::cout << os.str();
    std::istringstream is(os.str());
    Job jc1, jc2, jc3;
    is >> jc1 >> jc2 >> jc3;

    REQUIRE(j1 == jc1);
    REQUIRE(j2 == jc2);
    REQUIRE(j3 == jc3);

}

TEST_CASE("Serializer: string to object") {

}


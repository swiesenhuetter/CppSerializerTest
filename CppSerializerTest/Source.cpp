#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "serialization.h"
#include "job.h"
#include "Json.hpp"
#include "JsonSerialization.h"
#include <codecvt>
#include <fcntl.h>

TEST_CASE("Serializer: object to json and back") {

    using nlohmann::json;

    Job j1(1, 2.2, "tamping", true);
    Job j2(3, 4.4, "slabtrack", false);
    Job j3(5, 6.6, u8"üäöéàè", true);

    json jsn;
    to_json(jsn, j1);
    auto jsntext1 = jsn.dump();
    to_json(jsn, j2);
    auto jsntext2 = jsn.dump();
    to_json(jsn, j3);
    auto jsntext3 = jsn.dump();

    Job jc1, jc2, jc3;

    from_json(json::parse(jsntext1), jc1);
    REQUIRE(j1 == jc1);
    from_json(json::parse(jsntext2), jc2);
    REQUIRE(j2 == jc2);
    from_json(json::parse(jsntext3), jc3);
    REQUIRE(j3 == jc3);
}

TEST_CASE("Serializer: object vector to json and back") {

    using nlohmann::json;

    std::vector<Job> jobs{
    {1, 2.2, "tamping", true},
    {3, 4.4, "slabtrack", false },
    {5, 6.6, "slabtrack", true },
    {7, 8.8, u8"sürvey", false } };

    json jsn = json(jobs);

    ::SetConsoleOutputCP(CP_UTF8);
    setvbuf(stdout, nullptr, _IOFBF, 4000);


    std::cout << jsn.dump(4);
    std::vector<Job> jobsCpy = jsn;

    for (size_t i = 0; i < jobs.size(); ++i)
    {
        REQUIRE(jobs[i] == jobsCpy[i]);
    }
}

TEST_CASE("Serializer: preprocessor magic")
{
    const int myinteger{12345};
    auto mi = KEY_VAL(myinteger);

    REQUIRE(mi.first == "myinteger");
    REQUIRE(mi.second == myinteger);

    decltype(mi.first) other{};

    mi.second = 111;
    REQUIRE(mi.second != myinteger);

    int int2{ 6789 };

    auto m2 = KEY_VALPTR(int2);
    REQUIRE(m2.first == "int2");
    REQUIRE(*m2.second == int2);
    *m2.second = 111;
    REQUIRE(*m2.second == int2);
    REQUIRE(int2 == 111);
}

namespace ns {
    // a simple struct to model a person
    struct person {
        std::string name;
        std::string address;
        int age;
    };
}

TEST_CASE("Serializer: using nlohmann json header")
{
    using nlohmann::json;

    auto res = ::SetConsoleOutputCP(CP_UTF8);
    setvbuf(stdout, nullptr, _IOFBF, 4000);

    ns::person p = { u8"Wiesenhütter", u8"Sapînta", 60 };
    json j;
    j["name"] = p.name;
    j["address"] = p.address;
    j["age"] = p.age;
    
    std::vector<Job> jobs{
        { 1, 2.2, u8"tamping Üø", true },
        { 3, 4.4, "slabtrack", false },
        { 5, 6.6, u8"süürvey", true } };

    j["jobs"] = jobs;

    std::cout  << std::setw(2) << j << std::flush;

    std::ostringstream oss;
    oss << std::setw(2) << j << std::flush;

    json copy1{ oss.str() };
}


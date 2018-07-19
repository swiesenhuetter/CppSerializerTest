#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <codecvt>
#include "serialization.h"
#include "job.h"
#include "JsonSerialization.h"

TEST_CASE("Serializer: object to stream, string and back") {
    Job j1(1, 2.2, "tamping", true);
    Job j2(3, 4.4, "slabtrack", false);
    Job j3(5, 6.6, "üהציאט", true);

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



TEST_CASE("Serializer: object to stream (New method, variadic template)") {
    std::vector<Job> jobs{
    {1, 2.2, "tamping", true},
    {3, 4.4, "slabtrack", false},
    {5, 6.6, "sürvey", true } };

    std::ostringstream os;

//    jobs[0].toJson(os);
//    jobs[1].toJson(os);
//    jobs[2].toJson(os);
    atjson::toJson(os, jobs);

    std::cout << os.str();

    std::istringstream is(os.str());
    auto jobsRestored = atjson::fromJson<std::vector<Job>>(is);



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


TEST_CASE("Serializer: using nlohmann json header")
{
    nlohmann::json j;

    std::vector<int> ints{ 3,5,76,78,8,9, 90,0,-3,-3 };
    j["ints"] = ints;
    std::cout << j.dump();
}


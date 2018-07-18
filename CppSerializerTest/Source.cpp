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

    jobs[0].toJson(os);
    jobs[1].toJson(os);
    jobs[2].toJson(os);
    atjson::toJson(os, jobs);

    std::cout << os.str();

    std::vector<Job> jobsRestored;


}

TEST_CASE("Serializer: preprocessor magic")
{
    int myinteger{12345};
    auto mi = KEY_VAL(myinteger);
    REQUIRE(mi.first == "myinteger");
    REQUIRE(mi.second == myinteger);
}


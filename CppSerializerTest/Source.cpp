#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <codecvt>
#include "serialization.h"
#include "job.h"

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

TEST_CASE("Serializer: string to object") {

}


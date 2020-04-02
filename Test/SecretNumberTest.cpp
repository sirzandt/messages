#include "catch.hpp"
#include <string>
#include <vector>
#include "SecretNumber.hpp"
#include "openssl/rand.h"

TEST_CASE("Test single secret number with empty seed", "SecretNumber")
{
    std::vector<unsigned char> seed = std::vector<unsigned char>{0,0,0,0,0,0,0,0};
    SecretNumber testSecretNumber(seed);

    std::string sn = testSecretNumber.GetRandomNumberAsString();
    REQUIRE(sn == "66674061");
}

TEST_CASE("Test double secret number with default seed is random", "SecretNumber")
{
    SecretNumber testSecretNumber;
    SecretNumber testSecretNumber2;

    std::string sn = testSecretNumber.GetRandomNumberAsString();
    std::string sn2 = testSecretNumber2.GetRandomNumberAsString();

    REQUIRE(sn != sn2);
}

TEST_CASE("Test single secret number with non-empty seed", "SecretNumber")
{
    std::vector<unsigned char> seed = std::vector<unsigned char>{1,2,3,4,5,8,7,6};
    SecretNumber testSecretNumber(seed);

    std::string sn = testSecretNumber.GetRandomNumberAsString();
    REQUIRE(sn == "03213611");
}

TEST_CASE("Test single secret number with previous result as seed", "SecretNumber")
{
    std::vector<unsigned char> seed = std::vector<unsigned char>{0,0,0,0,0,0,0,0};
    SecretNumber testSecretNumber(seed);

    std::string sn = testSecretNumber.GetRandomNumberAsString();
    REQUIRE(sn == "66674061");

    std::vector<unsigned char> randonNumber = testSecretNumber.GetRandomNumber();

    SecretNumber testSecretNumber2(randonNumber);

    std::string sn2 = testSecretNumber2.GetRandomNumberAsString();
    REQUIRE(sn2 == "45197814");
}

TEST_CASE("Test single secret number with empty seed and specific time", "SecretNumber")
{
    std::vector<unsigned char> seed = std::vector<unsigned char>{0,0,0,0,0,0,0,0};

    long timeValue = 0;
    SecretNumber testSecretNumber(seed, timeValue);

    std::string sn = testSecretNumber.GetRandomNumberAsString();
    REQUIRE(sn == "66674061");

    timeValue = 1;
    SecretNumber testSecretNumber2(seed, timeValue);

    std::string sn2 = testSecretNumber2.GetRandomNumberAsString();
    REQUIRE(sn2 != sn);
    REQUIRE(sn2 == "66772963");
}

TEST_CASE("Test single secret number with default seed and specific time", "SecretNumber")
{
    long timeValue = 0;
    SecretNumber testSecretNumber(timeValue);

    std::string sn = testSecretNumber.GetRandomNumberAsString();
    
    timeValue = 1;
    SecretNumber testSecretNumber2(timeValue);

    std::string sn2 = testSecretNumber2.GetRandomNumberAsString();
    REQUIRE(sn2 != sn);
}

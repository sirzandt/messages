#include "catch.hpp"
#include "Envelope.hpp"
#include <string>
#include <vector>

TEST_CASE("Test correct encode to base64", "Envelope")
{
    std::string testString = "1234567890\n";
    std::string base64String = "MTIzNDU2Nzg5MAo=";
    std::vector<unsigned char> testData;

    for (char c: testString)
    {
        testData.push_back(static_cast<unsigned char>(c));
    }

    auto result = EnvelopeSeal::EncodeToBase64(testData.data(), testData.size());
    REQUIRE(result == base64String);
}

TEST_CASE("Test correct encode to base64 (size 2)", "Envelope")
{
    std::string testString = "12345678901\n";
    std::string base64String = "MTIzNDU2Nzg5MDEK";
    std::vector<unsigned char> testData;

    for (char c: testString)
    {
        testData.push_back(static_cast<unsigned char>(c));
    }

    auto result = EnvelopeSeal::EncodeToBase64(testData.data(), testData.size());
    REQUIRE(result == base64String);
}

TEST_CASE("Test correct encode to base64 (size 3)", "Envelope")
{
    std::string testString = "123456789012\n";
    std::string base64String = "MTIzNDU2Nzg5MDEyCg==";
    std::vector<unsigned char> testData;

    for (char c: testString)
    {
        testData.push_back(static_cast<unsigned char>(c));
    }

    auto result = EnvelopeSeal::EncodeToBase64(testData.data(), testData.size());
    REQUIRE(result == base64String);
}

TEST_CASE("Test correct encode to base64 (size 4)", "Envelope")
{
    std::string testString = "Welcome to openssl wik\n\n\n";
    std::string base64String = "V2VsY29tZSB0byBvcGVuc3NsIHdpawoKCg==";
    std::vector<unsigned char> testData;

    for (char c: testString)
    {
        testData.push_back(static_cast<unsigned char>(c));
    }

    auto result = EnvelopeSeal::EncodeToBase64(testData.data(), testData.size());
    REQUIRE(result == base64String);
}

TEST_CASE("Test correct decode to from base64 (size 1)", "Envelope")
{
    //std::string testString = "Welcome to openssl wiki";
    std::string base64String = "MTIzNDU2Nzg5MAo=";
    std::vector<unsigned char> testData {'1','2','3','4','5','6','7','8','9','0','\n'};

    std::vector<unsigned char> result = EnvelopeOpen::DecodeFromBase64(base64String);
    REQUIRE(testData == result);
}

TEST_CASE("Test correct decode to from base64 (size 2)", "Envelope")
{
    //std::string testString = "Welcome to openssl wiki";
    std::string base64String = "MTIzNDU2Nzg5MDEK";
    std::vector<unsigned char> testData {'1','2','3','4','5','6','7','8','9','0','1','\n'};

    std::vector<unsigned char> result = EnvelopeOpen::DecodeFromBase64(base64String);
    REQUIRE(testData == result);
}

TEST_CASE("Test correct decode to from base64 (size 3)", "Envelope")
{
    //std::string testString = "Welcome to openssl wiki";
    std::string base64String = "MTIzNDU2Nzg5MDEyCg==";
    std::vector<unsigned char> testData {'1','2','3','4','5','6','7','8','9','0','1','2','\n'};

    std::vector<unsigned char> result = EnvelopeOpen::DecodeFromBase64(base64String);
    REQUIRE(testData == result);
}

TEST_CASE("Generate keys for an seal envelope", "Envelope")
{    
    std::map<std::string, std::string> encryptedMap;

    std::string testMessage = "1234567890\n";

    OPENSSL_init();

    EnvelopeSeal testEnvelope("../Test/resources/my_public_key.pem");
    testEnvelope.AddMessage(testMessage);
    testEnvelope.FinalizeMessage();
    testEnvelope.GetEncryptedMap(encryptedMap);

    EnvelopeOpen testEnvelope2(
        "../Test/resources/my_private_key.pem", 
        encryptedMap[EnvelopeSeal::MapKeyEncryptedKey],
        encryptedMap[EnvelopeSeal::MapKeyVi]
        );
    testEnvelope2.AddMessage(encryptedMap[EnvelopeSeal::MapKeyCipher]);
    testEnvelope2.FinalizeMessage();

    auto result = testEnvelope2.GetPlainText();
    REQUIRE(result == testMessage);
}


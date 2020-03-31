#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "MessageBase.hpp"
#include "json.hpp"

TEST_CASE("New Message is empty", "MessageBase")
{
    MessageBase testMessage;

    REQUIRE(testMessage.GetMessageType() == "");
    REQUIRE(testMessage.GetSessionId() == "");

    auto emptyJ = "{\"MessageType\":\"\",\"SessionId\":\"\"}";
    REQUIRE(testMessage.GetJSonString() == emptyJ);
}

TEST_CASE("New base message", "Messagebase")
{
    MessageBase testMessage("12345", "InitMessage");

    REQUIRE(testMessage.GetSessionId() == "12345");
    REQUIRE(testMessage.GetMessageType() == "InitMessage");
    
    auto newJ = "{\"MessageType\":\"InitMessage\",\"SessionId\":\"12345\"}";
    REQUIRE(testMessage.GetJSonString() == newJ);
}

TEST_CASE("Received Message is interpreted correctly", "Messagebase")
{
    MessageBase testMessage;

    auto testJson = "{\"MessageType\":\"InitMessage\",\"SessionId\":\"12345\"}";
    testMessage.Parse(testJson);

    REQUIRE(testMessage.GetSessionId() == "12345");
    REQUIRE(testMessage.GetMessageType() == "InitMessage");
}

TEST_CASE("Received Message is interpreted without sessionId", "Messagebase")
{
    MessageBase testMessage;

    auto testJson = "{\"MessageType\":\"InitMessage\"}";
    testMessage.Parse(testJson);

    REQUIRE(testMessage.GetSessionId() == "");
    REQUIRE(testMessage.GetMessageType() == "InitMessage");
}

TEST_CASE("Received Message is interpreted without messageType", "Messagebase")
{
    MessageBase testMessage;

    auto testJson = "{\"SessionId\":\"12345\"}";
    testMessage.Parse(testJson);

    REQUIRE(testMessage.GetSessionId() == "12345");
    REQUIRE(testMessage.GetMessageType() == "");
}

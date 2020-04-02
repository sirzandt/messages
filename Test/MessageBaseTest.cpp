#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "MessageBase.hpp"
#include "json.hpp"
#include "Envelope.hpp"

/*TEST_CASE("New Message is empty", "MessageBase")
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
*/

TEST_CASE("SendMessage contains a json message with all fields", "MessageBase")
{
    OPENSSL_init();

    SendMessage sendMessage(1, "Peer1", "../Test/resources/my_public_key.pem", "MessageType");

    std::string jsonMessageToSend = sendMessage.GetJSonString();

    nlohmann::json parsedJson = nlohmann::json::parse(jsonMessageToSend);
    REQUIRE(parsedJson["PeerId"] == "Peer1");
    REQUIRE(parsedJson["MessageType"] == "MessageType");
    REQUIRE(parsedJson.contains("EMessage"));
    nlohmann::json eMessage = parsedJson["EMessage"];
    REQUIRE(eMessage.contains("P1"));
    REQUIRE(eMessage.contains("P2"));
    REQUIRE(eMessage.contains("P3"));
}

TEST_CASE("SendMessage contains a json message that can be decoded", "MessageBase")
{
    OPENSSL_init();

    SendMessage sendMessage(1, "Peer1", "../Test/resources/my_public_key.pem", "MessageType");

    std::string jsonMessageToSend = sendMessage.GetJSonString();

    nlohmann::json parsedJson = nlohmann::json::parse(jsonMessageToSend);

    std::string encryptedKey = parsedJson["EMessage"][EnvelopeSeal::MapKeyEncryptedKey];
    std::string iv = parsedJson["EMessage"][EnvelopeSeal::MapKeyVi];
    std::string encryptedMessage = parsedJson["EMessage"][EnvelopeSeal::MapKeyCipher];
    EnvelopeOpen envelopeOpen("../Test/resources/my_private_key.pem", encryptedKey, iv);

    //std::string decryptedMessage = envelopeOpen.
}
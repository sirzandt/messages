#include "MessageBase.hpp"
#include "json.hpp"
#include "SecretNumber.hpp"

/*MessageBase::MessageBase()
{

}

MessageBase::MessageBase(const std::string sessionId, const std::string messageType)
{
    mSessionId = sessionId;
    mMessageType = messageType;
}

void MessageBase::Parse(const std::string newMessage)
{
    auto j = nlohmann::json::parse(newMessage);

    if (j.contains("SessionId"))
    {
        mSessionId = j["SessionId"];
    }
    if (j.contains("MessageType"))
    {
        mMessageType = j["MessageType"];
    }
}

const std::string MessageBase::GetJSonString()
{
    nlohmann::json j;

    j["SessionId"] = mSessionId;
    j["MessageType"] = mMessageType;

    return j.dump();
}
*/

/*
        std::string mPeerId;
        std::string mMessageType;
        SecretNumber mSecretNumber;
        EnvelopeSeal mSealEnvelope;
*/
SendMessage::SendMessage(
    const std::string peerId, 
    const std::string peerKeyFilename, 
    const std::string messageType) 
{
    CreateJsonString(0, peerId, peerKeyFilename, messageType);
}

SendMessage::SendMessage(
    const long timeValue,
    const std::string peerId, 
    const std::string peerKeyFilename, 
    const std::string messageType) 
{
    CreateJsonString(timeValue, peerId, peerKeyFilename, messageType);
}

void SendMessage::CreateJsonString(const long timeValue, const std::string peerId, const std::string peerKeyFilename, const std::string messageType)
{
    nlohmann::json j;

    // Create secret number and prepare string to send
    SecretNumber secretNumber(timeValue);

    // Create seal envelope
    EnvelopeSeal envelopeSeal(peerKeyFilename);

    j["MessageType"] = messageType;
    j["peerId"] = peerId;

    nlohmann::json secretToSendNumber(secretNumber.GetRandomNumberAsString());
    nlohmann::json secretToSendHint(secretNumber.GetRandomNumber());
    nlohmann::json secretToSend;

    secretToSend["Number"] = secretToSendNumber;
    secretToSend["Hint"] = secretToSendHint;

    // Envelope the message
    envelopeSeal.AddMessage(secretToSend.dump());
    envelopeSeal.FinalizeMessage();
    std::map<std::string, std::string> encryptedMap;
    envelopeSeal.GetEncryptedMap(encryptedMap);

    // Add the encrypted message to the json string
    j["EMessage"][EnvelopeSeal::MapKeyCipher] = encryptedMap[EnvelopeSeal::MapKeyCipher];
    j["EMessage"][EnvelopeSeal::MapKeyEncryptedKey] = encryptedMap[EnvelopeSeal::MapKeyEncryptedKey];
    j["EMessage"][EnvelopeSeal::MapKeyVi] = encryptedMap[EnvelopeSeal::MapKeyVi];

    // dump the created string
    mJsonString = j.dump();

}

#include "MessageBase.hpp"
#include "json.hpp"

MessageBase::MessageBase()
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
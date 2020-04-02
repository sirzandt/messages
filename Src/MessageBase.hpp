#include <string>

class MessageBase
{
    private:
        std::string mPeerId;
        std::string mMessageType;
        std::string mSessionId;

    public:
        MessageBase();
        MessageBase(const std::string sessionId, const std::string messageType);

        std::string GetSessionId() { return mSessionId;}
        std::string GetMessageType() { return mMessageType; }

        void Parse(const std::string newMessage);

        const std::string GetJSonString();
};
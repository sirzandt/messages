#ifndef __MESSAGEBASE_HPP__
#define __MESSAGEBASE_HPP__

#include <string>
#include "SecretNumber.hpp"
#include "Envelope.hpp"

/*class MessageBase
{
    private:
        std::string mPeerId;
        std::string mMessageType;
        SecretNumber mSecretNumber;
        EnvelopeOpen mEnvelope;

    public:
        MessageBase(const std::string newMessage);
        MessageBase(const std::string peerId, const std::string messageType);

        std::string GetPeerId() { return mPeerId;}
        std::string GetMessageType() { return mMessageType; }

        const std::string GetJSonString();
};*/

class SendMessage
{
    private:
        std::string mJsonString;
        void CreateJsonString(const long timeValue, const std::string peerId, const std::string peerKeyFilename, const std::string messageType);

    public:
        SendMessage(const std::string peerId, const std::string peerKeyFilename, const std::string messageType);
        SendMessage(const long timeValue, const std::string peerId, const std::string peerKeyFilename, const std::string messageType);

        const std::string GetJSonString() {return mJsonString; };

};

#endif /* __MESSAGEBASE_HPP__ */
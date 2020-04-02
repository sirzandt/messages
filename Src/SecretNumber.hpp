#ifndef __SECRET_NUMBER_HPP__
#define __SECRET_NUMBER_HPP__

#include "openssl/evp.h"
#include "openssl/hmac.h"
#include <string>
#include <vector>

class SecretNumber
{
    private:
        std::vector<unsigned char> mRandomNumber;

        void CalculateRandomNumber(const std::vector<unsigned char> &seed, const std::vector<unsigned char> &dataBuffer);
        std::vector<unsigned char> GetDefaultSeed();
        std::vector<unsigned char> GetDataBufferFromTime(const long timeValue);

    public:
        SecretNumber();
        SecretNumber(const std::vector<unsigned char> seed);
        SecretNumber(const long timeValue);
        SecretNumber(const std::vector<unsigned char> seed, const long timeValue);

        std::string GetRandomNumberAsString();
        std::vector<unsigned char> GetRandomNumber();
};


#endif /* __SECRET_NUMBER_HPP__ */
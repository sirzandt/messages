#include "openssl/sha.h"
#include "SecretNumber.hpp"
#include <cstdio>
#include <stdexcept>
#include <openssl/rand.h>

SecretNumber::SecretNumber()
{
    CalculateRandomNumber(GetDefaultSeed(), GetDataBufferFromTime(0));
};

SecretNumber::SecretNumber(const std::vector<unsigned char> seed)
{
    CalculateRandomNumber(seed, GetDataBufferFromTime(0));
};

SecretNumber::SecretNumber(const long timeValue)
{
    CalculateRandomNumber(GetDefaultSeed(), GetDataBufferFromTime(timeValue));
};

SecretNumber::SecretNumber(const std::vector<unsigned char> seed, const long timeValue)
{
    CalculateRandomNumber(seed, GetDataBufferFromTime(timeValue));
};

std::vector<unsigned char> SecretNumber::GetDataBufferFromTime(const long timeValue)
{
    std::vector<unsigned char> result = std::vector<unsigned char>(8);

    long timeValueCopy = timeValue;

    for (int i = 7; i >= 0; i--)
    {
        result[i] = timeValueCopy & 0xFF;
        timeValueCopy >>= 8;
    }

    return result;
};

std::vector<unsigned char> SecretNumber::GetDefaultSeed()
{
    unsigned char seedBuffer[64];
    int rc = RAND_bytes(seedBuffer, 64);

    if (rc != 1)
    {
        throw std::runtime_error("Problem in generating a random number");
    }

    return std::vector<unsigned char>(seedBuffer, seedBuffer + 64);
}

void SecretNumber::CalculateRandomNumber(const std::vector<unsigned char> &seed, const std::vector<unsigned char> &dataBuffer)
{
    HMAC_CTX *pCtx = HMAC_CTX_new();

    if (pCtx == NULL)
    {
        throw std::runtime_error("No valid HMAC handler");
    }

    HMAC_Init_ex(pCtx, seed.data(), seed.size(), EVP_sha512(), NULL);
    HMAC_Update(pCtx, dataBuffer.data(), dataBuffer.size());

    unsigned char digestBuffer[SHA512_DIGEST_LENGTH];
    unsigned int digestBufferLength = 0;
    HMAC_Final(pCtx, digestBuffer, &digestBufferLength);

    mRandomNumber.clear();
    for (int i = 0; i < digestBufferLength; i++)
    {
        mRandomNumber.push_back(digestBuffer[i]);
    }

    HMAC_CTX_free(pCtx);
}

std::string SecretNumber::GetRandomNumberAsString()
{
    int offset = mRandomNumber[mRandomNumber.size()-1] & 0x0F;
    int res = (mRandomNumber[offset] & 0x7F) << 24;
    res |= (mRandomNumber[offset + 1] << 16);
    res |= (mRandomNumber[offset + 2] << 8);
    res |= (mRandomNumber[offset + 3]);

    int otp = res % 100000000L;

    char tmpBuf[9];
    std::sprintf(tmpBuf, "%08d", otp);

    return std::string(tmpBuf);
};

std::vector<unsigned char> SecretNumber::GetRandomNumber()
{
    return mRandomNumber;
};

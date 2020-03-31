#ifndef _KEYS_HPP
#define _KEYS_HPP

#include <string>
#include "openssl/evp.h"

class BaseKey
{
    protected:
        EVP_PKEY *mpKey;
    
    public:
        EVP_PKEY *GetKey() { return mpKey;}
};

class PrivateKey : public BaseKey
{
    public:
        PrivateKey() {};
        PrivateKey(std::string fileName);
        ~PrivateKey();
};

class PublicKey : public BaseKey
{
    public:
        PublicKey(std::string fileName);
        ~PublicKey();
};

#endif
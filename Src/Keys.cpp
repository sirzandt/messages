#include "Keys.hpp"
#include "openssl/pem.h"

PrivateKey::PrivateKey(std::string fileName)
{
    BIO *tBio;

    tBio = BIO_new_file(fileName.c_str(), "r");

    mpKey = PEM_read_bio_PrivateKey(tBio, NULL, NULL, NULL);

    BIO_free(tBio);
}

PrivateKey::~PrivateKey()
{
    if (mpKey)
    {
        EVP_PKEY_free(mpKey);
    }
}


PublicKey::PublicKey(std::string fileName)
{
    BIO *tBio;

    tBio = BIO_new_file(fileName.c_str(), "r");

    mpKey = PEM_read_bio_PUBKEY(tBio, NULL, NULL, NULL);

    BIO_free(tBio);
}

PublicKey::~PublicKey()
{
    if (mpKey)
    {
        EVP_PKEY_free(mpKey);
    }
}


#include "catch.hpp"
#include "Keys.hpp"
#include "openssl/ossl_typ.h"
#include "crypto/evp.h"
#include "openssl/evp.h"
#include "openssl/rsa.h"
#include <string>

TEST_CASE("Read private key", "Keys")
{
    PrivateKey testKey("../Test/resources/my_private_key.pem");

    EVP_PKEY * myKey;

    myKey = testKey.GetKey();
    REQUIRE(myKey != NULL);
    REQUIRE(myKey->type == 6);
}

TEST_CASE("Read public key", "Keys")
{
    PublicKey testKey("../Test/resources/my_public_key.pem");

    EVP_PKEY * myKey;

    myKey = testKey.GetKey();
    REQUIRE(myKey != NULL);
    REQUIRE(myKey->type == 6);
}

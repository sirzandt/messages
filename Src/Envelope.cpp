#include "openssl/ossl_typ.h"
#include "Keys.hpp"
#include "Envelope.hpp"
#include "openssl/pem.h"
#include "openssl/evp.h"
#include <vector>
#include <array>
#include <algorithm>

const std::string EnvelopeSeal::MapKeyCipher = "P1";
const std::string EnvelopeSeal::MapKeyEncryptedKey = "P2";
const std::string EnvelopeSeal::MapKeyVi = "P3";


EnvelopeSeal::EnvelopeSeal(const std::string publicKeyFilename)
{
    // Get the needed public key of the peer(s)
    PublicKey publicKey(publicKeyFilename);
    std::vector<EVP_PKEY *> publicKeys = { publicKey.GetKey()};

    // Create a context
    mpCtx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(mpCtx);

    // Prepare the iv
    int ivLength = EVP_CIPHER_iv_length(EVP_aes_256_cbc());
    mIv = std::vector<unsigned char>(ivLength);

    // Prepare the encrypted keys
    mKeyList = KeyList();
    int encryptedKeyLength = 0;
    for (auto key : publicKeys)
    {
        mKeyList.AddKey(key);
    }

    // Initialise the envelope
    auto result = EVP_SealInit(
        mpCtx, 
        EVP_aes_256_cbc(), 
        mKeyList.GetKeyStorePtr(), 
        &encryptedKeyLength, 
        mIv.data(), 
        publicKeys.data(), 
        publicKeys.size()
    );
    if (result != 0)
    {
        return;
    }
}

EnvelopeSeal::~EnvelopeSeal()
{
    // Clean the context
    if (mpCtx != NULL)
    {
        EVP_CIPHER_CTX_free(mpCtx);
    }
}

void EnvelopeSeal::AddMessage(const std::string messageToEncrypt)
{
    mPlainText += messageToEncrypt;
}

void EnvelopeSeal::FinalizeMessage()
{
    // Calculate the length of the Cipher message
    // Cipher
    // Padding (worst case)
    int cipherLength = mPlainText.size() + 16;

    // Allocate the Cypher (including the padding)
    unsigned char *pCipherBuffer = reinterpret_cast<unsigned char *>(malloc(cipherLength));

    // Update
    EVP_SealUpdate(mpCtx, pCipherBuffer, &cipherLength, reinterpret_cast<const unsigned char *>(mPlainText.c_str()), mPlainText.size());
    // note: cipherLength now contains the new length (no padding yet)

    int finalLength;
    EVP_SealFinal(mpCtx, pCipherBuffer + cipherLength, &finalLength);
    cipherLength += finalLength;
    // note: cipherLength now contains the final length (Including padding)

    // Construct final message
    mCipherText = EncodeToBase64(pCipherBuffer, cipherLength);

    free(pCipherBuffer);
}

void EnvelopeSeal::GetEncryptedMap(std::map<std::string, std::string> &encryptedMap)
{
    encryptedMap[MapKeyCipher] = mCipherText;
    encryptedMap[MapKeyEncryptedKey] = EncodeToBase64(mKeyList.getKeyPointer(0), mKeyList.getKeySize(0));
    encryptedMap[MapKeyVi] = EncodeToBase64(mIv.data(), mIv.size());
}

std::string EnvelopeSeal::EncodeToBase64(unsigned char *input, int length)
{
    // Prepare the output buffer
    const auto bufLength = 4*((length + 2)/3);
    auto output = reinterpret_cast<char *>(calloc(bufLength + 1, 1));
    EVP_EncodeBlock(reinterpret_cast<unsigned char *>(output), input, length);
    
    // Convert to a string and get rid of the buffer
    std::string result;
    result = output;
    free(output);

    return result;
}


std::vector<unsigned char> EnvelopeOpen::DecodeFromBase64(std::string input)
{
    auto result = std::vector<unsigned char>();

    // Reserve some memory
    int toLength = 3 * input.size()/4;
    auto toBlock = reinterpret_cast<unsigned char *>(calloc(toLength+1, 1));    

    toLength = EVP_DecodeBlock(toBlock, reinterpret_cast<const unsigned char *>(input.data()), input.size());

    // Compensate for padding
    int padCount = std::count_if(input.cbegin(), input.cend(), [](const char c){ return c == '=';});
    toLength -= padCount;

    // Copy to result;
    for (int i = 0; i < toLength; i++)
    {
        result.push_back(toBlock[i]);
    }

    free(toBlock);

    return result;
}

EnvelopeOpen::EnvelopeOpen(const std::string privateKeyFilename, const std::string encryptedKey, const std::string iv) : mPrivateKey(privateKeyFilename)
{
    //mPrivateKey = PrivateKey(privateKeyFilename);
    // Create a context
    mpCtx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(mpCtx);

    // Convert encryptedKey to array
    mEncryptedKeyBuffer = DecodeFromBase64(encryptedKey);
    mIvBuffer = DecodeFromBase64(iv);

    EVP_OpenInit(mpCtx, EVP_aes_256_cbc(), mEncryptedKeyBuffer.data(), mEncryptedKeyBuffer.size(), mIvBuffer.data(), mPrivateKey.GetKey());
}

EnvelopeOpen::~EnvelopeOpen()
{
    // Clean the context
    if (mpCtx != NULL)
    {
        EVP_CIPHER_CTX_free(mpCtx);
    }

}

void EnvelopeOpen::AddMessage(const std::string messageToDecrypt)
{
    mCipherText += messageToDecrypt;
}


void EnvelopeOpen::FinalizeMessage()
{
    // Decode ciphertext
    std::vector<unsigned char> cipherTextBuffer = DecodeFromBase64(mCipherText);

    //int plainTextLength = mCipherText.size() * 3 + 16;
    int plainTextLength = 1024;
    unsigned char* plainTextBuffer = reinterpret_cast<unsigned char *>(calloc(plainTextLength, 1));

    EVP_OpenUpdate(mpCtx, plainTextBuffer, &plainTextLength, cipherTextBuffer.data(), cipherTextBuffer.size());
    EVP_OpenFinal(mpCtx, plainTextBuffer + plainTextLength, &plainTextLength);

    mPlainText.clear();
    for (int i = 0; i < plainTextLength; i++)
    {
        mPlainText.push_back(static_cast<char> (plainTextBuffer[i]));
    }

    free(plainTextBuffer);
}

std::string EnvelopeOpen::GetPlainText()
{
    return mPlainText;
}

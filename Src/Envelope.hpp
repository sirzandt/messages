#include <string>
#include <map>
#include <vector>
#include "openssl/evp.h"
#include "Keys.hpp"

class KeyList
{
    private:
        std::vector<unsigned char *> mKeyArray;
        std::vector<int> mKeySize;
    
    public:
        KeyList()
        {
            mKeyArray = std::vector<unsigned char *>();
            mKeySize = std::vector<int>();
        }
        ~KeyList()
        {
            // clean the array after freeing all key-stores
            for (auto key : mKeyArray)
            {
                free((void *)key);
            }
            mKeyArray.clear();
        }

        void AddKey(EVP_PKEY *pKey)
        {
            auto const keySize = EVP_PKEY_size(pKey);
            unsigned char *pKeyStore = (unsigned char *)malloc(keySize);
            mKeyArray.push_back(pKeyStore);
            mKeySize.push_back(keySize);
        }
        unsigned char** GetKeyStorePtr()
        {
            return mKeyArray.data();
        }
        unsigned char * getKeyPointer(int index)
        {
            return mKeyArray[index];
        }

        int getKeySize(int index)
        {
            return mKeySize[index];
        }
};



class EnvelopeSeal
{
    private:
        EVP_CIPHER_CTX *mpCtx;
        KeyList mKeyList;
        std::vector<unsigned char> mIv;
        std::string mPlainText;
        std::string mCipherText;
    
    public:
        static std::string EncodeToBase64(unsigned char *input, int length);

    public:
        static const std::string MapKeyCipher;
        static const std::string MapKeyEncryptedKey;
        static const std::string MapKeyVi;

    public:
        EnvelopeSeal(const std::string publicKeyFilename);
        ~EnvelopeSeal();

        void AddMessage(const std::string messageToEncrypt);
        void FinalizeMessage();
        void GetEncryptedMap(std::map<std::string, std::string> &encryptedMap);
};

class EnvelopeOpen
{
    private:
        EVP_CIPHER_CTX *mpCtx;
        std::string mCipherText;
        std::string mPlainText;
        PrivateKey mPrivateKey;
        std::vector<unsigned char> mEncryptedKeyBuffer;
        std::vector<unsigned char> mIvBuffer;

    public:
        static std::vector<unsigned char> DecodeFromBase64(std::string input);

    public:
        EnvelopeOpen(const std::string privateKeyFilename, const std::string encryptedKey, const std::string iv);
        ~EnvelopeOpen();

        void AddMessage(const std::string messageToDecrypt);
        void FinalizeMessage();
        std::string GetPlainText();
};
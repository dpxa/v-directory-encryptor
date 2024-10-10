#ifndef ENCRYPTED_H
#define ENCRYPTED_H
#include <string>

// key has a cipher and an arbitrary starting position (not necessary)
struct EncryptionKey {
    std::string cipher;
    int startPosition = 0;
};

class EncryptedText {
private:
    // text to encrypt
    std::string text;
    EncryptionKey key;

    bool encrypted = false;

    // static const so EncryptedText can be a copyable class
    // characters that can be in the key
    static const std::string characterSet;
    static const int keyCreationSize;

    void generateKey();
    void saveKeyToFile(const std::string& outputFile) const ;

    void encrypt();
    void decrypt();

public:
    EncryptedText() = default;

    // setText called during each file reached in dfs
    void setText(const std::string& filePath);
    // for validateKeyFile, it is set if it is valid (we need to check contents, not just file existance)
    bool validateKeyFile(const std::string& inputFile);
    bool createKeyFile(const std::string& inputFile);
    void setState(bool encrypt);

    // decide whether to encrypt or decrypt text based on class member encrypted
    void process();

    void saveTextToFile(const std::string& outputFile) const;
};

#endif // ENCRYPTED_H

#include "../header/Encrypted.h"
#include <random>

void EncryptedText::encrypt() {
    // if text is already encrypted, leave
    if (isEncrypted)
        return;

    // current offset in cipher and position in text we are encrypting
    size_t offset = 0;
    size_t pos = key.startPosition;
    size_t ts = text.size(), cs = key.cipher.size();
    
    for (size_t i = 0; i < ts; ++i) {
        // get current character and manipulate it
        char& currentChar = text[pos];
        currentChar = (currentChar + key.cipher[offset]) % 128;
        
        // move to next position in cipher and text
        offset = (offset + 1) % cs;
        pos = (pos + 1) % ts;
    }
        
    isEncrypted = true;
}

void EncryptedText::decrypt() {
    if (!isEncrypted)
        return;

    // current offset in cipher and position in text we are decrypting
    size_t offset = 0;
    size_t pos = key.startPosition;
    size_t ts = text.size(), cs = key.cipher.size();
        
    for (size_t i = 0; i < ts; ++i) {
        // get current character and manipulate it
        char& currentChar = text[pos];
        currentChar = (currentChar + 128 - key.cipher[offset]) % 128;
        
        // move to next position in cipher and text
        offset = (offset + 1) % cs;
        pos = (pos + 1) % ts;
    }
    
    isEncrypted = false;   
}

// create a random key for encryption
void EncryptedText::generateKey() {
    hasCustomKey = false;

    // create rng
    std::random_device rd;
    std::mt19937 rng(rd());

    // uniform distribution over all indexes in characterSet
    std::uniform_int_distribution<int> charDistribution(0, characterSet.size() - 1);
    key.cipher.clear();
    
    // get random characters
    for (int i = 0; i < keyCreationSize; ++i)
        key.cipher += characterSet[charDistribution(rng)];

    // uniform distribution over all three digit numbers (want starting position to be last 3 characters of key in file)
    std::uniform_int_distribution<int> posDistribution(100, 999);
    key.startPosition = posDistribution(rng);
}
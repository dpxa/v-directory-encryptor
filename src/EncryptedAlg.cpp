#include "../header/Encrypted.h"
#include <random>

void EncryptedText::process() {
    (doEncrypt) ? encrypt() : decrypt();
}

void EncryptedText::encrypt() {
    // current offset in cipher and position in text we are encrypting
    size_t ts = text.size();
    // if text size is 0, there is nothing to encrypt (error when % 0)
    if (ts == 0)
        return;
    
    size_t cs = key.cipher.size();
    size_t offset = 0;
    size_t pos = key.startPosition % ts;
    
    for (size_t i = 0; i < ts; ++i) {
        char& currentChar = text[pos];

        // if character is ascii, encrypt it
        if (static_cast<unsigned char>(currentChar) <= 127) {
            currentChar = (currentChar + key.cipher[offset]) % 128;
        }
        
        // move to next position in cipher and text (no mod - mod operations are heavy)
        if (++offset == cs) offset = 0;
        if (++pos == ts) pos = 0;
    }
}

void EncryptedText::decrypt() {
    // current offset in cipher and position in text we are decrypting
    size_t ts = text.size();
    // if text size is 0, there is nothing to decrypt (error when % 0)
    if (ts == 0)
        return;
    
    size_t cs = key.cipher.size();
    size_t offset = 0;
    size_t pos = key.startPosition % ts;
        
    for (size_t i = 0; i < ts; ++i) {
        char& currentChar = text[pos];
        
        // if character is ascii, decrypt it
        if (static_cast<unsigned char>(currentChar) <= 127) {
            currentChar = (currentChar - key.cipher[offset] + 128) % 128;
        }
        
        // move to next position in cipher and text (no mod - mod operations are heavy)
        if (++offset == cs) offset = 0;
        if (++pos == ts) pos = 0;
    }
}

void EncryptedText::generateKey() {
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

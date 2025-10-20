#include <string.h>
#include "encrypt.h"

void xor_encrypt(char* data, int length, char key)
{
    int i;
    for (i = 0; i < length; i++) {
        data[i] = data[i] ^ key;
    }
}


void xor_decrypt(char* data, int length, char key)
{
    xor_encrypt(data, length, key);
}

void encrypt_string(char* str, char key)
{
    int length = strlen(str);
    xor_encrypt(str, length, key);
}

void decrypt_string(char* str, char key)
{
    xor_encrypt(str, strlen(str), key);
}

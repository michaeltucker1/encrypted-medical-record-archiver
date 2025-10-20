#ifndef ENCRYPT_H
#define ENCRYPT_H


void xor_encrypt(char* data, int length, char key);
void xor_decrypt(char* data, int length, char key);


void encrypt_string(char* str, char key);
void decrypt_string(char* str, char key);

#endif 

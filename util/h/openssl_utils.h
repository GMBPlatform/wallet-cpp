//
// openssl_utils.h
//

#ifndef OPENSSL_UTILS_H__
#define OPENSSL_UTILS_H__

#pragma once

#define PBKDF2_SHA_1_LEN   20
#define PBKDF2_SHA_384_LEN 48
#define PBKDF2_SHA_512_LEN 64

void PBKDF2_HMAC_SHA_1(const char* pass, const unsigned char* salt, int iterations, unsigned int outputBytes, char* hexResult, unsigned char* binResult);
void PBKDF2_HMAC_SHA_384(const char* pass, const unsigned char* salt, int iterations, unsigned int outputBytes, char* hexResult, unsigned char* binResult);
void PBKDF2_HMAC_SHA_512(const char* pass, const unsigned char* salt, int iterations, unsigned int outputBytes, char* hexResult, unsigned char* binResult);

#endif // OPENSSL_UTILS_H__

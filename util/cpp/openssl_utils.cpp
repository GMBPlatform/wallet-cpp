#include <stdio.h>
#include <iostream>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/obj_mac.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
// crypto.h used for the version
#include <openssl/crypto.h>

#include "..\h\openssl_utils.h"

#pragma comment(lib, "libeay32.lib") 
#pragma comment(lib, "ssleay32.lib") 

using namespace std;

void PBKDF2_HMAC_SHA_1(const char* pass, const unsigned char* salt, int iterations, unsigned int outputBytes, char* hexResult, unsigned char* binResult)
{
	unsigned int i;
	//unsigned char digest[outputBytes];
	unsigned char *digest = new unsigned char[outputBytes];

	PKCS5_PBKDF2_HMAC_SHA1(pass, strlen(pass), salt, strlen((char *)salt), iterations, outputBytes, digest);
	for (i = 0; i < sizeof(digest); i++)
	{
		sprintf(hexResult + (i * 2), "%02X", 255 & digest[i]);
		binResult[i] = digest[i];
	}
}

void PBKDF2_HMAC_SHA_384(const char* pass, const unsigned char* salt, int iterations, unsigned int outputBytes, char* hexResult, unsigned char* binResult)
{
	unsigned int i;
	//unsigned char digest[outputBytes];
	unsigned char *digest = new unsigned char[outputBytes];

	PKCS5_PBKDF2_HMAC(pass, strlen(pass), salt, strlen((char *)salt), iterations, EVP_sha384(), outputBytes, digest);
	for (i = 0; i < sizeof(digest); i++)
	{
		sprintf(hexResult + (i * 2), "%02X", 255 & digest[i]);
		binResult[i] = digest[i];
	}
}

void PBKDF2_HMAC_SHA_512(const char* pass, const unsigned char* salt, int iterations, unsigned int outputBytes, char* hexResult, unsigned char* binResult)
{
	unsigned int i;
	unsigned char digest[PBKDF2_SHA_512_LEN];
	//unsigned char *digest = new unsigned char[outputBytes];



	PKCS5_PBKDF2_HMAC(pass, strlen(pass), salt, strlen((char *)salt), iterations, EVP_sha512(), outputBytes, digest);
	for (i = 0; i < sizeof(digest); i++)
	{
		sprintf(hexResult + (i * 2), "%02X", 255 & digest[i]);
		binResult[i] = digest[i];
	};

}
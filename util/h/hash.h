////////////////////////////////////////////////////////////////////////////////
//
// hash.h
//
// Copyright (c) 2011-2012 Eric Lombrozo
// Copyright (c) 2011-2016 Ciphrex Corp.
//
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.
//

#ifndef __HASH_H___
#define __HASH_H___

#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include <openssl/hmac.h>

#include "uchar_vector.h"

const uchar_vector BITCOIN_SEED("426974636f696e2073656564"); // key = "Bitcoin seed"

inline uchar_vector hmac_sha512(const uchar_vector& data)
{
	const uchar_vector& key = BITCOIN_SEED;
	unsigned char* digest = HMAC(EVP_sha512(), (unsigned char*)&key[0], key.size(), (unsigned char*)&data[0], data.size(), NULL, NULL);
	return uchar_vector(digest, 64);
}
inline uchar_vector sha256(const uchar_vector& data)
{
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, &data[0], data.size());
	SHA256_Final(hash, &sha256);
	uchar_vector rval(hash, SHA256_DIGEST_LENGTH);
	return rval;
}
inline uchar_vector sha256_2(const uchar_vector& data)
{
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, &data[0], data.size());
	SHA256_Final(hash, &sha256);
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, hash, SHA256_DIGEST_LENGTH);
	SHA256_Final(hash, &sha256);
	uchar_vector rval(hash, SHA256_DIGEST_LENGTH);
	return rval;
}
inline uchar_vector ripemd160(const uchar_vector& data)
{
	unsigned char hash[RIPEMD160_DIGEST_LENGTH];
	RIPEMD160_CTX ripemd160;
	RIPEMD160_Init(&ripemd160);
	RIPEMD160_Update(&ripemd160, &data[0], data.size());
	RIPEMD160_Final(hash, &ripemd160);
	uchar_vector rval(hash, RIPEMD160_DIGEST_LENGTH);
	return rval;
}
inline uchar_vector hmac_sha512(const uchar_vector& key, const uchar_vector& data)
{
	unsigned char* digest = HMAC(EVP_sha512(), (unsigned char*)&key[0], key.size(), (unsigned char*)&data[0], data.size(), NULL, NULL);
	return uchar_vector(digest, 64);
}

inline uchar_vector hmac_sha256(const uchar_vector& key, const uchar_vector& data)
{
	unsigned char* digest = HMAC(EVP_sha256(), (unsigned char*)&key[0], key.size(), (unsigned char*)&data[0], data.size(), NULL, NULL);
	return uchar_vector(digest, 32);
}
#endif
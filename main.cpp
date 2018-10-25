// Copyright (c) 2017 The GMB Core developers and mikks
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <iostream>
#include <sstream>
//#include <boost/algorithm/string/replace.hpp>

#include "..\wallet\util\h\seed_create.h"
#include "..\wallet\util\h\hmac_sha512_create.h"
#include "..\wallet\util\h\get_public_key.h"
#include "..\wallet\util\h\Base58Check.h"
#include "..\wallet\util\h\typedefs.h"
#include "..\wallet\util\h\HDKeychain.h"
#include "..\wallet\util\h\ext_key_decode.h"
#include "..\wallet\util\h\ckd.h"
#include "..\wallet\util\qrcode\QREncode.h"
using namespace std;

void main()
{
	qrcode qr_code;
	/******************************************************************
	wallet
	******************************************************************/
	SeedCreate seed_create;
	SeedKey seed_key;
	GetPublicKey getpublickey;
	HDExtendedKey  HD_Extended_Key;

	char * seed_key_str = seed_create.m_seed_create();
	string hmac_sha512_str = seed_key.m_seed_key(seed_key_str);
	string private_key_str = hmac_sha512_str.substr(hmac_sha512_str.length() - hmac_sha512_str.length(), hmac_sha512_str.length() / 2);
	string chain_code_str = hmac_sha512_str.substr(hmac_sha512_str.length() / 2, hmac_sha512_str.length());
	char * get_public_key_str = getpublickey.m_get_public_key(private_key_str); // NID_X9_62_prime256v1 = secp256r1 , NID_secp256k1 = secp256k1
	uchar_vector get_address_str; get_address_str = get_public_key_str;
	uchar_vector tmp_ddress_str; tmp_ddress_str.push_back(0x00);
	tmp_ddress_str += ripemd160(sha256(get_address_str));
	
	cout << "seed key : " << seed_key_str << endl;
	cout << "hmac sha512 : " << hmac_sha512_str << endl;
	cout << "private key : " << private_key_str << endl;
	cout << "chain code : " << chain_code_str << endl;
	cout << "public key" << "( " << SN_secp256k1 << " )" << " : " << get_public_key_str << endl;
	cout << "address : " << toBase58Check(tmp_ddress_str) << endl; //0 = addressVersion 
	qr_code.m_qrcode(toBase58Check(tmp_ddress_str).c_str());

	/******************************************************************
	hd wallet
	******************************************************************/
	bytes_t HDKeychain_private_key;
	bytes_t HDKeychain_public_key;
	bytes_t HDKeychain_chain_code;
	string toBase58Check_HDKeychain_private_key;
	string toBase58Check_HDKeychain_public_key;

	HDKeychain_private_key = uchar_vector(private_key_str);
	HDKeychain_public_key = uchar_vector(get_public_key_str);

	HDKeychain_chain_code = uchar_vector(chain_code_str);

	HDKeychain_private_key = HD_Extended_Key.m_HD_Extende_dKey(HDKeychain_private_key, HDKeychain_chain_code, 0, 0, 0);
	HDKeychain_public_key = HD_Extended_Key.m_HD_Extende_dKey(HDKeychain_public_key, HDKeychain_chain_code, 0, 0, 0);

	toBase58Check_HDKeychain_private_key = toBase58Check(HDKeychain_private_key);
	toBase58Check_HDKeychain_public_key = toBase58Check(HDKeychain_public_key);

	cout << "hd private key : " << toBase58Check_HDKeychain_private_key << endl;
	cout << "hd public key : " << toBase58Check_HDKeychain_public_key << endl;
	/******************************************************************
	Decode
	******************************************************************/
	ExtKeyDecode ext_key_decode;
	string decode_base58_private_str;
	string decode_base58_public_str;
	vector<unsigned char> tmp;

	DecodeBase58(toBase58Check_HDKeychain_private_key.c_str(), tmp);
	decode_base58_private_str = uchar_vector(tmp).getHex().substr(uchar_vector(tmp).getHex().length() - uchar_vector(tmp).getHex().length(), uchar_vector(tmp).getHex().length() - 8);

	DecodeBase58(toBase58Check_HDKeychain_public_key.c_str(), tmp);
	decode_base58_public_str = uchar_vector(tmp).getHex().substr(uchar_vector(tmp).getHex().length() - uchar_vector(tmp).getHex().length(), uchar_vector(tmp).getHex().length() - 8);

	ext_key_decode.m_ext_key_Decode(uchar_vector(decode_base58_private_str));
	ext_key_decode.m_ext_key_Decode(uchar_vector(decode_base58_public_str));
	/******************************************************************
	CKD
	******************************************************************/
	string index;
	ckd ckd;
	cout << "index 입력" << endl;
	getline(cin, index);
	ckd.m_ckd(ext_key_decode.Decode_key, ext_key_decode.Decode_chain_code_, ext_key_decode.Decode_depth, atoi(index.c_str()));
	cout << ckd.get_child_private_key << endl;
	cout << ckd.get_child_public_key << endl;
	/******************************************************************
	Decode -> CKD
	******************************************************************/
	cout << "index 입력" << endl;
	getline(cin, index);
	DecodeBase58(ckd.get_child_private_key.c_str(), tmp);
	decode_base58_private_str = uchar_vector(tmp).getHex().substr(uchar_vector(tmp).getHex().length() - uchar_vector(tmp).getHex().length(), uchar_vector(tmp).getHex().length() - 8);

	DecodeBase58(ckd.get_child_public_key.c_str(), tmp);
	decode_base58_public_str = uchar_vector(tmp).getHex().substr(uchar_vector(tmp).getHex().length() - uchar_vector(tmp).getHex().length(), uchar_vector(tmp).getHex().length() - 8);

	ext_key_decode.m_ext_key_Decode(uchar_vector(decode_base58_private_str));
	ext_key_decode.m_ext_key_Decode(uchar_vector(decode_base58_public_str));

	ckd.m_ckd(ext_key_decode.Decode_key, ext_key_decode.Decode_chain_code_, ext_key_decode.Decode_depth, atoi(index.c_str()));
	cout << ckd.get_child_private_key << endl;
	cout << ckd.get_child_public_key << endl;
	getchar();
}
























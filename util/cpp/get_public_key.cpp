#include <string>

#include <sstream>

#include <openssl/ec.h>

#include "..\wallet\util\h\get_public_key.h"
#include "..\wallet\util\h\secp256k1_openssl.h"
char* GetPublicKey::m_get_public_key(std::string private_key)
{
	std::string algorithm_str;
	int algorithm;
	std::cout << "algorithm ¼±ÅÃ" << std::endl;
	std::cout << "1. secpk1" << std::endl;
	std::cout << "2. secpr1" << std::endl;
	std::getline(std::cin, algorithm_str);
	if (algorithm_str.compare("1") == 0)
		algorithm = NID_secp256k1;
	else
		algorithm = NID_secp256k1;//NID_X9_62_prime256v1;
	EC_KEY *eckey = EC_KEY_new_by_curve_name(algorithm);
	const EC_GROUP *group = EC_KEY_get0_group(eckey);
	EC_POINT *pub_key = EC_POINT_new(group);
	BIGNUM start;
	BN_init(&start);
	BIGNUM *res = &start;
	BN_CTX *ctx = BN_CTX_new();


	BN_hex2bn(&res, private_key.c_str());

	EC_KEY_set_private_key(eckey, res);
	EC_KEY_set_public_key(eckey, pub_key);

	if (!EC_POINT_mul(group, pub_key, res, NULL, NULL, ctx))
		printf("Error at EC_POINT_mul.\n");

	char *pub_str = EC_POINT_point2hex(group, pub_key, POINT_CONVERSION_COMPRESSED, ctx);

	EC_POINT_free(pub_key);
	BN_CTX_free(ctx);

	return pub_str;
}

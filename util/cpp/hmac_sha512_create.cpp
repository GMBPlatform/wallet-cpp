#include <iostream>

//#include <boost/algorithm/string/replace.hpp>

#include "..\wallet\util\h\typedefs.h"
#include "..\wallet\util\h\hex_out.h"
#include "..\wallet\util\h\hmac_sha512_create.h"
using namespace std;
string SeedKey::m_seed_key(char *seed)
{
	char *seed_key_str = seed;
	const uchar_vector seed_key(seed_key_str);
	bytes_t digest = hmac_sha512(seed_key);
	return uchar_vector(digest).getHex();
}

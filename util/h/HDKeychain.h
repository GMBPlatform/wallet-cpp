#include <iostream>
#include "..\wallet\util\h\secp256k1_openssl.h"
#include "..\wallet\util\h\uchar_vector.h"
#include "..\wallet\util\h\BigInt.h"
using namespace std;
class HDExtendedKey
{
public:
	bytes_t m_HD_Extende_dKey(bytes_t& key, bytes_t& chain_code, uint32_t child_num, uint32_t parent_fp, uint32_t depth);
	bytes_t m_extkey(bytes_t& key, uint32_t version, bytes_t& chain_code, uint32_t child_num, uint32_t parent_fp, uint32_t depth);

};



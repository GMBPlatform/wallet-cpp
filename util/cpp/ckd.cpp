#include "..\wallet\util\h\ckd.h"
#include "..\wallet\util\h\define.h"
#include "..\wallet\util\h\get_public_key.h"
#include "..\wallet\util\h\HDKeychain.h"
#include "..\wallet\util\h\Base58Check.h"
uint32_t fp(bytes_t key);
void ckd::m_ckd(bytes_t private_key, bytes_t chain_code, uint32_t depth, uint32_t index)
{



	if ((private_key.size() != 33 && private_key[0] != 0x00))
	{
		throw runtime_error("Cannot do private key derivation on public key.");
	}
	define_ m_define;
	GetPublicKey getpublickey;
	uchar_vector data;
	bytes_t public_key = uchar_vector(getpublickey.m_get_public_key(uchar_vector(private_key).getHex()));

	if ((depth % 2) != 0)
	{
		index = m_define.P(index);
		data += private_key;
	}
	else
	{
		data += public_key;
	}

	HDExtendedKey  HD_Extended_Key;

	data.push_back(index >> 24);
	data.push_back((index >> 16) & 0xff);
	data.push_back((index >> 8) & 0xff);
	data.push_back(index & 0xff);

	unsigned char child_depth_ = depth + 1;
	uint32_t child_parent_fp_ = fp(public_key);;
	uint32_t child_child_num_ = index;
	bytes_t child_chain_code_;
	bytes_t digest = hmac_sha512(chain_code, data);
	child_chain_code_.assign(digest.begin() + 32, digest.end());
	bytes_t left32(digest.begin(), digest.begin() + 32);
	BigInt Il(left32);
	bytes_t child_key_;

	if (Il >= m_define.CURVE_ORDER()) throw runtime_error("Keychain is invalid.");

	/******************************************************************
	private_key
	******************************************************************/
	BigInt k(private_key);
	//cout << k.getDec() << endl;
	k += Il;
	//cout << k.getDec() << endl;
	k %= m_define.CURVE_ORDER();
	//cout << k.getDec() << endl;
	if (k.isZero()) throw runtime_error("Keychain is invalid.");
	bytes_t child_key = k.getBytes();
	uchar_vector padded_key(33 - child_key.size(), 0);
	padded_key += child_key;
	child_key_ = padded_key;

	get_child_private_key = toBase58Check(HD_Extended_Key.m_HD_Extende_dKey(child_key_, child_chain_code_, child_child_num_, child_parent_fp_, child_depth_));

	/******************************************************************
	public_key
	******************************************************************/
	CoinCrypto::secp256k1_point K;
	K.bytes(public_key);
	K.generator_mul(left32);
	if (K.is_at_infinity()) throw runtime_error("Keychain is invalid.");
	child_key_ = K.bytes();

	get_child_public_key = toBase58Check(HD_Extended_Key.m_HD_Extende_dKey(child_key_, child_chain_code_, child_child_num_, child_parent_fp_, child_depth_));

}
uint32_t fp(bytes_t key)
{
	bytes_t hash = ripemd160(sha256(key));
	return (uint32_t)hash[0] << 24 | (uint32_t)hash[1] << 16 | (uint32_t)hash[2] << 8 | (uint32_t)hash[3];
}

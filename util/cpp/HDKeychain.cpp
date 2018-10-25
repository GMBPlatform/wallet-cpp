#include "..\wallet\util\h\HDKeychain.h"
#include "..\wallet\util\h\define.h"
bytes_t HDExtendedKey::m_HD_Extende_dKey(bytes_t& key, bytes_t& chain_code, uint32_t child_num, uint32_t parent_fp, uint32_t depth)
{
	define_ m_define;
	if (chain_code.size() != 32)
	{
		throw std::runtime_error("Invalid chain code.");
	}
	if (key.size() == 33 && key[0] == 0x00)
	{
		uchar_vector tmp_str;
		tmp_str = uchar_vector(key).getHex().substr(uchar_vector(key).getHex().length() - uchar_vector(key).getHex().length() + 2, uchar_vector(key).getHex().length());
		key = tmp_str;
	}
	if (key.size() == 32)
	{
		
		// key is private
		BigInt n(key);
		if (n >= m_define.CURVE_ORDER() || n.isZero()) {
			throw std::runtime_error("Invalid key.");
		}

		uchar_vector privkey;
		privkey.push_back(0x00);
		privkey += key;
		key = privkey;
	}
	else if (key.size() == 33)
	{
		// key is public
		try
		{

			CoinCrypto::secp256k1_point K(key);
		}
		catch (...)
		{
			throw std::runtime_error("Invalid key.");
		}
	}
	else
	{
		throw std::runtime_error("Invalid key.");
	}
	bool isPrivate = (key.size() == 33 && key[0] == 0x00);
	uint32_t version_ = isPrivate ? m_define.priv_version() : m_define.pub_version();

	return HDExtendedKey::m_extkey(key, version_, chain_code, child_num, parent_fp, depth);
}


bytes_t HDExtendedKey::m_extkey(bytes_t& key, uint32_t version, bytes_t& chain_code, uint32_t child_num, uint32_t parent_fp, uint32_t depth)
{
	uchar_vector extkey;

	extkey.push_back((uint32_t)version >> 24);
	extkey.push_back(((uint32_t)version >> 16) & 0xff);
	extkey.push_back(((uint32_t)version >> 8) & 0xff);
	extkey.push_back((uint32_t)version & 0xff);

	extkey.push_back(depth);

	extkey.push_back((uint32_t)parent_fp >> 24);
	extkey.push_back(((uint32_t)parent_fp >> 16) & 0xff);
	extkey.push_back(((uint32_t)parent_fp >> 8) & 0xff);
	extkey.push_back((uint32_t)parent_fp & 0xff);

	extkey.push_back((uint32_t)child_num >> 24);
	extkey.push_back(((uint32_t)child_num >> 16) & 0xff);
	extkey.push_back(((uint32_t)child_num >> 8) & 0xff);
	extkey.push_back((uint32_t)child_num & 0xff);

	extkey += chain_code;
	extkey += key;

	return extkey;
}
#include "..\wallet\util\h\ext_key_decode.h"
#include "..\wallet\util\h\define.h"

using namespace std;

void ExtKeyDecode::m_ext_key_Decode(const bytes_t& extkey)
{

	string m_version;
	uint32_t	version_;
	uint32_t depth_;
	uint32_t	parent_fp_;
	uint32_t	child_num_;
	bytes_t chain_code_;
	bytes_t key_;
	string key_str;
	define_ m_defile;
	if (extkey.size() == 78)
	{


		version_ = ((uint32_t)extkey[0] << 24) | ((uint32_t)extkey[1] << 16) | ((uint32_t)extkey[2] << 8) | (uint32_t)extkey[3];
		depth_ = extkey[4];
		parent_fp_ = ((uint32_t)extkey[5] << 24) | ((uint32_t)extkey[6] << 16) | ((uint32_t)extkey[7] << 8) | (uint32_t)extkey[8];
		child_num_ = ((uint32_t)extkey[9] << 24) | ((uint32_t)extkey[10] << 16) | ((uint32_t)extkey[11] << 8) | (uint32_t)extkey[12];
		chain_code_.assign(extkey.begin() + 13, extkey.begin() + 45);
		key_.assign(extkey.begin() + 45, extkey.begin() + 78);

		if (version_ == m_defile.priv_version())
		{
			m_version = "private";
			key_str = uchar_vector(key_).getHex().substr(uchar_vector(key_).getHex().length() - uchar_vector(key_).getHex().length(), uchar_vector(key_).getHex().length());
			Decode_depth = extkey[4];
			Decode_chain_code_.assign(extkey.begin() + 13, extkey.begin() + 45);
			Decode_key = uchar_vector(key_str);
		}
		else
		{
			m_version = "public";
			key_str = uchar_vector(key_).getHex().substr(uchar_vector(key_).getHex().length() - uchar_vector(key_).getHex().length(), uchar_vector(key_).getHex().length());
		}
		cout << m_version << "======decryption(" << uchar_vector(extkey).getHex() << ")======" << endl;
		cout << "version : " << version_ << endl;
		cout << "depth : " << dec << depth_ << endl;
		cout << "parent_fp : " << parent_fp_ << endl;
		cout << "child_num : " << child_num_ << endl;
		cout << "chain_code_ : " << uchar_vector(chain_code_).getHex() << endl;
		cout << "key_ : " << key_str << endl;

	}
	else
	{
		throw runtime_error("Invalid extended key length.");
	}
}
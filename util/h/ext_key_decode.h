#include "..\wallet\util\h\typedefs.h"

class ExtKeyDecode
{
public:
	void m_ext_key_Decode(const bytes_t& extkey);
	uint32_t Decode_depth;
	bytes_t Decode_chain_code_;
	bytes_t Decode_key;


};




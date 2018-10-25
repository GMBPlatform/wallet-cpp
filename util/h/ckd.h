#include "..\wallet\util\h\typedefs.h"
using namespace std;
class ckd
{
public:
	void m_ckd(bytes_t private_key, bytes_t chain_code, uint32_t depth, uint32_t index);
	string get_child_private_key;
	string get_child_public_key;


};

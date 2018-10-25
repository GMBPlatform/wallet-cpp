#include "..\wallet\util\h\uchar_vector.h"
#include "..\wallet\util\h\BigInt.h"
class define_
{
public:
#define ImageData "ImageData"
#define QR_File_Extension "image/jpeg" //"image/jpeg", "image/bmp", "image/gif", "image/tiff", "image/png"
#define QR_File_Name_Temp "qrcode.jpg"
#define QR_File_Name "mofas.jpg"
#define QR_File_Logo "logo.jpg"
#define QR_File_Edge "edge.jpg"
#define QR_File_Size_width 100
#define QR_File_Size_height 100
	BigInt CURVE_ORDER() const { return uchar_vector("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141"); }
	uint32_t priv_version()const { return 0x0488ade4; }
	uint32_t pub_version()const { return 0x0488b21e; } 
	uint32_t P(uint32_t i) { return 0x80000000 | i; }
	uint32_t CKD_private_default = P(2147483648);
	
};




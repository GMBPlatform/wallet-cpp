#include <string>
#include <iomanip>

#include "..\wallet\util\h\utf8_encode.h"
#include "..\wallet\util\h\openssl_utils.h"
#include "..\wallet\util\h\wordlist_kr.h"
#include "..\wallet\util\h\password_utf8.h"
#include "..\wallet\util\h\select_wordlist.h"
#include "..\wallet\util\h\utf8proc.h"
#include "..\wallet\util\h\str_conv.h"
#include "..\wallet\util\h\seed_create.h"
using namespace std;
char *SeedCreate::m_seed_create()
{
	/******************************************************************
	class
	******************************************************************/
	strconv str_conv; // wstring to string
	WordListKR wordlist_kr; // word list class
	SelectWordlist select_wordlist; // Select word list class
	PasswordUTF8 password_utf8; // password to hex class
	UTF8Encode utf8_encode; // utf8 encode
	/******************************************************************
	variable
	******************************************************************/
	char hexResult[2 * PBKDF2_SHA_512_LEN + 1];
	unsigned char binResult[PBKDF2_SHA_512_LEN];
	/******************************************************************
	etc
	******************************************************************/
	srand(time(NULL)); // 난수 발생을 위한 초기화
	/******************************************************************
	Json
	******************************************************************/
	password_utf8.m_PasswordUTF8();		// Password to utf-8
	password_utf8.m_sentenceUTF8();		// sentence to utf-8
	select_wordlist.m_colors();				// colors to utf-8
	select_wordlist.m_locations();			// locations to utf-8
	select_wordlist.m_shapes();				// shapes to utf-8
	/******************************************************************
	salt to utf8
	******************************************************************/
	system("cls"); // 청소~!
	wstring ran = str_conv.m_strconv(wordlist_kr.m_WordListKR[rand() % 2048]); //word list 랜덤 선택
	string m_salt_str = select_wordlist.m_colors_str + " " + select_wordlist.m_location_str + " " + select_wordlist.m_shapes_str + " " + str_conv.m_strconv(ran) + " " + password_utf8.m_password_str; // 색상,지역,도형,랜덤wordlist,패스워드
	cout << "salt : " << m_salt_str << endl;
	m_salt_str = utf8_encode.m_utf8_encode(str_conv.m_strconv(m_salt_str));
	string out_u8_salt_str = string((char *)utf8proc_NFKD((unsigned char *)m_salt_str.c_str()));
	/******************************************************************
	password to utf8
	******************************************************************/
	string m_password_str = password_utf8.m_sentence_str + " " + password_utf8.m_password_str; // 패스워드,문장
	cout << "password salt : " << m_password_str << endl;
	m_password_str = utf8_encode.m_utf8_encode(str_conv.m_strconv(m_password_str));
	string out_u8_password_str = string((char *)utf8proc_NFKD((unsigned char *)m_password_str.c_str()));
	/******************************************************************
	PBKDF2_HMAC_SHA_512 : seed create
	******************************************************************/
	PBKDF2_HMAC_SHA_512(out_u8_password_str.c_str(), (unsigned char *)out_u8_salt_str.c_str(), 2048, PBKDF2_SHA_512_LEN, hexResult, binResult);
	return hexResult;
}

#include <string>
#include <iomanip>
#include <sstream>

#include "..\h\password_utf8.h"
#include "..\h\ansi_to_utf8.h"
#include "..\h\hex_out.h"

string PasswordUTF8::m_PasswordUTF8()
{
	
	ANSIToUTF8 to_str;
	HexOut hex_str;
	string user_pw_str;
	cout << "패스워드 입력" << endl;
	getline(cin, m_password_str);
	const char* loc_ptr = m_password_str.c_str();

	char* utf8_char = to_str.m_ANSIToUTF8(loc_ptr);
	user_pw_str = string(utf8_char);



	cout << hex << setfill('0');
	hex_str.m_HexOutString(user_pw_str);

	return  hex_str.m_HexOutString(user_pw_str);
}

string PasswordUTF8::m_sentenceUTF8()
{

	ANSIToUTF8 to_str;
	HexOut hex_str;
	string user_sen_str;
	cout << "문장 입력" << endl;
	getline(cin, m_sentence_str);
	const char* loc_ptr = m_sentence_str.c_str();

	char* utf8_char = to_str.m_ANSIToUTF8(loc_ptr);
	user_sen_str = string(utf8_char);


	cout << hex << setfill('0');
	hex_str.m_HexOutString(user_sen_str);

	return  hex_str.m_HexOutString(user_sen_str);
}
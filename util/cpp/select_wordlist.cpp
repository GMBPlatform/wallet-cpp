#include <string>
#include <iomanip>
#include <sstream>
#include <fstream>

#include <json/json.h>

#include "..\h\select_wordlist.h"
#include "..\h\ansi_to_utf8.h"
#include "..\h\hex_out.h"

#define dir "util\\json\\select_wordlist.json"

#ifdef _DEBUG
#pragma comment(lib, "json_vc141_libmtd.lib") // json debug lib
#else
#pragma comment(lib, "json_vc141_libmt.lib") // json release lib
#endif

ANSIToUTF8 to_str;
HexOut hex_str;


string SelectWordlist::m_colors()
{
	string sel_colors;
	string user_colors_str;
	Json::Value root;
	ifstream json_file(dir, std::ifstream::binary);
	json_file >> root;
	int i = 1;
	for (auto it = root["colors"].begin(); it != root["colors"].end(); it++,i++)
	{
		
			std::cout << (i) << std::dec << " : " << (*it) << std::endl;
	
	}
	cout << "colors 입력" << endl;
	getline(cin, sel_colors);
	std::cout << root["colors"][atoi(sel_colors.c_str())-1].asString() << endl;
	m_colors_str = string(root["colors"][atoi(sel_colors.c_str())-1].asString());
	const char* colors_ptr = m_colors_str.c_str();
	char* utf8_char = to_str.m_ANSIToUTF8(colors_ptr);
	user_colors_str = string(utf8_char);
	return hex_str.m_HexOutString(user_colors_str);
}
string SelectWordlist::m_locations()
{
	string sel_locations;
	string user_loc_str;
	Json::Value root;
	ifstream json_file(dir, std::ifstream::binary);
	json_file >> root;
	int i = 1;
	for (auto it = root["locations"].begin(); it != root["locations"].end(); it++, i++)
	{

		std::cout << (i) << std::dec << " : " << (*it) << std::endl;

	}
	cout << "locations 입력" << endl;
	getline(cin, sel_locations);
	std::cout << root["locations"][atoi(sel_locations.c_str())-1].asString() << endl;
	m_location_str = string(root["locations"][atoi(sel_locations.c_str())-1].asString());
	const char* loc_ptr = m_location_str.c_str();
	char* utf8_char = to_str.m_ANSIToUTF8(loc_ptr);
	user_loc_str = string(utf8_char);
	return hex_str.m_HexOutString(user_loc_str);
}
string SelectWordlist::m_shapes()
{
	string sel_shapes;
	string user_shapes_str;
	Json::Value root;
	ifstream json_file(dir, std::ifstream::binary);
	json_file >> root;
	int i = 1;
	for (auto it = root["shapes"].begin(); it != root["shapes"].end(); it++, i++)
	{

		std::cout << (i) << std::dec << " : " << (*it) << std::endl;

	}
	cout << "shapes 입력" << endl;
	getline(cin, sel_shapes);
	std::cout << root["shapes"][atoi(sel_shapes.c_str())-1].asString() << endl;
	m_shapes_str = string(root["shapes"][atoi(sel_shapes.c_str())-1].asString());
	const char* shapes_ptr = m_shapes_str.c_str();
	char* utf8_char = to_str.m_ANSIToUTF8(shapes_ptr);
	user_shapes_str = string(utf8_char);
	return hex_str.m_HexOutString(user_shapes_str);
}

#include "c_files.hpp"

#define WIN32_LEAN_AND_MEAN  
#include <Windows.h>
#include <filesystem>

c_files* cs_files = new c_files();

void c_files::setup(const char* file_name, const char* file_dir)
{
	memset(m_fileName, 0x00, 255);
	strcpy_s(m_fileName, file_dir);
	strcat_s(m_fileName, file_name);
}

int c_files::read_int(const char* element, const char* item)
{
	return GetPrivateProfileIntA(element, item, NULL, m_fileName);
}

int c_files::read_int_param(const char* element, const char* item, const char* param)
{
	char m_ParamName[255];
	memset(m_ParamName, 0x00, 255);
	strcpy_s(m_ParamName, item);
	strcat_s(m_ParamName, param);
	return GetPrivateProfileIntA(element, m_ParamName, NULL, m_fileName);
}

float c_files::read_float(const char* element, const char* item)
{
	char result[255];
	GetPrivateProfileStringA(element, item, NULL, result, 255, m_fileName);
	return std::stof(result);
}

float c_files::read_float_param(const char* element, const char* item, const char* param)
{
	char m_ParamName[255];
	memset(m_ParamName, 0x00, 255);
	strcpy_s(m_ParamName, item);
	strcat_s(m_ParamName, param);

	char result[255];
	GetPrivateProfileStringA(element, m_ParamName, NULL, result, 255, m_fileName);
	return std::stof(result);
}

bool c_files::read_bool(const char* element, const char* item)
{
	char result[255];
	GetPrivateProfileStringA(element, item, NULL, result, 255, m_fileName);
	return (strcmp(result, "True") == 0 || strcmp(result, "true") == 0) ? true : false;
}

bool c_files::read_bool_param(const char* element, const char* item, const char* param)
{
	char m_ParamName[255];
	memset(m_ParamName, 0x00, 255);
	strcpy_s(m_ParamName, item);
	strcat_s(m_ParamName, param);

	char result[255];
	GetPrivateProfileStringA(element, m_ParamName, NULL, result, 255, m_fileName);
	return (strcmp(result, "True") == 0 || strcmp(result, "true") == 0) ? true : false;
}

char* c_files::read_char(const char* element, const char* item)
{
	static char result[255];
	GetPrivateProfileStringA(element, item, NULL, result, 255, m_fileName);
	return result;
}

char* c_files::read_char_param(const char* element, const char* item, const char* param)
{
	char m_ParamName[255];
	memset(m_ParamName, 0x00, 255);
	strcpy_s(m_ParamName, item);
	strcat_s(m_ParamName, param);

	static char result[255];
	GetPrivateProfileStringA(element, m_ParamName, NULL, result, 255, m_fileName);
	return result;
}

void c_files::write_int(const char* element, const char* item, int value)
{
	char temp[255];
	sprintf_s(temp, "%d", value);
	WritePrivateProfileStringA(element, item, temp, m_fileName);
}

void c_files::write_int_param(const char* element, const char* item, const char* param, int value)
{
	char m_ParamName[255];
	memset(m_ParamName, 0x00, 255);
	strcpy_s(m_ParamName, item);
	strcat_s(m_ParamName, param);

	char temp[255];
	sprintf_s(temp, "%d", value);
	WritePrivateProfileStringA(element, m_ParamName, temp, m_fileName);
}

void c_files::write_float(const char* element, const char* item, float value)
{
	char temp[255];
	sprintf_s(temp, "%f", value);
	WritePrivateProfileStringA(element, item, temp, m_fileName);
}

void c_files::write_float_param(const char* element, const char* item, const char* param, float value)
{
	char m_ParamName[255];
	memset(m_ParamName, 0x00, 255);
	strcpy_s(m_ParamName, item);
	strcat_s(m_ParamName, param);

	char temp[255];
	sprintf_s(temp, "%f", value);
	WritePrivateProfileStringA(element, m_ParamName, temp, m_fileName);
}

void c_files::write_bool(const char* element, const char* item, bool value)
{
	char temp[255];
	sprintf_s(temp, "%s", value ? "true" : "false");
	WritePrivateProfileStringA(element, item, temp, m_fileName);
}

void c_files::write_bool_param(const char* element, const char* item, const char* param, bool value)
{
	char m_ParamName[255];
	memset(m_ParamName, 0x00, 255);
	strcpy_s(m_ParamName, item);
	strcat_s(m_ParamName, param);

	char temp[255];
	sprintf_s(temp, "%s", value ? "true" : "false");
	WritePrivateProfileStringA(element, m_ParamName, temp, m_fileName);
}

void c_files::write_char(const char* element, const char* item, char* value)
{
	WritePrivateProfileStringA(element, item, value, m_fileName);
}

void c_files::write_char_param(const char* element, const char* item, const char* param, char* value)
{
	char m_ParamName[255];
	memset(m_ParamName, 0x00, 255);
	strcpy_s(m_ParamName, item);
	strcat_s(m_ParamName, param);

	WritePrivateProfileStringA(element, m_ParamName, value, m_fileName);
}
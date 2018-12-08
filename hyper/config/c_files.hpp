#pragma once

class c_files
{
public:
	void setup(const char* file_name, const char* file_dir);

	int	read_int(const char* element, const char* item);
	int	read_int_param(const char* element, const char* item, const char* param);
	float read_float(const char* element, const char* item);
	float read_float_param(const char* element, const char* item, const char* param);
	bool read_bool(const char* element, const char* item);
	bool read_bool_param(const char* element, const char* item, const char* param);
	char* read_char(const char* element, const char* item);
	char* read_char_param(const char* element, const char* item, const char* param);

	void write_int(const char* element, const char* item, int value);
	void write_int_param(const char* element, const char* item, const char* param, int value);
	void write_float(const char* element, const char* item, float value);
	void write_float_param(const char* element, const char* item, const char* param, float value);
	void write_bool(const char* element, const char* item, bool value);
	void write_bool_param(const char* element, const char* item, const char* param, bool value);
	void write_char(const char* element, const char* item, char* value);
	void write_char_param(const char* element, const char* item, const char* param, char* value);

private:
	char m_fileName[255];
};

extern c_files* cs_files;
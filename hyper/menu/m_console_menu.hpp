#pragma once

class m_console_menu
{
public:
	void update();
	void show_console_cursor(bool flag);
};

extern m_console_menu* cs_console_menu;
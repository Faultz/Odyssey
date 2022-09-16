#include "stdafx.h"

#include <cell/cell_fs.h>

keyboard_t g_keyboard;

void keyboard_t::call(std::string title, key_callback_t callback)
{
	m_flags |= key_open;
	g_menu.remove_flags(open);

	m_strTitle = title;
	m_strBuffer.clear();
	m_callback = callback;
}

void keyboard_t::add(const char key)
{
	m_strBuffer.push_back(key);
}

void keyboard_t::on_scroll()
{
	if (!handle_keyboard());
	handle_history();
}

char keyboard_t::get_text_type(int row, int column)
{
	char table_value;
	if (test_flag(key_symbol))
	{
		table_value = g_keyTable[row][column].alt_key;
		if (table_value == '\0')
		{
			if (!isdigit(table_value))
				table_value = toupper(g_keyTable[row][column].key);
		}
	}
	else
		table_value = tolower(g_keyTable[row][column].key);
	return table_value;
}

void keyboard_t::execute()
{
	g_history.write_history(m_strBuffer);

	g_log.print("user input made with string - '%s'", m_strBuffer.data());
	m_callback(m_strBuffer);

	m_strBuffer.clear();

	m_flags = 0;
	g_menu.set_flags(open);
}

bool keyboard_t::handle_keyboard()
{
	if (test_flag(key_history))
		return false;

	if (g_menu.is_clicked(BUTTON_R2))
	{
		if (test_flag(key_symbol))
			m_flags &= ~key_symbol;
		else
			m_flags |= key_symbol;
	}

	if (g_menu.is_clicked(BUTTON_TRIANGLE))
	{
		auto file_size = libpsutil::filesystem::get_file_size(HISTORY_PATH + std::string("history.ody"));

		if (m_strBuffer.length() >= 25 || file_size == 0)
			return false;

		m_strBuffer.push_back('\x20');
	}

	if (g_menu.is_clicked(BUTTON_SELECT))
	{
		auto file_size = libpsutil::filesystem::get_file_size(HISTORY_PATH + std::string("history.ody"));

		if (file_size == -1)
			return false;

		g_history.parse_history();
		m_flags |= key_history;
	}

	if (g_menu.is_clicked(BUTTON_LEFT, 110))
	{
		auto back_value = m_cellColumn - 1;
		if (back_value == -1)
			m_cellColumn = 9;
		else
			m_cellColumn--;
	}
	else if (g_menu.is_clicked(BUTTON_RIGHT, 90))
	{
		auto forward_value = m_cellColumn + 1;
		if (forward_value == 10)
			m_cellColumn = 0;
		else
			m_cellColumn++;
	}


	if (g_menu.is_clicked(BUTTON_UP, 90))
	{
		if (m_cellRow == 0)
			m_cellRow = 3;
		else
			m_cellRow--;
	}
	else if (g_menu.is_clicked(BUTTON_DOWN, 90))
	{
		if (m_cellRow == 3)
			m_cellRow = 0;
		else
			m_cellRow++;
	}

	if (g_menu.is_clicked(BUTTON_X))
	{
		if (m_strBuffer.length() >= 25)
			return true;

		char value = get_text_type(m_cellRow, m_cellColumn);

		m_strBuffer.push_back(value);
	}
	else if (g_menu.is_clicked(BUTTON_SQUARE))
	{
		if (m_strBuffer.empty())
			return true;

		m_strBuffer.erase(m_strBuffer.end() - 1, m_strBuffer.end());
	}

	if (g_menu.is_clicked(BUTTON_START))
	{
		execute();
	}

	return true;
}

void keyboard_t::handle_history()
{
	if (g_history.m_historyList.empty())
		return;

	if (g_menu.is_clicked(BUTTON_X) && g_history.m_historyCursor != -1 && g_history.m_historyCursor < g_history.m_historyList.size())
	{
		m_strBuffer.clear();
		m_strBuffer = g_history.m_historyList[g_history.m_historyCursor];

		auto carriage_return = std::remove(m_strBuffer.begin(), m_strBuffer.end(), '\x0D');
		m_strBuffer.erase(carriage_return, m_strBuffer.end());

		m_flags &= ~key_history;
	}
	else if (g_menu.is_clicked(BUTTON_CIRCLE))
	{
		m_flags &= ~key_history;
	}

	if (g_menu.is_clicked(BUTTON_DOWN))
	{
		int m_Scroll = g_history.m_historyCursor + 1;
		if (g_history.m_historyList.size() > 12)
			if (m_Scroll >= 12)
				g_history.m_historyOffset = g_history.m_historyOffset + 1;
		if (m_Scroll > g_history.m_historyList.size() - 1)
		{
			m_Scroll = 0;
			if (g_history.m_historyList.size() > 12)
			{
				g_history.m_historyOffset = 0;
			}
		}

		g_history.m_historyCursor = m_Scroll;
	}
	else if (g_menu.is_clicked(BUTTON_UP))
	{
		int m_Scroll = g_history.m_historyCursor - 1;
		if (m_Scroll < 0)
		{
			m_Scroll = g_history.m_historyList.size() - 1;
			g_history.m_historyOffset = (g_history.m_historyList.size() - 12 + 1);
			if (g_history.m_historyOffset < 0)
				g_history.m_historyOffset = 0;
		}
		if (g_history.m_historyOffset > 0)
		{
			g_history.m_historyOffset = (g_history.m_historyOffset - 1);
		}

		g_history.m_historyCursor = m_Scroll;
	}
}

void keyboard_t::update()
{
	if (!test_flag(key_open))
		return;

	on_scroll();
}

void keyboard_t::render()
{
	if (!test_flag(key_open))
		return;

	const float m_flTitleHeight = 20.f;
	const float m_flTitleSpacing = 34.f;

	GColor accent_color { g_vars->menu.accent_color.set(3,  g_alphaColor * 255.f) };
	GColor secondary_color { 255, 0, 255, g_alphaColor };
	GColor background_color { accent_color.set_vibrance(.1f) };
	GColor header_color { accent_color.set_vibrance(.07f) };
	GColor text_color { g_vars->menu.text_color, g_alphaColor };

	g_graphics.add_filled_rect(m_keyboardRect, background_color);
	g_graphics.add_rect(m_keyboardRect, 2, accent_color);

	rect_t m_titleRect { m_keyboardRect.x + 5 + 2, m_keyboardRect.y, m_keyboardRect.w - 20, m_flTitleSpacing };

	rect_t m_displayTextRect { m_keyboardRect.x + 5, m_keyboardRect.y + m_flTitleSpacing, m_keyboardRect.w - 10, 30 };
	rect_t m_textRect { m_keyboardRect.x + 7, m_keyboardRect.y + m_flTitleSpacing, m_keyboardRect.w - 10, m_displayTextRect.h };

	rect_t m_keyRect { m_displayTextRect.x, m_displayTextRect.y + m_displayTextRect.h + 5, m_keyboardRect.w - 10, m_keyboardRect.h - (m_flTitleHeight + m_flTitleSpacing + 20.f) };

	rect_t m_selectedCell { m_keyRect.x + (m_cellColumn * (m_keyRect.w / 10)), m_keyRect.y + (m_cellRow * (m_keyRect.h / 4)), (m_keyRect.w / 10), (m_keyRect.h / 4) };

	// display title text
	g_graphics.add_text(m_strTitle.length() == 0 ? "Title Here" : m_strTitle.data(), m_titleRect, 1, horz_left | vert_center | underline, .4f, .4f * 1.5f, text_color);

	// draw display rect
	g_graphics.add_filled_rect(m_displayTextRect, background_color.set(3, 100));
	g_graphics.add_rect(m_displayTextRect, 2, g_vars->menu.accent_color.set(3, 100));

	// draw display string
	g_graphics.add_text(m_strBuffer.data(), m_textRect, 1, horz_left | vert_center, .6f, .6f * 1.5f, text_color);

	// draw keyboard rect
	//g_graphics.add_filled_rect(m_keyRect, g_vars->menu.m_backgroundColor);

	g_graphics.add_filled_rect(m_selectedCell, g_vars->menu.accent_color.set(3, (g_menu.buttons.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & CELL_PAD_CTRL_CROSS) ? 255 : 100));

	for (int row = 0; row < 4; row++)
	{
		for (int column = 0; column < 10; column++)
		{
			rect_t m_cKeyRect { m_keyRect.x + (column * (m_keyRect.w / 10)), m_keyRect.y + (row * (m_keyRect.h / 4)), (m_keyRect.w / 10), (m_keyRect.h / 4) };

			g_graphics.add_text(va("%c", get_text_type(row, column)), m_cKeyRect, 1, horz_center | vert_center, .8f, .8f * 1.5f, text_color);
		}
	}

	if (test_flag(key_history))
	{
		rect_t m_historyRect { m_keyboardRect.x + m_keyboardRect.w + 5, m_keyboardRect.y, g_history.m_historyWidth > 100 ? g_history.m_historyWidth + 5 : 100, m_keyboardRect.h };

		g_graphics.add_filled_rect(m_historyRect, background_color);

		for (int i = 0; i < g_history.m_historyList.size(); i++)
		{
			if (i - g_history.m_historyOffset >= 0 && i < g_history.m_historyOffset + 12)
			{
				rect_t m_historyText { m_historyRect.x + 2, (m_historyRect.y + 5) + ((i - g_history.m_historyOffset) * 13), m_historyRect.w - 4, 16 };

				g_graphics.add_text(g_history.m_historyList[i].data(), m_historyText, 1, horz_left | vert_center, .37f, .37f * 1.5f, text_color);

				if (i == g_history.m_historyCursor)
					g_graphics.add_filled_rect(m_historyText, g_vars->menu.accent_color.set(3, 90));
			}
		}
	}
}

bool keyboard_t::test_flag(short flags)
{
	return ((m_flags & flags) == flags);
}

void keyboard_t::keyboard_history::write_history(std::string str)
{
	std::string m_historyFile = HISTORY_PATH + std::string("history.ody");

	auto file_size = libpsutil::filesystem::get_file_size(HISTORY_PATH + std::string("history.ody"));
	if (file_size > 1000)
		return;

	bool is_already_added = false;
	for (int i = 0; i < m_historyList.size(); i++)
	{
		if (!strcmp(m_historyList[i].c_str(), str.data()))
			is_already_added = true;
	}

	if (is_already_added)
		return;

	char buf[30];
	sprintf(buf, "%s\n", str.data());

	int fd;
	if (cellFsOpen(m_historyFile.data(), CELL_FS_O_RDWR | CELL_FS_O_APPEND | CELL_FS_O_CREAT, &fd, 0, 0) == 0)
	{
		uint64_t pos, nwrite;
		cellFsLseek(fd, 0, CELL_FS_SEEK_SET, &pos);
		cellFsWrite(fd, buf, strlen(buf), &nwrite);
		cellFsClose(fd);
	}
}

void keyboard_t::keyboard_history::parse_history()
{
	auto file_size = libpsutil::filesystem::get_file_size(HISTORY_PATH + std::string("history.ody"));
	if (file_size > 1000 || file_size == 0)
		return;

	std::string buffer;
	buffer.resize(1000);
	libpsutil::filesystem::read_file(HISTORY_PATH + std::string("history.ody"), (char*)buffer.data(), 1000);

	buffer.resize(buffer.length());
	m_historyList = libpsutil::string::split(buffer, '\n');

	m_historyList.erase(std::remove_if(m_historyList.begin(), m_historyList.end(), [](const std::string& list1) -> bool {
		return list1[0] == '\0';
	}), m_historyList.end());

	g_menu.read_size(m_historyList, .47f, m_historyWidth);
}

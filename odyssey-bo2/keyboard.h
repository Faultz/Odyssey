#pragma once

struct key_node
{
	char key;
	char alt_key;
};

#define HISTORY_PATH "/dev_hdd0/tmp/"

static key_node g_keyTable[4][10] =
{
	{
		{ '1', '!' },
	{ '2', '@' },
	{ '3', '#' },
	{ '4', '$' },
	{ '5', '%' },
	{ '6', '^' },
	{ '7', '&' },
	{ '8', '*' },
	{ '9', '{' },
	{ '0', '}' },
	},
	{
		{ 'Q' },
	{ 'W' },
	{ 'E' },
	{ 'R' },
	{ 'T' },
	{ 'Y' },
	{ 'U' },
	{ 'I' },
	{ 'O' },
	{ 'P' },
	},
	{
		{ 'A' },
	{ 'S' },
	{ 'D' },
	{ 'F' },
	{ 'G' },
	{ 'H' },
	{ 'J' },
	{ 'K' },
	{ 'L' },
	{ ':', ';' },
	},
	{
		{ 'Z' },
	{ 'X' },
	{ 'C' },
	{ 'V' },
	{ 'B' },
	{ 'N' },
	{ 'M' },
	{ ',', '"' },
	{ '.', '_' },
	{ '/', '?' },
	}
};

enum keyboard_type
{
	key_open = (1 << 1),
	key_symbol = (1 << 2),
	key_history = (1 << 4)
};

typedef void (*key_callback_t)(std::string data);

class keyboard_t
{
public:
	keyboard_t()
	{
	};
	~keyboard_t()
	{
	};
	struct keyboard_history
	{
		void write_history(std::string str);
		void parse_history();

		char fwkak[3];
		float m_historyWidth;
		short m_historyCursor;
		short m_historyOffset;
		std::vector<std::string> m_historyList;
	}g_history;

	void call(std::string title, key_callback_t callback);
	void update();
	void render();
	bool test_flag(short flags);

	rect_t m_keyboardRect;
	short m_flags;
private:
	void add(const char key);

	void on_scroll();

	char get_text_type(int row, int column);

	void execute();

	bool handle_keyboard();
	void handle_history();

	short m_cellRow, m_cellColumn;

	key_callback_t m_callback;

	std::string m_strBuffer;
	std::string m_strTitle;
};

extern keyboard_t g_keyboard;
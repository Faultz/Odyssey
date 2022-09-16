#pragma once

extern bool g_listIsDropdown;
extern float m_hsvH, m_hsvS, m_hsvV;
extern vec2_t m_hsvBarPos, m_pickerPos;
extern color m_clrTemporary;
extern rect_t m_hsvRect, m_pickerRect;
extern std::string g_searchStr;
typedef uint32_t window_id;

static Material* g_curMaterial;
static MaterialConstantDef* g_curConstant;
static MaterialTextureDef* g_curTexture;

struct analog_t
{
	vec2_t m_LeftAnalog;
	vec2_t m_RightAnalog;
};

enum button_t
{
	BUTTON_L2,
	BUTTON_R2,
	BUTTON_L1,
	BUTTON_R1,
	BUTTON_TRIANGLE,
	BUTTON_CIRCLE,
	BUTTON_X,
	BUTTON_SQUARE,
	BUTTON_SELECT,
	BUTTON_L3,
	BUTTON_R3,
	BUTTON_START,
	BUTTON_UP,
	BUTTON_RIGHT,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_MAX
};

enum menu_flags_t
{
	open = (1 << 1),
	panel_interactive = (1 << 2),
	slider_interactive = (1 << 3),
	list_interactive = (1 << 4),
	colorpicker_interactive = (1 << 5),
	skip_user_input = (1 << 6),
	keyboard_interactive = (1 << 7)
};

struct timer
{
public:

	bool ready()
	{
		return ((get_time_now() - ticker) >= waiter);
	};

	void wait(int timeout)
	{
		ticker = get_time_now();
		waiter = timeout;
	};
	int ticker;
	int waiter;
};

struct menu_settings_t
{
	bool watermark;
	bool draw_debug_text;
};

struct menu_vars_t
{
	menu_settings_t settings;
	color accent_color;
	color text_color;
};

struct variables
{
	menu_vars_t menu;
};

class panel_t
{
public:
	panel_t(std::string name) {
		zero_memory(this, sizeof(panel_t));
		this->name = name;
	}
	~panel_t() 
	{ 
		name.clear();
		zero_memory(this, sizeof(panel_t));
	}

	std::string name;
	rect_t screen_rect;
	rect_t rect;
	float scroll_offset;
	short widget_id;
	short widget_count;
};
extern panel_t* g_curPanel;

class window
{
public:
	window(std::string name) {
		zero_memory(this, sizeof(window));
		this->name	= name;
		id			= hash(name.data());
	}
	~window() {
		for (panel_t* cpanel : panel_list)
			delete cpanel;

		current_panel = nullptr;
		name.clear();
		panel_list.clear();
		zero_memory(this, sizeof(window));
	}

	std::string name;
	window_id id;
	std::vector<panel_t*> panel_list;
	std::vector<panel_t*> sub_panel_list;
	panel_t* current_panel;
	short panel_id;
};
extern window* g_curWindow;
class menu
{
public:
	~menu()
	{
		for(window* begin : window_list)
			delete begin;
		
		current_window = nullptr;
		material_list.clear();
		window_list.clear();
	}

	bool is_open()
	{
		return ((state & open) == open);
	};

	void run_material_window();

	// begin/end
	bool begin(std::string name, const std::vector<std::string> tab_names);
	void end();

	void build();

	bool begin_panel(std::string name);
	void end_panel();

	bool begin_subpanel(std::string name);
	void end_subpanel();

	// begin window
	bool begin_window(std::string name);
	void end_window();

	bool is_clicked(int input_key, int delay = 250);

	window* get_current_window();
	window* get_window(std::string name);
	window* get_window(uint32_t id);

	Material* get_material(std::string name);

	panel_t* get_panel(std::string name);
	panel_t* get_subpanel(std::string name);

	void label(std::string name, menu_callback_t callback = 0);
	void divider(std::string name, menu_callback_t callback = 0);
	void submenu(std::string name, std::string submenu_name, menu_callback_t callback = 0);
	void checkbox(std::string name, bool& var, menu_callback_t callback = 0);
	void slider(std::string name, float& var, float increment, float min, float max, int frame_delay = 90, menu_callback_t callback = 0);
	void slider(std::string name, int& var, int increment, int min, int max, int frame_delay = 90, menu_callback_t callback = 0);
	void list(std::string name, int& var, std::vector<std::string> str_list, bool dropdown = true, menu_callback_t callback = 0);
	void colorpicker(std::string name, color& var, menu_callback_t callback = 0);
	
	void read_analog_input(analog_t* analog);

	void set_flags(int flags);
	bool check_state(int flags);
	void remove_flags(int flags);

	void push_call(menu_callback_t callback);

	static void menu_thread(uint64_t data);

	// init
	static void start();
	static void stop();

	static void read_size(std::vector<std::string> array, float scale, float& size);

	static bool region_intersects(vec2_t region, GRect rect)
	{
		return (region.x >= rect.x && region.y >= rect.y && region.x <= rect.x + rect.w && region.y <= rect.y + rect.h);
	}

	std::string name;
	rect_t screen_rect;
	rect_t panel_space_rect;
	rect_t option_rect;
	vec2_t next_item_pos;
	window* current_window;
	uint32_t window_cursor;
	unsigned short state;
	CellPadData buttons;
	analog_t analog_inputs;

	int option_value;
	std::vector<std::string> temp_list {};
	std::vector<Material*> material_list {};
	std::vector<window*> window_list {};
	std::vector<menu_callback_t> thread_callbacks {};

	std::stack<panel_t*> panel_stack {};
};

extern menu g_menu;
extern variables* g_vars;
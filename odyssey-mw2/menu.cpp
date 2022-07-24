#include "stdafx.h"

#define DEBUG 1

bool g_listIsDropdown = true;
float m_hsvH, m_hsvS, m_hsvV;
vec2_t m_hsvBarPos, m_pickerPos;
color m_clrTemporary { 0.f, 0.f, 0.f, 1.0f };
rect_t m_hsvRect, m_pickerRect;
std::string g_searchStr = "mtl_weapon";

window* g_curWindow;
panel_t* g_curPanel;

menu g_menu;
variables* g_vars;

bool menu::begin(std::string name, const std::vector<std::string> tab_names)
{
	for (int i = 0; i < tab_names.size(); i++)
	{
		if (get_window(tab_names[i]) == nullptr)
		{
#ifdef DEBUG
			printf("adding window with name \"%s\"\n", tab_names[i].data());
#endif
			window_list.push_back(new window(tab_names[i]));
		}
	}
	const bool open_key = is_clicked(BUTTON_R1, true) && is_clicked(BUTTON_R3, true);

	if (open_key)
		this->state |= open;

	if (g_vars->menu.settings.draw_debug_text)
	{
		std::string data = va("open state: %s | panel interactive state: %s | slider interactive state: %s\nlist interactive state: %s | %s",
							  (state & open) ? "true" : "false",
							  (state & panel_interactive) ? "true" : "false",
							  (state & slider_interactive) ? "true" : "false",
							  (state & list_interactive) ? "true" : "false",
							  (state & colorpicker_interactive) ? "true" : "false");

		GRect panel_test { 10, 10, g_graphics.get_text_width(data, .4f) + 5.f, g_fontNormal->pixelHeight * .9f + 5.f };

		g_graphics.add_text(data.data(), panel_test, 3, vert_center | horz_center, .4f, .4f * 1.5f, color(255, 255, 255, 255));
	}

	if ((this->state & open) == 0)
		return false;

	g_graphics.begin();

	static bool stack_is_not_empty = false;
	if (!stack_is_not_empty)
	{
		stack_is_not_empty = true;

		this->name		= name;
		current_window	= window_list.front();
		window_cursor	= 0;
	}

	const bool back_key		= is_clicked(BUTTON_CIRCLE);
	const bool left_key		= is_clicked(BUTTON_LEFT);
	const bool right_key	= is_clicked(BUTTON_RIGHT);
	const bool up_key		= is_clicked(BUTTON_UP);
	const bool down_key		= is_clicked(BUTTON_DOWN);


	if (check_state(panel_interactive) == 0)
	{
		const bool pressed_key = is_clicked(BUTTON_X);
		if (pressed_key)
		{
			set_flags(skip_user_input);
			set_flags(panel_interactive);
		}
	}
	
	if (back_key)
	{
		if (panel_stack.size() == 0 && check_state(panel_interactive) != false && check_state(slider_interactive) == false && check_state(list_interactive) == false && check_state(colorpicker_interactive) == false)
		{
			remove_flags(panel_interactive);
		}
		else if (panel_stack.size() != 0 && check_state(slider_interactive) == false && check_state(list_interactive) == false && check_state(colorpicker_interactive) == false)
		{
			panel_stack.pop();
			if (panel_stack.empty())
				current_window->current_panel = current_window->panel_list[current_window->panel_id];
			else
				current_window->current_panel = panel_stack.top();
		}
		else if (check_state(slider_interactive) == false && check_state(list_interactive) == false && check_state(colorpicker_interactive) == false)
			if (check_state(open))
				remove_flags(open);
	}

	if (check_state(panel_interactive) != false)
	{
		if (check_state(slider_interactive) == false && check_state(colorpicker_interactive) == false)
		{
			read_analog_input(&analog_inputs);

			if (analog_inputs.m_RightAnalog.y != 0)
			{
				current_window->current_panel->scroll_offset += analog_inputs.m_RightAnalog.y * 15.5f;

				if (0.f >= current_window->current_panel->scroll_offset)
					current_window->current_panel->scroll_offset = 0.f;
				else if (current_window->current_panel->scroll_offset >= current_window->current_panel->rect.h)
					current_window->current_panel->scroll_offset = current_window->current_panel->rect.h;
			}
		}
		if (check_state(slider_interactive) == false && check_state(list_interactive) == false && check_state(colorpicker_interactive) == false)
		{
			if (current_window->current_panel == nullptr)
				return;

			if (up_key)
			{
				short& widget_cursor = current_window->current_panel->widget_id;
				int widget_count = current_window->current_panel->widget_count - 1;

				if (widget_cursor <= 0)
				{
					widget_cursor = widget_count;
					if(current_window->current_panel->rect.h <= current_window->current_panel->scroll_offset)
						current_window->current_panel->scroll_offset = current_window->current_panel->rect.h;
				}
				else
					widget_cursor--;
			}
			else if (down_key)
			{
				short& widget_cursor = current_window->current_panel->widget_id;
				int widget_count = current_window->current_panel->widget_count - 1;

				if (widget_cursor >= widget_count)
				{
					current_window->current_panel->scroll_offset = 0.f;
					widget_cursor = 0;
				}
				else
					widget_cursor++;
			}
			
			if (panel_stack.size() == 0)
			{
				if (left_key)
				{
					short& panel_cursor = current_window->panel_id;
					int panel_count = current_window->panel_list.size() - 1;

					if (panel_cursor == 0)
						panel_cursor = panel_count;
					else
						panel_cursor--;

					current_window->current_panel = current_window->panel_list[panel_cursor];
				}
				else if (right_key)
				{
					short& panel_cursor = current_window->panel_id;
					int panel_count = current_window->panel_list.size() - 1;

					if (panel_cursor == panel_count)
						panel_cursor = 0;
					else
						panel_cursor++;

					current_window->current_panel = current_window->panel_list[panel_cursor];
				}
			}
		}
	}
	else
	{
		if (up_key)
		{
			if (window_cursor == 0)
				window_cursor = window_list.size() - 1;
			else
				window_cursor--;

			current_window = window_list[window_cursor];

			if (current_window->panel_list.size() == 0)
				return;

			current_window->current_panel = current_window->panel_list[current_window->panel_id];
		}
		else if (down_key)
		{
			if (window_cursor == window_list.size() - 1)
				window_cursor = 0;
			else
				window_cursor++;

			current_window = window_list[window_cursor];

			if (current_window->panel_list.size() == 0)
				return;

			current_window->current_panel = current_window->panel_list[current_window->panel_id];
		}
	}

	return true;
}

void menu::end()
{
	remove_flags(skip_user_input);

	g_graphics.end();
}

bool menu::begin_panel(std::string name)
{
	// return if current window isn't found.
	if (g_curWindow == nullptr)
		return false;	

	static bool stack_is_not_empty = false;
	if (!stack_is_not_empty && !g_curWindow->panel_list.empty() || (!g_curWindow->panel_list.empty() && g_curWindow->current_panel == nullptr))
	{
		stack_is_not_empty = true;

		g_curWindow->current_panel	= g_curWindow->panel_list.front();
		g_curWindow->panel_id		= 0;
	}

	if (get_panel(name) == nullptr)
	{
#ifdef DEBUG
		printf("adding panel with name \"%s\" to window with name \"%s\"\n", name.data(), g_curWindow->name.data());
#endif
		g_curWindow->panel_list.push_back(new panel_t(name));
	}

	panel_t* temp_panel = get_panel(name);

	if (!panel_stack.empty() && g_menu.current_window->panel_list[g_menu.current_window->panel_id] == temp_panel)
		return false;

	g_curPanel = temp_panel;

	if (g_curPanel == nullptr) return;

	g_menu.next_item_pos = { g_curPanel->screen_rect.x + 5.f, g_curPanel->screen_rect.y - g_curPanel->scroll_offset + 5.f };

	bool is_selected = g_menu.current_window == g_curWindow && g_curPanel != nullptr;

	if (is_selected)
	{
		g_graphics.clip_rect(g_curPanel->screen_rect);

		g_curPanel->widget_count = 0;
	}

	return is_selected;
}

void menu::end_panel()
{
	g_curPanel->rect = { g_curPanel->screen_rect.x, g_curPanel->screen_rect.y + g_menu.next_item_pos.y, g_curPanel->screen_rect.w, next_item_pos.y };

	bool is_selected = g_menu.current_window == g_curWindow && g_curPanel != nullptr;

	if (is_selected)
	{
	}

	g_curPanel = nullptr;
}

bool menu::begin_subpanel(std::string name)
{	// return if current window isn't found.
	if (g_curWindow == nullptr || !g_menu.current_window->panel_list.size())
		return false;

	//static bool stack_is_not_empty = false;
	//if (!stack_is_not_empty && !g_curWindow->sub_panel_list.empty())
	//{
	//	stack_is_not_empty = true;

	//	g_curWindow->current_panel = g_curWindow->sub_panel_list.front();
	//	g_curWindow->panel_id = 0;
	//}

	if (get_subpanel(name) == nullptr)
	{
#ifdef DEBUG
		printf("adding sub panel with name \"%s\" to window with name \"%s\"\n", name.data(), g_curWindow->name.data());
#endif
		g_curWindow->sub_panel_list.push_back(new panel_t(name));
	}

	g_curPanel = get_subpanel(name);

	if (g_curPanel == nullptr) return false;
	
	bool is_selected = g_menu.current_window == g_curWindow && g_curPanel != nullptr && !g_menu.panel_stack.empty() && g_menu.panel_stack.top() == g_curPanel;

	g_curPanel->screen_rect = g_menu.current_window->panel_list[g_menu.current_window->panel_id]->screen_rect;

	g_menu.next_item_pos = { g_curPanel->screen_rect.x + 5.f, g_curPanel->screen_rect.y - g_curPanel->scroll_offset + 5.f };

	if (is_selected)
	{
		g_graphics.clip_rect(g_curPanel->screen_rect);

		g_curPanel->widget_count = 0;
	}

	return is_selected;
}

void menu::end_subpanel()
{
	g_curPanel->rect = { g_menu.current_window->current_panel->screen_rect.x,g_menu.current_window->current_panel->screen_rect.y + g_menu.next_item_pos.y, g_menu.current_window->current_panel->screen_rect.w, next_item_pos.y };

	bool is_selected = g_menu.current_window == g_curWindow && g_curPanel != nullptr;

	if (is_selected)
	{
	}

	g_curPanel = nullptr;
}

bool menu::begin_window(std::string name)
{
	// return to front window if end window hasn't been called.
	//if (g_curWindow != nullptr)
	//{
	//	printf("end window has not been called on %s\n", g_curWindow->name.data());
	//	g_curWindow = window_list.front();
	//	current_window = g_curWindow;
	//	window_cursor = 0;
	//	return false;
	//}

	g_curWindow = get_window(name);
	return g_curWindow != nullptr && current_window == g_curWindow;
}

void menu::end_window()
{
	g_curWindow = nullptr;
}

static uint64_t timers[BUTTON_MAX];
bool menu::is_clicked(int input_key, int delay)
{
	uint64_t timeNow = get_time_now();
	if (timeNow - timers[input_key] > delay)
	{
		uint32_t keyData = ((this->buttons.button[CELL_PAD_BTN_OFFSET_DIGITAL1] << 8) | this->buttons.button[CELL_PAD_BTN_OFFSET_DIGITAL2]);
		if (keyData & (1 << (input_key)))
		{
			timers[input_key] = timeNow;
			return true;
		}
	}
	return false;
}

window* menu::get_current_window()
{
	return current_window;
}

window* menu::get_window(std::string name)
{
	window_id name_hashed = hash(name.data());

	for(int i = 0; i < window_list.size(); i++)
	{
		if(window_list[i]->id == name_hashed)
			return window_list[i];
	}

	return nullptr;
}

window* menu::get_window(uint32_t id)
{
	for(int i = 0; i < window_list.size(); i++)
	{
		if(window_list[i]->id == id)
			return window_list[i];
	}

	return nullptr;
}

Material* menu::get_material(std::string name)
{
	auto nfound = std::find_if(material_list.begin(), material_list.end(), [=](Material* material) -> bool { return !strcmp(material->info.name, name.data()); });
	
	if (nfound == material_list.end())
		material_list.push_back(Material_RegisterHandle(name.data(), 7));

	for (int i = 0; i < material_list.size(); i++)
	{
		if (!strcmp(material_list[i]->info.name, name.data()))
			return material_list[i];
	}

	return nullptr;
}

panel_t* menu::get_panel(std::string name)
{
	for (int i = 0; i < g_curWindow->panel_list.size(); i++)
	{
		if (!strcmp(name.data(), g_curWindow->panel_list[i]->name.data()))
			return g_curWindow->panel_list[i];
	}

	return nullptr;
}

panel_t* menu::get_subpanel(std::string name)
{
	for (int i = 0; i < g_curWindow->sub_panel_list.size(); i++)
	{
		if (!strcmp(name.data(), g_curWindow->sub_panel_list[i]->name.data()))
			return g_curWindow->sub_panel_list[i];
	}

	return nullptr;
}

void menu::read_analog_input(analog_t* analog)
{
	static short analog_deadzone = 20;
	short m_RightX = this->buttons.button[4 + 0] - 128;
	short m_RightY = this->buttons.button[4 + 1] - 128;
	short m_LeftX = this->buttons.button[4 + 2] - 128;
	short m_LeftY = this->buttons.button[4 + 3] - 128;
	if ((m_RightX > -analog_deadzone) && (m_RightX < analog_deadzone))
		m_RightX = 0;

	if ((m_RightY > -analog_deadzone) && (m_RightY < analog_deadzone))
		m_RightY = 0;

	if ((m_LeftX > -analog_deadzone) && (m_LeftX < analog_deadzone))
		m_LeftX = 0;

	if ((m_LeftY > -analog_deadzone) && (m_LeftY < analog_deadzone))
		m_LeftY = 0;

	analog->m_RightAnalog.x = static_cast<double>(m_RightX) / 128.0f;
	analog->m_RightAnalog.y = static_cast<double>(m_RightY) / 128.0f;
	analog->m_LeftAnalog.x = static_cast<double>(m_LeftX) / 128.0f;
	analog->m_LeftAnalog.y = static_cast<double>(m_LeftY) / 128.0f;
}

void menu::set_flags(int flags)
{
	state |= (flags);
}

bool menu::check_state(int flags)
{
	return (state & flags) != 0;
}

void menu::remove_flags(int flags)
{
	state &= ~(flags);
}

detour* cellPad_GetData_d = nullptr;
int cellPad_GetData(uint32_t port_no, CellPadData* data)
{
	CellPadData tmpCellPad = { 0 };

	tmpCellPad.len = data->len;
	cellPadGetData(port_no, &tmpCellPad);
	memcpy((void*)data, (void*)&tmpCellPad, sizeof(CellPadData));

	if (data->len > 0 && port_no == 0)
		memcpy(&g_menu.buttons, &tmpCellPad, sizeof(CellPadData));

	if (data->len > 0 && g_menu.is_open() || g_keyboard.test_flag(key_open))
	{
		data->button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_X] = 0x80;
		data->button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_Y] = 0x80;
		data->button[CELL_PAD_BTN_OFFSET_DIGITAL1] &= ~(CELL_PAD_CTRL_UP | CELL_PAD_CTRL_DOWN | CELL_PAD_CTRL_LEFT |
			CELL_PAD_CTRL_RIGHT);
		data->button[CELL_PAD_BTN_OFFSET_DIGITAL2] &= ~(CELL_PAD_CTRL_CROSS | CELL_PAD_CTRL_CIRCLE | CELL_PAD_CTRL_SQUARE);
	}

	return 0;
}

void menu::read_size(std::vector<std::string> array, float scale, float& size)
{
	std::vector<float> sizes;
	for (auto& val : array)
		sizes.push_back(R_TextWidth(val.c_str(), 0x7F, g_fontNormal) * scale);

	std::sort(sizes.begin(), sizes.end(), std::greater<int>());
	size = sizes[0];
}

void menu::start()
{
	zero_memory(&g_menu, sizeof(menu));
	// allocate our variables
	g_vars = new variables();
	if (g_vars == nullptr)
		return;

	// clear memory for vars
	zero_memory(g_vars, sizeof(variables));

	g_materialCommands = reinterpret_cast<materialCommands_t*>(materialCommands_a);
	g_materialCommands->indexCount = g_materialCommands->indexCount;

	g_searchStr = "mtl_weapon";

	// vars
	g_gameWhite = Material_RegisterHandle("white", 7);
	g_fontNormal = R_RegisterFont("fonts/normalFont", 0);

	g_clrBlack = { 28, 28, 28, 255 };
	g_clrWhite = { 255, 255, 255, 255 };

	g_vars->menu.accent_color = { 117, 18, 255, 255 };
	g_vars->menu.text_color = { 221, 201, 255, 255 };

	g_menu.screen_rect = { 100, 200, 550, 615 };

	g_keyboard.m_keyboardRect = { (1280 / 2) - (450 / 2), (1080 / 2) - (270 / 2), 450, 270 };

	// schedule drawing for the menu
	scheduler::schedule(
	[]()
	{
		g_log.run();
		g_menu.build();
		g_keyboard.update();
		g_keyboard.render();
		g_anim.run_frame();
	}, 0u, scheduler::thread::render);

	// create our hooks
	cellPad_GetData_d = new detour(cellPad_GetData_t, cellPad_GetData);
}

void menu::stop()
{
	// remove our hooks
	delete cellPad_GetData_d;

	// clear our structures
	zero_memory(g_vars, sizeof(variables));
	zero_memory(&g_menu, sizeof(menu));

	// deallocate our structures
	delete g_vars;
}

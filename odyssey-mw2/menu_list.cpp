#include "stdafx.h"


void menu::list(std::string name, int& var, std::vector<std::string> str_list, bool dropdown, menu_callback_t callback)
{
	// return if current window wasn't found.
	if (g_curWindow == nullptr) return;

	float height = 50.f;

	GRect option_rect { g_menu.next_item_pos.x, g_menu.next_item_pos.y, g_curPanel->screen_rect.w, 50.f };
	vec2_t region = { g_curPanel->screen_rect.x + (g_curPanel->screen_rect.w / 2.f), g_curPanel->screen_rect.y + (g_curPanel->screen_rect.h / 2.f) };

	if (analog_inputs.m_RightAnalog.y != 0.f && check_state(list_interactive) == false && check_state(slider_interactive) == false)
		if (menu::region_intersects(region, option_rect))
			g_curPanel->widget_id = g_curPanel->widget_count;

	static timer key_timer;
	const bool hovered = g_curPanel->widget_id == g_curPanel->widget_count && g_curWindow->current_panel == g_curPanel;

	if (hovered)
	{
		const bool is_active = hovered && check_state(panel_interactive) && check_state(list_interactive);
		const bool key_pressed = is_clicked(BUTTON_X) && hovered;

		if (key_pressed)
		{
			//if (callback != 0)
			//	callback();
			if (check_state(list_interactive) == false)
			{
				g_menu.option_value = var;
				temp_list = str_list;
				set_flags(list_interactive);
			}
		}


		if (is_active)
		{
			const bool back_pressed = buttons.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & CELL_PAD_CTRL_CIRCLE;
			const bool up_pressed = buttons.button[CELL_PAD_BTN_OFFSET_DIGITAL1] & CELL_PAD_CTRL_UP;
			const bool down_pressed = buttons.button[CELL_PAD_BTN_OFFSET_DIGITAL1] & CELL_PAD_CTRL_DOWN;
			const bool left_pressed = buttons.button[CELL_PAD_BTN_OFFSET_DIGITAL1] & CELL_PAD_CTRL_LEFT;
			const bool right_pressed = buttons.button[CELL_PAD_BTN_OFFSET_DIGITAL1] & CELL_PAD_CTRL_RIGHT;

			if(g_listIsDropdown)
				height += 25.f * (str_list.size() - 1);

			if (key_pressed)
			{
				if (check_state(list_interactive) != false)
					remove_flags(list_interactive);

				temp_list.clear();
				var = g_menu.option_value;

				if (callback != 0)
					callback();

				key_timer.wait(150);
			}
			if ((dropdown ? up_pressed : left_pressed) && key_timer.ready())
			{
				if (g_menu.option_value <= 0)
					g_menu.option_value = str_list.size() - 1;
				else
					g_menu.option_value--;

				if (!g_listIsDropdown)
					var = g_menu.option_value;

				key_timer.wait(150);
			}
			else if ((dropdown ? down_pressed : right_pressed) && key_timer.ready())
			{
				if (g_menu.option_value >= str_list.size() - 1)
					g_menu.option_value = 0;
				else
					g_menu.option_value++;

				if (!g_listIsDropdown)
					var = g_menu.option_value;

				key_timer.wait(150);
			}
			else if (back_pressed && key_timer.ready())
			{
				if (check_state(list_interactive) != false)
					remove_flags(list_interactive);

				temp_list.clear();
				g_menu.option_value = 0;

				key_timer.wait(150);

			}
		}
	}

	if (region_intersects(vec2_t(option_rect.x + 1, option_rect.y + 1), g_curPanel->screen_rect) || region_intersects(vec2_t(option_rect.x + option_rect.x, option_rect.y + option_rect.h), g_curPanel->screen_rect))
		g_graphics.add_list(name, &var, dropdown, str_list);
	g_curPanel->widget_count++;

	g_menu.next_item_pos.y += height;
}

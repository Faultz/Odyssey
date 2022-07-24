#include "stdafx.h"

void menu::checkbox(std::string name, bool& var, menu_callback_t callback)
{
	//// return if current window wasn't found.
	//if (g_curWindow == nullptr) return;

	//GRect option_rect { g_menu.next_item_pos.x, g_menu.next_item_pos.y, g_curPanel->screen_rect.w, 35.f };
	//vec2_t region = { g_curPanel->screen_rect.x + (g_curPanel->screen_rect.w / 2.f), g_curPanel->screen_rect.y + (g_curPanel->screen_rect.h / 2.f) };

	//if (analog_inputs.m_RightAnalog.y != 0 && check_state(list_interactive) == false && check_state(slider_interactive) == false)
	//	if (menu::region_intersects(region, option_rect))
	//		g_curPanel->widget_id = g_curPanel->widget_count;

	//const bool is_hovered = g_curPanel->widget_id == g_curPanel->widget_count && check_state(panel_interactive) && g_curWindow->current_panel == g_curPanel;

	//if (is_hovered)
	//{
	//	const bool key_pressed = is_clicked(BUTTON_X);
	//	if (key_pressed)
	//	{
	//		var ^= true;

	//		if (callback != 0)
	//			callback();
	//	}
	//}

	//if (region_intersects(vec2_t(option_rect.x + 1, option_rect.y + 1), g_curPanel->screen_rect) || region_intersects(vec2_t(option_rect.x + option_rect.x, option_rect.y + option_rect.h), g_curPanel->screen_rect))
	//	g_graphics.add_checkbox(name, var);
	//g_curPanel->widget_count++;

	//g_menu.next_item_pos.y += 35.f;
}
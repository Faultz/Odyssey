#include "stdafx.h"

void menu::label(std::string name, menu_callback_t callback)
{
	// return if current window wasn't found.
	if (g_curWindow == nullptr) return;

	GRect option_rect { g_menu.next_item_pos.x, g_menu.next_item_pos.y, g_curPanel->screen_rect.w, 30.f };
	vec2_t region = { g_curPanel->screen_rect.x + (g_curPanel->screen_rect.w / 2.f), g_curPanel->screen_rect.y + (g_curPanel->screen_rect.h / 2.f) };
	const bool is_hovered = g_curPanel->widget_id == g_curPanel->widget_count && check_state(panel_interactive) && g_curWindow->current_panel == g_curPanel;
	
	if (analog_inputs.m_RightAnalog.y != 0 && check_state(list_interactive) == false && check_state(slider_interactive) == false)
		if (menu::region_intersects(region, option_rect))
			g_curPanel->widget_id = g_curPanel->widget_count;

	if (is_hovered && callback != 0)
	{
		const bool key_pressed = is_clicked(BUTTON_X);
		if (key_pressed)
		{
			callback();
		}
	}

	if (region_intersects(vec2_t(option_rect.x + 1, option_rect.y + 1), g_curPanel->screen_rect) || region_intersects(vec2_t(option_rect.x + option_rect.x, option_rect.y + option_rect.h), g_curPanel->screen_rect))
		g_graphics.add_label(name);
	g_curPanel->widget_count++;

	g_menu.next_item_pos.y += 30.f;
}

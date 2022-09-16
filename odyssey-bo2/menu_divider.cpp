#include "stdafx.h"

void menu::divider(std::string name, menu_callback_t callback)
{
	// return if current window wasn't found.
	if (g_curWindow == nullptr) return;

	GRect option_rect { g_menu.next_item_pos.x, g_menu.next_item_pos.y, g_curPanel->screen_rect.w, 40.f };

	if (region_intersects(vec2_t(option_rect.x + 1, option_rect.y + 1), g_curPanel->screen_rect) || region_intersects(vec2_t(option_rect.x + option_rect.x, option_rect.y + option_rect.h), g_curPanel->screen_rect))
		g_graphics.add_divider(name);

	g_menu.next_item_pos.y += 40.f;
}

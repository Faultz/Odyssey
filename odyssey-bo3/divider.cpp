#include "stdafx.h"

void graphics::add_divider(std::string name)
{
	panel_t& panel = *g_curPanel;
	const float& height = 40.f;

	GRect option_rect { g_menu.next_item_pos.x, g_menu.next_item_pos.y,  panel.screen_rect.w, height };
	GRect text_rect { option_rect.x + 5.f, option_rect.y, option_rect.w - 10.f, height - 5.f };
	GRect highlighted_rect { text_rect.x, text_rect.y + text_rect.h - 3.f, get_text_width(name.data(), .49f), 2.f * 1.5f };
	GColor accent_color { g_vars->menu.accent_color.set(3, g_alphaColor * 255.f) };

	GColor text_color { g_vars->menu.text_color, 0.8f };

	add_text(name.data(), text_rect, 0, vert_center | horz_left, .49f, .49f * 1.5f, text_color);
	add_filled_rect(highlighted_rect, accent_color);
}

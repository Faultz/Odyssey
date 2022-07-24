#include "stdafx.h"

void graphics::add_submenu(std::string name, std::string submenu_name)
{
	panel_t& panel = *g_curPanel;
	const float& height = 25.f;

	GRect option_rect { g_menu.next_item_pos.x, g_menu.next_item_pos.y,  panel.screen_rect.w, height };
	GRect text_rect { option_rect.x + 5.f, option_rect.y, option_rect.w - 10.f, height };
	GRect highlighted_rect { option_rect.x, option_rect.y, option_rect.w - 10.f, height };
	GColor accent_color { g_vars->menu.accent_color.set_vibrance(.07f).set(3, g_alphaColor * 255.f) };

	GColor text_color { g_vars->menu.text_color, 0.4f };

	if (panel.widget_id == panel.widget_count && g_curWindow->current_panel == g_curPanel)
		add_filled_rect_multicolor(highlighted_rect, g_vars->menu.accent_color, accent_color, accent_color, g_vars->menu.accent_color);

	add_text(name.data(), text_rect, 0, vert_center | horz_left, .49f, .49f * 1.5f, text_color);
}
#include "stdafx.h"

void graphics::add_checkbox(std::string name, bool var)
{
	panel_t& panel = *g_curPanel;
	const float& height = 35.f;

	auto name_width = get_text_width(name, .29f);

	GRect option_rect { g_menu.next_item_pos.x, g_menu.next_item_pos.y, panel.screen_rect.w, height };
	GRect checkbox_rect { g_menu.next_item_pos.x + 5.f, g_menu.next_item_pos.y + (height / 2.f) - ((15.f / 2.f) * 1.5f), 15.f, 15.f * 1.5f };
	GRect checkbox_highlight_rect { checkbox_rect.x + 2.f, checkbox_rect.y + 2.f, checkbox_rect.w - 4.f, checkbox_rect.h - 4.f };
	GRect name_rect { checkbox_rect.x + 18.f, checkbox_rect.y, name_width, checkbox_rect.h };
	GRect highlighted_rect { option_rect.x, option_rect.y, option_rect.w - 10.f, height };

	GColor accent_color { g_vars->menu.accent_color.set_vibrance(.27f).set(3, g_alphaColor * 255.f) };
	GColor text_color { g_vars->menu.text_color, g_alphaColor };
	GColor secondary_color { 255, 0, 255, g_alphaColor };

	if (panel.widget_id == panel.widget_count && g_curWindow->current_panel == g_curPanel)
		add_filled_rect_multicolor(highlighted_rect, g_vars->menu.accent_color, accent_color, accent_color, g_vars->menu.accent_color);

	add_text(name.data(), name_rect, 0, vert_center | horz_left, .48f, .48f * 1.5f, text_color);
	add_filled_rect(checkbox_rect, accent_color);
	if (var)
	{
		add_filled_rect(checkbox_highlight_rect, g_vars->menu.accent_color);
	}
}
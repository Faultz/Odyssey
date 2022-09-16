#include "stdafx.h"

void graphics::add_slider(std::string name, float var, float increment, float min, float max)
{
	panel_t& panel = *g_curPanel;
	const float& height = 35.f;

	GRect option_rect { g_menu.next_item_pos.x, g_menu.next_item_pos.y,  panel.screen_rect.w, height };
	GRect text_rect { option_rect.x + 5.f, option_rect.y + 4.f, (option_rect.w / 2) + 30.f, 15.f };
	GRect slider_bar_rect { text_rect.x, text_rect.y + text_rect.h + 3.f, text_rect.w, 10.f };
	GRect value_bar_rect { slider_bar_rect.x + 1.f, slider_bar_rect.y + 1.f, (((var - min) / (max - min)) * slider_bar_rect.w), slider_bar_rect.h - 2.f };
	GRect highlighted_rect { option_rect.x, option_rect.y, option_rect.w - 10.f, height };

	GColor text_color { g_vars->menu.text_color, g_alphaColor };
	GColor accent_color { g_vars->menu.accent_color.set_vibrance(.27f).set(3, g_alphaColor * 255.f) };

	if (panel.widget_id == panel.widget_count && g_curWindow->current_panel == g_curPanel)
		add_filled_rect(highlighted_rect, g_vars->menu.accent_color);

	add_text(name.data(), text_rect, 0, vert_center | horz_left, .48f, .48f * 1.5f, text_color);
	add_text(va("%.2f/%.2f", var, max), text_rect, 0, vert_center | horz_right, .44f, .44f * 1.5f, text_color);

	add_filled_rect(slider_bar_rect, accent_color);
	if (var != min)
		add_filled_rect(value_bar_rect, g_vars->menu.accent_color);
}

void graphics::add_slider(std::string name, int var, int increment, int min, int max)
{
	panel_t& panel = *g_curPanel;
	const float& height = 35.f;

	GRect option_rect { g_menu.next_item_pos.x, g_menu.next_item_pos.y,  panel.screen_rect.w, height };
	GRect text_rect { option_rect.x + 5.f, option_rect.y + 4.f, (option_rect.w / 2) + 30.f, 15.f };
	GRect slider_bar_rect { text_rect.x, text_rect.y + text_rect.h + 3.f, text_rect.w, 10.f };
	GRect value_bar_rect { slider_bar_rect.x + 1.f, slider_bar_rect.y + 1.f, ((((float)var - (float)min) / ((float)max - (float)min)) * slider_bar_rect.w), slider_bar_rect.h - 2.f };
	GRect highlighted_rect { option_rect.x, option_rect.y, option_rect.w - 10.f, height };

	GColor text_color { g_vars->menu.text_color, g_alphaColor };
	GColor accent_color { g_vars->menu.accent_color.set_vibrance(.27f).set(3, g_alphaColor * 255.f) };

	if (panel.widget_id == panel.widget_count && g_curWindow->current_panel == g_curPanel)
		add_filled_rect(highlighted_rect, g_vars->menu.accent_color);

	add_text(name.data(), text_rect, 0, vert_center | horz_left, .48f, .48f * 1.5f, text_color);
	add_text(va("%i/%i", var, max), text_rect, 0, vert_center | horz_right, .44f, .44f * 1.5f, text_color);

	add_filled_rect(slider_bar_rect, accent_color);
	if (var != 0)
		add_filled_rect(value_bar_rect, g_vars->menu.accent_color);
}

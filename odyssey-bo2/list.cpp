#include "stdafx.h"

void graphics::add_list(std::string name, int* var, bool dropdown, std::vector<std::string> str_list)
{
	panel_t& panel = *g_curPanel;
	float height = 50.f;

	if (dropdown)
	{
		GRect option_rect { g_menu.next_item_pos.x, g_menu.next_item_pos.y,  panel.screen_rect.w, height };
		GRect text_rect { option_rect.x + 5.f, option_rect.y + 2.f, (option_rect.w / 2) + 30.f, 15.f };
		GRect slider_bar_rect { text_rect.x, text_rect.y + text_rect.h + 5.f, text_rect.w, 25.f };
		GRect highlighted_rect { option_rect.x, option_rect.y, option_rect.w - 10.f, height };

		if (panel.widget_id == panel.widget_count && (g_menu.state & list_interactive) && g_curWindow->current_panel == g_curPanel)
		{
			g_menu.option_rect = slider_bar_rect;
		}

		GColor text_color { g_vars->menu.text_color, g_alphaColor };
		GColor accent_color { g_vars->menu.accent_color.set_vibrance(.27f).set(3, g_alphaColor * 255.f) };

		if (panel.widget_id == panel.widget_count && g_curWindow->current_panel == g_curPanel)
		{
			add_filled_rect(highlighted_rect, g_vars->menu.accent_color);
		}
		add_text(name.data(), text_rect, 0, vert_center | horz_left, .45f, .45f * 1.5f, text_color);

		add_filled_rect(slider_bar_rect, accent_color);
		add_text(str_list[*var].data(), slider_bar_rect, 0, vert_center | horz_center, .45f, .45f * 1.5f, text_color);
	}
	else
	{
		GRect option_rect { g_menu.next_item_pos.x, g_menu.next_item_pos.y,  panel.screen_rect.w, height };
		GRect text_rect { option_rect.x + 10.f, option_rect.y + 2.f, (option_rect.w / 2) + 30.f, 15.f };
		GRect bar_rect { text_rect.x, text_rect.y + text_rect.h + 5.f, text_rect.w, 25.f };
		GRect text_bar_rect { text_rect.x, text_rect.y + text_rect.h + 5.f, text_rect.w, 25.f };
		GRect highlighted_rect { option_rect.x, option_rect.y, option_rect.w - 10.f, height };

		if (panel.widget_id == panel.widget_count && (g_menu.state & list_interactive) && g_curWindow->current_panel == g_curPanel)
		{
			g_menu.option_rect = bar_rect;
			g_listIsDropdown = dropdown;
		}

		GColor text_color { g_vars->menu.text_color, g_alphaColor };
		GColor accent_color { g_vars->menu.accent_color.set_vibrance(.27f).set(3, g_alphaColor * 255.f) };

		if (panel.widget_id == panel.widget_count && g_curWindow->current_panel == g_curPanel)
		{
			add_filled_rect(highlighted_rect, g_vars->menu.accent_color);
		}

		add_text(name.data(), text_rect, 0, vert_center | horz_left, .45f, .45f * 1.5f, text_color);
		add_text(va("< %s >", str_list[*var].data()), text_bar_rect, 0, vert_center | horz_left, .45f, .45f * 1.5f, text_color);
		add_text(va("%i/%i", *var, str_list.size()), text_bar_rect, 0, vert_center | horz_right, .45f, .45f * 1.5f, text_color);
	}
}

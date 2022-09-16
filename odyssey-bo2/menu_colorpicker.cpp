#include "stdafx.h"

void menu::colorpicker(std::string name, color& var, menu_callback_t callback)
{	
	// return if current window wasn't found.
	if (g_curWindow == nullptr) return;

	float height = 45.f;

	GRect option_rect { g_menu.next_item_pos.x, g_menu.next_item_pos.y, g_curPanel->screen_rect.w, 45.f };
	vec2_t region = { g_curPanel->screen_rect.x + (g_curPanel->screen_rect.w / 2.f), g_curPanel->screen_rect.y + (g_curPanel->screen_rect.h / 2.f) };

	const bool hovered = g_curPanel->widget_id == g_curPanel->widget_count && g_curWindow->current_panel == g_curPanel;
	const bool is_active = hovered && check_state(panel_interactive) && check_state(colorpicker_interactive);
	
	if (analog_inputs.m_RightAnalog.y != 0 && check_state(list_interactive) == false && check_state(slider_interactive) == false)
		if (menu::region_intersects(region, option_rect))
			g_curPanel->widget_id = g_curPanel->widget_count;

	static timer key_timer;

	if (hovered)
	{
		const bool key_pressed = is_clicked(BUTTON_X);
		if (key_pressed)
		{
			if (check_state(colorpicker_interactive) == false)
			{
				set_flags(colorpicker_interactive);
				g_clrTemporary = &var;
			}
		}		
		
		if (is_active)
		{
			height += 280.f;

			const bool back_pressed = buttons.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & CELL_PAD_CTRL_CIRCLE;

			if (back_pressed)
			{
				if (check_state(colorpicker_interactive) != false)
					remove_flags(colorpicker_interactive);
			}
		}
	}

	if (region_intersects(vec2_t(option_rect.x + 1, option_rect.y + 1), g_curPanel->screen_rect) || region_intersects(vec2_t(option_rect.x + option_rect.w, option_rect.y + option_rect.h), g_curPanel->screen_rect))
		g_graphics.add_colorpicker(name, var);
	g_curPanel->widget_count++;

	g_menu.next_item_pos.y += height;
}
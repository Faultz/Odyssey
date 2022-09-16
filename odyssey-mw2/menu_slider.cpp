#include "stdafx.h"

void menu::slider(std::string name, float& var, float increment, float min, float max, int frame_delay, menu_callback_t callback)
{
	GRect option_rect { g_menu.next_item_pos.x, g_menu.next_item_pos.y, g_curPanel->screen_rect.w, 35.f };
	vec2_t region = { g_curPanel->screen_rect.x + (g_curPanel->screen_rect.w / 2.f), g_curPanel->screen_rect.y + (g_curPanel->screen_rect.h / 2.f) };

	if (analog_inputs.m_RightAnalog.y != 0 && check_state(list_interactive) == false && check_state(slider_interactive) == false)
		if (menu::region_intersects(region, option_rect))
			g_curPanel->widget_id = g_curPanel->widget_count;

	static timer key_timer;
	const bool hovered = g_curPanel->widget_id == g_curPanel->widget_count && g_curWindow->current_panel == g_curPanel;

	if (hovered)
	{
		const bool is_active = hovered && check_state(panel_interactive) && check_state(slider_interactive);
		const bool key_pressed = is_clicked(BUTTON_X) && hovered;

		if (key_pressed)
		{
			//if (callback != 0)
			//	callback();

			if (check_state(slider_interactive) == false)
				set_flags(slider_interactive);
		}

		if (is_active)
		{
			const bool back_pressed = buttons.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & CELL_PAD_CTRL_CIRCLE;
			const bool left_pressed = buttons.button[CELL_PAD_BTN_OFFSET_DIGITAL1] & CELL_PAD_CTRL_LEFT;
			const bool right_pressed = buttons.button[CELL_PAD_BTN_OFFSET_DIGITAL1] & CELL_PAD_CTRL_RIGHT;

			analog_t analog; read_analog_input(&analog);

			if (analog.m_RightAnalog.x != 0)
			{
				var += analog.m_RightAnalog.x * 2.9f;

				if (var <= min)
					var = max;

				if (var >= max)
					var = min;
			}

			if (left_pressed && key_timer.ready())
			{
				if (var <= min)
					var = max;
				else
					var -= increment;

				if (callback != 0)
					callback();

				key_timer.wait(frame_delay);
			}
			else if (right_pressed && key_timer.ready())
			{
				if (var >= max)
					var = min;
				else
					var += increment;

				if (callback != 0)
					callback();

				key_timer.wait(frame_delay);
			}

			if (check_state(slider_interactive) != false)
			{
				if (back_pressed && key_timer.ready())
				{
					remove_flags(slider_interactive);

					key_timer.wait(frame_delay);
				}
			}
		}
	}

	if (region_intersects(vec2_t(option_rect.x + 1, option_rect.y + 1), g_curPanel->screen_rect) || region_intersects(vec2_t(option_rect.x + option_rect.x, option_rect.y + option_rect.h), g_curPanel->screen_rect))
		g_graphics.add_slider(name, var, increment, min, max);

	g_curPanel->widget_count++;

	g_menu.next_item_pos.y += 35.f;
}

void menu::slider(std::string name, int& var, int increment, int min, int max, int frame_delay, menu_callback_t callback)
{
	// return if current window wasn't found.
	if (g_curWindow == nullptr) return;

	GRect option_rect { g_menu.next_item_pos.x, g_menu.next_item_pos.y, g_curPanel->screen_rect.w, 35.f };
	vec2_t region = { g_curPanel->screen_rect.x + (g_curPanel->screen_rect.w / 2.f), g_curPanel->screen_rect.y + (g_curPanel->screen_rect.h / 2.f) };

	if (analog_inputs.m_RightAnalog.y != 0 && check_state(list_interactive) == false && check_state(slider_interactive) == false)
		if (menu::region_intersects(region, option_rect))
			g_curPanel->widget_id = g_curPanel->widget_count;

	static timer key_timer;
	const bool hovered = g_curPanel->widget_id == g_curPanel->widget_count && g_curWindow->current_panel == g_curPanel;

	if (hovered)
	{
		const bool is_active = hovered && check_state(panel_interactive) && check_state(slider_interactive);
		const bool key_pressed = is_clicked(BUTTON_X, false) && hovered;

		if (key_pressed)
		{
			//if (callback != 0)
			//	callback();

			if (check_state(slider_interactive) == false)
				set_flags(slider_interactive);
		}

		if (is_active)
		{
			const bool back_pressed = buttons.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & CELL_PAD_CTRL_CIRCLE;
			const bool left_pressed = buttons.button[CELL_PAD_BTN_OFFSET_DIGITAL1] & CELL_PAD_CTRL_LEFT;
			const bool right_pressed = buttons.button[CELL_PAD_BTN_OFFSET_DIGITAL1] & CELL_PAD_CTRL_RIGHT;

			analog_t analog; read_analog_input(&analog);
			if (analog.m_RightAnalog.x != 0)
			{
				var += analog.m_RightAnalog.x * 2.9f;
				if (var <= min)
					var = max;

				if (var >= max)
					var = min;
			}

			if (left_pressed && key_timer.ready())
			{
				if (var <= min)
					var = max;
				else
					var -= increment;

				if (callback != 0)
					callback();

				key_timer.wait(frame_delay);
			}
			else if (right_pressed && key_timer.ready())
			{
				if (var >= max)
					var = min;
				else
					var += increment;

				if (callback != 0)
					callback();

				key_timer.wait(frame_delay);
			}

			if (check_state(slider_interactive) != false)
			{
				if (back_pressed && key_timer.ready())
				{
					remove_flags(slider_interactive);

					key_timer.wait(frame_delay);
				}
			}
		}
	}

	if (region_intersects(vec2_t(option_rect.x + 1, option_rect.y + 1), g_curPanel->screen_rect) || region_intersects(vec2_t(option_rect.x + option_rect.x, option_rect.y + option_rect.h), g_curPanel->screen_rect))
		g_graphics.add_slider(name, var, increment, min, max);

	g_curPanel->widget_count++;

	g_menu.next_item_pos.y += 35.f;
}
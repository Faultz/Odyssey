#include "stdafx.h"

void graphics::add_colorpicker(std::string name, color& option)
{
	panel_t& panel = *g_curPanel;
	float height = 45.f;

	auto name_width = get_text_width(name, .29f);

	GRect option_rect { g_menu.next_item_pos.x, g_menu.next_item_pos.y, panel.screen_rect.w, height };
	GRect checkbox_rect { g_menu.next_item_pos.x + 5.f, g_menu.next_item_pos.y + (height / 2.f) - ((15.f / 2.f) * 1.5f), 15.f, 15.f * 1.5f };
	GRect checkbox_highlight_rect { checkbox_rect.x + 2.f, checkbox_rect.y + 2.f, checkbox_rect.w - 4.f, checkbox_rect.h - 4.f };
	GRect name_rect { checkbox_rect.x + 18.f, checkbox_rect.y, name_width, checkbox_rect.h };
	GRect highlighted_rect { option_rect.x, option_rect.y, option_rect.w - 10.f, height };

	GColor accent_color { g_vars->menu.accent_color.set_vibrance(.27f).set(3, g_alphaColor * 255.f) };
	GColor text_color { g_vars->menu.text_color, g_alphaColor };
	GColor secondary_color { 255, 0, 255, g_alphaColor };
	
	if (panel.widget_id == panel.widget_count && (g_menu.state & colorpicker_interactive) && g_curWindow->current_panel == g_curPanel)
	{
		g_menu.option_rect = option_rect;
		control_colorpicker(name, option);
	}

	if (panel.widget_id == panel.widget_count && g_curWindow->current_panel == g_curPanel)
		add_filled_rect(highlighted_rect, g_vars->menu.accent_color);

	add_text(name.data(), name_rect, 0, vert_center | horz_left, .48f, .48f * 1.5f, text_color);
	add_filled_rect(checkbox_rect, option);
}

void graphics::control_colorpicker(std::string name, color& col)
{
	color& m_clr = col;
	color& m_clrTemp = m_clrTemporary;

	static bool set_alpha = false;
	if (!set_alpha)
	{
		set_alpha = true;
		m_clrTemporary.a = 1.f;
	}

	float h, s, v;
	rgb_to_hsv(m_clr.r, m_clr.g, m_clr.b, h, s, v);
	m_hsvBarPos.x = m_hsvRect.x;
	if (g_menu.region_intersects(vec2_t(m_hsvBarPos.x + 1, m_hsvBarPos.y + 1), m_hsvRect))
	{
		analog_t m_analogInput;
		zero_memory(&m_analogInput, sizeof(analog_t));

		g_menu.read_analog_input(&m_analogInput);

		if (m_analogInput.m_RightAnalog.y != 0.f && m_hsvBarPos.y < (m_hsvRect.y + m_hsvRect.h))
		{
			m_hsvBarPos.y += m_analogInput.m_RightAnalog.y * 4.5f;
		}

		h = static_cast<float>(1.f - (((m_hsvBarPos.y - m_hsvRect.y) / m_hsvRect.h)));
		h = fmodf(h, 360.f);

		m_hsvH = h;
	}
	else
	{
		if (m_hsvBarPos.y > (m_hsvRect.y + m_hsvRect.h) - 2)
			m_hsvBarPos.y = m_hsvRect.y;

		if (m_hsvBarPos.y < m_hsvRect.y)
			m_hsvBarPos.y = (m_hsvRect.y + m_hsvRect.h) - 3;
	}

	if (g_menu.region_intersects(vec2_t(m_pickerPos.x + 1, m_pickerPos.y + 1), m_pickerRect))
	{
		analog_t m_analogInput;
		zero_memory(&m_analogInput, sizeof(analog_t));

		g_menu.read_analog_input(&m_analogInput);

		if (m_analogInput.m_LeftAnalog.x != 0.f || m_analogInput.m_LeftAnalog.y != 0.f)
		{
			m_pickerPos.x += m_analogInput.m_LeftAnalog.x * 2.5f;
			m_pickerPos.y += m_analogInput.m_LeftAnalog.y * 2.5f;
		}

		s = static_cast<float>((((m_pickerPos.x - m_pickerRect.x) / (m_pickerRect.w - 5))));
		v = static_cast<float>(1.f - (((m_pickerPos.y - m_pickerRect.y) / (m_pickerRect.h - 5))));

		s = clamp(s, 0.f, 1.0f);
		v = clamp(v, 0.f, 1.0f);

		m_hsvS = s;
		m_hsvV = v;
	}
	else
	{
		if (m_pickerPos.x > (m_pickerRect.x + m_pickerRect.w) - 1)
		{
			m_pickerPos.x = (m_pickerRect.x + m_pickerRect.w) - 1;
		}

		if (m_pickerPos.y > (m_pickerRect.y + m_pickerRect.h) - 1)
		{
			m_pickerPos.y = (m_pickerRect.y + m_pickerRect.h) - 1;
		}

		if (m_pickerPos.x < m_pickerRect.x)
		{
			m_pickerPos.x = m_pickerRect.x + 1;
		}

		if (m_pickerPos.y < m_pickerRect.y)
		{
			m_pickerPos.y = m_pickerRect.y + 1;
		}
	}

	if (g_menu.is_clicked(BUTTON_RIGHT, 40))
	{
		float m_flIncrement = m_clr.a + 0.03f;
		if (m_flIncrement > 1.0f)
			m_flIncrement = 1.0f;

		m_clr.a = m_flIncrement;
		m_clrTemp.a = m_flIncrement;
	}
	if (g_menu.is_clicked(BUTTON_LEFT, 40))
	{
		float m_flIncrement = m_clr.a - 0.03f;
		if (m_flIncrement < 0.0f)
			m_flIncrement = 0.0f;

		m_clr.a = m_flIncrement;
		m_clrTemp.a = m_flIncrement;
	}

	m_clr.set_hsv(h, s, v, m_clr.a);
	m_clrTemp.set_hsv(h, 1.f, 1.f, 1.f);
}

void graphics::draw_colorpicker(std::string name, color& option, rect_t option_rect)
{
	if (g_menu.check_state(colorpicker_interactive) == false)
		return;

	panel_t& panel = *g_curPanel;

	rect_t m_menuRect{ g_menu.screen_rect };

	GColor background_color { g_vars->menu.accent_color.set_vibrance(.1f) };

	const float m_flPickerWidth = 170;
	const float m_flPickerHeight = 260;

	const float m_flPreviewBarSpacing = 20.f;
	const float m_flHSVBarSpacing = 20.f;

	const float m_flAdjustedX = 10.f;

	const float m_flPickerX = static_cast<float>((option_rect.x + m_flAdjustedX) + 5);
	const float m_flPickerY = static_cast<float>(option_rect.y + 7);

	const float m_flGradientWidth = static_cast<float>((m_flPickerWidth - m_flPreviewBarSpacing) - m_flHSVBarSpacing) + 8;
	const float m_flGradientHeight = static_cast<float>((m_flPickerHeight - m_flPreviewBarSpacing) - (m_flHSVBarSpacing));

	const float m_flHSVX = static_cast<float>(m_flPickerX + m_flGradientWidth) + 5;
	const float m_flHSVY = m_flPickerY;

	m_pickerRect = { m_flPickerX, m_flPickerY, m_flGradientWidth, m_flGradientHeight };
	m_hsvRect = { m_flHSVX, m_flHSVY, 16, m_flGradientHeight };

	if(m_hsvBarPos == vec3_t())
		m_hsvBarPos = { m_flHSVX, m_flHSVY };
	
	if (m_pickerPos == vec3_t())
		m_pickerPos = { m_flPickerX, m_flPickerY };

	// draw full container
	add_filled_rect(option_rect.x + m_flAdjustedX, option_rect.y, m_flPickerWidth - 1, m_flPickerHeight, background_color);
	add_rect(option_rect.x + m_flAdjustedX, option_rect.y, m_flPickerWidth - 1, m_flPickerHeight, 1, g_vars->menu.accent_color);

	// draw picker rect
	add_rect(m_flPickerX, m_flPickerY, m_flGradientWidth, m_flGradientHeight, 1, g_vars->menu.accent_color);
	add_filled_rect_multicolor(m_flPickerX, m_flPickerY, m_flGradientWidth, m_flGradientHeight, g_clrWhite, m_clrTemporary, m_clrTemporary, g_clrWhite);
	add_filled_rect_multicolor(m_flPickerX, m_flPickerY, m_flGradientWidth, m_flGradientHeight, color(0, 0, 0, 0), color(0, 0, 0, 0), g_clrBlack, g_clrBlack);

	// array of color hues
	const color col_hues[6 + 1] = {
		color(255, 0, 0, 255),
		color(255, 0, 255, 255),
		color(0, 0, 255, 255),
		color(0, 255, 255, 255),
		color(0, 255, 0, 255),
		color(255, 255, 0, 255),
		color(255, 0, 0, 255),
	};

	// draw outline of hsv table rect
	add_rect(m_flHSVX, m_flHSVY, 16, m_flGradientHeight, 1, g_vars->menu.accent_color);

	// draw hsv table
	for(int i = 0; i < 6; i++)
		add_filled_rect_multicolor(m_flHSVX, static_cast<float>(m_flHSVY + ((m_flGradientHeight / 6) * i)), 16, static_cast<float>(m_flGradientHeight / 6), col_hues[i], col_hues[i], col_hues[i + 1], col_hues[i + 1]);

	// draw current hsv bar position
	add_rect(m_flHSVX - 1, m_hsvBarPos.y, 18, 3, 1, g_vars->menu.accent_color);
	add_filled_rect(m_flHSVX - 1, m_hsvBarPos.y, 18, 3, color(10, 10, 10, 250));

	// draw alpha channel
	add_rect(m_flPickerX, m_flPickerY + m_flGradientHeight + 5, m_flGradientWidth + 5 + 16, 5 + 16, 1, g_vars->menu.accent_color);
	add_filled_rect_multicolor(m_flPickerX, m_flPickerY + m_flGradientHeight + 5, m_flGradientWidth + 5 + 16, 5 + 16, g_vars->menu.accent_color, color(0, 0, 0, 0), color(0, 0, 0, 0), g_vars->menu.accent_color);

	// draw current picker position
	add_rect(m_pickerPos.x, m_pickerPos.y, 1, 1 * 1.5f, 2, color(0, 0, 0, 255));
	add_filled_rect(m_pickerPos.x, m_pickerPos.y, 1, 1 * 1.5f, color(0, 0, 0, 255));

	const float m_flInfoBoxX = option_rect.x + m_flAdjustedX;
	const float m_flInfoBoxY = option_rect.y + m_flPickerHeight;

	// draw infomation
	add_rect(m_flInfoBoxX, m_flInfoBoxY, m_flPickerWidth - 1, 45.f, 1, g_vars->menu.accent_color);
	add_filled_rect(m_flInfoBoxX, m_flInfoBoxY, m_flPickerWidth - 1, 45.f, background_color);

	rect_t m_InfoHexRect{ m_flInfoBoxX + 5.f, m_flInfoBoxY + 7.f, m_flGradientWidth + 5 + 16, 20 };

	rect_t m_RGBRect{ m_InfoHexRect.x, m_InfoHexRect.y + m_InfoHexRect.h + 2, 100, 15 };
	rect_t m_HSVRect{ m_InfoHexRect.x + m_InfoHexRect.w - 100, m_InfoHexRect.y + m_InfoHexRect.h + 2, 100, 15 };
	rect_t m_ControlsRect{ m_flInfoBoxX, m_flInfoBoxY + 45.f, m_flPickerWidth - 1, 20 };

	color m_clrHex = option;

	unsigned int m_arrHex[4] = { uint32_t(m_clrHex.r * 255.f), uint32_t(m_clrHex.g * 255.f), uint32_t(m_clrHex.b * 255.f), uint32_t(m_clrHex.a * 255.f) };

	add_rect(m_InfoHexRect.x, m_InfoHexRect.y, m_InfoHexRect.w, m_InfoHexRect.h, 1, g_vars->menu.accent_color);

	add_text(va("#%02X%02X%02X%02X", m_arrHex[0], m_arrHex[1], m_arrHex[2], m_arrHex[3]), m_InfoHexRect, 1, horz_center | vert_center, .33f, .33f * 1.5f, g_vars->menu.text_color);
	add_text(va("%i, %i, %i, %i", m_arrHex[0], m_arrHex[1], m_arrHex[2], m_arrHex[3]), m_RGBRect, 1, horz_left | vert_center, .33f, .33f * 1.5f, g_vars->menu.text_color);
	add_text(va("%i°, %i, %i", int(m_hsvH * 360.f), int(m_hsvS * 100.f), int(m_hsvV * 100.f)), m_HSVRect, 1, horz_right | vert_center, .33f, .33f * 1.5f, g_vars->menu.text_color);

 	// draw controls
	add_text("\x10 = Picker | \x11 = HSV | \x05 & \x06 = Alpha", m_ControlsRect, 1, horz_center | vert_center, .39f, .39f, g_vars->menu.text_color);
}
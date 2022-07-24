#include "stdafx.h"

void graphics::add_text(const char* text, rect_t screen_rect, int style, int align, float xScale, float yScale, float* clr, float* underline_clr)
{
	float m_iAdjustedX = 0.f;
	float m_iAdjustedY = 0.f;

	float textWidth, textHeight;
	read_text_size(text, textWidth, textHeight, xScale, yScale);

	if (align & horz_left)
		m_iAdjustedX = screen_rect.x;
	else if (align & horz_center)
		m_iAdjustedX = (screen_rect.x + (screen_rect.w / 2) - (textWidth / 2));
	else if(align & horz_right)
		m_iAdjustedX = ((screen_rect.x + screen_rect.w) - textWidth);

	m_iAdjustedY = screen_rect.y;
	if (align & vert_top)
		m_iAdjustedY += screen_rect.h;
	if (align & vert_center)
		m_iAdjustedY += ((screen_rect.h / 2) + (textHeight / 2));
	else if (align & vert_bottom)
		m_iAdjustedY += ((screen_rect.h / 2) + textHeight);

	R_AddCmdDrawText(text, 0x7F, g_fontNormal, m_iAdjustedX, m_iAdjustedY, xScale, yScale, 0.0f, clr, style);

	if (align & underline && underline_clr != color())
		add_filled_rect(m_iAdjustedX, m_iAdjustedY + yScale, textWidth, 2.f, underline_clr);

	if (align & align_debug)
		add_rect(screen_rect.x, screen_rect.y, screen_rect.w, screen_rect.h, 1, color(255, 0, 0, 255));
}
void graphics::add_rect(vec2_t pos, vec2_t size, float thickness, float* clr)
{
	if (thickness == 0.f)
		return;

	draw_outlined_rect(pos.x, pos.y, size.x, size.y, clr, thickness);
}
void graphics::add_rect(float x, float y, float w, float h, float thickness, float* clr)
{
	if(thickness == 0.f)
		return;

	draw_outlined_rect(x, y, w, h, clr, thickness);
}
void graphics::add_rect(rect_t screen_rect, float thickness, float* clr)
{
	if (thickness == 0.f)
		return;

	draw_outlined_rect(screen_rect.x, screen_rect.y, screen_rect.w, screen_rect.h, clr, thickness);
}
void graphics::add_filled_rect(vec2_t pos, vec2_t size, float* clr)
{
	R_AddCmdDrawStretchPic(pos.x, pos.y, size.x, size.y, 0, 0, 1, 1, clr, g_gameWhite);
}
void graphics::add_filled_rect(float x, float y, float w, float h, float* clr)
{
	R_AddCmdDrawStretchPic(x, y, w, h, 0, 0, 1, 1, clr, g_gameWhite);
}
void graphics::add_filled_rect(rect_t screen_rect, float* clr)
{
	R_AddCmdDrawStretchPic(screen_rect.x, screen_rect.y, screen_rect.w, screen_rect.h, 0, 0, 1, 1, clr, g_gameWhite);
}
void graphics::add_filled_rect_multicolor(vec2_t pos, vec2_t size, color clr1, color clr2, color clr3, color clr4)
{
	vec2_t min(pos.x, pos.y);

	vec2_t max(min + vec2_t(pos.x, pos.y));

	vec2_t m_QuadPos[4] =
	{
		min,
		vec2_t(max.x, min.y),
		max,
		vec2_t(min.x, max.y)
	};

	color m_clrTable[4] =
	{
		clr1,
		clr2,
		clr3,
		clr4
	};

	//R_AddCmdDrawStretchPic(pos.x, pos.y, size.x, size.y, 0, 0, 1, 1, clr1, g_gameWhite);

	R_AddCmdDrawQuadMultiColor(m_QuadPos, m_clrTable, g_gameWhite);
}
void graphics::add_filled_rect_multicolor(float x, float y, float w, float h, color clr1, color clr2, color clr3, color clr4)
{
	vec2_t min(x, y);

	vec2_t max(min + vec2_t(w, h));

	vec2_t m_QuadPos[4] =
	{
		min,
		vec2_t(max.x, min.y),
		max,
		vec2_t(min.x, max.y)
	};

	color m_clrTable[4] =
	{
		clr1,
		clr2,
		clr3,
		clr4
	};

	//R_AddCmdDrawStretchPic(x, y, w, h, 0, 0, 1, 1, clr1, g_gameWhite);

	R_AddCmdDrawQuadMultiColor(m_QuadPos, m_clrTable, g_gameWhite);
}
void graphics::add_filled_rect_multicolor(rect_t screen_rect, color clr1, color clr2, color clr3, color clr4)
{
	vec2_t min(screen_rect.x, screen_rect.y);

	vec2_t max(min + vec2_t(screen_rect.w, screen_rect.h));

	vec2_t m_QuadPos[4] =
	{
		min,
		vec2_t(max.x, min.y),
		max,
		vec2_t(min.x, max.y)
	};

	color m_clrTable[4] =
	{
		clr1,
		clr2,
		clr3,
		clr4
	};

	//R_AddCmdDrawStretchPic(screen_rect.x, screen_rect.y, screen_rect.w, screen_rect.h, 0, 0, 1, 1, clr1, g_gameWhite);
	
	R_AddCmdDrawQuadMultiColor(m_QuadPos, m_clrTable, g_gameWhite);
}

void graphics::add_image_rect(rect_t screen_rect, float* clr, std::string name)
{
	Material* material = g_menu.get_material(name.data());
	if (material == nullptr) return;

	R_AddCmdDrawStretchPic(screen_rect.x, screen_rect.y, screen_rect.w, screen_rect.h, 0, 0, 1, 1, clr, material);
}

void graphics::begin()
{
	const float& title_spacing = 35.f;
	const float& header_spacing = 60.f;
	const float& menu_name_width = get_text_width(g_menu.name, .6f);

	GRect m_rect { g_menu.screen_rect };
	GRect user_profile_rect { m_rect.x, m_rect.y, m_rect.w, title_spacing };
	GRect title_top_line_rect { m_rect.x, m_rect.y + title_spacing, m_rect.w, 3.f };
	GRect title_rect { m_rect.x + 5.f, m_rect.y + title_spacing + 3.f, menu_name_width + 10.f, header_spacing };
	GRect window_name_divider { m_rect.x + title_rect.w, title_rect.y + title_top_line_rect.y + 7.f, 3.f, title_rect.h - 15.f };

	g_menu.panel_space_rect = { title_rect.x + title_rect.w,
								title_rect.y + title_rect.h, 
								m_rect.w - 5.f - title_rect.w,
								m_rect.h - title_spacing - header_spacing - 2.f };

	GRect panel_space_rect { g_menu.panel_space_rect };

	GColor accent_color { g_vars->menu.accent_color.set(3,  g_alphaColor * 255.f) };
	GColor secondary_color { 255, 0, 255, g_alphaColor };
	GColor background_color { accent_color.set_vibrance(.1f) };
	GColor header_color { accent_color.set_vibrance(.07f) };
	GColor text_color { g_vars->menu.text_color, g_alphaColor };

	// background
	add_filled_rect(m_rect, background_color);

	// title and user profile divider
	add_filled_rect(title_top_line_rect, accent_color);

	// title
	add_text(g_menu.name.data(), title_rect, 0, vert_center | horz_center, .6f, .6f * 1.5f, text_color);

	// window stuff
	float window_text_width = 0.f;
	const float& window_rect_width = title_rect.w - (title_rect.x - window_name_divider.x) - 2.f;
	GRect window_rect { title_rect.x, title_rect.y + title_rect.h, title_rect.w, m_rect.h - title_spacing - header_spacing - 11.f };
	GRect window_outline { window_rect.x - 1.f, window_rect.y - 1.f, window_rect.w + 2.f, window_rect.h + 2.f };

	//add_filled_rect(window_outline, accent_color);
	add_filled_rect(window_rect, background_color);

	for (int i = 0; i < g_menu.window_list.size(); i++)
	{
		window& window = *g_menu.window_list[i];

		GRect window_text_rect { window_rect.x + 2.f, window_rect.y + 4.f + (i * 40.f), window_rect.w - 4.f, 35.f };

		if (i == g_menu.window_cursor)
		{
			add_filled_rect(window_text_rect, g_vars->menu.accent_color.set_vibrance(0.4f));
			add_text(window.name.data(), window_text_rect, 0, vert_center | horz_center, .48f, .48f * 1.5f, text_color);
		
		}
		else
			add_text(window.name.data(), window_text_rect, 0, vert_center | horz_center, .48f, .48f * 1.5f, text_color);

		window_text_width += get_text_width(window.name, .48f) + 5.f;
	}

	if (g_menu.current_window == nullptr)
		return;

	window& window = *g_menu.current_window;
	float window_size_split = (panel_space_rect.w / window.panel_list.size());

	int panel_even = 0;
	int panel_c = 0;
	for (int ipanel = 0; ipanel < window.panel_list.size(); ipanel++)
	{
		panel_t& panel = *window.panel_list[ipanel];

		GRect panel_rect;

		if ((window.panel_list.size() % 2) == 0)
		{
			if (panel_even == 0 && ipanel > 2 && ipanel == (window.panel_list.size() / 2))
			{
				panel_even = 1;
				panel_c = 0;
				panel_rect.x = panel_space_rect.x;
			}

			float x = panel_space_rect.x + ((panel_space_rect.w / (window.panel_list.size())) * ipanel);
			float w = window_size_split - 10.f;
			float h = panel_space_rect.h - 25.f;
			if (panel_even)
			{
				panel_c++;
				x = panel_space_rect.x + ((panel_space_rect.w / (window.panel_list.size() / 2)) * panel_c);
				w = (panel_space_rect.w / (window.panel_list.size() / 2)) - 10.f;
				h = (panel_space_rect.h / 2) - 25.f;
			}

			panel_rect = { x, panel_space_rect.y + ((panel_space_rect.h / 2) * panel_even), w, h };
		}
		else
		{
			panel_rect = { panel_space_rect.x + ((panel_space_rect.w / window.panel_list.size()) * ipanel), panel_space_rect.y, window_size_split - 10.f, panel_space_rect.h - 25.f };
		}

		panel_rect.x += 5.f;
		panel_rect.y += 15.f;
		GRect panel_text_rect { panel_rect.x, panel_rect.y, panel_rect.w, 30.f };
		GRect panel_text_divider { panel_text_rect.x, panel_text_rect.y + panel_text_rect.h, panel_rect.w, 3.f };
		GRect panel_opt_rect { panel_text_rect.x, panel_text_rect.y + panel_text_rect.h + 3.f, panel_rect.w, panel_rect.h - 30.f - 3.f };
		GRect panel_highlight_rect { panel.screen_rect.x - 1.5f, panel.screen_rect.y - 1.5f, panel.screen_rect.w + 3.f, panel.screen_rect.h + 3.f };

		GRect panel_counter_rect { panel_rect.x, panel_rect.y - 25.f, panel_rect.w, 25.f};

		if (g_menu.panel_stack.empty())
		{
			add_text(va("%i/%i", panel.widget_id, panel.widget_count), panel_counter_rect, 0, horz_right | vert_center, .4f, .4f * 1.5f, text_color);
			add_text(va("%s", panel.name.data()), panel_counter_rect, 0, horz_left | vert_center, .4f, .4f * 1.5f, text_color);
		}
		else if(g_menu.panel_stack.size() != 0 && window.panel_id == ipanel)
		{
			add_text(va("%i/%i", g_menu.panel_stack.top()->widget_id, g_menu.panel_stack.top()->widget_count), panel_counter_rect, 0, horz_right | vert_center, .4f, .4f * 1.5f, text_color);
			add_text(va("%s", g_menu.panel_stack.top()->name.data()), panel_counter_rect, 0, horz_left | vert_center, .4f, .4f * 1.5f, text_color);
		}

		GColor panel_color { accent_color.set_vibrance(.05f) };

		panel.screen_rect = { panel_rect.x, panel_rect.y, panel_rect.w, panel_rect.h };
		
		if (window.panel_id == ipanel)
			add_filled_rect(panel_highlight_rect, accent_color);

		add_filled_rect(panel_rect, header_color);
	}
}

bool graphics::clip_rect(rect_t screen_rect)
{
	auto clip_cmd_ptr = R_AddCmdSetScissorRect(static_cast<int>(screen_rect.x), static_cast<int>(screen_rect.y), static_cast<int>(screen_rect.w), static_cast<int>(screen_rect.h));
	return clip_cmd_ptr != nullptr;
}

void graphics::end_clipping()
{
	R_AddCmdClearScissorRect();
}

void graphics::end()
{
	if (g_menu.check_state(list_interactive) && g_listIsDropdown)
	{
		GRect full_clip_rect { g_menu.option_rect.x, g_menu.option_rect.y, g_menu.option_rect.w, g_menu.temp_list.size() * g_menu.option_rect.h };
		GRect option_rect { g_menu.option_rect.x, g_menu.option_rect.y + (g_menu.option_value * g_menu.option_rect.h), g_menu.option_rect.w, g_menu.option_rect.h };

		if (!g_menu.current_window->current_panel) return;

		//g_graphics.clip_rect(full_clip_rect);

		GColor text_color { g_vars->menu.text_color, g_alphaColor };
		GColor accent_color { g_vars->menu.accent_color.set_vibrance(.27f).set(3, g_alphaColor * 255.f) };

		add_filled_rect(full_clip_rect, accent_color);
		add_filled_rect(option_rect, g_vars->menu.accent_color.set_vibrance(.8f));

		for (int i = 0; i < g_menu.temp_list.size(); i++)
		{
			GRect text_rect { g_menu.option_rect.x, g_menu.option_rect.y + (i * g_menu.option_rect.h), g_menu.option_rect.w, g_menu.option_rect.h };
			
			if (g_menu.region_intersects(vec2_t(text_rect.x + 1, text_rect.y + 1), g_menu.current_window->current_panel->screen_rect) || g_menu.region_intersects(vec2_t(text_rect.x + text_rect.x, text_rect.y + text_rect.h), g_menu.current_window->current_panel->screen_rect))
				add_text(g_menu.temp_list[i].data(), text_rect, 0, vert_center | horz_center, .45f, .45f * 1.5f, text_color);
		}

		g_graphics.end_clipping();
	}

	GRect option_rect { g_menu.option_rect.x, g_menu.option_rect.y + (45.f / 2.f) - ((15.f / 2.f)), g_menu.option_rect.w, 200.f };
	draw_colorpicker("colorpicker", *g_clrTemporary, option_rect);
}

void graphics::read_text_size(std::string text, float& width, float& height, float xScale, float yScale)
{
	width = R_TextWidth(text.data(), text.length(), g_fontNormal) * xScale;
	height = g_fontNormal->pixelHeight * yScale;
}

float graphics::get_text_width(std::string text, float xScale)
{
	return R_TextWidth(text.data(), text.length(), g_fontNormal) * xScale;
}

float graphics::get_text_height(std::string text, float yScale)
{
	return g_fontNormal->pixelHeight * yScale;
}
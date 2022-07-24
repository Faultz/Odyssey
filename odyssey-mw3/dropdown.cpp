#include "stdafx.h"

void graphics::add_array(rect_t text_rect, rect_t box_rect, int index, menu_dropdown* option)
{
	if (option->m_OptionActive)
		return;

	float m_flAdjustedX = (g_vars->menu.m_menuRect.w - 50);

	float m_flScale = (R_TextWidth(option->m_OptionDisplayed.data(), 0x7F, g_fontNormal) * .4f) + 10;

	m_flAdjustedX -= m_flScale;

	rect_t m_textRect{ box_rect.x + ((g_vars->menu.m_menuRect.w - 15) - m_flScale), box_rect.y, m_flScale + 5, box_rect.h };

	add_rect(m_textRect.x, m_textRect.y, m_textRect.w, m_textRect.h, 1, g_clrWhite);
	add_text(option->m_OptionDisplayed.data(), m_textRect, 1, align_center | align_middle, g_vars->m_flTextScale, g_vars->m_flTextScale, g_vars->menu.m_textColor);
}

void graphics::draw_array(rect_t text_rect, rect_t box_rect, int index, menu_dropdown* option)
{
	if(!option->m_OptionActive)
		return;

	float m_flAdjustedX = (g_vars->menu.m_menuRect.w - 50);

	float m_flScale = option->m_OptionSize + 5;

	m_flAdjustedX -= m_flScale;
	rect_t m_boxRect{ box_rect.x + ((g_vars->menu.m_menuRect.w - 15) - m_flScale), box_rect.y, m_flScale + 5, box_rect.h * (option->m_OptionArray.size()) };

	add_filled_rect(m_boxRect.x, m_boxRect.y, m_boxRect.w, m_boxRect.h, g_vars->menu.m_backgroundColor);
	add_filled_rect(m_boxRect.x, box_rect.y + (option->m_OptionPosition * box_rect.h), m_boxRect.w, box_rect.h, g_vars->menu.m_backgroundColor.set(1, 60).set(0, 60).set(2, 60));
	add_rect(m_boxRect.x, m_boxRect.y, m_boxRect.w, m_boxRect.h, 1, g_clrWhite);

	for(int i = 0; i < option->m_OptionArray.size(); i++)
	{
		rect_t m_textRect{ box_rect.x + ((g_vars->menu.m_menuRect.w - 15) - m_flScale), box_rect.y + (i * box_rect.h), m_flScale + 5, box_rect.h };

		add_text(option->m_OptionArray[i].data(), m_textRect, 1, align_center | align_middle, g_vars->m_flTextScale, g_vars->m_flTextScale, g_clrWhite);
	}
}
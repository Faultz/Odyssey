#include "stdafx.h"

bool g_testBool;
int g_testInt;
int g_testList;
float g_testFloat;
color g_testColor;

int material_count;

void menu::build()
{
	auto& var = *g_vars;
	if (begin("odyssey.bonk", { "aimbot", "visual", "misc", "players", "settings" }))
	{
		if (begin_window("aimbot"))
		{
			if (begin_panel("aimbot"))
			{
				/*
					almost all options have an event where it will call the callback, eg:
					list = when you select the option in the list.
					slider = when slider value is updated.
					colorpicker = can be added to where you feel necessary.
					checkbox & label = when the trigger button is called (X)
				*/
				label("keyboard option", []() {
					g_keyboard.call("Title", [](std::string str) {
						printf("This will triggered when an input has been made on the keyboard.");
					});
				});
				checkbox("test checkbox option.", g_testBool);
				slider("test float slider.", g_testFloat, 1.f, 0.f, 90.f, 5);
				slider("test int slider.", g_testInt, 1, 0, 90, 5);
				list("test list box.", g_testList, { "these", "are", "test", "values" }, true);
				colorpicker("test color picker.", g_testColor);
				end_panel();
			}
			end_window();
		}

		if (begin_window("visual"))
		{
			if (begin_panel("visual"))
			{
				end_panel();
			}

			end_window();
		}

		if (begin_window("misc"))
		{
			if (begin_panel("misc"))
			{
				end_panel();
			}

			end_window();
		}

		if (begin_window("settings"))
		{
			if (begin_panel("panel 1"))
			{
				submenu("logger", "logger tweaks");
				checkbox("draw watermark", g_vars->menu.settings.watermark);
				checkbox("draw debug info", g_vars->menu.settings.draw_debug_text);
				colorpicker("accents color", g_vars->menu.accent_color);
				colorpicker("text color", g_vars->menu.text_color);
				end_panel();
			}

			if (begin_panel("panel 2"))
			{
				checkbox("toggle", g_testBool);
				slider("slider", g_testInt, 1, 0, 90, 5);
				end_panel();
			}

			if (begin_subpanel("logger tweaks"))
			{
				checkbox("draw logger", g_log.enabled);
				slider("grace period", g_log.grace_period, 50, 0, 15000, 5);
				end_subpanel();
			}
			end_window();
		}
		end();
	}
}
#include "stdafx.h"

bool g_testBool;
int g_testInt;
int g_testList;
float g_testFloat;
color g_testColor;

bool g_noRecoil;

int material_count;
std::string g_dvarSearch;
dvar_t* g_selectedDvar;


void menu::build()
{
	auto& var = *g_vars;
	if (begin("odyssey.bonk", { "aimbot", "visual", "materials", "dvars", "misc", "music", "players", "settings" }))
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
				divider("general");
				label("keyboard option", []() {
					g_keyboard.call("Title", [](std::string str) {
						symbol<void(int, const char*)>{0x01D9EC0}(0, str.data());
					});
				});
				checkbox("no recoil", g_noRecoil, []() {
					libpsutil::memory::set(0x010219, g_noRecoil ? 1 : 0);
				});

				divider("test");
				checkbox("test checkbox option.", g_testBool);
				slider("test float slider.", g_testFloat, 1.f, 0.f, 90.f, 5);
				slider("test int slider.", g_testInt, 1, 0, 200, 5);
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

		run_material_window();

		if (begin_window("dvars"))
		{
			if (begin_panel("list"))
			{
				label(va("search: %s", g_dvarSearch.data()), []() {
					g_keyboard.call("dvar name", [](std::string name) {
						g_dvarSearch = name;
					});
				});

				for (int i = 0; i < *(int*)0x1D52EAC; i++)
				{
					dvar_t* dvar = (dvar_t*)*(int*)(0x1D53EB0 + (i * 4));

					if (!dvar)
						continue;

					if (strstr(dvar->name, g_dvarSearch.data()))
					{
						if(dvar->current.value == dvar->reset.value)
							submenu(va("%s", dvar->name), "selected dvar", [&]() { g_selectedDvar = dvar; });
						else
							submenu(va("%s - modified", dvar->name), "selected dvar", [&]() { g_selectedDvar = dvar; });
					}
				}
				end_panel();
			}

			if (begin_subpanel("selected dvar"))
			{
				switch (g_selectedDvar->type)
				{
					case dvarType_t::DVAR_TYPE_BOOL:
						checkbox("toggle", g_selectedDvar->current.enabled);
						label("reset", []() {
							g_selectedDvar->current.enabled = g_selectedDvar->reset.enabled;
						});
						break;
					case dvarType_t::DVAR_TYPE_COLOR:
						colorpicker("color", *(color*)&g_selectedDvar->current.vectorv);
						label("reset", []() {
							memcpy(&g_selectedDvar->current.vectorv, &g_selectedDvar->reset.vectorv, 16);
						});
						break;
					case dvarType_t::DVAR_TYPE_FLOAT_3:
						static color dvar_color;

						if (g_menu.check_state(colorpicker_interactive))
						{
							memcpy(&dvar_color, &g_selectedDvar->current.vectorv, sizeof(float) * 3);
						}

						colorpicker("color", *(color*)&dvar_color);
						label("reset", []() {
							memcpy(&g_selectedDvar->current.vectorv, &g_selectedDvar->reset.vectorv, 16);
						});
						break;
					case dvarType_t::DVAR_TYPE_INT:
						slider("int", g_selectedDvar->current.integer, 1, g_selectedDvar->domain.integer.min, g_selectedDvar->domain.integer._max);
						label("reset", []() {
							g_selectedDvar->current.integer = g_selectedDvar->reset.integer;
						});
						break;
					case dvarType_t::DVAR_TYPE_FLOAT:
						slider("float", g_selectedDvar->current.value, 1.f, g_selectedDvar->domain.value.min, g_selectedDvar->domain.value._max);
						label("reset", []() {
							g_selectedDvar->current.value = g_selectedDvar->reset.value;
						});
						break;
					case dvarType_t::DVAR_TYPE_ENUM:
						slider("enum", g_selectedDvar->current.integer, 1.f, 0, g_selectedDvar->domain.enumeration.stringCount);
						label("reset", []() {
							g_selectedDvar->current.integer = g_selectedDvar->reset.integer;
						});
						break;

				}
				end_subpanel();
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

		if (begin_window("music"))
		{
			static std::vector<std::string> song_list;
			static int selected_song;
			if (begin_panel("music"))
			{
				submenu("songs", "song list", []() {
					song_list = libpsutil::filesystem::list_files(AUDIO_PATH, true);
				});
				slider("volume", g_audio.volume, 0.1f, 0.f, 3.f, 90, []() {
					g_audio.set_volume(g_audio.volume);
				});
				end_panel();
			}

			if (begin_subpanel("song list"))
			{
				for (int i = 0; i < song_list.size(); i++)
				{
					submenu(song_list[i].data(), "song", [&i]() {
						selected_song = i;
					});
				}

				end_subpanel();
			}

			if (begin_subpanel("song"))
			{
				label("play", []() {
					g_audio.play(song_list[selected_song]);
				});
				label("stop", []() {
					g_audio.stop_playing();
				});
				end_subpanel();
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
				static symbol<void(int, const char* name)> Cbuf_AddText { 0x1D9EC0 };
			
				checkbox("toggle", g_testBool);
				slider("slider", g_testInt, 1, 0, 90, 5);
				label("send command", []() {
					g_keyboard.call("enter command", [](std::string str) {
						Cbuf_AddText(0, str.data());
					});
				});
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
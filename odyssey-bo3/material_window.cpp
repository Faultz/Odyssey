#include "stdafx.h"

void menu::run_material_window()
{
	if (begin_window("materials"))
	{
		static std::vector<std::string> image_list;
		if (begin_panel("#materials"))
		{
			label(va("Search: %s", g_searchStr.data()), []() {
				g_keyboard.call("Material Name", [](std::string name) {
					g_searchStr = name;
				});
			});

			static int material_count = DB_GetAllXAssetOfType(ASSET_TYPE_MATERIAL, (XAssetHeader*)0x10040000);
			for (int i = 0; i < material_count; i++)
			{
				auto material = (Material*)*(int*)(0x10040000 + (i * 4));

				if (strstr(material->info.name, g_searchStr.data()))
				{
					submenu(va("%s", material->info.name), "selected material", [=]() { g_curMaterial = (Material*)*(int*)(0x10040000 + (i * 4)); });
				}
			}
			end_panel();
		}


		if (begin_subpanel("selected material"))
		{
			label("print address", []() {
				printf("name: %s\naddress: 0x%X\nbitflag: %i", g_curMaterial->info.name, g_curMaterial, (g_curMaterial->info.drawSurf.packed >> 53) & 0x3C);
				g_log.print("name: %s address: 0x%X\n", g_curMaterial->info.name, g_curMaterial);
			});
			submenu("textures", "texture list");
			submenu("constants", "constant list");

			end_subpanel();
		}

		if (begin_subpanel("texture list"))
		{
			for (int i = 0; i < g_curMaterial->textureCount; i++)
			{
				submenu(g_curMaterial->textureTable[i].image->name, "selected texture", [=]() { g_curTexture = &g_curMaterial->textureTable[i]; });
			}
			end_subpanel();
		}

		if (begin_subpanel("constant list"))
		{
			for (int i = 0; i < g_curMaterial->constantCount; i++)
			{
				submenu(g_curMaterial->constantTable[i].name, "selected constant", [=]() { g_curConstant = &g_curMaterial->constantTable[i]; });
			}
			end_subpanel();
		}

		if (begin_subpanel("selected texture"))
		{
			divider(va("dimensions: %ix%i", g_curTexture->image->width, g_curTexture->image->height));

			auto convert_color_to_argb8 = [](float* flColor, uint32_t& color) {
				uint8_t buf[4];

				buf[0] = static_cast<uint8_t>(flColor[3] * 255.f);
				buf[1] = static_cast<uint8_t>(flColor[0] * 255.f);
				buf[2] = static_cast<uint8_t>(flColor[1] * 255.f);
				buf[3] = static_cast<uint8_t>(flColor[2] * 255.f);

				color = ((uint8_t)buf[0] << 24 | (uint8_t)buf[1] << 16 | (uint8_t)buf[2] << 8 | (uint8_t)buf[3]);
			};

			static int format;
			list("Format", format, { "DXT1", "DXT3", "DXT5", "ARGB8" }, true, [=]() {
				const int id[4] =
				{
					0x86,
					0x87,
					0x88,
					0x85
				};

				g_curTexture->image->format = id[format];
			});
			static color clr;
			if (check_state(menu_flags_t::colorpicker_interactive))
			{
				for (int i = 0; i < 4; i++)
				{
					g_curTexture->image->streamData[i].width = 1;
					g_curTexture->image->streamData[i].height = 1;
				}
				g_curTexture->image->width = 1;
				g_curTexture->image->height = 1;
				convert_color_to_argb8(clr, *(uint32_t*)g_curTexture->image->pixels);
			}

			submenu("injector", "image injector", []() {
				image_list = libpsutil::filesystem::list_files(AUDIO_PATH, true);
			});

			colorpicker("color", clr);
			end_subpanel();
		}

		if (begin_subpanel("image injector"))
		{
			for (int i = 0; i < image_list.size(); i++)
			{
				label(image_list[i].data(), [&](){
					auto& image = image_list[i];

					cell_fs file(image.data(), "rb");
					if (file.is_open())
					{
						DDS_HEADER header;

						file.read(&header, sizeof(DDS_HEADER), 1);
						file.seek(0, SEEK_SET);
						int file_size = file.tell();
						printf("%i\n", file_size);
					}
				});
			}
			
			end_subpanel();
		}

		if (begin_subpanel("selected constant"))
		{
			if (strstr(g_curConstant->name, "col"))
			{
				colorpicker("color", *(color*)&g_curConstant->literal);
			}
			else
			{
				divider("divider");
				slider("tweak x", g_curConstant->literal.x, 0.f, 0.f, 1000.f);
				slider("tweak y", g_curConstant->literal.y, 0.f, 0.f, 1000.f);
				slider("tweak z", g_curConstant->literal.w, 0.f, 0.f, 1000.f);
				slider("tweak w", g_curConstant->literal.z, 0.f, 0.f, 1000.f);
			}
			end_subpanel();
		}

		end_window();
	}
}
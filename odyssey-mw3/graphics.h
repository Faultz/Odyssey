#pragma once

enum text_alignment
{
	// x axis
	horz_left = (1 << 1),
	horz_center = (1 << 2),
	horz_right = (1 << 3),

	// y axis
	vert_top = (1 << 4),
	vert_center = (1 << 5),
	vert_bottom = (1 << 6),

	align_debug = (1 << 7),

	// text accounted
	underline = (1 << 8),

	// max
	align_max = 69420
};

class graphics
{
public:
	// text drawing
	void add_text(const char* text, rect_t screen_rect, int style, int align, float xScale, float yScale, float* clr, float* underline_clr = color());
	void add_text(const char* text, float x, float y, int style, float x_margin, float y_margin, float xScale, float yScale, float* clr, float* underline_clr = color());

	// rect drawing
	void add_rect(float x, float y, float w, float h, float thickness = 1.f, float* clr = g_clrWhite);
	void add_filled_rect(float x, float y, float w, float h, float* clr = g_clrWhite);
	void add_filled_rect_multicolor(float x, float y, float w, float h, color clr1 = color(), color clr2 = color(), color clr3 = color(), color clr4 = color());

	void add_rect(vec2_t pos, vec2_t size, float thickness, float* clr = g_clrWhite);
	void add_filled_rect(vec2_t pos, vec2_t size, float* clr = g_clrWhite);
	void add_filled_rect_multicolor(vec2_t pos, vec2_t size, color clr1 = color(), color clr2 = color(), color clr3 = color(), color clr4 = color());

	void add_rect(rect_t screen_rect, float thickness = 1.f, float* clr = g_clrWhite);
	void add_filled_rect(rect_t screen_rect, float* clr);
	void add_filled_rect_multicolor(rect_t screen_rect, color clr1 = color(), color clr2 = color(), color clr3 = color(), color clr4 = color());

	void add_image_rect(rect_t screen_rect, float* clr, std::string name = "white");

	void begin();

	bool clip_rect(rect_t screen_rect);
	void end_clipping();

	void add_label(std::string name);
	void add_checkbox(std::string name, bool var);
	void add_slider(std::string name, float var, float increment, float min, float max);
	void add_slider(std::string name, int var, int increment, int min, int max);
	void add_list(std::string name, int* var, bool dropdown, std::vector<std::string> str_list);
	void add_submenu(std::string name, std::string submenu_name);

	void draw_colorpicker(std::string name, color& col, rect_t option_rect);
	void control_colorpicker(std::string name, color& col);
	void add_colorpicker(std::string name, color& var);

	void read_text_size(std::string text, float& width, float& height, float xScale, float yScale);

	float get_text_width(std::string text, float xScale);
	float get_text_height(std::string text, float yScale);

	void end();
};

static graphics g_graphics;
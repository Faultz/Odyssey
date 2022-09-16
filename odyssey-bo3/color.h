#pragma once

void rgb_to_hsv(float r, float g, float b, float& out_h, float& out_s, float& out_v);
void hsv_to_rgb(float h, float s, float v, float& out_r, float& out_g, float& out_b);

class color
{
public:
	float r, g, b, a;

	color() = default;
	color(const color& clr, float alpha) : r(clr.r), g(clr.g), b(clr.b), a(alpha) {}
	color(const color& clr, int alpha) : r(clr.r), g(clr.g), b(clr.b), a(alpha / 255.f) {}
	color(int r, int g, int b, int a) : r(r / 255.f), g(g / 255.f), b(b / 255.f), a(a / 255.f){}
	color(int r, int g, int b) : r(r / 255.f), g(g / 255.f), b(b / 255.f), a(1.0f){}
	color(int r, int g, int b, float a) : r(r / 255.f), g(g / 255.f), b(b / 255.f), a(a) { }
	color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a){}
	color(int color) : r((color >> 16 & 0xff) / 255.f), g((color >> 8 & 0xff) / 255.f), b((color & 0xff) / 255.f), a((color >> 24 & 0xff) / 255.f){}

	color operator*(const float& val)
	{
		color out{ *this };

		out.r *= val;
		out.g *= val;
		out.b *= val;
		out.a *= val;

		return out;
	}
	void interpolate(float offset)
	{
		static float rainbow_tick = 0.f;
		rainbow_tick += 1.0f;

		// 0.00001f - this is the speed
		this->r = (sinf(0.009f * rainbow_tick + 0.f + offset) * 127.f + 128.f) / 255.f;
		this->g = (sinf(0.009f * rainbow_tick + 2.f + offset) * 127.f + 128.f) / 255.f;
		this->b = (sinf(0.009f * rainbow_tick + 4.f + offset) * 127.f + 128.f) / 255.f;
		this->a = 1.f;
		if(rainbow_tick > 1000000.0f) rainbow_tick = 0.f;
	}
	color set(int index, float col);
	color set(int index, int col);
	operator float* ()
	{
		return reinterpret_cast<float*>(&this[0]);
	}

	inline color& set_hue(float hue, float a = 1.0f)
	{
		static color out_color;

		float o_h, o_s, o_v;
		rgb_to_hsv(r, g, b, o_h, o_s, o_v);

		hsv_to_rgb(hue, o_s, o_v, out_color.r, out_color.g, out_color.b); out_color.a = this->a;

		return out_color;
	}

	inline color& set_saturation(float saturation, float a = 1.0f)
	{
		static color out_color;

		float o_h, o_s, o_v;
		rgb_to_hsv(r, g, b, o_h, o_s, o_v);

		hsv_to_rgb(o_h, saturation, o_v, out_color.r, out_color.g, out_color.b); out_color.a = this->a;

		return out_color;
	}

	inline color& set_vibrance(float vibrance, float a = 1.0f)
	{
		static color out_color;

		float o_h, o_s, o_v;
		rgb_to_hsv(r, g, b, o_h, o_s, o_v);

		hsv_to_rgb(o_h, o_s, vibrance, out_color.r, out_color.g, out_color.b); out_color.a = this->a;

		return out_color;
	}
	inline void set_hsv(float h, float s, float v, float a = 1.0f)
	{
		hsv_to_rgb(h, s, v, this->r, this->g, this->b); this->a = a;
	}

	void operator=(const vec4_t& vec)
	{
		this->r = vec.x;
		this->g = vec.y;
		this->b = vec.z;
		this->a = vec.w;
	}
};
typedef color GColor;
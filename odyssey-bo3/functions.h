#pragma once

extern Material* g_gameDarkMatter;

extern Material* g_gameWhite;
extern Font* g_fontNormal;

extern color g_clrWhite; 
extern color g_clrBlack;
extern color* g_clrTemporary;

extern int g_horzAlign;
extern int g_vertAlign;

extern float g_alphaColor;

inline void draw_outlined_rect(float x, float y, float width, float height, float* color, float thickness)
{
	R_AddCmdDrawStretchPic(x - thickness, y - thickness, width + (thickness * 2), thickness, 0, 0, 1, 1, color, g_gameWhite); // Top>
	R_AddCmdDrawStretchPic(x - thickness, y + height, width + (thickness * 2), thickness, 0, 0, 1, 1, color, g_gameWhite); // Bottom>
	R_AddCmdDrawStretchPic(x - thickness, y - thickness, thickness, height + thickness, 0, 0, 1, 1, color, g_gameWhite); // Left>
	R_AddCmdDrawStretchPic(x + width, y - thickness, thickness, height + (thickness * 2), 0, 0, 1, 1, color, g_gameWhite); // Right>
}

class rect_t
{
public:
	float x, y, w, h;

	rect_t() = default;
	rect_t(float X, float Y, float W, float H) : x(X), y(Y), w(W), h(H)
	{
		//draw_outlined_rect(X, Y, W, H, color(255, 255, 255, 100), 1);
	}
};
typedef rect_t GRect;

template<typename ...params>
const char* va(const char* format, params... param)
{
	static char m_strBuffer[0x1000];
	Com_Sprintf(m_strBuffer, 0x1000, format, param...);

	return m_strBuffer;
}

inline void calculate_interpolation(float formula, int div, float& ptr)
{
	if (ptr == formula)
		return;

	float value_overflow = fabsf(ptr - formula) / 2;
	if (ptr < formula && value_overflow)
		ptr += value_overflow / div;
	else if (ptr > formula && value_overflow != 0)
		ptr -= value_overflow / div;
}
static inline float inverse_length(const vec3_t& lhs, float fail_value) {	float d = lhs.x * lhs.x + lhs.y * lhs.y; if(d > 0.0f) return 1.0f / sqrtf(d); return fail_value; }

bool RB_CheckOverflow(int vertCount, int indexCount);
void R_ConvertColor(float* flColor, uint32_t& clr);

//void* R_GetCommandBuffer(int cmd, int size);

void R_AddCmdDrawQuadMultiColor(vec2_t* vert, color* clr, Material* material);
void R_SetVertex2d(GfxVertex* vert, float x, float y, float s, float t, unsigned int color);
void R_SetVertex4D(GfxVertex* vert, float x, float y, float z, float w, float s, float t, unsigned int color);

void DrawRectList(float x, float y, float w, float h, color m_color1, color m_color2, color m_color3, color m_color4, int horz = g_horzAlign, int vert = g_vertAlign);

bool CInRegion(vec2_t pos, rect_t clip_rect);
#include "stdafx.h"

Material* g_gameDarkMatter;

Material* g_gameWhite;
Font* g_fontNormal;

color g_clrWhite;
color g_clrBlack;
color* g_clrTemporary;

int g_horzAlign;
int g_vertAlign;

float g_alphaColor = 1.f;

bool RB_CheckOverflow(int vertCount, int indexCount)
{
	int vCount = g_materialCommands->vertexCount;
	int iCount = g_materialCommands->indexCount;

    if((vCount + vertCount) > 0x800 || (iCount + indexCount) > 0xC00)
    {
		RB_EndTessSurface();

        return true;
    }

    return false;
}

void R_ConvertColor(float* flColor, uint32_t& clr)
{
	uint8_t buf[4];

	buf[0] = static_cast<uint8_t>(flColor[0] * 255.f);
	buf[1] = static_cast<uint8_t>(flColor[1] * 255.f);
	buf[2] = static_cast<uint8_t>(flColor[2] * 255.f);
	buf[3] = static_cast<uint8_t>(flColor[3] * 255.f);

	clr = ((uint8_t)buf[0] << 24 | (uint8_t)buf[1] << 16 | (uint8_t)buf[2] << 8 | (uint8_t)buf[3]);
}

//void* R_GetCommandBuffer(int cmd, int size)
//{
//	GfxCmdArray* s_cmdList = reinterpret_cast<GfxCmdArray*>(GfxCmdArray_t);
//	auto& renderCommandSize = *(int*)RenderCmdBufSize_t;
//	//
//	if (s_cmdList->usedCritical - s_cmdList->usedTotal + renderCommandSize - 0x2000 >= size)
//	{
//		GfxCmdHeader* cmd_header = (GfxCmdHeader*)&s_cmdList->cmds[s_cmdList->usedTotal];
//		s_cmdList->usedTotal += size;
//		s_cmdList->usedCritical += cmd >= 9 ? 0 : size;
//		s_cmdList->lastCmd = cmd_header;
//
//		cmd_header->byteCount = size;
//		cmd_header->id = cmd;
//		
//		return (void*)cmd_header;
//	}
//	else
//	{
//		s_cmdList->lastCmd = nullptr;
//	}
//
//	return nullptr;
//}

void R_AddCmdDrawQuadMultiColor(vec2_t* vert, color* clr, Material* material)
{
	GfxCmdDrawQuadPic* quad_pic = reinterpret_cast<GfxCmdDrawQuadPic*>(R_GetCommandBuffer(RB_DrawQuadPic_id_t, sizeof(GfxCmdDrawQuadPic)));

	uint32_t col[4];
	R_ConvertColor((float*)&clr[0], col[0]);
	R_ConvertColor((float*)&clr[1], col[1]);
	R_ConvertColor((float*)&clr[2], col[2]);
	R_ConvertColor((float*)&clr[3], col[3]);

	quad_pic->material = material;

	if(!material)
		quad_pic->material = g_gameWhite;

	quad_pic->verts[0] = vert[0];
	quad_pic->color[0].packed = col[0];

	quad_pic->verts[1] = vert[1];
	quad_pic->color[1].packed = col[1];

	quad_pic->verts[2] = vert[2];
	quad_pic->color[2].packed = col[2];

	quad_pic->verts[3] = vert[3];
	quad_pic->color[3].packed = col[3];
	//quad_pic->multicolor = true;
}

void __cdecl R_SetVertex2d(GfxVertex* vert, float x, float y, float s, float t, unsigned int color)
{
	vert->xyzw.x = x;
	vert->xyzw.y = y;
	vert->xyzw.z = 0.0f;
	vert->xyzw.w = 1.0f;
	vert->normal.packed = 2143289344;
	vert->color.packed = color;
	vert->texCoord.x = s;
	vert->texCoord.y = t;
}

void R_SetVertex4D(GfxVertex* vert, float x, float y, float z, float w, float s, float t, unsigned int color)
{
	vert->xyzw.x = x;
	vert->xyzw.y = y;
	vert->xyzw.z = z;
	vert->xyzw.w = w;
	vert->normal.packed = 2143289344;
	vert->color.packed = color;
	vert->texCoord.x = s;
	vert->texCoord.y = t;
}

void DrawRectList(float x, float y, float w, float h, color m_color1, color m_color2, color m_color3, color m_color4, int horz, int vert)
{
    //GfxQuadVertex* m_QuadList = reinterpret_cast<GfxQuadVertex*>(R_AddCmdDrawQuadList2D(g_gameWhite, 1));

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
		m_color1,
		m_color2,
		m_color3,
		m_color4
	};

	R_AddCmdDrawQuadMultiColor(m_QuadPos, m_clrTable, g_gameWhite);
}

bool CInRegion(vec2_t pos, rect_t clip_rect)
{
    return (pos.x > clip_rect.x && pos.y > clip_rect.y && pos.x < clip_rect.x + clip_rect.w && pos.y < clip_rect.y + clip_rect.h);
}
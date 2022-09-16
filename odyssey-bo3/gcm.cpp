
#include "stdafx.h"

gcm_renderer g_renderer;

cell::Gcm::Inline::CellGcmContext& gcm_renderer::g_gcmContext = *reinterpret_cast<cell::Gcm::Inline::CellGcmContext*>(0x02530CCC);

detour* cellGcmSetFlipCommand_d = nullptr;
gcm_renderer::gcm_renderer()
{
}

gcm_renderer::~gcm_renderer()
{
	delete cellGcmSetFlipCommand_d;
}

void gcm_renderer::cellGcmSetFlipCommand_h(CellGcmContextData* gcmThis, uint8_t id)
{
	g_renderer.draw_quad(50, 50, 0.f, 500, 500, color(255, 255, 255, 255));

	cellGcmSetFlipCommand_d->invoke(gcmThis, id);
}

void gcm_renderer::draw_quad(float x, float y, float z, float width, float height, float* color)
{
	unsigned int vertex_size = sizeof(GfxPointerVertex);
	GfxPointerVertex vertex[4] = {
		{ { x + width, y, 0.f, 1.0f }, color, { 0.f, 0.0f }, { 2143289344 } },
		{ { x, y, 0.f, 1.0f, }, color, { 0.f, 1.0f }, { 2143289344 } },
		{ { x, y + height, 0.f, 1.0f }, color, { 1.f, 1.0f }, { 2143289344 } },
		{ { x + width, y + height, 0.f, 1.0f }, color, { 0.0f, 1.0f }, { 2143289344 } }
	};

	g_gcmContext.SetVertexDataArray(ATTR_POSITION, 0, vertex_size, 4, CELL_GCM_VERTEX_F, CELL_GCM_LOCATION_LOCAL, 0);
	g_gcmContext.SetVertexDataArray(ATTR_COLOR, 0, vertex_size, 4, CELL_GCM_VERTEX_F, CELL_GCM_LOCATION_LOCAL, 0);
	g_gcmContext.SetVertexDataArray(ATTR_TEXCOORD0, 0, vertex_size, 2, CELL_GCM_VERTEX_F, CELL_GCM_LOCATION_LOCAL, 0);
	g_gcmContext.SetVertexDataArray(ATTR_NORMAL, 0, vertex_size, 1, CELL_GCM_VERTEX_F, CELL_GCM_LOCATION_LOCAL, 0);

	// draw
	g_gcmContext.SetDrawInlineArray(CELL_GCM_PRIMITIVE_POLYGON, 4 * vertex_size / sizeof(float), vertex);

	// invalidate vertex attribute
	g_gcmContext.SetVertexDataArray(ATTR_POSITION, 0, 0, 0, CELL_GCM_VERTEX_F, CELL_GCM_LOCATION_LOCAL, 0);
	g_gcmContext.SetVertexDataArray(ATTR_COLOR, 0, 0, 0, CELL_GCM_VERTEX_F, CELL_GCM_LOCATION_LOCAL, 0);
	g_gcmContext.SetVertexDataArray(ATTR_TEXCOORD0, 0, 0, 0, CELL_GCM_VERTEX_F, CELL_GCM_LOCATION_LOCAL, 0);
	g_gcmContext.SetVertexDataArray(ATTR_NORMAL, 0, 0, 0, CELL_GCM_VERTEX_F, CELL_GCM_LOCATION_LOCAL, 0);
}

detour* Scr_UpdateFrame_d = nullptr;
void Scr_UpdateFrame()
{
	Scr_UpdateFrame_d->invoke();
}

void gcm_renderer::start()
{
	cellGcmSetFlipCommand_d = new detour(0x045F380, cellGcmSetFlipCommand_h);
	Scr_UpdateFrame_d = new detour(0x00B54B0, Scr_UpdateFrame);
}

void gcm_renderer::stop()
{
	delete cellGcmSetFlipCommand_d;
	delete Scr_UpdateFrame_d;
}
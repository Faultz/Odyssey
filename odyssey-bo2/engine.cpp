#include "stdafx.h"

struct opd_t
{
	unsigned int address;
	unsigned int toc;
};

opd_t** g_renderCommandTable = reinterpret_cast<opd_t**>(RenderCommandTable_t);
materialCommands_t* g_materialCommands;

detour* RB_DrawQuadPic_d = nullptr;
void RB_DrawQuadPic(GfxRenderCommandExecState* execState)
{
	GfxCmdDrawQuadPic* cmd = (GfxCmdDrawQuadPic*)execState->cmd;

	// normal - 0x2C
	// modified - 0x38
	if (cmd->header.byteCount == sizeof(GfxCmdDrawQuadPic))
	{
		auto& materialCmd = *g_materialCommands;

		int vertCount = materialCmd.vertexCount;
		int indexCount = materialCmd.indexCount;

		GfxVertex* vert = materialCmd.verts;
		uint16_t* indices = materialCmd.indices;

		R_SetTessMaterial(cmd->material, 4);

		if ((vertCount + 4) > 0x800 || (indexCount + 6) > 0xC00)
		{
			RB_EndTessSurface();
			vertCount = materialCmd.vertexCount;
			indexCount = materialCmd.indexCount;
		}

		materialCmd.vertexCount += 4;
		materialCmd.indexCount += 6;

		indices[indexCount] = vertCount + 3;
		indices[indexCount + 1] = vertCount;
		indices[indexCount + 2] = vertCount + 2;
		indices[indexCount + 3] = vertCount + 2;
		indices[indexCount + 4] = vertCount;
		indices[indexCount + 5] = vertCount + 1;

		R_SetVertex4D(&vert[vertCount], cmd->verts[0].x, cmd->verts[0].y, 0.0f, 1.0f, 0.0, 0.0, cmd->color[0].packed);
		R_SetVertex4D(&vert[vertCount + 1], cmd->verts[1].x, cmd->verts[1].y, 0.0f, 1.0f, 1.0, 0.0, cmd->color[1].packed);
		R_SetVertex4D(&vert[vertCount + 2], cmd->verts[2].x, cmd->verts[2].y, 0.0f, 1.0f, 1.0, 1.0, cmd->color[2].packed);
		R_SetVertex4D(&vert[vertCount + 3], cmd->verts[3].x, cmd->verts[3].y, 0.0f, 1.0f, 0.0, 1.0, cmd->color[3].packed);

		*(uint32_t*)execState += sizeof(GfxCmdDrawQuadPic);
	}
	else
	{
		RB_DrawQuadPic_d->invoke(execState);
	}
}

void renderables::start()
{
	RB_DrawQuadPic_d = new detour(g_renderCommandTable[RB_DrawQuadPic_id_t]->address, RB_DrawQuadPic);
}

void renderables::stop()
{
	delete RB_DrawQuadPic_d;
}
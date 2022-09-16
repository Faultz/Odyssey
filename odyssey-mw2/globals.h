#pragma once

// hook addresses
#define cellPad_GetData_t 0x0A47EF4 
#define ScrPlace_EndFrame_t 0x0413314

// function addresses
#define R_AddCmdDrawStretchPic_t 0x04119C8
#define CG_DrawRotatedPicPhysical_t 0x00E2178
#define R_AddCmdDrawText_t 0x0411E18
#define Material_RegisterHandle_t 0x040A27C
#define R_RegisterFont_t 0x04012B4
#define R_TextWidth_t 0x0401688
#define Com_Sprintf_t 0x063A264
#define DB_GetAllXAssetOfType_t 0x0625CB8

#define R_AddCmdSetScissorRect_t 0x0412D60

#define RB_EndTessSurface_t 0x03DC794
#define R_SetTessMaterial_t 0x03DC700

#define R_GetCommandBuffer_t 0x0411460

// quad pic id
#define RB_DrawQuadPic_id_t 0xE

// structure addresses
#define GfxCmdArray_t 0x22AC990
#define RenderCmdBufSize_t 0x02176E08
#define RenderCommandTable_t 0x0A86B30
#define materialCommands_a 0x1979920

static symbol<void(char*, int, const char* ...)> Com_Sprintf { Com_Sprintf_t };

static symbol<void* (bool, int, int, int, int)> R_AddCmdSetScissorRect { R_AddCmdSetScissorRect_t };

static symbol<void(float x, float y, float w, float h, float s0, float t0, float s1, float t1, const float* color, Material* material)> R_AddCmdDrawStretchPic { R_AddCmdDrawStretchPic_t };
static symbol<void(ScreenPlacement* scrPlace, float x, float y, float width, float height, float angle, const float* color, Material* material)> CG_DrawRotatedPicPhysical { CG_DrawRotatedPicPhysical_t };
static symbol<void(const char* text, unsigned int maxChars, Font* font, float x, float y, float xScale, float yScale, float rotation, const float* color, int style)> R_AddCmdDrawText { R_AddCmdDrawText_t };

static symbol<int(int local, const char* text, int maxChars, Font* font)> R_TextWidth { R_TextWidth_t };
static symbol<Material* (const char* name, int imageTrack, int time, bool errorIfMissing)> Material_RegisterHandle { Material_RegisterHandle_t };
static symbol<Font* (const char* text)> R_RegisterFont { R_RegisterFont_t };

static symbol<int(XAssetType, XAssetHeader*)> DB_GetAllXAssetOfType { DB_GetAllXAssetOfType_t };

static symbol<void()> RB_EndTessSurface { RB_EndTessSurface_t };
static symbol<void(Material*, int, int)> R_SetTessMaterial { R_SetTessMaterial_t };
static symbol<void* (int cmd, int size)> R_GetCommandBuffer { R_GetCommandBuffer_t };
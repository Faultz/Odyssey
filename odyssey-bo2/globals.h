#pragma once

// TOC2
#define TOC_2 0x0734BE8

// hook addresses
#define cellPad_GetData_t 0x050F7C4 
#define ScrPlace_EndFrame_t 0x00DB918

// function addresses
#define R_AddCmdDrawStretchPic_t 0x0097D60
#define CG_DrawRotatedPicPhysical_t 0x004FE80
#define R_AddCmdDrawText_t 0x04C7248
#define Material_RegisterHandle_t 0x004C6D28
#define R_RegisterFont_t 0x0A8510
#define R_TextWidth_t 0x04C6D48
#define Com_Sprintf_t 0x027E2F8
#define DB_GetAllXAssetOfType_t 0x025ED50

#define R_AddCmdSetScissorRect_t 0x04C7A88
#define R_AddCmdClearScissorRect_t 0x04C7A78

#define RB_EndTessSurface_t 0x0394AE8
#define R_SetTessMaterial_t 0x0394DB8

#define R_GetCommandBuffer_t 0x0369490

// quad pic id
#define RB_DrawQuadPic_id_t 13

// structure addresses
#define GfxCmdArray_t 0x22AC990
#define RenderCmdBufSize_t 0x02176E08
#define RenderCommandTable_t 0x06F7014
#define materialCommands_a 0x23A5730

static symbol<void(char*, int, const char* ...)> Com_Sprintf{ Com_Sprintf_t };

static symbol<void*(int, int, int, int)> R_AddCmdSetScissorRect { R_AddCmdSetScissorRect_t };
static symbol<void()> R_AddCmdClearScissorRect { R_AddCmdClearScissorRect_t };

static symbol<void(float x, float y, float w, float h, float s0, float t0, float s1, float t1, const float* color, Material* material)> R_AddCmdDrawStretchPic{ R_AddCmdDrawStretchPic_t };
static symbol<void(ScreenPlacement* scrPlace, float x, float y, float width, float height, float angle, const float* color, Material* material)> CG_DrawRotatedPicPhysical{ CG_DrawRotatedPicPhysical_t };
static symbol<void(const char* text, unsigned int maxChars, Font* font, float x, float y, float xScale, float yScale, float rotation, const float* color, int style)> R_AddCmdDrawText{ R_AddCmdDrawText_t };

static symbol<int(const char* text, int maxChars, Font* font)> R_TextWidth{ R_TextWidth_t };
static symbol<Material* (const char* name, int imageTrack)> Material_RegisterHandle{ Material_RegisterHandle_t };
static symbol<Font* (const char* text, int track)> R_RegisterFont{ R_RegisterFont_t };

static symbol<int(XAssetType, XAssetHeader*)> DB_GetAllXAssetOfType { DB_GetAllXAssetOfType_t };

static symbol<void()> RB_EndTessSurface{ RB_EndTessSurface_t };
static symbol<void(Material*, int)> R_SetTessMaterial{ R_SetTessMaterial_t, TOC_2 };
static libpsutil::symbol<void* (int cmd, int size)> R_GetCommandBuffer { R_GetCommandBuffer_t, TOC_2 };
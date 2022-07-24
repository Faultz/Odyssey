#pragma once

// hook addresses
#define cellPad_GetData_t 0x051B6F4 
#define R_EndFrame_t 0x039492C
#define R_UpdateFrameFog_t 0x0477C20

// function addresses
#define R_AddCmdDrawStretchPic_t 0x0392D78
#define CG_DrawRotatedPicPhysical_t 0x04FE80
#define R_AddCmdDrawText_t 0x0393640
#define Material_RegisterHandle_t 0x038B044
#define R_RegisterFont_t 0x03808B8
#define R_TextWidth_t 0x0380928
#define Com_Sprintf_t 0x0298874
#define DB_GetAllXAssetOfType_t 0x0288F10

#define R_AddCmdSetScissorRect_t 0x039434C
#define R_AddCmdClearScissorRect_t 0x03943C8

#define RB_EndTessSurface_t 0x03C453C
#define R_SetTessMaterial_t 0x03C4458

// quad pic id
#define RB_DrawQuadPic_id_t 14

// structure addresses
#define UiContext_t 0x13DAD58
#define GfxCmdArray_t 0x1FD2D0C
#define RenderCmdBufSize_t 0x1DFB780
#define RenderCommandTable_t 0x056C0F4
#define materialCommands_a 0x210C3B0

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
static symbol<void(Material*, int)> R_SetTessMaterial{ R_SetTessMaterial_t };
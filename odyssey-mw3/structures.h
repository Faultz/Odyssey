#pragma once

enum tag_t
{
	auto_bone,
	head,
	neck,
	j_mainroot,
	spine,
	spine_upper,
	spine_lower,
	j_hip_le,
	j_hip_ri,
	j_shoulder_le,
	j_shoulder_ri,
	//j_elbow_le,
	//j_elbow_ri,
	//wrist_le,
	//wrist_ri,
	//knee_le,
	//knee_ri,
	//ankle_le,
	//ankle_ri,
	//ball_le,
	//ball_ri,
	bone_max,
};

struct Font
{
	const char* fontName;
	int pixelHeight;
};

struct GfxCmdHeader
{
	unsigned short id;
	unsigned short byteCount;
};

struct GfxViewport
{
	int x;
	int y;
	int width;
	int height;
};

struct GfxCmdSetScissor
{
	GfxCmdHeader header;
	bool enabled;
	GfxViewport scissor;
};

union GfxColor
{
	char array[4];
	unsigned int packed;
};

struct GfxQuadVertex
{
	vec2_t xy;
	vec2_t st;
	GfxColor color;
};

union PackedUnitVec
{
	unsigned int packed;
	char array[4];
};

struct GfxPointerVertex
{
	vec4_t xyzw;
	vec4_t color;
	vec2_t texCoord;
	PackedUnitVec normal;
};

enum corner_flags_t
{
	cornerFlags_topLeft = 1 << 0,
	cornerFlags_topRight = 1 << 1,
	cornerFlags_botLeft = 1 << 2,
	cornerFlags_botRight = 1 << 3,
	cornerFlags_Top = cornerFlags_topLeft | cornerFlags_topRight,
	cornerFlags_Bot = cornerFlags_botLeft | cornerFlags_botRight,
	cornerFlags_Left = cornerFlags_topLeft | cornerFlags_botLeft,
	cornerFlags_Right = cornerFlags_topRight | cornerFlags_botRight,
	cornerFlags_all = 0xF
};

/*

This file contains definitions used by the Hex-Rays decompiler output.
It has type definitions and convenience macros to make the
output more readable.

Copyright (c) 2007-2011 Hex-Rays

*/
#define POW(x)     ((x) * (x))
#define RAD2DEG( a ) ( ( (a) * 180.0f ) / M_PI )

#define ANGLE2SHORT(x) ((int)((x) * 65536.0f / 360.0f ) & 65535)
#define M_PI 3.14159265359f
#define	SHORT2ANGLE(x)	((x) * ( 360.0f / 65536.0f ))

#define M_E      2.71828182845904523536f
#define DotProduct(a,b)   ((a).x*(b).x+(a).y*(b).y+(a).z*(b).z)
#define VectorSubtract(a,b,c) ((c).x=(a).x-(b).x,(c).y=(a).y-(b).y,(c).z=(a).z-(b).z)
#define VectorAdd(a,b,c)  ((c).x=(a).x+(b).x,(c).y=(a).y+(b).y,(c).z=(a).z+(b).z)
#define VectorCopy(a,b)   ((b).x=(a).x,(b).y=(a).y,(b).z=(a).z)
#define VectorScale(v, s, o) ((o).x=(v).x*(s),(o).y=(v).y*(s),(o).z=(v).z*(s))
#define VectorDistance(a,b)  (sqrtf(POW(a.x-b.x) + POW(a.y-b.y) + POW(a.z-b.z)))
#define VectorMA(v, s, b, o) ((o).x=(v).x+(b).x*(s),(o).y=(v).y+(b).y*(s),(o).z=(v).z+(b).z*(s))
#define VectorCmp(a,b)   (((a).x==(b).x)&&((a).y==(b).y)&&((a).z==(b).z))
#define VectorClear(a)   { (a).x=0.0f; (a).y=0.0f; (a).z=0.0f; }
#define DistanceBox(a,b,c)  ((c*84.0f/(cg->refdef.width*100.0f)) * 18.0f / (VectorDistance(a,b)/55.0f))
#define ScreenDistance(a,b)  (sqrtf(POW(a.x-b.x) + POW(a.y-b.y)))
#define VectorLength(a)         sqrtf(POW((a).x)+POW((a).y)+POW((a).z))
#define VectorDivide(v, s, o)((o).x=(v).x/=(s),(o).y=(v).y/=(s),(o).z=(v).z/=(s))
#define VectorNormalize(a)      {Vector3 l=VectorLength(a);(a).x/=l;(a).y/=l;(a).z/=l;}
#define VectorTransform(a,b,c){(c).x=DotProduct((a),(b).x)+(b)[0].z;(c).y=DotProduct((a),(b).y)+(b)[1].z;(c).z=DotProduct((a),(b).z)+(b)[2].z;}

enum pm_flags_t
{
	pm_prone = 0x1,
	pm_crouch = 0x2,
	pm_running = 0x4000
};

extern int TCLoop;
#if defined(__GNUC__)
typedef          long long ll;
typedef unsigned long long ull;
#define __int64 long long
#define __int32 int
#define __int16 short
#define __int8  char

#define __mask_neg_float -1.0f

#define EF1_CROUCH 0x4
#define EF1_PRONE 0x8
#define EF1_FIRE 0x40
#define EF1_STREAK 0x4000
#define EF1_SPRINT 0x20000
#define EF1_DEAD 0x40000
#define EF1_ZOOM 0x80000
#define EF2_MANTLE 0x400

#define ByteToFloat(a) ((float)(a)/255.0f)
#define FloatToByte(a) ((BYTE)(a)*255.0f)
#define DwordFromBytes(a) (((BYTE)(a)[0])|((BYTE)(a)[1]<<8)|((BYTE)(a)[2]<<16)|((BYTE)(a)[3]<<24))
#define GetSign(a) ((a)?((*(int*)&(a)>>31)|1):0)
#define DegreesToRadians(a) ((a)*((float)M_PI/180.0f))
#define RadiansToDegrees(a) ((a)*(180.0f/(float)M_PI))
#define AngleToShort(a) ((int)((a)*(65536/360.0f))&65535)
#define ShortToAngle(a) (float)((float)((a)*(360.0f/65536)))
#define AngleNormalize(a) (ShortToAngle(AngleToShort((a))))
#define Dereference(a) (*(decltype(a)*)(a))

struct pointer_t
{
	template<class T>
	T get(uint32_t pos)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + pos);
	}
	template<class T>
	void set(T* pointer)
	{
		*(T**)this = (T*)pointer;
	}
};

struct ScreenPlacement : pointer_t
{
	vec2_t scaleVirtualToReal;
	vec2_t scaleVirtualToFull;
	vec2_t scaleRealToVirtual;
	vec2_t virtualViewableMin;
	vec2_t virtualViewableMax;
	vec2_t virtualTweakableMin;
	vec2_t virtualTweakableMax;
	vec2_t realViewportBase;
	vec2_t realViewportSize;
	vec2_t realViewportMid;
	vec2_t realViewableMin;
	vec2_t realViewableMax;
	vec2_t realTweakableMin;
	vec2_t realTweakableMax;
	vec2_t subScreen;
	float hudSplitscreenScale;
};

struct GfxVertex
{
	vec4_t xyzw;
	GfxColor color;
	vec2_t texCoord;
	PackedUnitVec normal;
};
struct GfxRenderCommandExecState
{
	int* cmd;
};

struct GfxPointVertex
{
	vec3_t xyz;
	GfxColor color;
};

struct MaterialInfo
{
	const char* name;
};

struct Material
{
	MaterialInfo info;
};
struct WeaponDef;
union XAssetHeader
{
	void* xmodelPieces;
	void* physPreset;
	void* parts;
	void* model;
	Material* material;
	void* pixelShader;
	void* vertexShader;
	void* techniqueSet;
	void* image;
	void* sound;
	void* sndCurve;
	void* clipMap;
	void* comWorld;
	void* gameWorldSp;
	void* gameWorldMp;
	void* mapEnts;
	void* gfxWorld;
	void* lightDef;
	Font* font;
	void* menuList;
	void* g_menu;
	void* localize;
	WeaponDef* weapon;
	void* sndDriverGlobals;
	void* fx;
	void* impactFx;
	void* rawfile;
	void* stringTable;
	void* data;
};

enum XAssetType
{
	ASSET_TYPE_MODEL = 0x4,
	ASSET_TYPE_MATERIAL = 0x5,
	ASSET_TYPE_IMAGE = 0x9,
	ASSET_TYPE_TRACER = 39
};

struct GfxCmdDrawPoints
{
	GfxCmdHeader header;
	void* material;
	int points_count;
	GfxPointVertex* verts;
	GfxColor outline_clr;
	float thickness;
	bool outline;
	bool pad[8];
};

struct GfxCmdDrawQuadPicW
{
	GfxCmdHeader header;
	Material* material;
	float verts[4][2];
	GfxColor color;
};

struct GfxCmdDrawQuadPic
{
	GfxCmdHeader header;
	Material* material;
	vec2_t verts[4];
	GfxColor color[4];
	//int multicolor;
};

struct GfxCmdArray
{
	unsigned __int8* cmds;
	int usedTotal;
	int usedCritical;
	GfxCmdHeader* lastCmd;
};

struct __declspec(align(8)) materialCommands_t
{
	GfxVertex verts[2048];
	unsigned short indices[3072];
	int vertDeclType;
	unsigned int vertexSize;
	unsigned int indexCount;
	unsigned int vertexCount;
	unsigned int firstVertex;
	unsigned int lastVertex;
	bool finishedFilling;
};

#pragma endregion
#endif
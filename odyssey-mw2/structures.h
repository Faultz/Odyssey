#pragma once

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

struct __attribute__((aligned(4))) GfxCmdHeader
{
	unsigned int byteCount;
	char id;
	char ui3d;
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

struct GfxStreamedPartInfo
{
	unsigned int levelCountAndSize;
	unsigned int hash;
	unsigned __int16 width;
	unsigned __int16 height;
	unsigned __int32 offset : 32;
	unsigned __int32 size : 28;
	unsigned __int32 ipakIndex : 4;
	unsigned __int32 adjacentLeft : 15;
	unsigned __int32 adjacentRight : 15;
	unsigned __int32 compressed : 1;
	unsigned __int32 valid : 1;
};

typedef char* GfxImagePixels;
struct GfxImageStreamedData
{
	unsigned short width;
	unsigned short height;
	unsigned int size;
};

struct __attribute__((aligned(4))) GfxImage
{
	const char* name;
	char _0x4[6];
	char format;
	uint32_t remap;
	unsigned short width;
	unsigned short height;
	unsigned int size;
	char _pad_0x4[0x10];
	void* pixels;
	char pad[0xC];
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


enum pm_flags_t
{
	pm_prone = 0x1,
	pm_crouch = 0x2,
	pm_running = 0x4000
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

struct GfxDrawSurfFields
{
	unsigned __int64 objectId : 16;
	unsigned __int64 reflectionProbeIndex : 8;
	unsigned __int64 hasGfxEntIndex : 1;
	unsigned __int64 customIndex : 5;
	unsigned __int64 materialSortedIndex : 12;
	unsigned __int64 prepass : 2;
	unsigned __int64 useHeroLighting : 1;
	unsigned __int64 sceneLightIndex : 8;
	unsigned __int64 surfType : 4;
	unsigned __int64 primarySortKey : 6;
	unsigned __int64 unused : 1;
};

union GfxDrawSurf
{
	GfxDrawSurfFields fields;
	unsigned __int64 packed;
};

struct __declspec(align(8)) MaterialInfo
{
	const char* name;
	char gameFlags;
	char sortKey;
	char textureAtlasRowCount;
	char textureAtlasColumnCount;
	GfxDrawSurf drawSurf;
	unsigned int surfaceTypeBits;
};

union MaterialPixelShaderProgram
{
	void* sv;
};

struct MaterialPixelShader
{
	const char* name;
	MaterialPixelShaderProgram prog;
};

struct MaterialArgumentCodeConst
{
	unsigned __int16 index;
	unsigned __int8 firstRow;
	unsigned __int8 rowCount;
};

union MaterialArgumentDef
{
	const float* literalConst;
	MaterialArgumentCodeConst constantCode;
	unsigned int codeSampler;
	unsigned int nameHash;
};

struct MaterialShaderArgument
{
	unsigned __int16 type;
	unsigned __int16 dest;
	MaterialArgumentDef u;
};

struct MaterialPass
{
	void* vertexDecl;
	void* vertexShader;
	MaterialPixelShader* pixelShader;
	unsigned char perPrimArgCount;
	unsigned char perObjArgCount;
	unsigned char stableArgCount;
	unsigned char customSamplerFlags;
	unsigned int precompiledIndex;
	MaterialShaderArgument* args;
};

struct MaterialTechnique
{
	const char* name;
	unsigned __int16 flags;
	unsigned __int16 passCount;
	MaterialPass passArray[1];
};

#pragma pack(push, 2)
struct MaterialTechniqueSet
{
	const char* name;
	char worldVertFormat;
	char unused[2];
	MaterialTechniqueSet* remappedTechniqueSet;
	MaterialTechnique* techniques[26];
};
#pragma pack(pop)

struct MaterialWaterDef
{
	int textureWidth;
	float horizontalWorldLength;
	float verticalWorldLength;
	float amplitude;
	float windSpeed;
	float windDirection[2];
	void* map;
};

enum dvarType_t
{
	DVAR_TYPE_BOOL = 0x0,
	DVAR_TYPE_FLOAT = 0x1,
	DVAR_TYPE_FLOAT_2 = 0x2,
	DVAR_TYPE_FLOAT_3 = 0x3,
	DVAR_TYPE_FLOAT_4 = 0x4,
	DVAR_TYPE_INT = 0x5,
	DVAR_TYPE_ENUM = 0x6,
	DVAR_TYPE_STRING = 0x7,
	DVAR_TYPE_COLOR = 0x8,
	DVAR_TYPE_FLOAT_3_COLOR = 0x9,
	DVAR_TYPE_COUNT = 0xA,
};

union DvarLimits
{
	struct
	{
		int stringCount;
		const char** strings;
	} enumeration;

	struct
	{
		int min;
		int _max;
	} integer;

	struct
	{
		float min;
		float _max;
	} value, vector_;

	struct
	{
		int64_t min;
		int64_t _max;
	} integer64;
};

union DvarValue
{
	bool enabled;
	int integer;
	unsigned int unsignedInt;
	float value;
	const char* string;
	vec3_t color;
	float vectorv[4];
};

#pragma pack(push, 4)
struct dvar_t
{
	char* name;
	char pad[4];
	__int16 flags;
	char type;
	char modified;
	DvarValue current;
	DvarValue reset;
	char c[0xC];
	char x[0x4];
	DvarLimits domain;
};
#pragma pack(pop)

struct MaterialTextureDef
{
	uint32_t nameHash;
	vec2_t uvScale;
	GfxImage* image;
	char texSizeShift[2];
	uint16_t uvOffset[3];
};

struct GfxStateBits
{
	unsigned int loadBits[2];
};

static int R_HashString(const char* string)
{
	int hash = 0;
	while (*string)
		hash = (*string++ | 0x20) ^ (33 * hash);
	return hash;
}

struct MaterialConstantDef
{
	uint32_t nameHash;
	char name[12];
	vec4_t vVec4;
};
struct Material
{
	const char* name;
	char _028[0x18];
	MaterialTextureDef* textureTable;
	MaterialConstantDef* constantTable;

	int get_texture_count()
	{
		return (*(int*)(reinterpret_cast<uintptr_t>(this) + 0x14) & 0xF);
	}
	int get_constant_count()
	{
		return (*(int*)(reinterpret_cast<uintptr_t>(this) + 0x14) >> 4) & 0xF;
	}
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
	ASSET_TYPE_MATERIAL = 6,
	ASSET_TYPE_IMAGE = 10,
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

struct GfxCmdBufContext
{
	void* source;
	void* state;
};

enum MaterialTechniqueType
{
	TECHNIQUE_DEPTH_PREPASS = 0x0,
	TECHNIQUE_BUILD_FLOAT_Z = 0x1,
	TECHNIQUE_BUILD_SHADOWMAP_DEPTH = 0x2,
	TECHNIQUE_BUILD_SHADOWMAP_COLOR = 0x3,
	TECHNIQUE_UNLIT = 0x4,
	TECHNIQUE_EMISSIVE = 0x5,
	TECHNIQUE_EMISSIVE_DFOG = 0x6,
	TECHNIQUE_EMISSIVE_SHADOW = 0x7,
	TECHNIQUE_EMISSIVE_SHADOW_DFOG = 0x8,
	TECHNIQUE_LIT_BEGIN = 0x9,
	TECHNIQUE_LIT = 0x9,
	TECHNIQUE_LIT_DFOG = 0xA,
	TECHNIQUE_LIT_SUN = 0xB,
	TECHNIQUE_LIT_SUN_DFOG = 0xC,
	TECHNIQUE_LIT_SUN_SHADOW = 0xD,
	TECHNIQUE_LIT_SUN_SHADOW_DFOG = 0xE,
	TECHNIQUE_LIT_SPOT = 0xF,
	TECHNIQUE_LIT_SPOT_DFOG = 0x10,
	TECHNIQUE_LIT_SPOT_SHADOW = 0x11,
	TECHNIQUE_LIT_SPOT_SHADOW_DFOG = 0x12,
	TECHNIQUE_LIT_SPOT_SHADOW_CUCOLORIS = 0x13,
	TECHNIQUE_LIT_SPOT_SHADOW_CUCOLORIS_DFOG = 0x14,
	TECHNIQUE_LIT_OMNI = 0x15,
	TECHNIQUE_LIT_OMNI_DFOG = 0x16,
	TECHNIQUE_LIT_OMNI_SHADOW = 0x17,
	TECHNIQUE_LIT_OMNI_SHADOW_DFOG = 0x18,
	TECHNIQUE_LIT_END = 0x19,
	TECHNIQUE_LIGHT_SPOT = 0x19,
	TECHNIQUE_LIGHT_OMNI = 0x1A,
	TECHNIQUE_LIGHT_SPOT_SHADOW = 0x1B,
	TECHNIQUE_LIGHT_SPOT_SHADOW_CUCOLORIS = 0x1C,
	TECHNIQUE_FAKELIGHT_NORMAL = 0x1D,
	TECHNIQUE_FAKELIGHT_VIEW = 0x1E,
	TECHNIQUE_SUNLIGHT_PREVIEW = 0x1F,
	TECHNIQUE_CASE_TEXTURE = 0x20,
	TECHNIQUE_WIREFRAME_SOLID = 0x21,
	TECHNIQUE_WIREFRAME_SHADED = 0x22,
	TECHNIQUE_THERMAL = 0x23,
	TECHNIQUE_DEBUG_BUMPMAP = 0x24,
	TECHNIQUE_COUNT = 0x25,
	TECHNIQUE_TOTAL_COUNT = 0x26,
	TECHNIQUE_NONE = 0x27,
};

struct DDS_PIXELFORMAT
{
	uint32_t dwSize;
	uint32_t dwFlags;
	uint32_t dwFourCC;
	uint32_t dwRGBBitCount;
	uint32_t dwRBitMask;
	uint32_t dwGBitMask;
	uint32_t dwBBitMask;
	uint32_t dwABitMask;
};

struct DDS_HEADER
{
	uint32_t dwSize;
	uint32_t dwFlags;
	uint32_t dwHeight;
	uint32_t dwWidth;
	uint32_t dwPitchOrLinearSize;
	uint32_t dwDepth;
	uint32_t dwMipMapCount;
	uint32_t dwReserved1[11];
	DDS_PIXELFORMAT ddspf;
	uint32_t dwCaps;
	uint32_t dwCaps2;
	uint32_t dwCaps3;
	uint32_t dwCaps4;
	uint32_t dwReserved2;
};

#endif
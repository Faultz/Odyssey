#pragma once

// vertex attributes - for testing
#define ATTR_POSITION		(CG_ATTR0 - CG_ATTR0) //J 頂点（float4）
#define ATTR_BLENDWEIGHT	(CG_ATTR1 - CG_ATTR0) //J 頂点ウェイト（float）
#define ATTR_NORMAL		(CG_ATTR2 - CG_ATTR0) //J 法線（float3）
#define ATTR_COLOR			(CG_ATTR3 - CG_ATTR0) //J カラー（float4）
#define ATTR_COLOR0		(CG_ATTR3 - CG_ATTR0) //J カラー（float4）
#define ATTR_DIFFUSE		(CG_ATTR3 - CG_ATTR0) //J カラー（float4）
#define ATTR_COLOR1		(CG_ATTR4 - CG_ATTR0) //J セカンダリカラー（float4）
#define ATTR_SPECULAR		(CG_ATTR4 - CG_ATTR0) //J セカンダリカラー（float4）
#define ATTR_FOGCOORD		(CG_ATTR5 - CG_ATTR0) //J フォグ座標（float）
#define ATTR_TESSFACTOR	(CG_ATTR5 - CG_ATTR0) //J フォグ座標（float）
#define ATTR_PSIZE			(CG_ATTR6  - CG_ATTR0) //J ポイントサイズ（float）
#define ATTR_BLENDINDICES	(CG_ATTR7  - CG_ATTR0) //J スキニング用のパレットインデックス（float4）
#define ATTR_TEXCOORD0		(CG_ATTR8  - CG_ATTR0) //J テクスチャ座標0（float4）
#define ATTR_TEXCOORD1		(CG_ATTR9  - CG_ATTR0) //J テクスチャ座標1（float4）
#define ATTR_TEXCOORD2		(CG_ATTR10 - CG_ATTR0) //J テクスチャ座標2（float4）
#define ATTR_TEXCOORD3		(CG_ATTR11 - CG_ATTR0) //J テクスチャ座標3（float4）
#define ATTR_TEXCOORD4		(CG_ATTR12 - CG_ATTR0) //J テクスチャ座標4（float4）
#define ATTR_TEXCOORD5		(CG_ATTR13 - CG_ATTR0) //J テクスチャ座標5（float4）
#define ATTR_TEXCOORD6		(CG_ATTR14 - CG_ATTR0) //J テクスチャ座標6（float4）
#define ATTR_TANGENT		(CG_ATTR14 - CG_ATTR0) //J 正接ベクトル（float4）
#define ATTR_TEXCOORD7		(CG_ATTR15 - CG_ATTR0) //J テクスチャ座標7（float4）
#define ATTR_BINORMAL		(CG_ATTR15 - CG_ATTR0) //J 従法線ベクトル（float4）

struct gcm_renderer
{
	static cell::Gcm::Inline::CellGcmContext& g_gcmContext;

	gcm_renderer();
	~gcm_renderer();
	static void gcm_renderer::cellGcmSetFlipCommand_h(CellGcmContextData* gcmThis, uint8_t id);

	void draw_quad(float x, float y, float z, float width, float height, float* color);

	static void start();
	static void stop();
};

extern gcm_renderer g_renderer;

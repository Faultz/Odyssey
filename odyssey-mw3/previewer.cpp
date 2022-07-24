#include "stdafx.h"

//cg_t* cg;
//
//model_previewer g_previewer;
//
//static symbol<unsigned int(const char*)> GScr_AllocString { 0x01A4DA8 };
//
//detour* R_AddDObjToScene_d = nullptr;
//void model_previewer::R_AddDObjToScene_hk(DObj* obj, cpose_t* pose, unsigned int entnum, unsigned int renderFxFlags, float* lightingOrigin, float materialTime)
//{
//	cg = reinterpret_cast<cg_t*>(0x38967700);
//	if (cg && cg->get_local_client() == (entnum - 1))
//	{
//		memcpy(&g_previewer.entity, pose, sizeof(cpose_t));
//		R_AddDObjToScene_d->invoke<void>(obj, &g_previewer.entity.pose, entnum, renderFxFlags, lightingOrigin, materialTime);
//	}
//
//	R_AddDObjToScene_d->invoke<void>(obj, pose, entnum, renderFxFlags, lightingOrigin, materialTime);
//}
//
//static symbol<void(DObj* obj, cpose_t*, unsigned int, unsigned int, float*, float)> R_AddDObjToScene { 0x0476BE8 };
//static symbol<DObj* (int, int)> Com_GetClientDObj { 0x02BB688 };
//static symbol<void(DObjModel_s*, int, XAnimTree*, char*, int)> DObjCreate { 0x0392220 };
//
//void model_previewer::create()
//{
//	//model.model = R_RegisterModel("weapon_scar_h_orange_fall");
//	//model.boneName = GScr_AllocString("tag_weapon_right");
//
//	//DObj* my_object = Com_GetClientDObj(cg->get_local_client(), 0);
//	//if (my_object == nullptr) return;
//
//	//DObjCreate(&model, 1, (XAnimTree*)my_object->tree, object_data, 0);
//
//	//R_AddDObjToScene_d = new detour(0x0476BE8, R_AddDObjToScene_hk);
//}
//
//void model_previewer::render()
//{
//	//float lightingOrigin[3] = { 0.f, 0.f, 0.f };
//	//object = (DObj*)&object_data;
//
//	//if (g_localEntity == nullptr) return;
//
//	//DObj * my_object = Com_GetClientDObj(g_cg->get_local_client(), 0);
//	//if (my_object == nullptr) return;
//
//	//object->models = my_object->models;
//
//	//memcpy(&tree, my_object->tree, sizeof(XAnimTree));
//	//memcpy(&g_localEntity->pose, &entity.pose, sizeof(cpose_t));
//
//	//entity.pose.origin = g_cg->refdef.viewOrigin.angles_forward(g_cg->get_client(g_cg->get_local_client())->playerAngles, 60.f);
//
//	//R_AddDObjToScene(object, &entity.pose, 0x7FE, 0, lightingOrigin, 0.0f);
//}
//
//void model_previewer::remove()
//{
//	//delete R_AddDObjToScene_d;
//}

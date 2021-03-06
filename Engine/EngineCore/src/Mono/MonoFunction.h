
//********************************** Effective 3D Engine **************************************************//
//******************* Copyright (c) 2017-12-1 PanQingyun. All rights reserved. *************************//

#pragma once
#include "../Utils/Include.h"

//////////////////////////////////////////////////////////////////////////
// Mono global function
//////////////////////////////////////////////////////////////////////////

//  [12/8/2017 panqingyun]
// https://www.cnblogs.com/murongxiaopifu/p/4557365.html

#define CS_STRING MonoString*
#define CS_OBJECT MonoObject*
#define CPP_OBJECT void*
#define CS_BOOL	MonoBoolean
#define CS_ARRAY MonoArray*

#define VOID void

#define CPP_OBJ_PTR_NAME "CppObject"

template<typename T> inline T * getCppObject(CS_OBJECT obj)
{
	if (obj == nullptr)
	{
		return nullptr;
	}
	MonoClass *klass = mono_object_get_class(obj);
	MonoClassField *  field = mono_class_get_field_from_name(klass, CPP_OBJ_PTR_NAME);

	if (field == nullptr)
	{
		return nullptr;
	}
	void * value = nullptr;
	mono_field_get_value(obj, field, &value);
	return static_cast<T*>(value);
}

void RegisterMonoFunction();

#define TRANSFER_PROPERTY(_property)\
	mBehaviour->SetPropertyValue(#_property, &_property);

#define TRANSFER_FIELD_VALUE(_field)\
	mBehaviour->SetFieldValue(#_field, &_field);

#define TRANSFER_FIELD_OBJECT(_field)\
	mBehaviour->SetFieldValue(#_field, _field->GetMonoBehaviour()->GetMonoObject());

#define _0_PARAM_FUNCTION(className, funcName)\
	className##_##funcName()

#define _1_PARAM_FUNCTION(className, funcName, paramType, paramName)\
	className##_##funcName(paramType paramName)

#define _2_PARAM_FUNCTION(className, funcName, param1Type, param1Name, param2Type, param2Name)\
	className##_##funcName(param1Type param1Name,param2Type param2Name)

#define _3_PARAM_FUNCTION(className, funcName, param1Type, param1Name, param2Type, param2Name, param3Type, param3Name)\
	className##_##funcName(param1Type param1Name,param2Type param2Name, param3Type param3Name)

#define _4_PARAM_FUNCTION(className, funcName, param1Type, param1Name, param2Type, param2Name, param3Type, param3Name, param4Type, param4Name)\
	className##_##funcName(param1Type param1Name,param2Type param2Name, param3Type param3Name, param4Type param4Name)

#define _5_PARAM_FUNCTION(className, funcName, param1Type, param1Name, param2Type, param2Name, param3Type, param3Name, param4Type, param4Name, param5Type, param5Name)\
	className##_##funcName(param1Type param1Name,param2Type param2Name, param3Type param3Name, param4Type param4Name, param5Type param5Name)

#define _6_PARAM_FUNCTION(className, funcName, param1Type, param1Name, param2Type, param2Name, param3Type, param3Name, param4Type, param4Name, param5Type, param5Name, param6Type, param6Name)\
	className##_##funcName(param1Type param1Name,param2Type param2Name, param3Type param3Name, param4Type param4Name, param5Type param5Name, param6Type param6Name)


#define IMPL_GET_FUNCTION(className,funcName)\
	__internal_call_function static CS_OBJECT className##_funcName(CS_OBJECT cs_obj)\
	{\
		className * cpp_obj = static_cast<className*>(getCppObject(cs_obj));\
		if(cpp_obj == nullptr)\
		{\
			return nullptr;\
		}\
		if (cpp_obj->funcName() == nullptr)\
		{\
			return nullptr;\
		}\
		return cpp_obj->funcName()->GetMonoBehaviour()->GetMonoObject();\
	}
using namespace std;
#define __STRINGIFY(x)   #x
#define REGISTER_INTERNAL_CALL(className, funcName)\
	mono_add_internal_call((string("E3DEngine.") + __STRINGIFY(className) + "::" + __STRINGIFY(funcName)).c_str(), (const void*)className##_##funcName);

#ifndef WIN32
#define UINT unsigned int
#endif

VOID _1_PARAM_FUNCTION(GameObject		, newGameObject		, CS_OBJECT		, cs_obj);
VOID _1_PARAM_FUNCTION(Camera			, renderCamera		, CS_OBJECT		, cs_obj);
VOID _1_PARAM_FUNCTION(Debug			, log_error			, CS_STRING		, err);
VOID _1_PARAM_FUNCTION(Debug			, log_warning		, CS_STRING		, warning);
VOID _1_PARAM_FUNCTION(Debug			, log_info			, CS_STRING		, info);
VOID _1_PARAM_FUNCTION(Renderer			, setVertex2Render	, CPP_OBJECT	, obj);
VOID _4_PARAM_FUNCTION(Transform		, getPosition		, CS_OBJECT		, obj		, float&	, x, float&	, y, float&	, z);
VOID _4_PARAM_FUNCTION(Transform		, getRotation		, CS_OBJECT		, obj		, float&	, x, float&	, y, float&	, z);
VOID _4_PARAM_FUNCTION(Transform		, getScale			, CS_OBJECT		, obj		, float&	, x, float&	, y, float&	, z);
VOID _4_PARAM_FUNCTION(Transform		, setPosition		, CS_OBJECT		, obj		, float		, x, float	, y, float	, z);
VOID _4_PARAM_FUNCTION(Transform		, setScale			, CS_OBJECT		, obj		, float		, x, float	, y, float	, z);
VOID _4_PARAM_FUNCTION(Transform		, setRotation		, CS_OBJECT		, obj		, float		, x, float	, y, float	, z);
VOID _4_PARAM_FUNCTION(Transform		, getForward		, CS_OBJECT		, obj		, float&	, x, float&	, y, float&	, z);
VOID _4_PARAM_FUNCTION(Transform		, getUp				, CS_OBJECT		, obj		, float&	, x, float&	, y, float&	, z);
VOID _4_PARAM_FUNCTION(Transform		, getRight			, CS_OBJECT		, obj		, float&	, x, float&	, y, float&	, z);
VOID _2_PARAM_FUNCTION(Renderer			, set_Material		, CS_OBJECT		, cs_boj	, CS_OBJECT	, material);
VOID _2_PARAM_FUNCTION(GameObject		, set_Active		, CS_OBJECT		, cs_boj	, CS_BOOL	, isActive);

VOID _2_PARAM_FUNCTION(GameObject		, set_LayerMask		, CS_OBJECT		, cs_boj	, UINT		, mask);
CS_OBJECT _2_PARAM_FUNCTION(GameObject	, addComponent		, CS_OBJECT		, obj		, CS_STRING	, class_name);
CS_OBJECT _2_PARAM_FUNCTION(GameObject	, getComponent		, CS_OBJECT		, obj		, CS_STRING	, class_name);
CS_OBJECT _1_PARAM_FUNCTION(Renderer	, get_Material		, CS_OBJECT		, cs_boj);
CS_BOOL _1_PARAM_FUNCTION(GameObject	, get_Active		, CS_OBJECT		, cs_boj);
UINT _1_PARAM_FUNCTION(Object			, get_ID			, CS_OBJECT		, cs_boj);
UINT _1_PARAM_FUNCTION(GameObject		, get_LayerMask		, CS_OBJECT		, cs_boj);
CS_OBJECT _2_PARAM_FUNCTION(Material	, createMaterial	, CS_STRING		, path		, int ,id);
CS_OBJECT _1_PARAM_FUNCTION(Renderer	, createRenderer	, UINT			, materialID);
CS_OBJECT _0_PARAM_FUNCTION(Renderer	, createRendererWithoutParam);
VOID _0_PARAM_FUNCTION(Application		, exitApplication);
CS_OBJECT _1_PARAM_FUNCTION(Scene		, createScene			, CS_STRING , path);
VOID _1_PARAM_FUNCTION(GameObject		, removeComponent		, CS_OBJECT		, cs_obj);
CS_ARRAY _2_PARAM_FUNCTION(ParticleSystem	, createParticle	, CS_OBJECT		, cs_obj, CS_STRING		, path);
VOID _2_PARAM_FUNCTION(ParticleGroup		, SetEmitterEnable	, CS_OBJECT		, cs_obj, CS_BOOL		, enable);
VOID _4_PARAM_FUNCTION(Camera				, screen2WorldPoint	, CS_OBJECT		, cs_obj, float&		, x		, float&, y, float&	, z);
VOID _4_PARAM_FUNCTION(Camera				, world2ScreenPoint	, CS_OBJECT		, cs_obj, float&		, x		, float&, y, float&	, z);
VOID _1_PARAM_FUNCTION(GameObject			, destory			, CS_OBJECT		, cs_obj);
CS_OBJECT _2_PARAM_FUNCTION(GameObject		, findChildWithName , CS_OBJECT		, cs_obj, CS_STRING		, name);
CS_OBJECT _2_PARAM_FUNCTION(GameObject		, findChildWithID	, CS_OBJECT		, cs_obj, UINT			, id);
VOID _2_PARAM_FUNCTION(Object				, set_Name			, CS_OBJECT		, cs_obj, CS_STRING		, name);
CS_STRING _1_PARAM_FUNCTION(Object			, get_Name			, CS_OBJECT		, cs_obj);
VOID _2_PARAM_FUNCTION(GameObject			, AddChild			, CS_OBJECT		, cs_obj, CS_OBJECT		,child);
VOID _2_PARAM_FUNCTION(Renderer				, setDrawModule		, CS_OBJECT		, cs_obj		, UINT ,drawModule);
UINT _1_PARAM_FUNCTION(Renderer				, getDrawModule		, CS_OBJECT		, cs_obj);
VOID _5_PARAM_FUNCTION(Light				, setColor			, CS_OBJECT		, cs_obj, float, r, float, g, float, b, float, a);
VOID _2_PARAM_FUNCTION(Camera				, getViewMatrix		, CS_OBJECT		, cs_obj, CS_ARRAY&, data);
VOID _2_PARAM_FUNCTION(Camera				, getProjectionMatrix, CS_OBJECT	, cs_obj, CS_ARRAY& ,data);
VOID _2_PARAM_FUNCTION(PointLight			, set_Range			, CS_OBJECT		, light, float, range);
float _1_PARAM_FUNCTION(PointLight			, get_Range			, CS_OBJECT		, light);
UINT _1_PARAM_FUNCTION(Renderer				, get_RenderIndex	, CS_OBJECT		, cs_boj);
VOID _2_PARAM_FUNCTION(Renderer				, set_RenderIndex	, CS_OBJECT		, cs_boj, UINT, ri);
CS_OBJECT _0_PARAM_FUNCTION(Camera			, get_MainCamera);
VOID _2_PARAM_FUNCTION(RigidBody			, set_Mass			, CS_OBJECT		, rigibody, float, mass);
float _1_PARAM_FUNCTION(RigidBody			, get_Mass			, CS_OBJECT		, rigibody);
float _1_PARAM_FUNCTION(RigidBody			, get_Friction		, CS_OBJECT		, rigibody);
float _1_PARAM_FUNCTION(RigidBody			, get_Restitution	, CS_OBJECT		, rigibody);
VOID _2_PARAM_FUNCTION(RigidBody			, set_Friction		, CS_OBJECT		, rigibody, float, friction);
VOID _2_PARAM_FUNCTION(RigidBody			, set_Restitution	, CS_OBJECT		, rigibody, float, restitution);
VOID _2_PARAM_FUNCTION(Collider				, setSize			, CS_OBJECT		, collider, float, x, float, y, float, z);
VOID _2_PARAM_FUNCTION(Collider				, getSize			, CS_OBJECT		, collider, float&, x, float&, y, float&, z);
VOID _2_PARAM_FUNCTION(CapsuleCollider		, set_Radius		, CS_OBJECT		, capsuleCollider, float, radius);
float _1_PARAM_FUNCTION(CapsuleCollider		, get_Radius		, CS_OBJECT		, capsuleCollider);
VOID _2_PARAM_FUNCTION(CapsuleCollider		, set_Height		, CS_OBJECT		, capsuleCollider, float, height);
float _1_PARAM_FUNCTION(CapsuleCollider		, get_Height		, CS_OBJECT		, capsuleCollider);
CS_OBJECT _1_PARAM_FUNCTION(GameObject		, get_Renderer		, CS_OBJECT		, gameObject);
VOID _3_PARAM_FUNCTION(Material, UpdateFloatValue, CS_OBJECT, material, CS_STRING, name, float, value);
VOID _4_PARAM_FUNCTION(Material, UpdateFloat2Value, CS_OBJECT, material, CS_STRING, name, float, value1, float, value2);
VOID _5_PARAM_FUNCTION(Material, UpdateFloat3Value, CS_OBJECT, material, CS_STRING, name, float, value1, float, value2, float, value3);
VOID _6_PARAM_FUNCTION(Material, UpdateFloat4Value, CS_OBJECT, material, CS_STRING, name, float, value1, float, value2, float, value3, float ,value4);
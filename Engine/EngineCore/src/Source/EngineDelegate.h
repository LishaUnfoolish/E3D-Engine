//
//  EngineDelegate.hpp
//  Obj
//
//  Created by 潘庆云 on 2017/1/17.
//  Copyright © 2017年 潘庆云. All rights reserved.
//

#ifndef __E3D_ENGINE_DELEGATE_HPP__
#define __E3D_ENGINE_DELEGATE_HPP__

#pragma unmanaged
#include "../Camera/E3DCamera.h"
#include "../ParticleSystem/E3DParticleFactory.hpp"
#include "../ParticleSystem/E3DParticleSystem.hpp"
#include "../ParticleSystem/E3DParticlePool.h"
#include "../Effect/E3DEffectFactory.hpp"
#include "../Effect/E3DTextureFrameEffect.hpp"
#include "../Light/E3DLight.hpp"
#include "../Scene/E3DScene.hpp"
#include "../Scene/E3DSceneManager.hpp"
#include "../Mesh/E3DMesh.hpp"
#include "../Component/E3DRectangle.hpp"
#include "../Object/E3DTransform.hpp"
#include "E3DDebug.h"
#include "E3DTimer.hpp"
#include "ClassFactory.h"
#include "../RenderSystem/E3DRenderSystem.hpp"
#include "E3DAutoreleasePool.h"
#include "../Component/E3DBox.h"
#include "../Component/E3DSphere.h"
#include "../Physics/E3DPhysics.h"
#include "NativeSystem.h"
#include "Application.h"
#include "../Mono/MonoScriptManager.h"
#include "../Physics/E3DCollider.hpp"
#include "../Component/E3DTerrain.h"
#include "../Component/E3DSkyBox.h"
#include "../Object/E3DPrefab.h"
#include "E3DThreadTool.h"

#define RAD2DEG(x) (x * 180.0 / M_PI)
#define CAMERA_Z_POS 50.0

#define MainCameraName "main_camera"
#define MeshCamera "mesh_camera"
#define HL_VIDEO_CAMERA "HL_VIDEO_CAMERA"
#define LAYER_CONFIAG_NAME Application::ResourcePath + "LayerConfig.xml"

namespace E3DEngine
{
	class E3D_EXPORT_DLL EngineDelegate : public IObject
	{
	public:
		static EngineDelegate &GetInstance();;
		~EngineDelegate();
		void Initilize(bool isEditor = false);
		void Destory();
		
	public:
		void SetEnginePause(bool bPause);
		bool GetEnginePause();
		bool GetIsRun();
		void SetIsRun(bool isRun);
		void SetPauseRender(bool isPause);

	private:
		EngineDelegate();
	private:
		bool									m_bIsInited;
		bool									m_bPause;
		bool									m_bRun;
		bool									m_bPauseRender;
		bool									m_bRenderPaused;
	public:
		void UpdateRender();
		void UpdateLogic(float deltaTime);
		void UpdatePhysics(float deltaTime);
	};
}

#endif /* EngineDelegate_hpp */

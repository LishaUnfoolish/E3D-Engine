﻿//
//  E3DScene.cpp
//  demo
//
//  Created by 潘庆云 on 2017/7/11.
//

#include "E3DScene.hpp"
#include "../Source/E3DDebug.h"
#include "../Source/EngineDelegate.h"
#include "../RenderSystem/E3DRenderQueue.h"
#include "../Source/FilePath.h"
#include "../Config/TableRegister.h"
#include "../Object/E3DPrefab.h"
#include "E3DSceneLoader.h"

namespace E3DEngine
{		
	Scene::Scene()
	{
		m_nObjectID = 0;
		rootObject = new GameObject;
		rootObject->SetActive(true);
		usedDirectionLight = nullptr;
		m_pSceneMainCamera = nullptr;
#ifdef __E3D_EDITOR__
		pSelectCamera = nullptr;
#endif
		CreateBehaviour();

	}

	void Scene::Create(std::string filePath)
	{
		LoadSceneObjects(filePath);
		mName = GetFileName(filePath);
		m_scenePath = filePath;
	}

	int xc = 0;
	void Scene::Update(float deltaTime)
	{
		// 运行时执行，编辑器下不执行
		if (EngineDelegate::GetInstance().GetIsRun())
		{
			rootObject->Update(deltaTime);
		}
		rootObject->Transform->Update();
		if (!GetRenderSystem()->getIsMutilThreadRender())
		{
			// 渲染场景
			GetRenderSystem()->BeginFrame();
			RenderScene();
			GetRenderSystem()->EndFrame();
		}
		// 运行时执行，编辑器下不执行
		if (EngineDelegate::GetInstance().GetIsRun())
		{
			rootObject->AfterUpdate(deltaTime);
		}
	}

	GameObject* Scene::GetRootObject()
	{
		return rootObject;
	}
	
	void Scene::Destory()
	{
		PhysicWorld::GetInstance().Cleanup();
		SAFE_DELETE(rootObject);
		GetRenderSystem()->Cleanup();
		m_mapRenders.clear();
		m_mapLights.clear();
		m_vecCamera.clear();
		m_pSceneMainCamera = nullptr;
		TableRegister::Destory();
#ifdef __E3D_EDITOR__
		const std::vector<Camera*> &cameras = GetEditorCameras();
		for (auto &camera : cameras)
		{
			camera->GetRenderQueue()->Clear();
			camera->ClearTargetTexture();
		}
#endif
	}

	void Scene::DestoryAllOjectImmediately()
	{
		
	}

	void Scene::RenderScene()
	{
		if (EngineDelegate::GetInstance().GetIsRun())
		{
			for (Camera * camera : m_vecCamera)
			{
				if (camera->IsActive)
				{
					camera->Render();
				}
			}
		}
#ifdef __E3D_EDITOR__
		else
		{
			if (usedDirectionLight != nullptr && object_cast<bool>(Light::GetCreateShadow(usedDirectionLight)))
			{
				usedDirectionLight->GetShadowCamera()->Render();
			}
			if (pSelectCamera != nullptr)
			{
				pSelectCamera->Render();
			}
			const std::vector<Camera*> &cameras = GetEditorCameras();
			for (auto &camera : cameras)
			{
				camera->Render();
			}
		}
#endif
	}

	void Scene::ChangeRenderIndex(UINT id, eRenderIndex index)
	{
        for(Camera *camera : m_vecCamera)
		{
			camera->GetRenderQueue()->ChangeRenderQueue(id, index);
		}
#if __E3D_EDITOR__
		const std::vector<Camera*> &cameras = GetEditorCameras();
		for (auto &camera : cameras)
		{
			camera->GetRenderQueue()->ChangeRenderQueue(id, index);
		}
#endif
	}

	void Scene::AddCamera(Camera * pCamera)
	{
		bool isCanInsert = true;
        for(Camera * camera : m_vecCamera)
		{
			if (pCamera == camera)
			{
				isCanInsert = false;
			}
		}
		if (isCanInsert)
		{
			if (!pCamera->mGameObject->Tag.empty())
			{
				if (object_cast<string>(pCamera->mGameObject->Tag) == MAIN_CAMERA_TAG)
				{
					m_pSceneMainCamera = pCamera;
				}
			}
			m_vecCamera.emplace_back(pCamera);
			ChangeCameraObject(pCamera);
		}
#if __E3D_EDITOR__
		const std::vector<Camera*> &cameras = GetEditorCameras();
		for (auto &camera : cameras)
		{
			ChangeCameraObject(camera);
		}
#endif
	}

	void Scene::SortCamera()
	{
		std::sort(m_vecCamera.begin(), m_vecCamera.end(),
		[](Camera * camera1, Camera * camera2)
		{
			return camera1->Depth < camera2->Depth;
		});
	}

	void Scene::ChangeCameraObject(Camera * pCamera)
	{
		for (auto &obj : m_mapRenders)
		{
			if (obj.second->GetLayerMask() & pCamera->mGameObject->GetLayerMask())
			{
				pCamera->GetRenderQueue()->Add(obj.second);
			}
		}
	}

	void Scene::ChangeRenderObjectLayer(RenderObject * rb)
	{
		if (m_mapRenders.find(rb->ID) == m_mapRenders.end())
		{
			return;
		}

		for (Camera * camera : m_vecCamera)
		{
			if (camera->mGameObject->GetLayerMask() & rb->GetLayerMask())
			{
				camera->GetRenderQueue()->Add(rb);
			}
			else
			{
				camera->GetRenderQueue()->Remove(rb);
			}
		}

#if __E3D_EDITOR__
		const std::vector<Camera*> &cameras = GetEditorCameras();
		for (auto &camera : cameras)
		{
			if (camera->mGameObject->GetLayerMask() & rb->GetLayerMask())
			{
				camera->GetRenderQueue()->Add(rb);
			}
			else
			{
				camera->GetRenderQueue()->Remove(rb);
			}
		}
#endif
	}


	Camera* Scene::GetCamera(UINT cameraID)
	{
        for (Camera * camera : m_vecCamera)
		{
			if (camera->ID == cameraID)
			{
				return camera;
			}
		}
		return nullptr;
	}


	Camera * Scene::GetMainCamera()
	{
		if (EngineDelegate::GetInstance().GetIsRun())
		{
			return m_pSceneMainCamera;
		}
		else
		{
#ifdef __E3D_EDITOR__
			return GetEditorMainCamera();
#endif
		}
	}

	void Scene::ChangeViewportSize(float w, float h)
	{
        for (auto &camera : m_vecCamera) 
		{
            camera->ChangeViewport(w / h);
        }
#ifdef __E3D_EDITOR__
		const std::vector<Camera*> &cameras = GetEditorCameras();
		for (auto &camera : cameras)
		{
			camera->ChangeViewport(w / h);
		}
#endif
	}

	void Scene::AddRenderObject(RenderObject* rb, UINT layer)
	{
		if (rb == nullptr)
		{
			return;
		}

		if (m_mapRenders.find(rb->ID) != m_mapRenders.end())
		{
			return;
		}

		m_mapRenders[rb->ID] = rb;
		
        for(Camera * camera : m_vecCamera)
		{
			if (camera->mGameObject->GetLayerMask() & layer)
			{
				camera->GetRenderQueue()->Add(rb);
			}
		}
#ifdef __E3D_EDITOR__
		const std::vector<Camera*> &cameras = GetEditorCameras();
		for (auto &camera : cameras)
		{
			if (camera->mGameObject->GetLayerMask() & layer)
			{
				camera->GetRenderQueue()->Add(rb);
			}
		}
#endif
	}

	void Scene::CreateBehaviour()
	{
		mBehaviour->SetImage(MonoScriptManager::GetInstance().GetEngineImage());
		mBehaviour->Create(NAME_SPACE, "Scene");
		setBehaviourDefaultValue();
	}

	Scene::~Scene()
	{

	}

	void Scene::AddLight(Light * light)
	{
		if (m_mapLights.find(light->ID) != m_mapLights.end())
		{
			return;
		}

		if (light->Type == eDIRECTION_LIGHT)
		{
			usedDirectionLight = (DirectionLight*)light;
		}
		else
		{
			m_mapLights[light->ID] = light;
		}
	}


	void Scene::DeleteLight(Light *light)
	{
		if (light == nullptr)
		{
			return;
		}

		if (light->Type == eDIRECTION_LIGHT)
		{
			SAFE_DELETE(usedDirectionLight);
		}
		else if (light->Type == ePOINT_LIGHT)
		{
			if (m_mapLights.find(light->ID) != m_mapLights.end())
			{
				m_mapLights.erase(light->ID);
				SAFE_DELETE(light);
			}
		}
	}

	Light * Scene::GetDirectionalLight()
	{ 
		return usedDirectionLight; 
	}

	std::map<UINT, Light*>& Scene::GetPointLights()
	{
		return m_mapLights;
	}

	void Scene::AddObject(Object * obj)
	{
		if (obj == nullptr)
		{
			return;
		}
		GameObject *go = static_cast<GameObject*>(obj);
		if (rootObject->FindChild(go->ID))
		{
			return;
		}
		rootObject->AddChild(go);
		/*if (static_cast<GameObject*>(obj)->GetComponent(CAMERA_NAME) != nullptr)
		{
			AddCamera((Camera*)static_cast<GameObject*>(obj)->GetComponent("E3DEngine.Camera"));
		}*/
	}

	void Scene::RemoveObject(Object * node)
	{
		if (node == nullptr)
		{
			return;
		}
		DWORD id = node->ID;

		if (rootObject->FindChild(id))
		{
			GameObject * go = static_cast<GameObject*>(node);

			rootObject->RemoveChild(go);
		}
	}

	void Scene::RemoveObject(UINT ID)
	{
		if (rootObject->FindChild(ID))
		{
			rootObject->RemoveChild(ID);
		}
	}

	UINT Scene::GenObjectID()
	{
		return  ++m_nObjectID;
	}

	std::string Scene::GetScenePath()
	{
		return m_scenePath;
	}


	void Scene::SetScenePath(std::string path)
	{
		m_scenePath = path;
	}

	void Scene::Save()
	{
		SaveCurrentScene();
	}

}

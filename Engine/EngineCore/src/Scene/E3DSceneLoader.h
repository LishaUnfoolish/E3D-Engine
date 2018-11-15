//********************************** Effect 3D Engine **************************************************//
//******************* Copyright (c) 2018-9-29  PanQingyun. All rights reserved. ************************//
#pragma once
#include "..\Utils\E3DUtil.hpp"

namespace E3DEngine
{
	class Prefab;
	class GameObject;
	/*
		����Ԥ��
	*/
	E3D_EXPORT_DLL GameObject * LoadPrefab(std::string path);

	/*
		���س���
	*/
	E3D_EXPORT_DLL void LoadSceneObjects(std::string sceneFilePath);

	/*
		ע���������
	*/
	void RegistScnObjCreateFunc();

	/*
		���浱ǰ����
	*/
	E3D_EXPORT_DLL void SaveCurrentScene();

	/*
		����Ԥ��
	*/
	E3D_EXPORT_DLL void SavePrefab(Prefab *prefab);

	/*
		��ȡ��ǰ���ظ�·��
	*/
	E3D_EXPORT_DLL std::string GetCurLoadRootPath();
}
//********************************** Effect 3D Engine **************************************************//
//******************* Copyright (c) 2018-9-29  PanQingyun. All rights reserved. ************************//
#pragma once
#include "../Config/TableRegister.h"

namespace E3DEngine
{
	class Prefab;
	/*
		����Ԥ��
	*/
	GameObject * LoadPrefab(std::string path);

	/*
		���س���
	*/
	void LoadSceneObjects(string sceneFilePath);

	/*
		ע���������
	*/
	void RegistScnObjCreateFunc();

	/*
		���浱ǰ����
	*/
	void SaveCurrentScene();

	/*
		����Ԥ��
	*/
	void SavePrefab(Prefab *prefab);
}
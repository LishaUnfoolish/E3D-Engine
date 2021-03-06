
//********************************** Effect 3D Engine **************************************************//
//******************* Copyright (c) 2017-7-7  PanQingyun. All rights reserved. *************************//

#ifndef __PLANE_H__
#define __PLANE_H__
#include "E3DGameObject.h"

namespace E3DEngine
{
	class Plane : public GameObject
	{
	public:
		Plane();
		~Plane();
	public:
		void Create(int width, int height);

	private:
		float m_EachGridWidth;
		float m_EachGridHeight;
		//Renderer *m_Renderer;
	};
}

#endif
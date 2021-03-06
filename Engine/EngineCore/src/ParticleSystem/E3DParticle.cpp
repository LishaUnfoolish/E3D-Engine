﻿//********************************** Effective 3D Engine **************************************************//
//******************* Copyright (c) 2017-1-17  PanQingyun. All rights reserved. ************************//
#include "E3DParticle.h"
#include "../Effect/E3DEffectFactory.hpp"
#include "../Source/EngineDelegate.h"
#include "E3DParticleEmitter.hpp"
#include "E3DParticleAffector.hpp"
#include "../Config/TableRegister.h"

namespace E3DEngine
{
	Particle::Particle()
	{
		autoRelease();
		Transform = new CTransform();
		m_FrameEffect = new TextureFrameEffect();
		m_FrameEffect->IsParticleEffect = false;
	}
	
	void Particle::Create(float width, float height, bool ownDimension, float color, vec3f position, int fsIndex,float time2Live)
	{
		m_fTimeToLive = time2Live;
		m_fTotalTimeToLive = time2Live;
		m_fWidth = width;
		m_fHeight = height;
		m_vDirection = vec3f(0, 0, 0);
		fShaderIndex = fsIndex;
		
		ReSet();
		m_fTextureCoord[0][0] = 0;
		m_fTextureCoord[0][1] = 0;
		m_fTextureCoord[1][0] = 0;
		m_fTextureCoord[1][1] = 1;
		m_fTextureCoord[2][0] = 1;
		m_fTextureCoord[2][1] = 1;
		m_fTextureCoord[3][0] = 1;
		m_fTextureCoord[3][1] = 0;
		m_NotChangeTextureCoord[0].x = 0;
		m_NotChangeTextureCoord[0].y = 0;
		m_NotChangeTextureCoord[1].x = 0;
		m_NotChangeTextureCoord[1].y = 1;
		m_NotChangeTextureCoord[2].x = 1;
		m_NotChangeTextureCoord[2].y = 1;
		m_NotChangeTextureCoord[3].x = 1;
		m_NotChangeTextureCoord[3].y = 0;
		
		Transform->SetLocalPosition(position);
		vec3f up = vec3f(0, 1, 0);
		

		bFristUpdte = true;
		m_rotateSpeed = 0.0f;
		m_rotateAngle = 0.0f;
		m_RandomForceID = 0;
		const uint indexs[] =
		{
			0, 1, 2,
			2, 3, 0
		};
		memcpy(Indices, indexs, sizeof(uint)* 6);
		StartEffect();
		initVertex();
	}
	
	void Particle::ResetFrameEffect()
	{
		m_FrameEffect->ReSet();
	}
	
	void Particle::SetTextureUVCoord(float x, float y, vec2f scale)
	{
		float absScaleX = abs(scale.x);
		float absScaleY = abs(scale.y);
		for (int i = 0; i < 4; i++)
		{
			float _x = (m_NotChangeTextureCoord[i].x - 0.5) * (scale.x / absScaleX) + 0.5; // 如果X上的缩放是负的 则 表示纹理左右颠倒
			float _y = (m_NotChangeTextureCoord[i].y - 0.5) * (scale.y / absScaleY) + 0.5; // 如果Y上的缩放是负的 则 表示纹理上下颠倒
			m_fTextureCoord[i][0] = (x + _x) * absScaleX;
			m_fTextureCoord[i][1] = (y + _y) * absScaleY;
		}
	}
	
	void Particle::EffectFrameChange(IObject *_sender, EventArgs *e)
	{
		if (e == nullptr)
		{
			return;
		}
		
		SetTextureUVCoord(e->lParam, e->rParam, m_FrameEffect->GetTextureFrameScale());
	}

	void Particle::EffectEnd(IObject *_sender, EventArgs *e)
	{
		if (e == nullptr)
		{
			return;
		}

		SetTextureUVCoord(e->lParam, e->rParam, m_FrameEffect->GetTextureFrameScale());
	}
	
	void Particle::PlayDurationEnd(IObject * sender, EventArgs * e)
	{

	}

	void Particle::ResetTextureUVCoord(float whscale)
	{
		float w = m_wh.x;
		w = w*whscale;
		m_fTextureCoord[2][0] = m_uv.x + w;
		m_fTextureCoord[3][0] = m_uv.x + w;
		w = m_fWidth*whscale - (m_fWidth/2);
		m_Point[2] = vec3f(w, -m_fHeight / 2, 0);
		m_Point[3] = vec3f(w, m_fHeight / 2, 0);
	}
	
	void Particle::SetTextureUVCoord(vec2f uv, vec2f wh)
	{
		m_fTextureCoord[0][0] = uv.x;
		m_fTextureCoord[0][1] = uv.y;
		m_fTextureCoord[1][0] = uv.x;
		m_fTextureCoord[1][1] = uv.y + wh.y;
		m_fTextureCoord[2][0] = uv.x + wh.x;
		m_fTextureCoord[2][1] = uv.y + wh.y;
		m_fTextureCoord[3][0] = uv.x + wh.x;;
		m_fTextureCoord[3][1] = uv.y;
		
		m_uv = uv;
		m_wh = wh;
	}
	
	void Particle::ReSet()
	{
		m_fColor[0] = 1;
		m_fColor[1] = 1;
		m_fColor[2] = 1;
		m_fColor[3] = 1;
		m_Point[0] = vec3f(-m_fWidth / 2, m_fHeight / 2, 0);
		m_Point[1] = vec3f(-m_fWidth / 2, -m_fHeight / 2, 0);
		m_Point[2] = vec3f(m_fWidth / 2, -m_fHeight / 2, 0);
		m_Point[3] = vec3f(m_fWidth / 2, m_fHeight / 2, 0);
		Transform->SetLocalRotation(vec3f(0,0,0));
		Transform->SetLocalScale(vec3f(1,1,1));
		m_fSpeed = 0;
		m_vDirection = vec3f(0, 0, 0);
		if (m_FrameEffect != nullptr)
		{
			m_FrameEffect->Stop(true);
		}
	}
	
	Particle::~Particle()
	{
		SAFE_DELETE(Transform);
	}
	
	float * Particle::GetColor()
	{
		return m_fColor;
	}

	Vertex * Particle::getVertex(mat4f worldMatrix)
	{
		return Vertes;
	}


	BatchVertex * Particle::getBatchVertex()
	{
		return BVertes;
	}

	void Particle::SetSize(Vector2 size)
	{
		m_fWidth = size.x;
		m_fHeight = size.y;
		m_Point[0] = vec3f(-m_fWidth / 2, m_fHeight / 2, 0);
		m_Point[1] = vec3f(-m_fWidth / 2, -m_fHeight / 2, 0);
		m_Point[2] = vec3f(m_fWidth / 2, -m_fHeight / 2, 0);
		m_Point[3] = vec3f(m_fWidth / 2, m_fHeight / 2, 0);
	}

	void Particle::SetColor(long color)
	{
		vec4f vColor = Convert::ToColorRGBA(color);
		m_fColor[0] = vColor.r;
		m_fColor[1] = vColor.g;
		m_fColor[2] = vColor.b;
		m_fColor[3] = vColor.a;
		
	}
	
	void Particle::SetColor(float r, float g, float b)
	{
		m_fColor[0] = RangeNumber(r, 1, 0);
		m_fColor[1] = RangeNumber(g, 1, 0);
		m_fColor[2] = RangeNumber(b, 1, 0);
	}
	
	void Particle::SetColor(float r, float g, float b, float a)
	{
		m_fColor[0] = RangeNumber(r, 1, 0);
		m_fColor[1] = RangeNumber(g, 1, 0);
		m_fColor[2] = RangeNumber(b, 1, 0);
		m_fColor[3] = RangeNumber(a, 1, 0);
	}
	
	void Particle::SetDimension(float width, float height)
	{
		
	}
	
	// 根据旋转和 位移计算 顶点 新的坐标
	void Particle::updateVertexCoord()
	{
		// 1---------4
		// |\        |
		// |  \      |
		// |    \    |   (1,2,3),(3,4,1)
		// |      \  |
		// |        \|
		// 2---------3
		
		BVertes[0].SetTransformPosition(Transform->Position.x, Transform->Position.y, Transform->Position.z);
		BVertes[0].SetTransformScale(Transform->Scale.x, Transform->Scale.y, Transform->Scale.z);
		BVertes[0].SetTransformRotate(Transform->RotationEuler.x, Transform->RotationEuler.y, Transform->RotationEuler.z);
		BVertes[0].SetColor(m_fColor[0], m_fColor[1], m_fColor[2], m_fColor[3]);
		
		BVertes[1].SetTransformPosition(Transform->Position.x, Transform->Position.y, Transform->Position.z);
		BVertes[1].SetTransformScale(Transform->Scale.x, Transform->Scale.y, Transform->Scale.z);
		BVertes[1].SetTransformRotate(Transform->RotationEuler.x, Transform->RotationEuler.y, Transform->RotationEuler.z);
		BVertes[1].SetColor(m_fColor[0], m_fColor[1], m_fColor[2], m_fColor[3]);
		
		BVertes[2].SetTransformPosition(Transform->Position.x, Transform->Position.y, Transform->Position.z);
		BVertes[2].SetTransformScale(Transform->Scale.x, Transform->Scale.y, Transform->Scale.z);
		BVertes[2].SetTransformRotate(Transform->RotationEuler.x, Transform->RotationEuler.y, Transform->RotationEuler.z);
		BVertes[2].SetColor(m_fColor[0], m_fColor[1], m_fColor[2], m_fColor[3]);
		
		BVertes[3].SetTransformPosition(Transform->Position.x, Transform->Position.y, Transform->Position.z);
		BVertes[3].SetTransformScale(Transform->Scale.x, Transform->Scale.y, Transform->Scale.z);
		BVertes[3].SetTransformRotate(Transform->RotationEuler.x, Transform->RotationEuler.y, Transform->RotationEuler.z);
		BVertes[3].SetColor(m_fColor[0], m_fColor[1], m_fColor[2], m_fColor[3]);
	}
	
	void Particle::RotateByDir(vec3f dir,float force)
	{
		if(dir.length() > 0.0f)
		{
			dir.y *= -1.0f;
			float tana = dir.x / dir.y;
			float a = atan(tana);
			if(dir.y > 0)
			{
				a += M_PI;
			}
            
            /*if(force < 1.0f)
            {
                a = m_rotateAngle + a*force;
            }*/
            float offset = a - m_rotateAngle;
            a = m_rotateAngle + offset*force;
            
			Transform->SetLocalRotation(0, 0, a);
			SetRotateAngle(a);
		}
	}
	
	void Particle::move2Position(vec3f newPos)
	{
		
	}
	
	void Particle::Update(float deltaTime)
	{
		m_fTimeToLive -= deltaTime;
		
		if (m_FrameEffect != nullptr)
		{
			m_FrameEffect->Update(deltaTime);
		}
		updateVertexCoord();
	}

	uint * Particle::getIndices()
	{
		return Indices;
	}

	float Particle::getParticleTimeToLive() const
	{
		return m_fTimeToLive;
	}

	void Particle::SetScale(float scale)
	{
		float w = m_fWidth*scale;
		float h = m_fHeight*scale;
		m_Point[0] = vec3f(-w / 2, h / 2, 0);
		m_Point[1] = vec3f(-w / 2, -h / 2, 0);
		m_Point[2] = vec3f(w / 2, -h / 2, 0);
		m_Point[3] = vec3f(w / 2, h / 2, 0);
	}


	void Particle::SetTimeToLive(float totalTime)
	{
		m_fTotalTimeToLive = totalTime;
		m_fTimeToLive = m_fTotalTimeToLive;
	}


	void Particle::SetMoveDirction(vec3f direction)
	{
		m_vDirection = direction;
	}


	vec3f Particle::GetMoveDirection()
	{
		return m_vDirection;
	}

	void Particle::SetMoveSpeed(float speed)
	{
		m_fSpeed = speed;
	}


	float Particle::GetMoveSpeed()
	{
		return m_fSpeed;
	}


	vec3f Particle::GetReightVector()
	{
		return Convert::Vec4ToVec3<float>(m_Point[3] - m_Point[0]);
	}

	float Particle::getLiveTime()
	{
		return m_fTotalTimeToLive - m_fTimeToLive;
	}


	float Particle::getTotleLiveTime()
	{
		return m_fTotalTimeToLive;
	}


	void Particle::setAlpha(float alpha)
	{
		m_fColor[3] = alpha;
	}


	float Particle::GetAlpha()
	{
		return m_fColor[3];
	}


	void Particle::SetRotateSpeed(float speed)
	{
		m_rotateSpeed = speed;
	}


	float Particle::GetRotateSpeed()
	{
		return m_rotateSpeed;
	}


	void Particle::SetRotateAngle(float Angle)
	{
		float a = Angle * 180 / M_PI;
		if (a > 360.0 || Angle < -360.0)
		{
			Angle = 0.0;
		}
		m_rotateAngle = Angle;
	}


	float Particle::GetRotateAngle()
	{
		return m_rotateAngle;
	}


	void Particle::StartEffect()
	{
		if (m_FrameEffect != nullptr && m_FrameEffect->IsParticleEffect)
		{
			m_FrameEffect->Play();
			m_FrameEffect->AddSource(this);
			vec2f &&uvpos = m_FrameEffect->GetTextureFrameCoord(0);
			SetTextureUVCoord(uvpos.x, uvpos.y, m_FrameEffect->GetTextureFrameScale());
		}
	}

	void Particle::SetRandomForceID(int ID)
	{
		m_RandomForceID = ID;
	}

	int Particle::GetRandomForceID()
	{
		return m_RandomForceID;
	}

	void Particle::SetBornEmitterPos(vec3f pos)
	{
		m_vBornEmitterPosition = pos;
	}

	vec3f Particle::GetBornEmitterPos()
	{
		return m_vBornEmitterPosition;
	}

	void Particle::SetBornPos(vec3f pos)
	{
		m_vBornPosition = pos;
	}

	vec3f Particle::GetBornPos()
	{
		return m_vBornPosition;
	}

	vec2f Particle::GetUVCenter()
	{
		return m_uv + m_wh / 2;
	}

	void Particle::ActiveEffect(TextureUVConfig * cfg)
	{
		m_FrameEffect->Init(*cfg);
	}

	void Particle::initVertex()
	{
		// 1---------4
		// |\        |
		// |  \      |
		// |    \    |   (1,2,3),(3,4,1)
		// |      \  |
		// |        \|
		// 2---------3
		Vertes[0].SetPosition(m_Point[0].x, m_Point[0].y, m_Point[0].z);
		Vertes[0].SetNormal(0, 0, 1);
		Vertes[0].SettextureCoord1(m_fTextureCoord[0][0], m_fTextureCoord[0][1]);
		Vertes[0].SetTangent(-1, 1, 0);

		Vertes[1].SetPosition(m_Point[1].x, m_Point[1].y, m_Point[1].z);
		Vertes[1].SetNormal(0, 0, 1);
		Vertes[1].SettextureCoord1(m_fTextureCoord[1][0], m_fTextureCoord[1][1]);
		Vertes[1].SetTangent(-1, -1, 0);

		Vertes[2].SetPosition(m_Point[2].x, m_Point[2].y, m_Point[2].z);
		Vertes[2].SetNormal(0, 0, 1);
		Vertes[2].SettextureCoord1(m_fTextureCoord[2][0], m_fTextureCoord[2][1]);
		Vertes[2].SetTangent(1, -1, 0);

		Vertes[3].SetPosition(m_Point[3].x, m_Point[3].y, m_Point[3].z);
		Vertes[3].SetNormal(0, 0, 1);
		Vertes[3].SettextureCoord1(m_fTextureCoord[3][0], m_fTextureCoord[3][1]);
		Vertes[3].SetTangent(1, 1, 0);
	}


	ParticleGroup::ParticleGroup()
	{
		mName				= _ParticleGroupName;
		m_particleIndex		= 0;
		m_bIsPause			= false;
		m_nColor			= 0xffffffff;
		beGetFromLocalPool	= false;
		bIsEnable			= true;
		bLockEnable			= false;
		CREATE_BEHAVIOUR(ParticleGroup);
	}

	ParticleGroup::~ParticleGroup()
	{
		clearParticle2DeathPool(true);
		m_vecUVConfigs.clear();
		for (auto & emitter : m_particleEmitters)
		{
			SAFE_DELETE(emitter);
		}
		m_particleEmitters.clear();
		for (auto & affector : m_particleAffectors)
		{
			SAFE_DELETE(affector);
		}
		m_particleAffectors.clear();
	}
	
	void ParticleGroup::CleaParticle()
	{
		if (m_pRenderer != nullptr)
		{
			static_cast<BatchRenderer*>(m_pRenderer)->RemoveInRenderer(ID);
		}
	}
	
	share_pointer<Particle> ParticleGroup::createParticle()
	{
		if (m_DethParticles.empty())
		{
			return share_pointer<Particle>::null();
		}
		share_pointer<Particle> p = m_DethParticles.front();
		m_DethParticles.pop_front();
		return p;
	}
	
	bool ParticleGroup::activeParticleFromDeath(unsigned int particleNumber, float time2Live, vec3f pos, Vector2 size, float color, uint groupID,vec3f bornEmitterPos)
	{
		share_pointer<Particle> p = createParticle();
		if (p != share_pointer<Particle>::null() && p.getReferenceCount() != 0)
		{
			p.get_ptr()->GroupID = groupID;
			p.get_ptr()->StartEffect();
			p.get_ptr()->SetTimeToLive(time2Live);
			p.get_ptr()->SetColor(m_nColor);
			p.get_ptr()->SetSize(size);
			p.get_ptr()->SetMoveSpeed(0);
			p.get_ptr()->setAlpha(color);
			p.get_ptr()->Transform->SetLocalPosition(pos);
			p.get_ptr()->SetBornEmitterPos(bornEmitterPos);
			p.get_ptr()->bFristUpdte = true;
			p.get_ptr()->ResetFrameEffect();
			m_ActiveParticles.push_back(p);
			return true;
		}
		return false;
	}
	
	void ParticleGroup::clearParticle2DeathPool(bool beDelete)
	{
		beGetFromLocalPool = false;
		if (m_ParticlePool.empty())
		{
			return;
		}
		if (!beDelete)
		{
			if (m_pRenderer == nullptr)
			{
				return;
			}
			static_cast<BatchRenderer*>(m_pRenderer)->RemoveInRenderer(ID);
		}
		ParticlePool::GetInstance().DeleteParticleToDeathPool(m_ParticlePool);
		m_ParticlePool.clear();
		m_ActiveParticles.clear();
		m_DethParticles.clear();
	}
	
	void ParticleGroup::SetActive(bool isActive)
	{
		if (isActive == IsActive)
		{
			return;
		}
		IsActive = isActive;
		if (isActive)
		{
			createVetexIndex();
		}
		else
		{
			if (m_pRenderer != nullptr)
			{
				static_cast<BatchRenderer*>(m_pRenderer)->RemoveInRenderer(ID);
			}
		}
		GameObject::SetActive(isActive);
	}
	
	int ParticleGroup::GetVertexSize()
	{
		return m_ParticlePool.size() * 4;
	}

	std::vector<ParticleEmitter*> * ParticleGroup::GetEmitter()
	{
		return &m_particleEmitters;
	}

	std::vector<ParticleAffector*> * ParticleGroup::GetAffector()
	{
		return &m_particleAffectors;
	}

	void ParticleGroup::SetRenderer(BatchRenderer * buffer)
	{
		m_pRenderer = buffer;
		SceneManager::GetCurrentScene()->AddRenderObject(m_pRenderer, m_layerMask);
		m_pRenderer->CreateNewTransform();
		SetRenderIndex(eRI_TopMost);
		m_pRenderer->GetTransform()->SetIsBillBoard(true);
	}
	
	void ParticleGroup::CreateParticle(unsigned int particleNumber, float time2Live, vec3f pos, Vector2 size, float color, uint groupID,vec3f bornEmitterPos, bool isFirstCreate)
	{
		if (beGetFromLocalPool && !m_ParticlePool.empty())
		{
			createParticleFromDethPool(particleNumber, time2Live, pos, size, color, groupID, bornEmitterPos, particleNumber);
		}
		else
		{
			if (isFirstCreate)
			{
				initCfg = particleInitConfig(particleNumber, time2Live, pos, size, color, groupID, bornEmitterPos, true);
			}
			createParticleFromPool(initCfg.ParticleNumber, initCfg.Size, initCfg.Color, initCfg.Pos, initCfg.Time2Live, initCfg.GroupID, initCfg.BornEmitterPos, initCfg.IsFirstCreate);
			createVetexIndex();
			bIsEnable = true;
			if (!isFirstCreate)
			{
				createParticleFromDethPool(particleNumber, time2Live, pos, size, color, groupID, bornEmitterPos, particleNumber);
			}
		}
	}
	
	void ParticleGroup::SetMaxParticleNumber(UINT number)
	{
		m_uiMaxParticleNumber = number;
	}


	std::list<E3DEngine::share_pointer<E3DEngine::Particle>> * ParticleGroup::GetActiveParticles()
	{
		return &m_ActiveParticles;
	}

	void ParticleGroup::SetfShaderIndex(int index)
	{
		fShaderIndex = index;
	}

	void ParticleGroup::AddParticleEmitter(E3DEngine::ParticleEmitter *emitter)
	{
		m_particleEmitters.push_back(emitter);
	}
	
	void ParticleGroup::AddParticleAffector(ParticleAffector * affector)
	{
		m_particleAffectors.push_back(affector);
	}
	
	void ParticleGroup::createVetexIndex()
	{
		if (m_pRenderer == nullptr)
		{
			return;
		}
		static_cast<BatchRenderer*>(m_pRenderer)->RemoveInRenderer(ID);
		// 初始化粒子团内的所有粒子
		int ivertexTotal = 0;
		int iindexTotal = 0;
		BatchRenderer * render = static_cast<BatchRenderer*>(m_pRenderer);;
		render->FillBegin(ID);		

		for (share_pointer<Particle> &it : m_ParticlePool)
		{
			if (it.getReferenceCount() == 0)
			{
				continue;
			}
			Vertex *vertex = it.get_ptr()->getVertex(Transform->WorldMatrix);
			for (int vi = 0; vi < 4; vi++)
			{
				// TODO
				render->FillVertex(vertex[vi]);
			}
			BatchVertex *b_vertex = it.get_ptr()->getBatchVertex();
			for (int vi = 0; vi < 4; vi++)
			{
				// TODO
				render->FillBatchVertex(b_vertex[vi]);
			}
			// 4 个顶点有 6个索引,
			uint * index = it.get_ptr()->getIndices();
			for (int ii = 0; ii < 6; ii++)
			{
				uint idx = index[ii] + ivertexTotal;
				// TODO
				render->FillIndex(idx);
				iindexTotal++;
			}
			ivertexTotal += 4;
		}
		GetRenderSystem()->UseShareContext();
		render->FillEnd(ID, (DWORD)ivertexTotal, iindexTotal);
	}
	
	void ParticleGroup::TransformChange()
	{
		if (m_pRenderer == nullptr)
		{
			return;
		}
		m_pRenderer->SetTransform(Transform);	
		GameObject::TransformChange();
	}

	void ParticleGroup::UpdateParticles(float deltaTime)
	{
		if (static_cast<BatchRenderer*>(m_pRenderer)->GetRendererBuffer(ID) == nullptr)
		{
			return;
		}

		m_ActiveParticles.sort([=] (share_pointer<Particle> &p1, share_pointer<Particle>&p2)
		{
			float dis1 = abs(m_pRenderer->pCamera->Transform->Position.Distance(p1.get_ptr()->Transform->Position));
			float dis2 = abs(m_pRenderer->pCamera->Transform->Position.Distance(p2.get_ptr()->Transform->Position));
			return dis1 > dis2;
		});

		vertexStartIndex = static_cast<BatchRenderer*>(m_pRenderer)->GetRendererBuffer(ID)->VertextStartIndex;
		for (particleListIterator it = m_ActiveParticles.begin(); it != m_ActiveParticles.end();)
		{
			share_pointer<Particle> particle = *it;
			if (particle.getReferenceCount() == 0)
			{
				it ++;
				continue;
			}
			Particle *p = particle.get_ptr();
			if (p == nullptr)
			{
				it ++;
				continue;
			}
			if(p->getLiveTime() > 0.0f)
			{
				it ++;
				continue;
			}
			for (auto & m_particleAffector : m_particleAffectors)
			{
				if (particle.get_ptr()->GroupID != m_particleAffector->particleGroupID)
				{
					continue;
				}
				m_particleAffector->AffectParticle(deltaTime, particle);
			}
			
			float livetime = p->getParticleTimeToLive() - deltaTime;
			
			if (livetime <= 0)
			{
				particle.get_ptr()->Transform->Position = FAR_AWAY;
				it = deleteParticle(it);
			}
			else
			{
				it ++;
			}
			p->Update(deltaTime);
			BatchVertex* vertex = particle.get_ptr()->getBatchVertex();

			static_cast<BatchRenderer*>(m_pRenderer)->mBatchVertex[vertexStartIndex + p->Index * 4 + 0] = vertex[0];
			static_cast<BatchRenderer*>(m_pRenderer)->mBatchVertex[vertexStartIndex + p->Index * 4 + 1] = vertex[1];
			static_cast<BatchRenderer*>(m_pRenderer)->mBatchVertex[vertexStartIndex + p->Index * 4 + 2] = vertex[2];
			static_cast<BatchRenderer*>(m_pRenderer)->mBatchVertex[vertexStartIndex + p->Index * 4 + 3] = vertex[3];
		}
	}
	
	void ParticleGroup::checkParticleState(float deltaTime)
	{
		if (m_pRenderer->pCamera == nullptr)
		{
			return;
		}
		for (auto & m_particleEmitter : m_particleEmitters)
		{
			m_particleEmitter->SetEmitterPosition(Transform->GetPosition());
			m_particleEmitter->Update(deltaTime);
		}		
		
		m_ActiveParticles.sort([=](share_pointer<Particle> &p1, share_pointer<Particle>&p2)
		{
			float dis1 = abs(m_pRenderer->pCamera->Transform->Position.Distance(p1.get_ptr()->Transform->Position));
			float dis2 = abs(m_pRenderer->pCamera->Transform->Position.Distance(p2.get_ptr()->Transform->Position));
			return dis1 > dis2;
		});

		vertexStartIndex = static_cast<BatchRenderer*>(m_pRenderer)->GetRendererBuffer(ID)->VertextStartIndex;
		//Debug::Log(ell_Error, "Particle Number is %d", m_ActiveParticles.size());
		for (particleListIterator itr = m_ActiveParticles.begin(); itr != m_ActiveParticles.end();)
		{
			share_pointer<Particle> particle = *itr;
			if (particle.getReferenceCount() == 0)
			{
				itr ++;
				continue;
			}
			if (particle.get_ptr() == nullptr)
			{
				itr ++;
				continue;
			}
			for (auto & m_particleAffector : m_particleAffectors)
			{
				if (particle.get_ptr()->GroupID != m_particleAffector->particleGroupID)
				{
					continue;
				}
				m_particleAffector->AffectParticle(deltaTime, particle);
			}
			
			float livetime = particle.get_ptr()->getParticleTimeToLive() - deltaTime;
			
			if (livetime <= 0)
			{
				particle.get_ptr()->Transform->Position = FAR_AWAY;
				deleteParticle(itr);
			}
			else
			{
				itr ++;
			}
			particle.get_ptr()->Update(deltaTime);
			BatchVertex* vertex = particle.get_ptr()->getBatchVertex();
			static_cast<BatchRenderer*>(m_pRenderer)->mBatchVertex[vertexStartIndex + particle.get_ptr()->Index * 4 + 0] = vertex[0];
			static_cast<BatchRenderer*>(m_pRenderer)->mBatchVertex[vertexStartIndex + particle.get_ptr()->Index * 4 + 1] = vertex[1];
			static_cast<BatchRenderer*>(m_pRenderer)->mBatchVertex[vertexStartIndex + particle.get_ptr()->Index * 4 + 2] = vertex[2];
			static_cast<BatchRenderer*>(m_pRenderer)->mBatchVertex[vertexStartIndex + particle.get_ptr()->Index * 4 + 3] = vertex[3];
		}
		if (m_ActiveParticles.empty() && !bIsEnable )
		{
			clearParticle2DeathPool();
			ParticleFactory::GetInstance().ResumParticleGroup(ConfigName, this);
		}
	}
	
	void ParticleGroup::SetEmitterEnable(bool isEnable)
	{
		bIsEnable = isEnable;
		for (auto & m_particleEmitter : m_particleEmitters)
		{
			m_particleEmitter->SetEnable(isEnable);
		}
		
		if (isEnable && !beGetFromLocalPool)
		{
			CreateParticle(initCfg.ParticleNumber, initCfg.Time2Live, initCfg.Pos, initCfg.Size, initCfg.Color, initCfg.GroupID, initCfg.BornEmitterPos, initCfg.IsFirstCreate);
		}
		
		if (isEnable && !IsActive)
		{
			SetActive(true);
		}
	}
	
	void ParticleGroup::SetPause(bool bPause)
	{
		m_bIsPause = bPause;
	}

	particleListIterator ParticleGroup::deleteParticle(particleListIterator& pItr)
	{
		share_pointer<Particle> particle = *pItr;
		if (m_ActiveParticles.size() == 0)
		{
			return pItr;
		}
		if (particle.getReferenceCount() == 0)
		{
			return ++pItr;
		}
		m_DethParticles.emplace_back(particle);
		particle.get_ptr()->ReSet();
		particle.get_ptr()->SetColor(m_nColor);
		for (auto & m_particleEmitter : m_particleEmitters)
		{
			if(m_particleEmitter->m_CurrentParticleNumer > 0)
				m_particleEmitter->m_CurrentParticleNumer--;
		}
		pItr = m_ActiveParticles.erase(pItr);
		return pItr;
	}
	
	void ParticleGroup::AfterUpdate(float deltaTime)
	{
		if (m_bIsPause)
		{
			return;
		}
		if (IsActive && !m_ParticlePool.empty())
		{
			checkParticleState(deltaTime);

			GetRenderSystem()->UseShareContext();
			m_pRenderer->TransformChange();

			m_pRenderer->GetMaterial()->mShader->UpdateFloatValue("particleSize", pSzie.x);
		}
	}
	
	void ParticleGroup::SetParticleDir(vec3f TouchPos, vec3f EmitterPos)
	{
		if(m_isLock > 0)
		{
			vec3f d = vec3f(TouchPos.x, 0.0, 0.0);
			vec3f up = vec3f(0.0f, EmitterPos.y*m_DirOffset, 0.0f);
			m_ParticleDir = d - up;
		}
		else
		{
			//printf("TouchPos x:%f,y:%f   EmitterPos x:%f,y:%f\n",TouchPos.x,TouchPos.y,EmitterPos.x,EmitterPos.y);
			if(EmitterPos != TouchPos)
				m_ParticleDir = EmitterPos - TouchPos;
			else
				m_ParticleDir = vec3f(0.0f,0.0f,0.0f);
		}
		m_EmitterPositon = EmitterPos;
		m_ParticleDir.normalize();
	}
	
	void ParticleGroup::ParseUVConfig(std::string uvStr)
	{
		std::vector<std::string> uvVec = StringBuilder::Split(uvStr, ",");
		for (auto & i : uvVec)
		{
			std::string uvID = StringBuilder::Trim(i);
			if (uvID == "")
			{
				continue;
			}
			int id = Convert::ToInt(uvID);
			
			TextureUVConfig * config = TableRegister::GetTableManager(ConfigName.c_str())->Select<TextureUVConfig>(id);
			if (config == nullptr)
			{
				continue;
			}

			m_vecUVConfigs.push_back(config);
		}
	}
	
	void ParticleGroup::SetDontDestory(bool dontDestory)
	{
		DontDestoryOnLoad = dontDestory;
	}
	
	void ParticleGroup::createParticleFromPool(int createParticleNumber, Vector2 &size, float color, vec3f pos, float time2Live, uint groupID, vec3f bornEmitterPos, bool isFirstCreate)
	{
		m_particleIndex = 0;
		for (int i = 0; i < createParticleNumber; i++)
		{
			size_t num = m_vecUVConfigs.size();
			int _n = rand() % num;
			
			share_pointer<Particle> sp_P = ParticlePool::GetInstance().ActiveParticleFromPool();
			TextureUVConfig * cfg = m_vecUVConfigs[_n];
			sp_P.get_ptr()->ActiveEffect(cfg);
			sp_P.get_ptr()->Create(size.x, size.y, true, color, pos, fShaderIndex, time2Live);
			sp_P.get_ptr()->autoRelease();
			sp_P.get_ptr()->GroupID = groupID;
			sp_P.get_ptr()->Index = m_particleIndex;
			sp_P.get_ptr()->SetColor(m_nColor);
			sp_P.get_ptr()->SetMoveSpeed(0);
			sp_P.get_ptr()->setAlpha(color);
			sp_P.get_ptr()->SetBornEmitterPos(bornEmitterPos);
			m_ParticlePool.emplace_back(sp_P);
			if (isFirstCreate)
			{
				/// 创建足够的粒子放入死亡列表里
				m_DethParticles.emplace_back(sp_P);
			}
			else
			{
				m_ActiveParticles.emplace_back(sp_P);
			}
			m_particleIndex++;
		}
		beGetFromLocalPool = true;
	}
	
	void ParticleGroup::createParticleFromDethPool(unsigned int particleNumber, float time2Live, vec3f pos, Vector2 size, float color, uint groupID, vec3f bornEmitterPos, int createParticleNumber)
	{
		if (m_ParticlePool.size() >= m_uiMaxParticleNumber)
		{
			int particleCreatedNumber = 0;
			// 从死亡粒子池取粒子
			for (int i = 0; i < particleNumber; i++)
			{
				activeParticleFromDeath(particleNumber, time2Live, pos, size, color, groupID, bornEmitterPos);
			}
		}
	}
	
	
	particleInitConfig::particleInitConfig(unsigned int particleNumber, float time2Live, vec3f pos, Vector2 size, float color, uint groupID, vec3f bornEmitterPos, bool isFirstCreate /*= false*/)
	{
		ParticleNumber = particleNumber;
		Time2Live = time2Live;
		Pos = pos;
		Size = size;
		Color = color;
		GroupID = groupID;
		BornEmitterPos = bornEmitterPos;
		IsFirstCreate = isFirstCreate;
	}

}

#include "E3DGLMeshRender.h"

namespace E3DEngine
{
	GL_MeshRender::GL_MeshRender()
	{
		pRender = new GL_Renderer;
		pRender->mName = mName;
	}

	GL_MeshRender::~GL_MeshRender()
	{
		SAFE_DELETE(pRender);
	}

	void GL_MeshRender::FillBegin(UINT objId)
	{
		pRender->FillBegin(objId);
	}

	/*void MeshRender::initBoneMatrix()
	{
	VecBoneMatrix.resize( m_pMeshBuffer->VecBoneMatrix.size());
	for (map<string, Bone*>::iterator it = m_pMeshBuffer->BoneMapping.begin(); it != m_pMeshBuffer->BoneMapping.end(); ++it)
	{
	Bone * bone = new Bone(it->second);
	BoneMapping[it->first] = bone;

	VecBoneMatrix[it->second->ID] = &bone->Transform->WorldMatrix;
	}
	}*/

	void GL_MeshRender::FillVertex(Vertex vb)
	{
		pRender->FillVertex(vb);
	}


	void GL_MeshRender::FillIndex(uint ib)
	{
		pRender->FillIndex(ib);
	}


	void GL_MeshRender::FillEnd(UINT objId, uint vertexCount, uint indexCount)
	{
		pRender->FillEnd(objId, vertexCount, indexCount);
	}

	void GL_MeshRender::RemoveInRenderer(UINT objId)
	{
		pRender->RemoveInRenderer(objId);
	}


	E3DEngine::RendererBuffer* GL_MeshRender::GetRendererBuffer(UINT objID)
	{
		return pRender->GetRendererBuffer(objID);
	}


	void GL_MeshRender::TransformChange()
	{
		pRender->TransformChange();
	}


	void GL_MeshRender::Render()
	{
		pRender->Render();
	}

	void GL_MeshRender::SetDrawModule(DWORD module)
	{
		pRender->SetDrawModule(module);
	}


	UINT GL_MeshRender::GetDrawModule()
	{
		return pRender->GetDrawModule();
	}

	E3DEngine::CTransform * GL_MeshRender::GetTransform()
	{
		return pRender->GetTransform();
	}


	void GL_MeshRender::SetTransform(CTransform *_transform)
	{
		pRender->SetTransform(_transform);
	}


	void GL_MeshRender::CreateNewTransform()
	{
		pRender->CreateNewTransform();
	}

/*
	void GL_MeshRender::SetRenderIndex(DWORD index)
	{
		render->SetRenderIndex(index);
	}
*/

	void GL_MeshRender::SetMaterial(Material *material)
	{
		pRender->SetMaterial(material);
	}


	void GL_MeshRender::SetLayerMask(DWORD layer)
	{
		pRender->SetLayerMask(layer);
	}


	E3DEngine::Material * GL_MeshRender::GetMaterial()
	{
		return pRender->GetMaterial();
	}


	void GL_MeshRender::SetCamera(Camera * camera)
	{
		pRender->SetCamera(camera);
	}


	void GL_MeshRender::ClearVertexIndexBuffer()
	{
		pRender->ClearVertexIndexBuffer();
	}

	E3DEngine::Camera * GL_MeshRender::GetCamera()
	{
		return pRender->GetCamera();
	}

}

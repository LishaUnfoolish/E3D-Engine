//********************************** Effect 3D Engine **************************************************//
//********************* Copyright (c) 2017 PanQingyun. All rights reserved. ****************************//
#ifndef __FRAME_BUFFER_H__
#define __FRAME_BUFFER_H__

#include "../../../src/Utils/E3DUtil.hpp"
#include "../Include/Include.h"

namespace E3DEngine
{
	namespace GLESRenderSystem
	{
		class RenderTarget
		{
		public:
			virtual ~RenderTarget()
			{
				_GL_ES_2::DeleteRenderbuffers(1, &m_DepthBuffer);
			}
			GLuint	m_DepthBuffer;
			DWORD Type;
		};

		class RenderTexture : public RenderTarget
		{
		public:
			~RenderTexture() override;
			RenderTexture();
		public:
			GLuint	m_TextureBuffer;
		};

		class RenderBuffer : public RenderTarget
		{
		public:
			RenderBuffer();
			~RenderBuffer() override;
		public:
			GLuint	m_RenderBuffer;
		};

		class FrameBufferObject
		{
		public:
			FrameBufferObject();
			~FrameBufferObject();

		public:
			void Create(int width, int height, DWORD targetType);
			void Clear();
			void Bind();
			void SetClearColor(Color4 clearColor);
			void BindRenderBuffer();
			void BlitFrameBuffer(FrameBufferObject * dest, uint blitBuffer, uint filter = GL_NEAREST);

		public:
			GLuint GetTextureBufferID();
			GLuint GetFrameBufferID();
			GLuint GetRenderBufferID();
			GLuint GetDepthBufferID();
			GLbyte * GetPixels();
			GLint  GetReadBufferFormat();
			GLint  GetReadBufferType();
			int GetFrameBufferHeight();
			int GetFrameBufferWidth();
			uint GetTargetType();
		private:
			void createTarget(DWORD targetType);
		private:
			GLuint			m_FrameBuffer;
			RenderTarget  * m_renderTarget;
			int				m_FrameWidth;
			int				m_FrameHeight;
			Color4			m_ClearColor;
			GLint			m_BufferFormat;
			GLint			m_BufferType;
			GLbyte		  * m_BufferPixels;
			uint			m_TargetType;
		};
	}
}

#endif

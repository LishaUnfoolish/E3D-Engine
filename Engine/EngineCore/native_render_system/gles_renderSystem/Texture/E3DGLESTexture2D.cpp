﻿//
//  Texture.cpp
//
//  Created by 潘庆云 on 2017/3/30.
//
#include "E3DGLESTexture2D.hpp"
#include "../E3DGLESRenderSystem.hpp"

namespace E3DEngine
{

	GLES_Texture2D::~GLES_Texture2D()
	{
		if (m_nTextureBuffer != 0)
		{
			_GL_ES_2::DeleteTextures(1, &m_nTextureBuffer);
		}
	}

	void GLES_Texture2D::Create(Texture2dData *tData)
	{
		Texture2D::Create(tData);
		DWORD  rgbModule = 0;
		if (tData->rgbModule == PixelFormat::R8G8B8)
		{
			rgbModule = GL_RGB;
		}
		else if (tData->rgbModule == PixelFormat::R8G8B8A8)
		{
			rgbModule = GL_RGBA;
		}
		else if (tData->rgbModule == PixelFormat::L8A8)
		{
			rgbModule = GL_LUMINANCE_ALPHA;
		}
		else if (tData->rgbModule == PixelFormat::L8)
		{
			rgbModule = GL_LUMINANCE;
		}
		else
		{
			assert(false);
		}
		_GL_ES_2::GenTextures(1, &m_nTextureBuffer);
		_GL_ES_2::BindTexture(GL_TEXTURE_2D, m_nTextureBuffer);
		setTextureParam(tData);
		_GL_ES_2::TexImage2D(GL_TEXTURE_2D, 0, rgbModule, tData->width, tData->height, 0, rgbModule, GL_UNSIGNED_BYTE, tData->imgData);
		if (tData->useMipMap)
		{
			_GL_ES_2::GenerateMipmap(GL_TEXTURE_2D);
		}
		_GL_ES_2::BindTexture(GL_TEXTURE_2D, 0);
	}

	void GLES_Texture2D::setTextureParam(Texture2dData *tData)
	{
		unsigned int clampType = 0;
		unsigned int filterType = 0;
		switch (tData->clampType)
		{
		case CLAMP_TYPE::CLAMP_TO_EDGE:
			clampType = GL_CLAMP_TO_EDGE;
			break;
		case  CLAMP_TYPE::MIRRORED_REPEAT:
			clampType = GL_MIRRORED_REPEAT;
			break;
		case CLAMP_TYPE::REPEAT:
			clampType = GL_REPEAT;
			break;
		default:
			assert(false);
		}

		switch (tData->filterType)
		{
		case FILTER_TYPE::LINEAR:
			filterType = GL_LINEAR;
			break;
		case  FILTER_TYPE::NEAREST:
			filterType = GL_NEAREST;
			break;
		case FILTER_TYPE::LINEAR_MIPMAP_LINEAR:
			filterType = GL_LINEAR_MIPMAP_LINEAR;
			tData->useMipMap = true;
			break;
		case  FILTER_TYPE::LINEAR_MIPMAP_NEAREST:
			filterType = GL_LINEAR_MIPMAP_NEAREST;
			tData->useMipMap = true;
			break;
		case  FILTER_TYPE::NEAREST_MIPMAP_LINEAR:
			filterType = GL_NEAREST_MIPMAP_LINEAR;
			tData->useMipMap = true;
			break;
		case  FILTER_TYPE::NEAREST_MIPMAP_NEAREST:
			filterType = GL_NEAREST_MIPMAP_NEAREST;
			tData->useMipMap = true;
			break;
		default:
			assert(false);
		}

		_GL_ES_2::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterType);
		_GL_ES_2::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterType);
		_GL_ES_2::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clampType);
		_GL_ES_2::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clampType);
	}

	void GLES_Texture2D::SetTextureData(Texture2dData *tData)
	{
		Texture2D::SetTextureData(tData);
		_GL_ES_2::BindTexture(GL_TEXTURE_2D, m_nTextureBuffer);
		_GL_ES_2::TexImage2D(GL_TEXTURE_2D, 0, tData->rgbModule, tData->width, tData->height, 0, tData->rgbModule, GL_UNSIGNED_BYTE, tData->imgData);
		_GL_ES_2::BindTexture(GL_TEXTURE_2D, 0);
	}

	GLES_Texture2D::GLES_Texture2D()
	{
		m_nTextureBuffer = 0;
	}

	GLuint & GLES_Texture2D::GetTextureBuffer()
	{
		return m_nTextureBuffer;
	}

}

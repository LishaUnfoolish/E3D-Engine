﻿//
//  Shader.cpp
//
//  Created by 潘庆云 on 2017/3/30.
//

#include "E3DGLShader.hpp"
#include <src/Source/E3DDebug.h>
#include <src/RenderSystem/Material/E3DMaterial.hpp>
#include <src/Source/Helpers.h>
#include <src/Scene/E3DSceneManager.hpp>
#include "../E3DGL_RenderSystem.h"
#include <src/RenderSystem/Texture/E3DCubeMapTexture.hpp>
#include <src/RenderSystem/Texture/E3DRender2Texture.h>
#include <src/RenderSystem/Texture/E3DTexture2D.hpp>

#define NOT_USE_CONTINUE \
if (uniformKeyValue.second.UniformLocation == -1)\
{\
	continue;\
}

namespace E3DEngine
{
	using namespace GLRenderSystem;
	
	void GL_Shader::InitShaderVar()
	{
		uniformSetFunc["float"]		= &Shader::createFloat1Uniform;
		uniformSetFunc["float[]"]	= &Shader::createFloat1ArrayUniform;
		uniformSetFunc["vec2"]		= &Shader::createFloat2Uniform;
		uniformSetFunc["vec3"]		= &Shader::createFloat3Uniform;
		uniformSetFunc["vec4"]		= &Shader::createFloat4Uniform;
		uniformSetFunc["vec2[]"]	= &Shader::createFloat2ArrayUniform;
		uniformSetFunc["vec3[]"]	= &Shader::createFloat3ArrayUniform;
		uniformSetFunc["vec4[]"]	= &Shader::createFloat4ArrayUniform;
		uniformSetFunc["mat2"]		= &Shader::createMatrix2Uniform;
		uniformSetFunc["mat3"]		= &Shader::createMatrix3Uniform;
		uniformSetFunc["mat4"]		= &Shader::createMatrix4Uniform;
		uniformSetFunc["int"]		= &Shader::createInt1Uniform;
		uniformSetFunc["sampler2D"]			= &Shader::createSampler2DUniform;
		uniformSetFunc["sampler2DShadow"]	= &Shader::createSampler2DUniform;
		uniformSetFunc["samplerCube"]		= &Shader::createSamplerCubeUniform;
	}

	void GL_Shader::LoadShader(const char *vertexShader, const char *fragmentShader)
	{
		ShaderProgram = glCreateProgram();
		GLuint &&vertexShaderHandle = compileShader(vertexShader, GL_VERTEX_SHADER);
		GLuint &&fragmentShaderHandle = compileShader(fragmentShader, GL_FRAGMENT_SHADER);

		glAttachShader(ShaderProgram, vertexShaderHandle);
		glAttachShader(ShaderProgram, fragmentShaderHandle);
		glLinkProgram(ShaderProgram);
		GLint linkSuccess;
		glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &linkSuccess);
		if (linkSuccess != GL_TRUE)
		{
			GLint logLength;
			glGetShaderiv(ShaderProgram, GL_INFO_LOG_LENGTH, &logLength);
			if (logLength > 0)
			{
				GLchar *log = (GLchar *)malloc(logLength);
				glGetShaderInfoLog(ShaderProgram, logLength, &logLength, log);
				Debug::Log(ell_Error, "Shader: %s LINK ERROR shader link log\n: %s", mFileName.c_str(), log);
			}
			glDeleteShader(ShaderProgram);
		}
		
		if (vertexShaderHandle != 0)
		{
			glDeleteShader(vertexShaderHandle);
		}
		if (fragmentShaderHandle != 0)
		{
			glDeleteShader(fragmentShaderHandle);
		}
		loadUniformLocation();
		loadAttribLocation(STATIC_VERTEX);
		loadAttribLocation(DYNAMIC_VERTEX);
	}


	GLuint GL_Shader::compileShader(const char* shaderContent, GLenum shaderType)
	{
		if (shaderContent == nullptr)
		{
			return 0;
		}
		int shaderStringLength = (int)strlen(shaderContent);
		GLuint shaderHandle = glCreateShader(shaderType);
		glShaderSource(shaderHandle, 1, &shaderContent, &shaderStringLength);
		glCompileShader(shaderHandle);
		
		GLint status;
		glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE)
		{
			GLint logLength;
			glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &logLength);
			if (logLength > 0)
			{
				GLchar *log = (GLchar *)malloc(logLength);
				glGetShaderInfoLog(shaderHandle, logLength, &logLength, log);
				Debug::Log(ell_Error, "Shader %s compile error\n%s\nShaderContent:\n %s", mFileName.c_str(), log, shaderContent);
			}
			glDeleteShader(shaderHandle);
		}

		return shaderHandle;
	}


	void GL_Shader::compileShaders()
	{

	}

	void GL_Shader::UseProgram()
	{
		glUseProgram(ShaderProgram);
	}

	void GL_Shader::UseNullProgram()
	{
		glUseProgram(NULL_SHADER);
	}

	void GL_Shader::UpdateProgramUniformValue()
	{		
		for (auto & uniformKeyValue : int1UniformList)
		{
			NOT_USE_CONTINUE
			glUniform1i(uniformKeyValue.second.UniformLocation, uniformKeyValue.second.Value);
		}

		for (auto & uniformKeyValue : float1UniformList)
		{
			NOT_USE_CONTINUE
			glUniform1f(uniformKeyValue.second.UniformLocation, uniformKeyValue.second.Value);
		}

		for (auto & uniformKeyValue : float2UniformList)
		{
			NOT_USE_CONTINUE
			glUniform2f(uniformKeyValue.second.UniformLocation, uniformKeyValue.second.Value1, uniformKeyValue.second.Value2);
		}

		for (auto & uniformKeyValue : float1UniformArrayList)
		{
			if (uniformKeyValue.second.UniformLocation == -1 || uniformKeyValue.second.Value.empty())
			{
				continue;
			}
			glUniform1fv(uniformKeyValue.second.UniformLocation, uniformKeyValue.second.Count, uniformKeyValue.second.Value.data());
		}

		for (auto & uniformKeyValue : float2UniformArrayList)
		{
			if (uniformKeyValue.second.UniformLocation == -1 || uniformKeyValue.second.Value.empty())
			{
				continue;
			}
			glUniform2fv(uniformKeyValue.second.UniformLocation,uniformKeyValue.second.Count,uniformKeyValue.second.Value.data());
		}

		for (auto & uniformKeyValue : float3UniformArrayList)
		{
			if (uniformKeyValue.second.UniformLocation == -1 || uniformKeyValue.second.Value.empty())
			{
				continue;
			}
			glUniform3fv(uniformKeyValue.second.UniformLocation, uniformKeyValue.second.Count, uniformKeyValue.second.Value.data());
		}

		for (auto & uniformKeyValue : float4UniformArrayList)
		{
			if (uniformKeyValue.second.UniformLocation == -1 || uniformKeyValue.second.Value.empty())
			{
				continue;
			}
			glUniform4fv(uniformKeyValue.second.UniformLocation, uniformKeyValue.second.Count, uniformKeyValue.second.Value.data());
		}

		for (auto & uniformKeyValue : float3UniformList)
		{
			NOT_USE_CONTINUE
			glUniform3f(uniformKeyValue.second.UniformLocation, uniformKeyValue.second.Value1, uniformKeyValue.second.Value2, uniformKeyValue.second.Value3);
		}

		for (auto & uniformKeyValue : float4UniformList)
		{
			NOT_USE_CONTINUE
			glUniform4f(uniformKeyValue.second.UniformLocation, uniformKeyValue.second.Value1, uniformKeyValue.second.Value2, uniformKeyValue.second.Value3, uniformKeyValue.second.Value4);
		}

		for (auto & uniformKeyValue : matrix4UniformList)
		{
			NOT_USE_CONTINUE
			if (uniformKeyValue.second.Data == nullptr)
			{
				continue;
			}
			glUniformMatrix4fv(uniformKeyValue.second.UniformLocation, uniformKeyValue.second.Count, uniformKeyValue.second.Transpos, uniformKeyValue.second.Data);
		}

		for (auto & uniformKeyValue : matrix3UniformList)
		{
			NOT_USE_CONTINUE
			if (uniformKeyValue.second.Data == nullptr)
			{
				continue;
			}
			glUniformMatrix3fv(uniformKeyValue.second.UniformLocation, uniformKeyValue.second.Count, uniformKeyValue.second.Transpos, uniformKeyValue.second.Data);
		}

		for (auto & uniformKeyValue : matrix2UniformList)
		{
			NOT_USE_CONTINUE
			if (uniformKeyValue.second.Data == nullptr)
			{
				continue;
			}
			glUniformMatrix2fv(uniformKeyValue.second.UniformLocation, uniformKeyValue.second.Count, uniformKeyValue.second.Transpos, uniformKeyValue.second.Data);
		}

		for (auto & uniformKeyValue : sampler2DUniformList)
		{
			NOT_USE_CONTINUE
			if (uniformKeyValue.second.texture == nullptr)
			{
				continue;
			}
			glActiveTexture(GL_TEXTURE0 + uniformKeyValue.second.TextureIndex);
			glBindTexture(GL_TEXTURE_2D, uniformKeyValue.second.texture->GetTextureBuffer());
			glUniform1i(uniformKeyValue.second.UniformLocation, uniformKeyValue.second.TextureIndex);
		}

		for (auto & uniformKeyValue : samplerCubeUniformList)
		{
			NOT_USE_CONTINUE
			if (uniformKeyValue.second.texture == nullptr)
			{
				continue;
			}
			glActiveTexture(GL_TEXTURE0 + uniformKeyValue.second.TextureIndex);
			glBindTexture(GL_TEXTURE_CUBE_MAP, uniformKeyValue.second.texture->GetTextureBuffer());
			glUniform1i(uniformKeyValue.second.UniformLocation, uniformKeyValue.second.TextureIndex);
		}
	}

	void GL_Shader::UpdateAttribPointerValue(UINT vertexType)
	{
		EnableVertexAttribArray(vertexType);
		std::vector<Attribute> *attrList = nullptr;
		if (vertexType == STATIC_VERTEX)
		{
			attrList = &staticAttributeList;
		}
		else
		{
			attrList = &dynamicAttributeList;
		}

		for (auto & attrib : *attrList)
		{
			glVertexAttribPointer(attrib.AttributeLoaction, attrib.AttributeSize, attrib.VarType, attrib.Normalized, attrib.VertexStructSize, (GLfloat*)nullptr + attrib.StartPosition);
		}

	}

	void GL_Shader::DeleteShader()
	{
		glDeleteShader(ShaderProgram);
	}

	void GL_Shader::EnableVertexAttribArray(UINT vertexType)
	{
		std::vector<Attribute> *attrList = nullptr;
		if (vertexType == STATIC_VERTEX)
		{
			attrList = &staticAttributeList;
		}
		else
		{
			attrList = &dynamicAttributeList;
		}

		for (auto & attrib : *attrList)
		{
			glEnableVertexAttribArray(attrib.AttributeLoaction);
		}
		
	}


	void GL_Shader::DisableVertexAttribArray(UINT vertexType)
	{
		std::vector<Attribute> *attrList = nullptr;
		if (vertexType == STATIC_VERTEX)
		{
			attrList = &staticAttributeList;
		}
		else
		{
			attrList = &dynamicAttributeList;
		}

		for (auto & attrib : *attrList)
		{
			glDisableVertexAttribArray(attrib.AttributeLoaction);
		}
	}

	void GL_Shader::bindAttribLoaction(UINT vertexType)
	{
		std::vector<Attribute> *attrList = nullptr;
		if (vertexType == STATIC_VERTEX)
		{
			attrList = &staticAttributeList;
		}
		else
		{
			attrList = &dynamicAttributeList;
		}

		for (auto & attrib : *attrList)
		{
			glBindAttribLocation(ShaderProgram, attrib.BindLocation, attrib.VarName.c_str());
		}
	}

	void GL_Shader::loadAttribLocation(UINT vertexType)
	{
		std::vector<Attribute> *attrList = nullptr;
		if (vertexType == STATIC_VERTEX)
		{
			attrList = &staticAttributeList;
		}
		else
		{
			attrList = &dynamicAttributeList;
		}

		for (auto & attrib : *attrList)
		{
			attrib.AttributeLoaction = LoadSelfDefAttribuate(attrib.VarName);
		}
	}

	void GL_Shader::loadUniformLocation()
	{
		for (auto & uniformKeyValue : int1UniformList)
		{
			uniformKeyValue.second.UniformLocation = LoadSelfDefUniform(uniformKeyValue.first);
		}

		for (auto & uniformKeyValue : float1UniformList)
		{
			uniformKeyValue.second.UniformLocation = LoadSelfDefUniform(uniformKeyValue.first);
		}

		for (auto & uniformKeyValue : float2UniformList)
		{
			uniformKeyValue.second.UniformLocation = LoadSelfDefUniform(uniformKeyValue.first);
		}

		for (auto & uniformKeyValue : float3UniformList)
		{
			uniformKeyValue.second.UniformLocation = LoadSelfDefUniform(uniformKeyValue.first);
		}

		for (auto & uniformKeyValue : float4UniformList)
		{
			uniformKeyValue.second.UniformLocation = LoadSelfDefUniform(uniformKeyValue.first);
		}

		for (auto & uniformKeyValue : matrix2UniformList)
		{
			uniformKeyValue.second.UniformLocation = LoadSelfDefUniform(uniformKeyValue.first);
		}

		for (auto & uniformKeyValue : matrix3UniformList)
		{
			uniformKeyValue.second.UniformLocation = LoadSelfDefUniform(uniformKeyValue.first);
		}

		for (auto & uniformKeyValue : matrix4UniformList)
		{
			uniformKeyValue.second.UniformLocation = LoadSelfDefUniform(uniformKeyValue.first);
		}

		for (auto & uniformKeyValue : float1UniformArrayList)
		{
			uniformKeyValue.second.UniformLocation = LoadSelfDefUniform(uniformKeyValue.first);
		}

		for (auto & uniformKeyValue : float2UniformArrayList)
		{
			uniformKeyValue.second.UniformLocation = LoadSelfDefUniform(uniformKeyValue.first);
		}

		for (auto & uniformKeyValue : float3UniformArrayList)
		{
			uniformKeyValue.second.UniformLocation = LoadSelfDefUniform(uniformKeyValue.first);
		}

		for (auto & uniformKeyValue : float4UniformArrayList)
		{
			uniformKeyValue.second.UniformLocation = LoadSelfDefUniform(uniformKeyValue.first);
		}

		for (auto & uniformKeyValue : sampler2DUniformList)
		{
			uniformKeyValue.second.UniformLocation = LoadSelfDefUniform(uniformKeyValue.first);
		}

		for (auto & uniformKeyValue : samplerCubeUniformList)
		{
			uniformKeyValue.second.UniformLocation = LoadSelfDefUniform(uniformKeyValue.first);
		}
	}


	GL_Shader::~GL_Shader()
	{
		DeleteShader();
		float1UniformList.clear();
		float2UniformList.clear();
		float3UniformList.clear();
		float4UniformList.clear();
		matrix4UniformList.clear();
		matrix3UniformList.clear();
		matrix2UniformList.clear();
		int1UniformList.clear();
	}
	

	GLint GL_Shader::LoadSelfDefUniform(std::string name)
	{
		GLint location = glGetUniformLocation(ShaderProgram, name.c_str());
		uniformLocationMap[name] = location;
		return location;
	}

	GLuint GL_Shader::LoadSelfDefAttribuate(std::string name)
	{
		GLuint attr = glGetAttribLocation(ShaderProgram, name.c_str());
		return attr;
	}

	void GL_Shader::AddAttriList(Attribute attri, bool isStatic)
	{
		if (isStatic)
		{
			staticAttributeList.push_back(attri);
		}
		else
		{
			dynamicAttributeList.push_back(attri);
		}
	}

}

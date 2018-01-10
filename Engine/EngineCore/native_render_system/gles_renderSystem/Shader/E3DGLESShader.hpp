﻿//
//  Shader.hpp
//
//  Created by 潘庆云 on 2017/3/30.
//

#ifndef GLES_Shader_hpp
#define GLES_Shader_hpp

#include <src/RenderSystem/Shader/E3DShader.hpp>
#include "../Include/Include.h"

#define IF_AVAILABLE(value) if((int)value != -1)

namespace E3DEngine
{
	class GLES_Shader : public Shader
	{
	public:
		GLES_Shader()
		{

		}
		virtual ~GLES_Shader();
	public:
		GLuint			ShaderProgram;

	public:
		virtual GLint	LoadSelfDefUniform(std::string name);
		virtual GLuint	LoadSelfDefAttribuate(std::string name);

		virtual void	UpdateProgramUniformValue();
		virtual void	LoadShader(const char* vertexShader, const char * fragmentShader);
		virtual void	UseProgram();
		virtual void	UseNullProgram();
		virtual void    SetProgramUniform(){ }
		virtual void	DeleteShader();
		virtual void	UpdateAttribPointerValue();
		virtual void	EnableVertexAttribArray();
	protected:
		virtual GLuint	compileShader(const char*  shaderName, GLenum shaderType);
		virtual void	compileShaders();
		virtual void	loadAttribLocation();
		virtual void	loadUniformLocation();
		virtual void	bindAttribLoaction();
	};
}

#endif /* Shader_hpp */
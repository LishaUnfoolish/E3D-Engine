﻿//
//  Material.hpp
//
//  Created by 潘庆云 on 2017/3/30.
//

#ifndef _METRAIL_HPP_
#define _METRAIL_HPP_

#include "../Shader/E3DShader.hpp"
#include "../Texture/E3DTexture.hpp"
#include "../Texture/E3DCubeMapTexture.hpp"
#include "src/Config/Table.h"
#include "src/Source/E3DDebug.h"

namespace E3DEngine
{
    enum eBlendType
	{
        eBlendType_default = 0,
        eBlendType_One = 1,
        eBlendType_Text = 2,
		eBlendType_ONE_MINUS_SRC = 3,
		eBlendType_NONE = 4,

    };
	
	class Material extends Object
	{
	public:
		Material(); 
		// -----------------------------------------------
		// 渲染调用开始之前，使用材质
		//-----------------------------------------------
		virtual void UseMaterial();

		// -----------------------------------------------
		// 是材质失效
		//-----------------------------------------------
		virtual void InvalidMaterial();

		// -----------------------------------------------
		// 清理
		//-----------------------------------------------
		virtual void Destory();

		// -----------------------------------------------
		// 创建材质
		// @param 材质配置
		// @param Shader配置
		//-----------------------------------------------
		virtual void CreateMaterial(MaterialConfig * config, ShaderConfig * sCfg);

		// -----------------------------------------------
		// 给材质设置纹理
		// @param 纹理
		// @param 纹理索引
		//-----------------------------------------------
		virtual void SetTexture(Texture * texture, int index);

		// -----------------------------------------------
		// 绑定纹理
		//-----------------------------------------------
		virtual void BindTexture();

		// -----------------------------------------------
		// 启用Shader
		//-----------------------------------------------
		virtual void UseProgram() { }

		// -----------------------------------------------
		// 使用ID是0的shader 即不使用任何shader
		//-----------------------------------------------
		virtual void UseNullProgram() { }

		virtual void SetEnableDepthWrite(bool bEnable) { enablewriteDepth = bEnable; }
	public:

		// -----------------------------------------------
		// 创建Shader 
		// @param shader配置
		//-----------------------------------------------
		virtual void CreateShader(ShaderConfig *cfg) { }

		// -----------------------------------------------
		// 创建环境贴图
		// @param 上下左右前后的贴图文件名
		//-----------------------------------------------
		virtual void CreateCubeTexture( std::string dirPath,std::string xPName,
									   std::string xNName,
									   std::string yPName,std::string yNName,std::string zPName,std::string ZNName);

		// -----------------------------------------------
		// 创建Mono对象
		//-----------------------------------------------
		MonoBehaviour * GetBehaviour();

	protected:
		virtual void parseShaderConfig(ShaderConfig *cfg);
		virtual void enableStencil();
		virtual void beforeUpdate();
		virtual void afterUpdate();
		virtual void createTexture(TextureData& data);
		virtual void createTexture(Texture *texture, std::string textureUniform);

	public:
		long		Color;			// 颜色
		Shader *	pShader;		// shader
		float       alpha;
		vec4f		AmbientColor;
		vec4f		SpecularColor;
		vec4f		DiffuseColor;
		float		Shininess;		// 光泽度 镜面反光强度
		int			nFaceIndex;
		bool        enableDepthTest; //开启深度测试
		bool        enablewriteDepth;
		bool        enableStencilTest; //开启模版测试
		bool        enableDoubleSide;
		bool        turnOnBlend;
		eBlendType  blendType;
		TableManager * MaterialTableManager;
		std::map<UINT, Texture*>	Textures; // 纹理
		MaterialConfig	  *	pMaterialConfig;
		std::string filePath;

		
	};
}

#endif /* Material_hpp */

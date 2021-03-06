﻿
//********************************** Effective 3D Engine **************************************************//
//******************* Copyright (c)  2017-2-7 PanQingyun. All rights reserved. *************************//

#ifndef Table_hpp
#define Table_hpp

#include <stdio.h>
#include "TableBase.h"
#include "../Source/ClassFactory.h"
#include "TableManager.h"

namespace E3DEngine
{
	using namespace std;
	void RegisterTableCreateInstance();
	//球形发射器
	class E3D_EXPORT_DLL BallEmitterConfig : public TableBase
	{
		DECLARE_CLASS(BallEmitterConfig)
	public:
		BallEmitterConfig() : CreateParticleOnce(1), LiveTimeRandom(0), BornInterval(0)
		{
			Name = "BallEmitter";
		}
		DECLARE_MEMBER(BallEmitterConfig, int, ID);
		DECLARE_MEMBER(BallEmitterConfig, float, Radius);
		DECLARE_MEMBER(BallEmitterConfig, float, ParticleWidth);
		DECLARE_MEMBER(BallEmitterConfig, float, ParticleHeight);
		DECLARE_MEMBER(BallEmitterConfig, float, BlankTime);
		DECLARE_MEMBER(BallEmitterConfig, int, MaxParticleNumber);
		DECLARE_MEMBER(BallEmitterConfig, int, TotalParticleNumber);
		DECLARE_MEMBER(BallEmitterConfig, float, ParticleLiveTime);
		DECLARE_MEMBER(BallEmitterConfig, float, LiveTimeRandom);
		DECLARE_MEMBER(BallEmitterConfig, int, IsLoop);
		DECLARE_MEMBER(BallEmitterConfig, int, IsBillboard);
		DECLARE_MEMBER(BallEmitterConfig, int, CreateParticleOnce);
		DECLARE_MEMBER(BallEmitterConfig, float, BornInterval);
		DECLARE_MEMBER(BallEmitterConfig, string, ParticlePosition);
		DECLARE_MEMBER(BallEmitterConfig, string, ScaleRange);


		virtual void registProperty() override
		{
			SAVE_MEMBER(ID, int);
			SAVE_MEMBER(IsLoop, int);
			SAVE_MEMBER(Radius, float);
			SAVE_MEMBER(ParticleWidth, float);
			SAVE_MEMBER(ParticleHeight, float);
			SAVE_MEMBER(BlankTime, float);
			SAVE_MEMBER(MaxParticleNumber, int);
			SAVE_MEMBER(TotalParticleNumber, int);
			SAVE_MEMBER(CreateParticleOnce, int);
			SAVE_MEMBER(ParticleLiveTime, float);
			SAVE_MEMBER(BornInterval, float);
			SAVE_MEMBER(LiveTimeRandom, float);
			SAVE_MEMBER(IsBillboard, int);
			SAVE_MEMBER(ParticlePosition, string);
			SAVE_MEMBER(ScaleRange, string);
		}
	};

	// 点发射器
	class E3D_EXPORT_DLL PointEmitterConfig : public TableBase
	{
		DECLARE_CLASS(PointEmitterConfig)
	public:
		PointEmitterConfig() : BornInterval(0)
		{
			Name = "PointEmitter";
		}
		DECLARE_MEMBER(PointEmitterConfig, int, ID);
		DECLARE_MEMBER(PointEmitterConfig, float, ParticleWidth);
		DECLARE_MEMBER(PointEmitterConfig, float, ParticleHeight);
		DECLARE_MEMBER(PointEmitterConfig, float, BlankTime);
		DECLARE_MEMBER(PointEmitterConfig, int, MaxParticleNumber);
		DECLARE_MEMBER(PointEmitterConfig, int, TotalParticleNumber);
		DECLARE_MEMBER(PointEmitterConfig, float, ParticleLiveTime);
		DECLARE_MEMBER(PointEmitterConfig, int, CreateParticleOnce);
		DECLARE_MEMBER(PointEmitterConfig, int, IsLoop);
		DECLARE_MEMBER(PointEmitterConfig, string, ParticlePosition);
		DECLARE_MEMBER(PointEmitterConfig, int, IsBillboard);
		DECLARE_MEMBER(PointEmitterConfig, float, BornInterval);

		virtual void registProperty() override
		{
			SAVE_MEMBER(ID, int);
			SAVE_MEMBER(ParticleWidth, float);
			SAVE_MEMBER(ParticleHeight, float);
			SAVE_MEMBER(BlankTime, float);
			SAVE_MEMBER(MaxParticleNumber, int);
			SAVE_MEMBER(TotalParticleNumber, int);
			SAVE_MEMBER(ParticleLiveTime, float);
			SAVE_MEMBER(ParticlePosition, string);
			SAVE_MEMBER(CreateParticleOnce, int);
			SAVE_MEMBER(IsLoop, int);
			SAVE_MEMBER(IsBillboard, int);
			SAVE_MEMBER(BornInterval, float);
		}
	};

	// 区域发射器
	class E3D_EXPORT_DLL AreaEmitterConfig : public TableBase
	{
		DECLARE_CLASS(AreaEmitterConfig)
	public:
		AreaEmitterConfig() : CreateParticleOnce(1), BornInterval(0)
		{
			Name = "AreaEmitter";
		}
		DECLARE_MEMBER(AreaEmitterConfig, int, ID);
		DECLARE_MEMBER(AreaEmitterConfig, float, ParticleWidth);
		DECLARE_MEMBER(AreaEmitterConfig, float, ParticleHeight);
		DECLARE_MEMBER(AreaEmitterConfig, float, BlankTime);
		DECLARE_MEMBER(AreaEmitterConfig, int, MaxParticleNumber);
		DECLARE_MEMBER(AreaEmitterConfig, int, TotalParticleNumber);
		DECLARE_MEMBER(AreaEmitterConfig, float, ParticleLiveTime);
		DECLARE_MEMBER(AreaEmitterConfig, int, IsLoop);
		DECLARE_MEMBER(AreaEmitterConfig, int, IsBillboard);
		DECLARE_MEMBER(AreaEmitterConfig, int, CreateParticleOnce);
		DECLARE_MEMBER(AreaEmitterConfig, string, ParticlePosition);
		DECLARE_MEMBER(AreaEmitterConfig, string, AreaRange);
		DECLARE_MEMBER(AreaEmitterConfig, string, ScaleRange);
		DECLARE_MEMBER(AreaEmitterConfig, string, AlphaRange);
		DECLARE_MEMBER(AreaEmitterConfig, float, BornInterval);

		virtual void registProperty() override
		{
			SAVE_MEMBER(ID, int);
			SAVE_MEMBER(ParticleWidth, float);
			SAVE_MEMBER(ParticleHeight, float);
			SAVE_MEMBER(BlankTime, float);
			SAVE_MEMBER(MaxParticleNumber, int);
			SAVE_MEMBER(TotalParticleNumber, int);
			SAVE_MEMBER(ParticleLiveTime, float);
			SAVE_MEMBER(IsLoop, int);
			SAVE_MEMBER(IsBillboard, int);
			SAVE_MEMBER(CreateParticleOnce, int);
			SAVE_MEMBER(ParticlePosition, string);
			SAVE_MEMBER(AreaRange, string);
			SAVE_MEMBER(ScaleRange, string);
			SAVE_MEMBER(AlphaRange, string);
			SAVE_MEMBER(BornInterval, float);
		}
	};

	// 线性外力影响器
	class E3D_EXPORT_DLL LinnerForceAffectorConfig : public TableBase
	{
		DECLARE_CLASS(LinnerForceAffectorConfig)
	public:
		LinnerForceAffectorConfig()
		{
			Name = "LinerForceAffector";
		}
		DECLARE_MEMBER(LinnerForceAffectorConfig, int, ID);
		DECLARE_MEMBER(LinnerForceAffectorConfig, float, Force);
		DECLARE_MEMBER(LinnerForceAffectorConfig, float, ForceRandomRange);
		DECLARE_MEMBER(LinnerForceAffectorConfig, string, Direction);

		virtual void registProperty() override
		{
			SAVE_MEMBER(ID, int);
			SAVE_MEMBER(Force, float);
			SAVE_MEMBER(ForceRandomRange, float);
			SAVE_MEMBER(Direction, string);
		}
	};

	// 随机外力影响器
	class E3D_EXPORT_DLL RandomForceAffectorConfig : public TableBase
	{
		DECLARE_CLASS(RandomForceAffectorConfig)
	public:
		RandomForceAffectorConfig() : StartTime(0), EndTime(0), rotateByDir(0), rotateByOrbit(0)
		{
			Name = "RandomForceAffector";
		}
		DECLARE_MEMBER(RandomForceAffectorConfig, int, ID);
		DECLARE_MEMBER(RandomForceAffectorConfig, float, StartTime);
		DECLARE_MEMBER(RandomForceAffectorConfig, float, EndTime);
		DECLARE_MEMBER(RandomForceAffectorConfig, float, Force);
		DECLARE_MEMBER(RandomForceAffectorConfig, string, xRandom);
		DECLARE_MEMBER(RandomForceAffectorConfig, string, yRandom);
		DECLARE_MEMBER(RandomForceAffectorConfig, string, zRandom);
		DECLARE_MEMBER(RandomForceAffectorConfig, string, ForceRange);
		DECLARE_MEMBER(RandomForceAffectorConfig, int, rotateByDir);
		DECLARE_MEMBER(RandomForceAffectorConfig, int, rotateByOrbit);

		virtual void registProperty() override
		{
			SAVE_MEMBER(ID, int);
			SAVE_MEMBER(Force, float);
			SAVE_MEMBER(StartTime, float);
			SAVE_MEMBER(EndTime, float);
			SAVE_MEMBER(xRandom, string);
			SAVE_MEMBER(yRandom, string);
			SAVE_MEMBER(zRandom, string);
			SAVE_MEMBER(ForceRange, string);
			SAVE_MEMBER(rotateByDir, int);
			SAVE_MEMBER(rotateByOrbit, int);
		}
	};

	class E3D_EXPORT_DLL RotationForceAffectorConfig : public TableBase
	{
		DECLARE_CLASS(RotationForceAffectorConfig)
	public:
		RotationForceAffectorConfig()
		{
			Name = "RotateForceAffector";
		}
		DECLARE_MEMBER(RotationForceAffectorConfig, int, ID);
		DECLARE_MEMBER(RotationForceAffectorConfig, float, Force);
		DECLARE_MEMBER(RotationForceAffectorConfig, float, ForceRandomRange);
		DECLARE_MEMBER(RotationForceAffectorConfig, string, axis);

		virtual void registProperty() override
		{
			SAVE_MEMBER(ID, int);
			SAVE_MEMBER(Force, float);
			SAVE_MEMBER(ForceRandomRange, float);
			SAVE_MEMBER(axis, string);
		}
	};

	// 移动到发射点影响器
	class E3D_EXPORT_DLL MoveToFiringPointForceAffectorConfig : public TableBase
	{
		DECLARE_CLASS(MoveToFiringPointForceAffectorConfig)
	public:
		MoveToFiringPointForceAffectorConfig() : RotateWithEmitterAngle(0), ForceRandomRange(0), Force(0)
		{
			Name = "MoveToFiringPointForceAffector";
		}
		DECLARE_MEMBER(MoveToFiringPointForceAffectorConfig, int, ID);
		DECLARE_MEMBER(MoveToFiringPointForceAffectorConfig, float, Force);
		DECLARE_MEMBER(MoveToFiringPointForceAffectorConfig, float, ForceRandomRange);
		DECLARE_MEMBER(MoveToFiringPointForceAffectorConfig, float, RotateWithEmitterAngle);

		virtual void registProperty() override
		{
			SAVE_MEMBER(ID, int);
			SAVE_MEMBER(Force, float);
			SAVE_MEMBER(ForceRandomRange, float);
			SAVE_MEMBER(RotateWithEmitterAngle, float);
		}
	};

	// 线性颜色变化影响器
	class E3D_EXPORT_DLL LinnerColorAffectorConfig : public TableBase
	{
		DECLARE_CLASS(LinnerColorAffectorConfig)
	public:
		LinnerColorAffectorConfig()
		{
			Name = "LinnerColorAffector";
		}
		DECLARE_MEMBER(LinnerColorAffectorConfig, int, ID);
		DECLARE_MEMBER(LinnerColorAffectorConfig, int, UseSrcAlpha);
		DECLARE_MEMBER(LinnerColorAffectorConfig, string, StartColor);
		DECLARE_MEMBER(LinnerColorAffectorConfig, string, EndColor);
		DECLARE_MEMBER(LinnerColorAffectorConfig, float, StartTime);
		DECLARE_MEMBER(LinnerColorAffectorConfig, float, EndTime);

		virtual void registProperty() override
		{
			SAVE_MEMBER(ID, int);
			SAVE_MEMBER(UseSrcAlpha, int);
			SAVE_MEMBER(StartColor, string);
			SAVE_MEMBER(EndColor, string);
			SAVE_MEMBER(StartTime, float);
			SAVE_MEMBER(EndTime, float);
		}
	};


	// 旋转影响器
	class E3D_EXPORT_DLL RotationAffectorConfig : public TableBase
	{
		DECLARE_CLASS(RotationAffectorConfig)
	public:
		RotationAffectorConfig()
		{
			Name = "GroupRotationAffector";
		}
		DECLARE_MEMBER(RotationAffectorConfig, int, ID);
		DECLARE_MEMBER(RotationAffectorConfig, float, RoteteSpeed);
		DECLARE_MEMBER(RotationAffectorConfig, string, RotateAxis);

		virtual void registProperty() override
		{
			SAVE_MEMBER(ID, int);
			SAVE_MEMBER(RoteteSpeed, float);
			SAVE_MEMBER(RotateAxis, string);
		}
	};

	// 自旋影响器
	class E3D_EXPORT_DLL SelfRotateAffectorConfig : public TableBase
	{
		DECLARE_CLASS(SelfRotateAffectorConfig)
	public:
		SelfRotateAffectorConfig() : StartRandom(0)
		{
			Name = "RotateAffector";
		}
		DECLARE_MEMBER(SelfRotateAffectorConfig, int, ID);
		DECLARE_MEMBER(SelfRotateAffectorConfig, int, StartRandom);
		DECLARE_MEMBER(SelfRotateAffectorConfig, float, RoteteSpeed);
		DECLARE_MEMBER(SelfRotateAffectorConfig, string, RotateAxis);
		DECLARE_MEMBER(SelfRotateAffectorConfig, string, RotateSpeedRange);

		virtual void registProperty() override
		{
			SAVE_MEMBER(ID, int);
			SAVE_MEMBER(StartRandom, int);
			SAVE_MEMBER(RoteteSpeed, float);
			SAVE_MEMBER(RotateAxis, string);
			SAVE_MEMBER(RotateSpeedRange, string);
		}
	};

	// 自缩放影响器
	class E3D_EXPORT_DLL SelfScalAffectorConfig : public TableBase
	{
		DECLARE_CLASS(SelfScalAffectorConfig)
	public:
		SelfScalAffectorConfig() : StartTime(0), EndTime(0)
		{
			Name = "ScaleAffector";
		}
		DECLARE_MEMBER(SelfScalAffectorConfig, int, ID);
		DECLARE_MEMBER(SelfScalAffectorConfig, float, StartScale);
		DECLARE_MEMBER(SelfScalAffectorConfig, float, EndScale);
		DECLARE_MEMBER(SelfScalAffectorConfig, float, StartTime);
		DECLARE_MEMBER(SelfScalAffectorConfig, float, EndTime);

		virtual void registProperty() override
		{
			SAVE_MEMBER(ID, int);
			SAVE_MEMBER(StartScale, float);
			SAVE_MEMBER(EndScale, float);
			SAVE_MEMBER(StartTime, float);
			SAVE_MEMBER(EndTime, float);
		}
	};

	// 粒子团
	class E3D_EXPORT_DLL ParticleConfig : public TableBase
	{
		DECLARE_CLASS(ParticleConfig)
	public:
		ParticleConfig() : LockEmitterMove(0), DirectionOffset(0)
		{
			Name = "ParticleConfig";
		}
		DECLARE_MEMBER(ParticleConfig, int, ID);
		DECLARE_MEMBER(ParticleConfig, string, Material);
		DECLARE_MEMBER(ParticleConfig, string, Affector);
		DECLARE_MEMBER(ParticleConfig, string, Emitter);
		DECLARE_MEMBER(ParticleConfig, string, ElementClassName);
		DECLARE_MEMBER(ParticleConfig, string, TextureUV);
		DECLARE_MEMBER(ParticleConfig, int, DisplayCamera);
		DECLARE_MEMBER(ParticleConfig, int, LockEmitterMove);
		DECLARE_MEMBER(ParticleConfig, int, fShaderIndex);
		DECLARE_MEMBER(ParticleConfig, float, DirectionOffset);
		DECLARE_MEMBER(ParticleConfig, int, LayerID);

		virtual void registProperty() override
		{
			SAVE_MEMBER(ID, int);
			SAVE_MEMBER(Material, string);
			SAVE_MEMBER(DisplayCamera, int);
			SAVE_MEMBER(LockEmitterMove, int);
			SAVE_MEMBER(fShaderIndex, int);
			SAVE_MEMBER(Affector, string);
			SAVE_MEMBER(Emitter, string);
			SAVE_MEMBER(ElementClassName, string);
			SAVE_MEMBER(TextureUV, string);
			SAVE_MEMBER(DirectionOffset, float);
			SAVE_MEMBER(LayerID, int);
		}
	};

	// 材质
	class E3D_EXPORT_DLL MaterialConfig : public TableBase
	{
		DECLARE_CLASS(MaterialConfig);
	public:
		MaterialConfig()
		{
			Name			= "MaterialConfig";
			EnableDepthTest	= 1;
			EnableWriteDepth = 1;
			CullFace		= 1;
			TextureClampType = 0;
			TextureFilterType = 0;
		}
		DECLARE_MEMBER(MaterialConfig, int, ID);
		DECLARE_MEMBER(MaterialConfig, string, Texture);
		DECLARE_MEMBER(MaterialConfig, int, ShaderID);
		DECLARE_MEMBER(MaterialConfig, string, Color);
		DECLARE_MEMBER(MaterialConfig, string, Textures);
		DECLARE_MEMBER(MaterialConfig, int, SrcBlendFactor);
		DECLARE_MEMBER(MaterialConfig, int, DstBlendFactor);
		DECLARE_MEMBER(MaterialConfig, int, CullFace);
		DECLARE_MEMBER(MaterialConfig, int, EnableDepthTest);
		DECLARE_MEMBER(MaterialConfig, int, EnableWriteDepth);
		DECLARE_MEMBER(MaterialConfig, int, TextureClampType);
		DECLARE_MEMBER(MaterialConfig, int, TextureFilterType);

		virtual void registProperty() override
		{
			SAVE_MEMBER(ID, int);
			SAVE_MEMBER(Texture, string);
			SAVE_MEMBER(ShaderID, int);
			SAVE_MEMBER(Color, string);
			SAVE_MEMBER(Textures, string);
			SAVE_MEMBER(SrcBlendFactor, int);
			SAVE_MEMBER(DstBlendFactor, int);
			SAVE_MEMBER(CullFace, int);
			SAVE_MEMBER(EnableDepthTest, int);
			SAVE_MEMBER(EnableWriteDepth, int);
			SAVE_MEMBER(TextureClampType, int);
			SAVE_MEMBER(TextureFilterType, int);
		}
	};

	// UV动画
	class E3D_EXPORT_DLL TextureUVConfig : public TableBase
	{
		DECLARE_CLASS(TextureUVConfig);
	public:
		TextureUVConfig()
		{
			Name = "TextureConfig";
		}
		DECLARE_MEMBER(TextureUVConfig, int, ID);
		DECLARE_MEMBER(TextureUVConfig, float, speed);
		DECLARE_MEMBER(TextureUVConfig, int, repeatStartFrame);
		DECLARE_MEMBER(TextureUVConfig, int, type);
		DECLARE_MEMBER(TextureUVConfig, int, isReverse);
		DECLARE_MEMBER(TextureUVConfig, int, leftRightMirror);
		DECLARE_MEMBER(TextureUVConfig, int, topBottomMirror);
		DECLARE_MEMBER(TextureUVConfig, int, widthTexture);
		DECLARE_MEMBER(TextureUVConfig, int, heightTexture);
		DECLARE_MEMBER(TextureUVConfig, int, widthPic);
		DECLARE_MEMBER(TextureUVConfig, int, heightPic);
		DECLARE_MEMBER(TextureUVConfig, int, startX);
		DECLARE_MEMBER(TextureUVConfig, int, startY);
		DECLARE_MEMBER(TextureUVConfig, float, postponeTime);
		DECLARE_MEMBER(TextureUVConfig, float, duration);
		DECLARE_MEMBER(TextureUVConfig, string, framePos);

		virtual void registProperty() override
		{
			SAVE_MEMBER(ID, int);
			SAVE_MEMBER(speed, float);
			SAVE_MEMBER(repeatStartFrame, int);
			SAVE_MEMBER(type, int);
			SAVE_MEMBER(isReverse, int);
			SAVE_MEMBER(leftRightMirror, int);
			SAVE_MEMBER(topBottomMirror, int);
			SAVE_MEMBER(widthTexture, int);
			SAVE_MEMBER(heightTexture, int);
			SAVE_MEMBER(widthPic, int);
			SAVE_MEMBER(heightPic, int);
			SAVE_MEMBER(startX, int);
			SAVE_MEMBER(startY, int);
			SAVE_MEMBER(postponeTime, float);
			SAVE_MEMBER(duration, float);
			SAVE_MEMBER(framePos, string);
		}
	};

	// 层
	class E3D_EXPORT_DLL LayerConfig : public TableBase
	{
		DECLARE_CLASS(LayerConfig);
	public:
		DECLARE_MEMBER(LayerConfig, int, ID);
		DECLARE_MEMBER(LayerConfig, int, BlendType);
		DECLARE_MEMBER(LayerConfig, string, Name);

		virtual void registProperty() override
		{
			SAVE_MEMBER(ID, int);
			SAVE_MEMBER(Name, string);
			SAVE_MEMBER(BlendType, int);

		};
	};

	// 着色器
	class E3D_EXPORT_DLL ShaderConfig : public TableBase
	{
		DECLARE_CLASS(ShaderConfig);
	public:
		DECLARE_MEMBER(ShaderConfig, int, ID);
		DECLARE_MEMBER(ShaderConfig, string, ShaderPath);
		DECLARE_MEMBER(ShaderConfig, string, UniformVariable);

		virtual void registProperty() override
		{
			SAVE_MEMBER(ID, int);
			SAVE_MEMBER(ShaderPath, string);
			SAVE_MEMBER(UniformVariable, string);

		}
	};

	class E3D_EXPORT_DLL TextureAtlasConfig : public TableBase
	{
		DECLARE_CLASS(TextureAtlasConfig);
	public:
		DECLARE_MEMBER(TextureAtlasConfig, int, ID);
		DECLARE_MEMBER(TextureAtlasConfig, string, Name);
		DECLARE_MEMBER(TextureAtlasConfig, float, x);
		DECLARE_MEMBER(TextureAtlasConfig, float, y);
		DECLARE_MEMBER(TextureAtlasConfig, float, w);
		DECLARE_MEMBER(TextureAtlasConfig, float, h);
		DECLARE_MEMBER(TextureAtlasConfig, float, width);
		DECLARE_MEMBER(TextureAtlasConfig, float, height);

		virtual void registProperty() override
		{
			SAVE_MEMBER(ID, int);
			SAVE_MEMBER(Name, string);
			SAVE_MEMBER(x, float);
			SAVE_MEMBER(y, float);
			SAVE_MEMBER(w, float);
			SAVE_MEMBER(h, float);
			SAVE_MEMBER(width, float);
			SAVE_MEMBER(height, float);
		}
	};

	class E3D_EXPORT_DLL SkyBoxConfig : public TableBase
	{
		DECLARE_CLASS(SkyBoxConfig)
	public:
		DECLARE_MEMBER(SkyBoxConfig, int, ID);
		DECLARE_MEMBER(SkyBoxConfig, int, Front);
		DECLARE_MEMBER(SkyBoxConfig, int, Back);
		DECLARE_MEMBER(SkyBoxConfig, int, Top);
		DECLARE_MEMBER(SkyBoxConfig, int, Down);
		DECLARE_MEMBER(SkyBoxConfig, int, Left);
		DECLARE_MEMBER(SkyBoxConfig, int, Right);

		virtual void registProperty() override
		{
			SAVE_MEMBER(ID, int);
			SAVE_MEMBER(Front, int);
			SAVE_MEMBER(Back, int);
			SAVE_MEMBER(Top, int);
			SAVE_MEMBER(Down, int);
			SAVE_MEMBER(Left, int);
			SAVE_MEMBER(Right, int);

		}

	};

	class E3D_EXPORT_DLL MeshConfig : public TableBase
	{
		DECLARE_CLASS(MeshConfig)
	public:
		DECLARE_MEMBER(MeshConfig, int, ID);
		DECLARE_MEMBER(MeshConfig, string, ModelFilePath);
		DECLARE_MEMBER(MeshConfig, string, Materials);

		virtual void registProperty() override
		{
			SAVE_MEMBER(ID, int);
			SAVE_MEMBER(ModelFilePath, string);
			SAVE_MEMBER(Materials, string);
		}
	};

	class RenderTextureConfig : public TableBase
	{
		DECLARE_CLASS(RenderTextureConfig)
	public:
		DECLARE_MEMBER(RenderTextureConfig, int, ID);
		DECLARE_MEMBER(RenderTextureConfig, float, Width);
		DECLARE_MEMBER(RenderTextureConfig, float, Height);
		DECLARE_MEMBER(RenderTextureConfig, int, Target);
		DECLARE_MEMBER(RenderTextureConfig, int, Multisample);
		virtual void registProperty() override
		{
			SAVE_MEMBER(ID, int);
			SAVE_MEMBER(Width, float);
			SAVE_MEMBER(Height, float);
			SAVE_MEMBER(Target, int);
			SAVE_MEMBER(Multisample, int);
		}
	};

	class CubeMapTextureConfig : public TableBase
	{
		DECLARE_CLASS(CubeMapTextureConfig)
	public:
		DECLARE_MEMBER(CubeMapTextureConfig, int, ID);
		DECLARE_MEMBER(CubeMapTextureConfig, string, Front);
		DECLARE_MEMBER(CubeMapTextureConfig, string, Back);
		DECLARE_MEMBER(CubeMapTextureConfig, string, Top);
		DECLARE_MEMBER(CubeMapTextureConfig, string, Down);
		DECLARE_MEMBER(CubeMapTextureConfig, string, Left);
		DECLARE_MEMBER(CubeMapTextureConfig, string, Right);

		virtual void registProperty() override
		{
			SAVE_MEMBER(ID,		int);
			SAVE_MEMBER(Front,	string);
			SAVE_MEMBER(Back,	string);
			SAVE_MEMBER(Top,	string);
			SAVE_MEMBER(Down,	string);
			SAVE_MEMBER(Left,	string);
			SAVE_MEMBER(Right,	string);
		}
	};
}

#endif /* Table_hpp */

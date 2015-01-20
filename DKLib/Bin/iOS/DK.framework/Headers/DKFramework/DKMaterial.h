//
//  File: DKMaterial.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKResource.h"
#include "DKColor.h"
#include "DKShader.h"
#include "DKShaderProgram.h"
#include "DKTexture.h"
#include "DKShaderConstant.h"
#include "DKVertexStream.h"
#include "DKBlendState.h"
#include "DKRenderState.h"
#include "DKTextureSampler.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKMaterial
//
// 하나의 씬(Scene)을 위한 머티리얼이다.
// 하나의 머티리얼 객체는 여러개의 쉐이더를 가질 수 있다 (RenderingProperty)
//
// 예:
//   pass1:라이트 위치에서 그림자 찍기,
//   pass2:뎁스버퍼 기록,
//   pass3:컬러 렌더링 (그림자, 노멀범프, 환경매핑 등),
//   pass4:특수효과 (불타는 효과, 얼어붙은 효과 등),
//
// Note: 머리티얼 객체의 렌더링 패스는 순서대로 되어있다.
// 따라서 그 순서에 맞는 인덱스를 사용하여 바인딩해야 한다.
// 해당 모델이 가지지 않은 쉐이딩을 렌더링 하기 위해서는 해당 쉐이더를 가진
// 머티리얼을 바인딩 해서 사용해야 한다. 머티리얼은 모델과 독립적이다.
//
// 주의: 행렬은 쉐이더로 보낼때 열우선(Column-Major) 행렬로 변환된다! 
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKLIB_API DKMaterial : public DKResource
	{
	public:

		////////////////////////////////////////////////////////////////////////////////
		// PropertyArray : ShadingProperty 로 값을 넘기기 위한 객체
		struct PropertyArray
		{
			DKFoundation::DKArray<int> integers;
			DKFoundation::DKArray<float> floatings;

			PropertyArray(void) {}
			PropertyArray(int value, size_t count = 1) : integers(value, count) {}
			PropertyArray(const int* value, size_t count) : integers(value, count) {}
			PropertyArray(float value, size_t count = 1) : floatings(value, count) {}
			PropertyArray(const float* value, size_t count) : floatings(value, count) {}
		};

		typedef DKFoundation::DKArray<DKFoundation::DKObject<DKTexture>> TextureArray;
		struct Sampler
		{
			TextureArray								textures;
			DKFoundation::DKObject<DKTextureSampler>	sampler;
		};

		////////////////////////////////////////////////////////////////////////////////
		// SamplerProperty : 텍스쳐 및 샘플링 정보가 들어있는 객체
		struct SamplerProperty
		{
			DKShaderConstant::Uniform					id;				// sampler
			DKShaderConstant::Type						type;
			TextureArray								textures;
			DKFoundation::DKObject<DKTextureSampler>	sampler;
		};
		////////////////////////////////////////////////////////////////////////////////
		// ShadingProperty : 쉐이더 상수 매칭에 사용되며 DKMaterial 안에 정의된 값은 기본값이다.
		// 실제 렌더링 할때는 DKModel 에서 모델 고유의 값을 넘겨 받으며, 고유의 값이 없을때 기본값을 사용한다.
		// 모델 고유의 값은 전부 UniformUserDefine 타입만 사용된다. 나머지는 DKSceneState 에서 사용된다.
		struct ShadingProperty
		{
			DKShaderConstant::Uniform		id;
			DKShaderConstant::Type			type;
			PropertyArray					value;			// 기본값, UniformUserDefine 타입일때만 사용됨
		};
		////////////////////////////////////////////////////////////////////////////////
		// StreamProperty : 버텍스 스트림을 매칭하기 위한 객체
		struct StreamProperty
		{
			DKVertexStream::Stream		id;
			DKVertexStream::Type		type;
			size_t						components;
		};
		////////////////////////////////////////////////////////////////////////////////
		// ShaderSource : 쉐이더 소스 (여러 소스를 합쳐서 하나의 프로그램으로 만든다)
		struct ShaderSource
		{
			DKFoundation::DKString				name;
			DKFoundation::DKString				source;
			DKShader::Type						type;
			DKFoundation::DKObject<DKShader>	shader;
		};
		////////////////////////////////////////////////////////////////////////////////
		// RenderingProperty : 실제 렌더링 할때 사용된다. 하나의 씬에는 하나의 RenderingProperty 를 사용하게 된다.
		// 한개의 쉐이더 프로그램과 여러개의 쉐이더소스를 가지고 있다.
		struct RenderingProperty
		{
			enum DepthFunc : unsigned char
			{
				DepthFuncNever = 0,
				DepthFuncAlways,
				DepthFuncLess,
				DepthFuncLessEqual,
				DepthFuncEqual,
				DepthFuncGreater,
				DepthFuncGreaterEqual,
				DepthFuncNotEqual,
			};

			DKFoundation::DKString						name;
			DepthFunc									depthFunc;
			bool										depthWrite;
			DKBlendState								blendState;
			DKFoundation::DKArray<ShaderSource>			shaders;
			DKFoundation::DKObject<DKShaderProgram>		program;
		};

		class PropertyCallback
		{
		public:
			typedef DKFoundation::DKStaticArray<int> IntArray;
			typedef DKFoundation::DKStaticArray<float> FloatArray;
			typedef DKMaterial::Sampler Sampler;

			virtual IntArray GetIntProperty(const DKShaderConstant&, int) = 0;
			virtual FloatArray GetFloatProperty(const DKShaderConstant&, int) = 0;
			virtual const Sampler* GetSamplerProperty(const DKShaderConstant&, int) = 0;
			virtual ~PropertyCallback(void) {}
		};

		DKMaterial(void);
		~DKMaterial(void);

		int IndexOfRenderPropertyName(const DKFoundation::DKString& name) const;
		size_t NumberOfRenderProperties(void) const;

		bool Bind(int programIndex, PropertyCallback* callback, const DKBlendState* blending = NULL) const;

		const DKFoundation::DKArray<DKVertexStream>& StreamArray(int state) const;

		struct BuildLog
		{
			DKFoundation::DKString errorLog;
			DKFoundation::DKString failedShader;
			DKFoundation::DKString failedProgram;
		};
		static bool CompileShaderSource(ShaderSource* src, DKFoundation::DKString& errorLog);
		bool BuildProgram(int index, BuildLog* log);
		bool Build(BuildLog* log);

		DKFoundation::DKObject<DKSerializer> Serializer(void);

		typedef DKFoundation::DKMap<DKFoundation::DKString, ShadingProperty>	ShadingPropertyMap;
		typedef DKFoundation::DKMap<DKFoundation::DKString, SamplerProperty>	SamplerPropertyMap;
		typedef DKFoundation::DKMap<DKFoundation::DKString, StreamProperty>		StreamPropertyMap;
		typedef DKFoundation::DKArray<ShaderSource>								ShaderSourceArray;
		typedef DKFoundation::DKArray<RenderingProperty>						RenderingPropertyArray;

		// 머티리얼의 기본 속성 (Callback에서 얻어오지 못하면, 이 데이터를 사용하게 된다)
		ShadingPropertyMap				shadingProperties;
		SamplerPropertyMap				samplerProperties;
		StreamPropertyMap				streamProperties;
		ShaderSourceArray				shaderProperties;
		RenderingPropertyArray			renderingProperties;

		bool Validate(void) override;
		bool IsValid(void) const;
	};
}

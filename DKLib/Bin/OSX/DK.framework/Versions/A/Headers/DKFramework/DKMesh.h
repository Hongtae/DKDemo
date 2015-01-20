//
//  File: DKMesh.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKModel.h"
#include "DKIndexBuffer.h"
#include "DKMaterial.h"
#include "DKPrimitiveIndex.h"
#include "DKSceneState.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKMesh
//
// DKMeshStream, DKMaterial, DKDynamicMesh 를 이용하여 매쉬를 랜더링 한다.
// 
// DKMesh
//  +-- DKMeshStream
//  +-- DKMaterial
//  +-- DKDynamicMesh (Material 에 정의된 stream 과 외부 데이터를 이용하여 매쉬 변경함)
//  +-- Material Properties
//  +-- Samplers
//
// DKScene 에서 실제로 사용하기 위해서는 Clone 을 호출하여 객체를 복사해야 한다.
// (동적으로 변하는 것들도 있기 때문에, 개별적으로 Clone 을 구현해야 함)
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKLIB_API DKMesh : public DKModel
	{
	public:
		virtual ~DKMesh(void);

		enum DrawFaceMode
		{
			DrawFaceNone = 0,	// draw nothing!
			DrawFaceCCW,		// front-face
			DrawFaceCW,			// back-face
			DrawFaceBoth,		// all-faces
		};
		using TextureArray = DKMaterial::TextureArray;
		using TextureSampler = DKMaterial::Sampler;
		using TextureSamplerMap = DKFoundation::DKMap<DKFoundation::DKString, TextureSampler>;
		using PropertyArray = DKMaterial::PropertyArray;
		using PropertyMap = DKFoundation::DKMap<DKFoundation::DKString, PropertyArray>;

		// 머티리얼 설정
		DKMaterial*			Material(void)								{return material;}
		const DKMaterial*	Material(void) const						{return material;}
		virtual bool		CanAdoptMaterial(const DKMaterial* m) const {return false;}
		virtual void		SetMaterial(DKMaterial* m);

		// 텍스쳐 설정
		void					AppendSampler(const DKFoundation::DKString& name, DKTexture* texture);
		void					SetSampler(const DKFoundation::DKString& name, const TextureArray& textures, DKTextureSampler* sampler);	// 샘플러에 텍스쳐 여러장 추가
		void					SetSampler(const DKFoundation::DKString& name, DKTexture* texture, DKTextureSampler* sampler);		// 샘플러에 텍스쳐 한장 설정, null 이면 제거함
		TextureSampler*			Sampler(const DKFoundation::DKString& name);
		const TextureSampler*	Sampler(const DKFoundation::DKString& name) const;
		size_t					SamplerCount(void) const;
		void					RemoveSampler(const DKFoundation::DKString& name);
		void					RemoveAllSamplers(void);

		// 상수 설정
		void					SetMaterialProperty(const DKFoundation::DKString& name, const PropertyArray& value);
		size_t					MaterialPropertyCount(const DKFoundation::DKString& name) const;
		const PropertyArray*	MaterialProperty(const DKFoundation::DKString& name) const;
		PropertyArray*			MaterialProperty(const DKFoundation::DKString& name);
		void					RemoveMaterialProperty(const DKFoundation::DKString& name);
		void					RemoveAllMaterialProperties(void);

		const PropertyMap&			MaterialPropertyMap(void) const	{return materialProperties;}
		const TextureSamplerMap&	SamplerMap(void) const			{ return samplers; }
		PropertyMap&				MaterialPropertyMap(void)		{return materialProperties;}
		TextureSamplerMap&			SamplerMap(void)				{ return samplers; }

		void SetDefaultPrimitiveType(DKPrimitive::Type t)		{ primitiveType = t; }
		DKPrimitive::Type DefaultPrimitiveType(void) const		{ return primitiveType; }
		virtual DKPrimitive::Type PrimitiveType(void) const		{ return primitiveType; }

		void SetDrawFace(DrawFaceMode face)		{drawFace = face;}
		DrawFaceMode DrawFace(void) const		{return drawFace;}

		void SetScale(float s)					{ scale = DKVector3(s, s, s); }
		void SetScale(const DKVector3& s)		{ scale = s; }
		const DKVector3& Scale(void) const		{ return scale; }

		bool IsHidden(void) const				{ return hidden; }
		void SetHidden(bool h)					{ hidden = h; }

		// 바운딩 정보 (옵션)
		void SetBoundingAABox(const DKAABox& box)		{boundingAABox = box;}
		void SetBoundingSphere(const DKSphere& sphere)	{boundingSphere = sphere;}
		const DKAABox& BoundingAABox(void) const		{return boundingAABox;}
		const DKSphere& BoundingSphere(void) const		{return boundingSphere;}

		DKAABox ScaledBoundingAABox(void) const;
		DKSphere ScaledBoundingSphere(void) const;
		const DKMatrix4& ScaledWorldTransformMatrix(void) const;

		struct RenderInfo
		{
			DKPrimitive::Type primitive;
			DKIndexBuffer::Type indexType;
			int numVerts;
			int numIndices;
			int numInstances;
		};
		bool Bind(DKSceneState&, RenderInfo* info, const DKBlendState* blending = NULL) const;

		virtual DKFoundation::DKObject<DKSerializer> Serializer(void) override;

	protected:
		virtual bool BindTransform(DKSceneState&) const;
		virtual bool BindPrimitiveIndex(DKPrimitive::Type*, int*, DKIndexBuffer::Type*) const = 0;
		virtual int BindStream(const DKVertexStream&) const = 0;

		void OnUpdateSceneState(const DKNSTransform&) override;

		DKMesh(void);
		DKMesh* Copy(UUIDObjectMap&, const DKMesh*);

		DrawFaceMode				drawFace;
		DKPrimitive::Type			primitiveType;

		bool		hidden;
		DKVector3	scale;
		DKAABox		boundingAABox;			// 바운딩 박스
		DKSphere	boundingSphere;			// 바운딩 스피어
		DKMatrix4	scaledTransformMatrix;

		// materialProperties 는 렌더링시에 DKMaterial 에 정의되어있는 상수를 대체하게 된다.
		// DKMaterial 에 정의된 상수중에 DKShaderConstant::UniformUserDefine 형식의 값만 대체할 수 있다.
		// 여기 없는 값은 DKMaterial 안에 정의된 기본값이 사용된다.
		PropertyMap	materialProperties;
		// textures 에 들어있는 텍스쳐가 머티리얼의 텍스쳐보다 우선한다.
		TextureSamplerMap	samplers;

		DKFoundation::DKObject<DKMaterial>	material;	// 머티리얼
	};
}

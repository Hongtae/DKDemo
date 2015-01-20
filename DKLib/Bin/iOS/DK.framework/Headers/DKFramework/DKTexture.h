//
//  File: DKTexture.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKSize.h"
#include "DKRect.h"
#include "DKResource.h"
#include "DKColor.h"
#include "DKVector3.h"

#define DKL_IS_POWER_OF_TWO(v)		((v & (v-1)) == 0)

namespace DKFramework
{
	inline bool IsPowerOfTwo(int value)	{return DKL_IS_POWER_OF_TWO(value);}

	class DKLIB_API DKTexture : public DKResource
	{
		friend class DKRenderTarget;
	public:
		enum Target
		{
			TargetUnknown = 0,
			Target2D,
			Target3D,
			TargetCube,
		};
		enum Format
		{
			FormatUnknown = 0,
			FormatAlpha,
			FormatRGB,
			FormatRGBA,
			FormatDepth16,
			FormatDepth24,
			FormatDepth32,
		};
		enum Type
		{
			TypeSignedByte = 0,
			TypeUnsignedByte,
			TypeSignedShort,
			TypeUnsignedShort,
			TypeSignedInt,
			TypeUnsignedInt,
			TypeFloat,
			TypeDouble,
		};

		DKTexture(void);				// 더미 생성용
		virtual ~DKTexture(void);

		virtual void Bind(void) const;
		virtual bool IsValid(void) const;
	
		Target		ObjectTarget(void) const		{return target;}
		Format		TextureFormat(void) const		{return format;}
		Type		ComponentType(void) const		{return type;}
		int			Width(void) const				{return width;}
		int			Height(void) const				{return height;}
		int			Depth(void) const				{return depth;}
		size_t		NumberOfComponents(void) const	{return components;}

		DKSize		Resolution(void) const;			// 2D 텍스쳐 해상도 (width x height)
		DKVector3	Dimensions(void) const;			// 2D,3D 텍스쳐 픽셀수 (width x height x depth)
		size_t		BytesPerPixel(void) const;

		// DKResource::Validate() override
		bool Validate(void)					{return IsValid();}
		bool IsColorTexture(void) const;
		bool IsDepthTexture(void) const;

		unsigned int TextureId(void) const	{return resourceId;}		// OpenGL resource id

		DKFoundation::DKObject<DKSerializer> Serializer(void);

		static int MaxTextureSize(void);

	protected:
		DKTexture(Target);

		unsigned int	resourceId;

		int				width;
		int				height;
		int				depth;		// 3D 텍스쳐의 깊이값, 그외(Cube 포함) 텍스쳐는 1임
		size_t			components;
		Format			format;
		Type			type;

	private:
		const Target	target;		// tex2D or Cube .. etc
	};
}

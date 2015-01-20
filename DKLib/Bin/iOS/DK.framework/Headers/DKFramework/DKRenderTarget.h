//
//  File: DKRenderTarget.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKTexture2D.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKRenderTarget
//
// 렌더타겟
// 컬러 버퍼는 텍스쳐로만 사용되며, 뎁스 버퍼는 뎁스 텍스쳐 또는 렌더 버퍼에서 사용된다.
// 렌더 버퍼를 컬러 버퍼로 사용하지는 않는다.
//
// Note: 이 객체는 thread-safe 하지 않음.
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKLIB_API DKRenderTarget
	{
	public:
		enum DepthFormat
		{
			DepthFormatNone = 0,
			DepthFormat16,
			DepthFormat24,
			DepthFormat32,
		};

		struct TargetQuery
		{
			unsigned int width;
			unsigned int height;
			DepthFormat depth;
			unsigned int fbo;
		};
		typedef DKFoundation::DKInvocation<TargetQuery> ProxyQuery;

		static DKFoundation::DKObject<DKRenderTarget> Create(int width, int height, DepthFormat depth);
		static DKFoundation::DKObject<DKRenderTarget> Create(ProxyQuery* proxy);

		DKRenderTarget(void);
		virtual ~DKRenderTarget(void);

		bool SetColorTextures(DKTexture2D** tex, size_t num);
		DKTexture2D* ColorTexture(int index);
		const DKTexture2D* ColorTexture(int index) const;
		size_t NumberOfColorTextures(void) const;
		
		static size_t MaxColorTextures(void);

		bool SetDepthTexture(DKTexture2D* tex);
		bool SetDepthBuffer(int width, int height, DepthFormat df);
		DKTexture2D* DepthTexture(void);
		const DKTexture2D* DepthTexture(void) const;
		DepthFormat DepthBufferFormat(void) const;

		virtual bool Bind(void) const;
		virtual bool IsValid(void) const;
		bool Validate(void);
		bool IsProxy(void) const;

		bool CopyColorTexture(int target, DKTexture2D* tex) const;
		bool CopyDepthTexture(DKTexture2D* tex) const;
		
		DKSize Resolution(void) const;

	private:
		DKSize depthResolution;
		DepthFormat	depthFormat;

		void UpdateResolution(void);

		DKFoundation::DKArray<DKFoundation::DKObject<DKTexture2D>>	colorTextures;
		DKFoundation::DKObject<DKTexture2D>							depthTexture;			// optional
		unsigned int												depthBuffer;

		DKFoundation::DKObject<ProxyQuery>	proxyQuery;		
	};
}

//
//  File: DKOpenGLContext.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKWindow.h"
#include "DKRenderState.h"

namespace DKFramework
{
	class DKOpenGLInterface;
	template <typename T> class DKContextScopeBinder
	{
	public:
		DKContextScopeBinder(T* ctxt) : context(ctxt)	{ if (context) context->Bind(); }
		~DKContextScopeBinder(void)						{ if (context) context->Unbind();}
		T* operator -> (void)							{return context;}
		const T* operator -> (void) const				{return context;}
	private:
		DKFoundation::DKObject<T> context;
	};
	template <typename T, typename R> class DKContextScopeBinderWithParam
	{
	public:
		DKContextScopeBinderWithParam(T* ctxt, R p) : context(ctxt)		{ if (context) context->Bind(p); }
		~DKContextScopeBinderWithParam(void)							{ if (context) context->Unbind();}
		T* operator -> (void)											{return context;}
		const T* operator -> (void) const								{return context;}
	private:
		DKFoundation::DKObject<T> context;
	};

	class DKLIB_API DKOpenGLContext : public DKFoundation::DKSharedInstance<DKOpenGLContext>
	{
	public:
		~DKOpenGLContext(void);

		bool IsBound(void) const;
		void Bind(const DKWindow* window = NULL) const;	// 쓰레드 바인딩
		void Unbind(void) const;						// 쓰레드 언바인딩

		void Flush(void) const;							// glFlush 및 디버깅 체크
		void Finish(void) const;						// glFinish 및 디버깅 체크
		void Present(void) const;						// 버퍼 스왑
		void Update(void) const;						// 뷰 상태 갱신

		bool GetSwapInterval(void) const;
		void SetSwapInterval(bool interval) const;

		static DKRenderState& RenderState(void);

		unsigned int FramebufferId(void) const;			// 현재 바인딩 된 윈도우의 프레임 버퍼 id
		
	private:
		friend class DKFoundation::DKObject<DKOpenGLContext>;
		friend class DKFoundation::DKSharedInstance<DKOpenGLContext>;
		DKOpenGLContext(void);
		DKOpenGLContext(const DKOpenGLContext&);
		DKOpenGLContext& operator = (const DKOpenGLContext&);
		
		typedef DKFoundation::DKMap<DKFoundation::DKThread::ThreadId, DKFoundation::DKObject<DKRenderState>> RenderStateMap;
		static RenderStateMap stateMap;
		static DKFoundation::DKSpinLock stateLock;
		DKOpenGLInterface* impl;	// 플랫폼 별로 구현된 컨텍스트
	};
}

//
//  File: DKOpenGLInterface.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2013-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../../DKFoundation.h"

namespace DKFramework
{
	class DKOpenGLContext;
	class DKOpenGLInterface
	{
	public:
		virtual ~DKOpenGLInterface(void) {}

		virtual void Bind(void* target) const = 0;		// 쓰레드 바인딩
		virtual void Unbind(void) const = 0;			// 쓰레드 언바인딩
		virtual bool IsBound(void) const = 0;

		virtual void Flush(void) const = 0;				// glFlush 및 디버깅 체크
		virtual void Finish(void) const = 0;			// glFinish 및 디버깅 체크
		virtual void Present(void) const = 0;			// 버퍼 스왑

		virtual void Update(void) const = 0;			// 뷰 상태 갱신

		virtual bool GetSwapInterval(void) const = 0;
		virtual void SetSwapInterval(bool) const = 0;

		virtual unsigned int FramebufferId(void) const		{return 0;}

		static DKOpenGLInterface* CreateInterface(DKOpenGLContext*);
	};
}

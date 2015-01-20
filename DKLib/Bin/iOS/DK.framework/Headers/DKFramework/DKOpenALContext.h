//
//  File: DKOpenALContext.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKOpenALContext
//
// OpenAL context 에 대한 객체
//
// iOS note:
// DKOpenALContext::IsActivated 는 iOS 의 경우엔 오디오 세션이 비활성화 된 경우엔 false 를 리턴함.
// DKApplication 을 사용하지 않을 경우
// 오디오 세션이 인터럽트되면 DKOpenALContext::Deactivate 를 호출해야 하고
// 오디오 세션 인터럽트가 끝나면 DKOpenALContext::Activate 를 호출해야 한다.
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKLIB_API DKOpenALContext : public DKFoundation::DKSharedInstance<DKOpenALContext>
	{
	public:
		~DKOpenALContext(void);

		bool IsBound(void) const;
		void Bind(void) const;
		void Unbind(void) const;

		bool IsActivated(void) const;		// 시스템이 오디오 재생이 가능한지 여부

		// Activate, Deactivate 는 DKApplication 상에서는 절대 호출하지 말것!
		static void Activate(void);
		static void Deactivate(void);
		
		struct DeviceList
		{
			struct Device
			{
				DKFoundation::DKString name;
				int majorVersion;
				int minorVersion;
			};
			DKFoundation::DKString			defaultDeivce;
			DKFoundation::DKArray<Device>	list;
		};

		//static bool debugMode;
	private:
		friend class DKFoundation::DKObject<DKOpenALContext>;
		friend class DKFoundation::DKSharedInstance<DKOpenALContext>;
		DKOpenALContext(void);
		DKOpenALContext(const DKOpenALContext&);
		DKOpenALContext& operator = (const DKOpenALContext&);

		static DKFoundation::DKSpinLock		contextLock;
		DeviceList							deviceList;
		void *device;		// OpenAL device
		void *context;		// OpenAL context
	};
}

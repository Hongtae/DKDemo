//
//  File: DKAudioListener.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKOpenALContext.h"
#include "DKVector3.h"
#include "DKQuaternion.h"
#include "DKMatrix3.h"
#include "DKMatrix4.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKAudioListener
//
// 오디오 환경 제어
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKLIB_API DKAudioListener : public DKFoundation::DKSharedInstance<DKAudioListener>
	{
	public:
		~DKAudioListener(void);

		void SetGain(float f);
		float Gain(void) const;
		
		void SetPosition(const DKVector3& v);
		const DKVector3& Position(void) const;
		
		void SetOrientation(const DKVector3& forward, const DKVector3& up);
		void SetOrientation(const DKMatrix3& m);
		
		const DKVector3& Forward(void) const;
		const DKVector3& Up(void) const;
		
		void SetVelocity(const DKVector3& v);
		const DKVector3& Velocity(void) const;
				
	private:
		DKVector3 position;
		DKVector3 velocity;
		DKVector3 forward;
		DKVector3 up;
		float gain;
		
		friend class DKFoundation::DKObject<DKAudioListener>;
		friend class DKFoundation::DKSharedInstance<DKAudioListener>;		
		DKAudioListener(void);
		DKAudioListener(const DKAudioListener&);
		DKAudioListener& operator = (const DKAudioListener&);		

		DKFoundation::DKObject<DKOpenALContext> context;
	};
}

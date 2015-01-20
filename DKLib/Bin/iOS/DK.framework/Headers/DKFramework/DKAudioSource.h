//
//  File: DKAudioSource.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKVector3.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKAudioSource
//
// 오디오 재생 관련 제어
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKLIB_API DKAudioSource
	{
	public:
		enum AudioState
		{
			StateUnknown = 0,
			StateStopped,
			StatePlaying,
			StatePaused,
		};

		DKAudioSource(void);
		virtual ~DKAudioSource(void);

		bool Play(void);
		bool Stop(void);
		bool Pause(void);

		bool EnqueueBuffer(int frequency, int bits, int channels, const void* data, size_t bytes, double timeStamp);
		void UnqueueBuffers(void) const;
		size_t QueuedBuffers(void) const;

		double TimePosition(void) const;
		void SetTimePosition(double t);
		double TimeOffset(void) const;
		void SetTimeOffset(double t);

		AudioState State(void) const;

		// AL_PITCH
		void SetPitch(float f);
		float Pitch(void) const;

		// AL_GAIN
		void SetGain(float f);
		float Gain(void) const;

		// AL_MIN_GAIN
		void SetMinGain(float f);
		float MinGain(void) const;

		// AL_MAX_GAIN
		void SetMaxGain(float f);
		float MaxGain(void) const;

		// AL_MAX_DISTANCE
		void SetMaxDistance(float f);
		float MaxDistance(void) const;

		// AL_ROLLOFF_FACTOR
		void SetRolloffFactor(float f);
		float RolloffFactor(void) const;

		// AL_CONE_OUTER_GAIN
		void SetConeOuterGain(float f);
		float ConeOuterGain(void) const;

		// AL_CONE_INNER_ANGLE
		void SetConeInnerAngle(float f);
		float ConeInnerAngle(void) const;

		// AL_CONE_OUTER_ANGLE
		void SetConeOuterAngle(float f);
		float ConeOuterAngle(void) const;

		// AL_REFERENCE_DISTANCE
		void SetReferenceDistance(float f);
		float ReferenceDistance(void) const;

		// AL_POSITION
		void SetPosition(const DKVector3& v);
		DKVector3 Position(void) const;

		// AL_VELOCITY
		void SetVelocity(const DKVector3& v);
		DKVector3 Velocity(void) const;

		// AL_DIRECTION
		void SetDirection(const DKVector3& v);
		DKVector3 Direction(void) const;


		static int Format(int bits, int channels);
		static int ErrorCode(void);
		static DKFoundation::DKString ErrorString(int errCode);
		
	private:
		unsigned int sourceId;

		struct BufferInfo
		{
			double timeStamp;
			size_t bytes;
			size_t bytesSecond;
			unsigned int bufferId;
		};
		DKFoundation::DKSpinLock bufferLock;
		mutable DKFoundation::DKArray<BufferInfo> buffers;
	};
}

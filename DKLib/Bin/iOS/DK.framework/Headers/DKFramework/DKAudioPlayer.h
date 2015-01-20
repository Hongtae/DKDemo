//
//  File: DKAudioPlayer.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKFoundation.h"
#include "DKVector3.h"
#include "DKAudioStream.h"
#include "DKAudioSource.h"

namespace DKFramework
{
	class DKLIB_API DKAudioPlayer
	{
	public:
		typedef DKFoundation::DKFunctionSignature<void (void*, size_t, double)>	StreamFilter;	// void (data, size, time)
		
		typedef DKAudioSource::AudioState AudioState;

		DKAudioPlayer(void);
		virtual ~DKAudioPlayer(void);

		static DKFoundation::DKObject<DKAudioPlayer> Create(DKFoundation::DKStream* stream);
		static DKFoundation::DKObject<DKAudioPlayer> Create(const DKFoundation::DKString& file);
		static DKFoundation::DKObject<DKAudioPlayer> Create(DKAudioStream* stream);		// stream 은 공유되선 안됨

		int Channels(void) const;
		int Bits(void) const;
		int Frequency(void) const;
		double Duration(void) const;

		double TimePosition(void) const;
		AudioState State(void) const;

		DKAudioStream* Stream(void)							{return stream;}
		const DKAudioSource* Source(void) const				{return source;}

		void PlayLoop(double pos, int loops);
		void Play(void);
		void Stop(void);
		void Pause(void);

		DKAudioSource* AudioSource(void);
		const DKAudioSource* AudioSource(void) const;

		// 음향효과 필터
		void SetStreamFilter(StreamFilter* f);
		
		// 버퍼링 관련, 0 을 넣으면 최소값으로 설정할수 있다. 너무 작게 설정하면 끊길 수 있다.
		void SetBufferingTime(double t);
		double BufferingTime(void) const;

	private:
		class AudioQueue;

		DKFoundation::DKObject<StreamFilter> filter;
		// 재생 관련
		double			bufferingTime;
		double			timePosition;
		double			duration;

		void UpdatePlaybackState(int ps, double tp);
		void UpdateBufferState(int bs, double tp);
		void ProcessStream(void *data, size_t size, double time);		// AudioController 에 의해 호출된다.

		AudioState	playerState;
		int			queuePlaybackState;
		int			queueBufferState;
		DKFoundation::DKObject<DKAudioStream>	stream;		// 오디오 데이터
		DKFoundation::DKObject<DKAudioSource>	source;
		DKFoundation::DKObject<AudioQueue>		queue;
		DKFoundation::DKSpinLock				lock;
	};
}

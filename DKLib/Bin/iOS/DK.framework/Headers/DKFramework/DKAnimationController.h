//
//  File: DKAnimationController.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKTransform.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKAnimationController
//
// 애니메이션 인터페이스
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKLIB_API DKAnimatedTransform
	{
	public:
		typedef DKFoundation::DKString NodeId;
		virtual ~DKAnimatedTransform(void) {}
		virtual void Update(double timeDelta, DKFoundation::DKTimeTick tick) {}
		virtual bool GetTransform(const NodeId& key, DKTransformUnit& out) = 0;
	};

	class DKLIB_API DKAnimationController : public DKAnimatedTransform
	{
	public:
		DKAnimationController(void);
		virtual ~DKAnimationController(void);

		void Update(double timeDelta, DKFoundation::DKTimeTick tick);

		virtual bool GetTransform(const NodeId& key, DKTransformUnit& out) = 0;

		virtual bool IsPlaying(void) const = 0;
		virtual float Duration(void) const = 0;

		virtual void Play(void) = 0;		// 애니메이션 재생을 시작함
		virtual void Stop(void) = 0;		// 애니메이션 재생을 중지함

		void SetFrame(float frame);			// 애니메이션 프레임(시간)을 재 설정함
		void SetSpeed(float speed);			// 재생 속도를 조정함
		float Frame(void) const				{return frame;}
		float Speed(void) const				{return speed;}		// 현재 재생 속도 (재생 여부와 상관 없음)

	protected:
		virtual void UpdateFrame(float frame) = 0;

		float			frame;
		float			speed;

		DKFoundation::DKTimeTick lastUpdatedTick;
	};
}

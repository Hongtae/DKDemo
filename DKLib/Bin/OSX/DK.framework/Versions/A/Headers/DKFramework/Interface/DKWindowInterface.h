//
//  File: DKWindowInterface.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2013-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../../DKFoundation.h"
#include "../DKRect.h"

namespace DKFramework
{
	class DKWindow;
	class DKWindowInterface
	{
	public:
		virtual ~DKWindowInterface(void) {}

		virtual bool Create(const DKFoundation::DKString& title, const DKSize& size, const DKPoint& origin, int style) = 0;
		virtual bool CreateProxy(void* systemHandle) = 0;
		virtual bool IsProxy(void) const = 0;
		virtual void UpdateProxy(void) = 0;
		virtual void Destroy(void) = 0;
		virtual void* PlatformHandle(void) const = 0;
		virtual bool IsValid(void) const = 0;

		virtual void ShowMouse(int deviceId, bool bShow) = 0;
		virtual bool IsMouseVisible(int deviceId) const = 0;
		virtual void HoldMouse(int deviceId, bool bHold) = 0;
		virtual bool IsMouseHeld(int deviceId) const = 0;

		virtual void Show(void) = 0;
		virtual void Hide(void) = 0;
		virtual void Activate(void) = 0;
		virtual void Minimize(void) = 0;

		// Origin, Resize 은 OS 의 포인트 유닛 단위.
		DKPoint Origin(void) const;
		virtual void SetOrigin(const DKPoint&) = 0;
		virtual void Resize(const DKSize&, const DKPoint* optionalOrigin) = 0;

		// ContentSize 는 내부 픽셀수 기준임.
		DKSize ContentSize(void) const;

		// 내부 픽셀수 대비 OS 포인트 유닛 비율
		virtual double ContentScaleFactor(void) const = 0;

		virtual void SetTitle(const DKFoundation::DKString& title) = 0;
		virtual DKFoundation::DKString Title(void) const = 0;

		virtual void SetMousePosition(int deviceId, const DKPoint& pt) = 0;
		virtual DKPoint MousePosition(int deviceId) const = 0;

		virtual void EnableTextInput(int deviceId, bool bTextInput) = 0;

		static DKWindowInterface* CreateInterface(DKWindow*);
	};
}

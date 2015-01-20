//
//  File: DKScreen.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKWindow.h"
#include "DKRenderer.h"
#include "DKPoint.h"
#include "DKSize.h"
#include "DKOpenALContext.h"
#include "DKOpenGLContext.h"
#include "DKFrame.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKScreen
//
// 스크린 객체, DKWindow의 컨텐츠 영역에 DKFrame 들을 그린다.
// DKFrame 객체들간의 메시지를 위해 DKRunLoop(런루프) 형식으로 되어있다.
// DKFrame 객체들을 관리하며, 윈도우에서 발생한 이벤트를 DKFrame 으로 전달해준다.
//
////////////////////////////////////////////////////////////////////////////////


namespace DKFramework
{
	class DKLIB_API DKScreen : public DKFoundation::DKRunLoop
	{
		friend class DKFoundation::DKObject<DKScreen>;
	public:
		DKScreen(void);
		~DKScreen(void);

		bool Run(DKWindow* window, DKFrame* rootFrame);

		DKFoundation::DKRunLoop* RunLoop(void) const;

		DKWindow* Window(void);										// 렌더러 윈도우
		const DKWindow* Window(void) const;							// 렌더러 윈도우

		DKFrame* RootFrame(void);									// 루트 프레임
		const DKFrame* RootFrame(void) const;						// 루트 프레임
		bool SetKeyFrame(int deviceId, DKFrame* frame);				// frame 이 키보드(deviceId) 입력을 선점하도록 한다
		bool SetFocusFrame(int deviceId, DKFrame* frame);			// frame 이 마우스(deviceId) 입력을 선점하도록 한다
		bool RemoveKeyFrame(int deviceId, DKFrame* frame);			// frame 이 키보드(deviceId) 입력을 선점하고 있으면 해제 한다. (이벤트 발생하지 않음)
		bool RemoveFocusFrame(int deviceId, DKFrame* frame);		// frame 이 마우스(deviceId) 입력을 선점하고 있으면 해제 한다. (이벤트 발생하지 않음)
		DKFrame* KeyFrame(int deviceId);							// 키보드 입력 받는 프레임
		const DKFrame* KeyFrame(int deviceId) const;				// 키보드 입력 받는 프레임
		DKFrame* FocusFrame(int deviceId);							// 마우스 입력 받는 프레임
		const DKFrame* FocusFrame(int deviceId) const;				// 마우스 입력 받는 프레임

		void RemoveKeyFrameForAnyDevices(DKFrame* frame, bool notify);		// frame 에 의해 선점된 키보드 정보 모두 해제
		void RemoveFocusFrameForAnyDevices(DKFrame* frame, bool notify);	// frame 에 의해 선점된 마우스 정보 모두 해제
		void RemoveAllKeyFramesForAnyDevices(bool notify);					// 모든 마우스에 대해 선점된 프레임 해제
		void RemoveAllFocusFramesForAnyDevices(bool notify);				// 모든 키보드에 대해 선점된 프레임 해제

		void Render(bool forced = false);							// 화면 갱신 이벤트를 보냄 (실제로 바로 갱신하지는 않음)
		void Suspend(void);											// 렌더링 일시 정지
		bool IsSuspended(void) const;								// 렌더링 일시 정지 여부 확인
		void Resume(void);											// 렌더링 계속함

		const DKSize& ScreenResolution(void) const;

		// 좌표 변환
		DKPoint WindowToScreen(const DKPoint& pt) const;
		DKPoint ScreenToWindow(const DKPoint& pt) const;
		DKSize WindowToScreen(const DKSize& size) const;
		DKSize ScreenToWindow(const DKSize& size) const;
		DKRect WindowToScreen(const DKRect& rect) const;
		DKRect ScreenToWindow(const DKRect& rect) const;

		void RegisterAutoUnloadFrame(DKFrame* frame);
		void UnregisterAutoUnloadFrame(DKFrame* frame);

		void SetActiveFrameLatency(double f);
		void SetInactiveFrameLatency(double f);
		double ActiveFrameLatency(void) const;
		double InactiveFrameLatency(void) const;
		
	protected:
		void OnInitialize(void) override;
		void OnTerminate(void) override;
		void OnIdle(void) override;
		void PerformOperation(const DKFoundation::DKOperation* operation) override;

		void OnWindowEvent(DKWindow::EventWindow type, DKSize contentSize, DKPoint windowOrigin);
		void OnMouseEvent(DKWindow::EventMouse type, int deviceId, int buttonId, DKPoint pos, DKVector2 delta);
		void OnKeyboardEvent(DKWindow::EventKeyboard type, int deviceId, DKVirtualKey key, DKFoundation::DKString text);

	private:
		DKFoundation::DKObject<DKRenderer>		renderer;			// 렌더러
		DKFoundation::DKMap<int, DKFrame*>		keyboardHolders;	// 키 입력받는 프레임들
		DKFoundation::DKMap<int, DKFrame*>		mouseHolders;		// 마우스 입력받는 프레임들
		DKFoundation::DKObject<DKWindow>		window;				// 드로잉 윈도우
		DKFoundation::DKObject<DKFrame>			rootFrame;			// 루트 프레임

		DKFoundation::DKSet<DKFrame*, DKFoundation::DKSpinLock>		autoUnloadFrames;	// 자동으로 Unload 시킬 프레임들

		bool						suspended;
		bool						activated;
		bool						visible;
		double						tickDelta;
		DKFoundation::DKDateTime	tickDate;
		DKFoundation::DKTimeTick	tickCount;
		DKFoundation::DKTimer		timer;
		double						activeFrameLatency;
		double						inactiveFrameLatency;
		DKSize						screenResolution;

		DKFoundation::DKObject<DKOpenALContext> alContext;
		DKFoundation::DKObject<DKOpenGLContext> glContext;
		
		void RenderScreen(bool invalidate);						// 실제 스크린에 드로잉을 한다 (DKOpenGLContext가 바인딩된 쓰레드에서 호출해야한다)
	};
}

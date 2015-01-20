//
//  File: DKWindow.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKFoundation.h"
#include "DKSize.h"
#include "DKRect.h"
#include "DKVKey.h"

////////////////////////////////////////////////////////////////////////////////
// DKWindow
//
//	시스템의 윈도우를 제어한다. 자체적으로 윈도우를 생성할 수 있으며,
//	이미 존재하는 윈도우에 잠시 붙일수도 있다.
//
//	Create 를 호출하면 윈도우를 새로 생성하고
//	CreateProxy 를 호출하면 존재하는 윈도우에 임시로 생성이 된다.
//	Proxy 윈도우로 생성하면, 모든 이벤트는 수동으로 생성해 줘야 한다.
//	또한 좌표계도 시스템의 좌표계에서 DKScreen 좌표계로 변경해줘야 한다.
//	
//	좌표계: 왼쪽 하단이 0,0 
//
////////////////////////////////////////////////////////////////////////////////


namespace DKFramework
{
	class DKWindowInterface;
	class DKLIB_API DKWindow
	{
	public:
		DKWindow(void);
		~DKWindow(void);

		////////////////////////////////////////////////////////////////////////////////
		// 윈도우 스타일
		enum Style : int
		{
			StyleTitle = 1,
			StyleCloseButton = 1 << 1,
			StyleMinimizeButton = 1 << 2,
			StyleMaximizeButton = 1 << 3,
			StyleResizableBorder = 1 << 4,
			StyleAcceptFileDrop = 1 << 8,
			StyleGeneralWindow = 0xff,	// all above without StyleAcceptFileDrop
		};

		////////////////////////////////////////////////////////////////////////////////
		// 이벤트 정의
		enum EventMouse				
		{
			EventMouseNothing = 0,
			EventMouseDown,
			EventMouseUp,
			EventMouseMove,
			EventMouseWheel,
		};
		enum EventKeyboard			
		{
			EventKeyboardNothing = 0,
			EventKeyboardDown,					// 키가 눌려졌을때
			EventKeyboardUp,					// 키가 띄어졌을때
			EventKeyboardTextInput,				// 텍스트가 입력되었을때 (글자 입력)
			EventKeyboardTextInputCandidate,	// 텍스트가 입력중일때.
		};
		enum EventWindow
		{
			EventWindowCreated = 0,
			EventWindowClosed,
			EventWindowHidden,
			EventWindowShown,
			EventWindowActivated,
			EventWindowInactivated,
			EventWindowMinimized,
			EventWindowMoved,				
			EventWindowResized,			
			EventWindowUpdate,
		};
		////////////////////////////////////////////////////////////////////////////////
		// 이벤트 핸들러 타입 정의
		typedef void WindowProc(EventWindow, DKSize, DKPoint);
		typedef void KeyboardProc(EventKeyboard, int, DKVirtualKey, DKFoundation::DKString);
		typedef void MouseProc(EventMouse, int, int, DKPoint, DKVector2);

		typedef DKFoundation::DKFunctionSignature<WindowProc> WindowEventHandler;
		typedef DKFoundation::DKFunctionSignature<KeyboardProc> KeyboardEventHandler;
		typedef DKFoundation::DKFunctionSignature<MouseProc> MouseEventHandler;

		////////////////////////////////////////////////////////////////////////////////
		// 윈도우 콜백
		struct WindowCallback
		{
			template <typename T> using Function = DKFoundation::DKObject<DKFoundation::DKFunctionSignature<T>>;
			using StringArray = DKFoundation::DKArray<DKFoundation::DKString>;

			Function<void (DKWindow*, const DKPoint&, const StringArray&)> filesDropped;
			Function<DKSize (DKWindow*)> contentMinSize;
			Function<DKSize (DKWindow*)> contentMaxSize;
			Function<bool (DKWindow*)> closeRequest;
		};

		////////////////////////////////////////////////////////////////////////////////
		// 윈도우 생성
		// origin 이 undefinedOrigin 이면 OS 기본값을 사용한다.
		// contentSize 가 1 보다 작으면 OS 기본값을 사용한다.
		static const DKPoint undefinedOrigin;
		static DKFoundation::DKObject<DKWindow> Create(const DKFoundation::DKString& title,		// 창 제목
													   const DKSize& contentSize,				// 윈도우 내부 크기
													   const DKPoint& origin = undefinedOrigin,	// 윈도우 원점 위치 (스크린상)
													   int style = StyleGeneralWindow,			// 윈도우 스타일
													   const WindowCallback& cb = WindowCallback());
		// 이미 존재하는 윈도우에 임시로 붙임. (프록시 윈도우)
		static DKFoundation::DKObject<DKWindow> CreateProxy(void* systemHandle);
		void UpdateProxy(void);										// 프록시 윈도우 상태 갱신
		bool IsProxy(void) const;
		void SetCallback(const WindowCallback& cb);
		const WindowCallback& Callback(void) const;

		void Close(void);
		void Show(void);
		void Hide(void);
		void Activate(void);								// 창을 활성화 한다.
		void Minimize(void);
		void Resize(const DKSize&);
		void Resize(const DKSize&, const DKPoint&);
		void SetOrigin(const DKPoint&);
		void SetTitle(const DKFoundation::DKString&);
		DKFoundation::DKString Title(void) const;

		////////////////////////////////////////////////////////////////////////////////
		// 이벤트 관련
		void AddObserver(void* context, WindowEventHandler*, KeyboardEventHandler*, MouseEventHandler*, DKFoundation::DKRunLoop*);
		void RemoveObserver(void* context);

		////////////////////////////////////////////////////////////////////////////////
		// 키보드 관련
		bool IsTextInputEnabled(int deviceId) const;							// 텍스트 입력모드가 활성화 되었는지 여부 확인
		void SetTextInputEnabled(int deviceId, bool enabled);					// 텍스트 입력 on/off
		bool KeyState(int deviceId, const DKVirtualKey& k) const;				// 키 상태 가져온다.
		void SetKeyState(int deviceId, const DKVirtualKey& k, bool down);
		void ResetKeyState(int deviceId);										// 모든 키를 눌려지지 않은 상태로 초기화
		void ResetKeyStateForAllDevices(void);
		static DKFoundation::DKString GetVKName(DKVirtualKey lKey);				// 디버깅용 가상키 이름 가져오기

		////////////////////////////////////////////////////////////////////////////////
		// 마우스 관련.
		void ShowMouse(int deviceId, bool bShow);								// 마우스 보임
		void HoldMouse(int deviceId, bool bHold);								// 마우스 위치 고정 (이상태로 움직이면 delta 값이 전달된다)
		bool IsMouseVisible(int deviceId) const;
		bool IsMouseHeld(int deviceId) const;
		DKPoint MousePosition(int deviceId) const;
		void SetMousePosition(int deviceId, const DKPoint& pt);

		////////////////////////////////////////////////////////////////////////////////
		// 윈도우 정보 가져오기
		DKPoint Origin(void) const;					// 윈도우 창 원점 위치 (OS 유닛단위)
		DKSize ContentSize(void) const;				// 윈도우 내부 영역 크기 (픽셀 단위)
		double ContentScaleFactor(void) const;		// 내부 픽셀수 대비 OS 포인트 유닛 비율

		bool IsVisible(void) const					{return visible;}		// 윈도우가 화면에 표시되고 있는지
		bool IsActive(void) const					{return activated;}		// 윈도우가 활성 상태인지 (키 윈도우인지)
		bool IsValid(void) const;											// 유효한 윈도우인지 검사
		void* PlatformHandle(void) const;

		////////////////////////////////////////////////////////////////////////////////
		// 핸들러 호출.
		// 윈도우가 자체적으로 생성된것이면 자동으로 호출이 되지만, 외부 윈도우를 임시로 사용하는 경우엔
		// 수동으로 호출해줘야 한다.
		void PostMouseEvent(EventMouse type, int deviceId, int buttonId, const DKPoint& pos, const DKVector2& delta, bool sync = false);
		void PostKeyboardEvent(EventKeyboard type, int deviceId, DKVirtualKey key, const DKFoundation::DKString& textInput, bool sync = false);
		void PostWindowEvent(EventWindow type, const DKSize& contentSize, const DKPoint& windowOrigin, bool sync = false);

	private:
		DKFoundation::DKCallback<WindowProc, void*, DKFoundation::DKSpinLock>		windowEventHandlers;
		DKFoundation::DKCallback<KeyboardProc, void*, DKFoundation::DKSpinLock>		keyboardEventHandlers;
		DKFoundation::DKCallback<MouseProc, void*, DKFoundation::DKSpinLock>		mouseEventHandlers;

		// 내부 저장 변수들
		struct KeyboardState
		{
			bool textInputEnabled;								// 텍스트 입력 사용 여부
			unsigned char keyStateBits[DKVK_MAXVALUE / 8 + 1];	// raw 키 입력 상태 저장 비트 (1:키다운, 0:키업)
		};
		mutable DKFoundation::DKMap<int, KeyboardState>		keyboardStateMap;
		KeyboardState& GetKeyboardState(int deviceId) const;	// Get key states without lock
		DKFoundation::DKSpinLock keyboardLock;

		DKPoint origin;		// 윈도우 원점 좌표 (테두리 포함한 스크린상 좌표)
		DKSize contentSize;			// 콘텐츠 크기
		bool activated;				// true 면 윈도우가 활성상태
		bool visible;				// true 면 윈도우가 화면에 표시되고 있음.

		WindowCallback callback;
		DKWindowInterface*	impl;
	};
}

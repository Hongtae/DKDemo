//
//  File: DKFrame.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKMatrix3.h"
#include "DKPoint.h"
#include "DKSize.h"
#include "DKRect.h"
#include "DKRenderTarget.h"
#include "DKTexture.h"
#include "DKRenderer.h"
#include "DKColor.h"
#include "DKBlendState.h"
#include "DKWindow.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKFrame
//
// 사용자 정의 렌더링은 이 클래스를 상속받아서 만든다.
// 계층구조를 이루고 있으며 모든 이벤트 핸들링은 여기서 하게 된다.
// 키보드 이벤트 및 마우스 이벤트를 받으려면 EnableKeyInput / EnableMouseInput 을 호출하여야 한다.
//
// * 모든 이벤트는 DKScreen 의 쓰레드에서 호출된다.
// 마우스 이벤트 : 마우스를 선점되지 않은 상태에선 마우스 위치에 따라 이벤트를 받는다. (Track,Wheel 은 선점 필요)
// 키보드 이벤트 : 키보드는 선점하여야 이벤트를 받게된다. CaptureKeyboard
//
// 업데이트 순서 : 부모->자식, (부모가 업데이트 할때 자식에게 값을 전달할 수 있다.)
// 렌더링 순서 : 자식->부모, (부모가 렌더링 할 때, 자식의 서피스를 가져올 수 있다.)
//
// 좌표계: 왼쪽 하단 0,0 - 우측 상단 1,1
//
// 계층구조를 이루지 않고 Offscreen 으로 사용할수도 있다. 이 경우 마우스 이벤트는 받을수 없다.
// 계층구조를 이루고 있으면 상위 프레임이 그려질때 자동으로 그려지게 된다.
//
// 프레임은 사용전 Load 를 호출하여 Screen 과 초기화 해야한다.
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKScreen;
	class DKLIB_API DKFrame
	{
		friend class DKScreen;
	public:
		typedef DKFoundation::DKArray<DKFoundation::DKObject<DKFrame>> FrameArray;

		DKFrame(void);
		virtual ~DKFrame(void);

		// 스크린 객체 조회 (offscreen 객체이면 NULL)
		DKScreen*		Screen(void);
		const DKScreen*	Screen(void) const;

		// 계층 구조 설정,해제
		bool			AddSubframe(DKFrame* frame);		// 자식 프레임을 붙인다. 
		void			RemoveSubframe(DKFrame* frame);		// 자식 프레임을 제거한다.
		void			RemoveFromSuperframe(void);			// 부모로부터 떨어진다.
		// 자식 프레임 순서 변경
		bool			BringSubframeToFront(DKFrame*);		// 자식 프레임을 제일 앞으로 가져온다.
		bool			SendSubframeToBack(DKFrame*);		// 자식 프레임을 맨 뒤로 보낸다.

		// 계층 구조 조회
		FrameArray		Subframes(void) const;
		DKFrame*		Superframe(void);
		const DKFrame*	Superframe(void) const;
		DKFrame*		SubframeAtIndex(unsigned int index);
		const DKFrame*	SubframeAtIndex(unsigned int index) const;
		bool			IsDescendantOf(const DKFrame* frame) const;		// 이 객체가 frame 과 같거나 frame 의 서브프레임이면 true
		size_t			NumberOfSubframes(void) const;
		size_t			NumberOfDescendants(void) const;

		// 프레임의 위치 및 모양 변경 (일반 트랜스폼은 정규화된 좌표계를 기준으로 함)
		void				SetTransform(const DKMatrix3& transform);	// 프레임 위치 변경 (상위프레임 좌표계상)
		const DKMatrix3&	Transform(void) const;						// 정규화된 로컬 좌표계 -> 상위프레임 좌표계
		const DKMatrix3&	TransformInverse(void) const;				// 상위프레임 좌표계 -> 정규화된 로컬 좌표계
		// 로컬 좌표계에서의 좌표변경 행렬 (스케일이 적용됨)
		DKMatrix3			LocalFromRootTransform(void) const;			// 로컬 좌표계 -> 최상위 프레임 좌표계
		DKMatrix3			LocalToRootTransform(void) const;			// 최상위 프레임 좌표계 -> 로컬 좌표계
		DKMatrix3			LocalFromSuperTransform(void) const;		// 로컬 좌표계 -> 부모 좌표계
		DKMatrix3			LocalToSuperTransform(void) const;			// 부모 좌표계 -> 로컬 좌표계

		// 프레임 컨텐츠 리사이즈
		// QueryContentResolution: 프레임 해상도 리턴, 특정 사이즈만 사용하려면 재정의 해야한다.
		virtual DKSize		QueryContentResolution(void) const;			// 프레임 해상도 변경을 제어할려면 재정의, 루트 프레임은 호출되지 않음.
		const DKSize&		ContentResolution(void) const;
		void				UpdateContentResolution();					// 해상도를 갱신한다. (스케일이나 트랜스폼이 변경된 후 호출함)

		// 컨텐츠 영역 확인 (특수한 모양의 프레임일 경우 재정의 해서 사용함)
		// false 를 리턴하면 마우스 이벤트가 부모로 전달됨. (기본은 true)
		virtual bool HitTest(const DKPoint& pos) const			{ return true; }
		// 자식(subframe)으로 메시지를 전달하는지 여부 false 리턴하면 자식으로 메시지가 가지 않음.
		virtual bool ContentHitTest(const DKPoint& pos) const	{ return true; }

		// 내부 좌표계 및 드로잉 스케일 단위 변환
		const DKSize&		ContentScale(void) const;
		void				SetContentScale(const DKSize& s);
		void				SetContentScale(float w, float h);
		DKRect				Bounds(void) const;					// 컨텐츠 좌표계 영역 리턴함 (0,0,scale.width,scale.height)
		DKRect				DisplayBounds(void) const;			// 화면에 표시되는 영역 리턴

		// 내부 좌표계 트랜스폼
		void				SetContentTransform(const DKMatrix3& m);
		const DKMatrix3&	ContentTransform(void) const;
		const DKMatrix3&	ContentTransformInverse(void) const;

		// 좌표 변환 (계층 구조간)
		DKPoint		LocalToSuper(const DKPoint& pt) const;
		DKPoint		SuperToLocal(const DKPoint& pt) const;

		// 좌표 변환 (로컬좌표 <-> 픽셀좌표)
		// 주의: 픽셀 단위로 변경할때는 contentTransform 을 적용하지 않음.
		DKPoint		LocalToPixel(const DKPoint& pt) const;
		DKPoint		PixelToLocal(const DKPoint& pt) const;
		DKSize		LocalToPixel(const DKSize& size) const;
		DKSize		PixelToLocal(const DKSize& size) const;
		DKRect		LocalToPixel(const DKRect& rect) const;
		DKRect		PixelToLocal(const DKRect& rect) const;

		// 키보드 마우스 선점
		bool		CaptureKeyboard(int deviceId);				// 키보드 선점
		bool		CaptureMouse(int deviceId);					// 마우스 선점
		void		ReleaseKeyboard(int deviceId);				// 키보드 선점 해제
		void		ReleaseMouse(int deviceId);					// 마우스 선점 해제
		void		ReleaseAllKeyboardsCapturedBySelf(void);	// 모든 키보드 선점 해제
		void		ReleaseAllMiceCapturedBySelf(void);			// 모든 마우스 선점 해제

		// 키보드 마우스 선점 상태 확인
		bool		IsKeybaordCapturedBySelf(int deviceId) const;
		bool		IsMouseCapturedBySelf(int deviceId) const;
		// 마우스 위치 가져오기
		DKPoint		MousePosition(int deviceId) const;
		bool		IsMouseHover(int deviceId) const;

		// 렌더링 관련
		void	SetRedraw(void) const;
		// 해당 프레임의 텍스쳐, (offscreen 일때 다른 프레임에서 사용가능함)
		const DKTexture2D* Texture(void) const;

		// Render: DKScreen::RootFrame() 의 자손 프레임의 경우 화면에 보여질때 자동으로 호출된다.
		// 자손 프레임이 아닌 독립된 프레임의 경우 직접 호출해줘야 한다. SetRedraw() 를 호출해야 새로 갱신한다.
		void Render(void) const;			// 자신의 contents 영역을 렌더링
		void Update(double tickDelta, DKFoundation::DKTimeTick tick, const DKFoundation::DKDateTime& tickDate);

		// Load, Unload 는 계층구조에 영향을 주지 않음. (주의:어디에도 속하지 않은 프레임의 경우 제거할때 꼭 Unload 를 해줘야한다!)
		// 이미 Load 된 프레임(DKFrame)에 자식으로 붙이면(AddSubframe) 자동으로 Load 된다. 부모프레임이 Unload 되면 자동으로 Unload 된다.
		void Load(DKScreen* screen, const DKSize& resolution);
		void Unload(void);
		bool IsLoaded(void) const;

		// 색상 및 블렌딩 제어
		void			SetColor(const DKColor& color);
		DKColor			Color(void) const;
		void					SetBlendState(const DKBlendState& blend);
		const DKBlendState&		BlendState(void) const;

		// 표시 여부, 활성화 여부 (비활성일때는 이벤트를 받지 않음)
		bool IsHidden(void) const		{return hidden;}
		bool IsEnabled(void) const		{return enabled;}
		void SetHidden(bool hidden);
		void SetEnabled(bool enabled);

		bool CanHandleKeyboard(void) const;
		bool CanHandleMouse(void) const;
		bool IsVisibleOnScreen(void) const;		// 자신의 hidden 여부와 상관 없이 스크린 상에 보여지는지 확인

		// 프레임의 depth-buffer 포맷
		void SetDepthFormat(DKRenderTarget::DepthFormat fmt);
		DKRenderTarget::DepthFormat DepthFormat(void) const;

		// Surface-Visibility-Test, 전체를 가리는 서브프레임이 있을경우, 드로잉 여부 결정
		// Renderer의 viewport 를 수정하거나 offset 을 설정하면 false 로 설정해야 한다.
		// true 일 경우, 자식프레임이 부모(this)를 다 가리게 되면 부모(this)는 그리지 않음.
		void SetSurfaceVisibilityTest(bool enabled);
		bool IsSurfaceVisibilityTestEnabled(void) const;

		void DiscardSurface(void);				// 렌더타겟 릴리즈 (필요할때 다시 생성됨)

	protected:
		// 프레임 이벤트
		virtual void OnRender(DKRenderer&) const;													// 프레임이 그려질때 호출됨
		virtual void OnUpdate(double, DKFoundation::DKTimeTick, const DKFoundation::DKDateTime&) {}	// 프레임이 갱신될때 호출됨
		virtual void OnLoaded(void) {}																// 프레임이 로딩되면 호출됨
		virtual void OnUnload(void) {}																// 프레임이 언로드 되어야할때 호출됨
		virtual void OnContentResized(void) {}														// 프레임 컨텐트의 해상도가 변경되었을때 호출된다.

		// 마우스 이벤트
		virtual void OnMouseDown(int deviceId, int buttonId, const DKPoint& pos) {}					// 마우스 버튼이 눌려짐
		virtual void OnMouseUp(int deviceId, int buttonId, const DKPoint& pos) {}					// 마우스 버튼이 올라감
		virtual void OnMouseMove(int deviceId, const DKPoint& pos, const DKVector2& delta) {}		// 마우스가 이동할때 호출됨.
		virtual void OnMouseWheel(int deviceId, const DKPoint& pos, const DKVector2& delta) {}		// 마우스 휠이 움직임
		virtual void OnMouseHover(int deviceId) {}													// 마우스가 프레임 영역에 들어옴
		virtual void OnMouseLeave(int deviceId) {}													// 마우스가 프레임 영역에서 벗어남
		virtual void OnMouseLost(int deviceId) {}

		// 키 입력 이벤트
		virtual void OnKeyDown(int deviceId, DKVirtualKey key) {}									// 키가 눌려짐
		virtual void OnKeyUp(int deviceId, DKVirtualKey key) {}										// 키가 올라감
		virtual void OnTextInput(int deviceId, const DKFoundation::DKString& str) {}				// 글자가 입력됨.
		virtual void OnTextInputCandidate(int deviceId, const DKFoundation::DKString& str) {}		// 글자 입력중
		virtual void OnKeyboardLost(int deviceId) {}

		// 이벤트 전처리. 메시지는 true 를 리턴할때 까지 부모 프레임으로 이동함.
		virtual bool PreprocessMouseEvent(DKWindow::EventMouse type, int deviceId, int buttonId, const DKPoint& pos, const DKVector2& delta)
		{
			return false;
		}
		virtual bool PreprocessKeyboardEvent(DKWindow::EventKeyboard type, int deviceId, DKVirtualKey key, const DKFoundation::DKString& text)
		{
			return false;
		}

	private:
		DKMatrix3		transform;				// 정규화된 로컬 좌표계에서 부모 좌표계로 변환
		DKMatrix3		transformInverse;		// 부모 좌표계에서 정규화된 로컬 좌표계로 변환

		FrameArray		subframes;
		DKFrame*		superframe;
		DKScreen*		screen;

		DKSize			contentResolution;
		DKSize			contentScale;			// 내부 좌표계 스케일 (정규화 한뒤에 스케일 곱해야함)
		DKMatrix3		contentTransform;		// 내부 트랜스폼 (스케일 적용된 후, 트랜스폼 적용됨)
		DKMatrix3		contentTransformInverse;
		DKColor::RGBA32 color;
		DKBlendState	blendState;

		DKFoundation::DKObject<DKRenderer>		renderer;
		DKFoundation::DKMap<int, bool>			mouseHover;			// 마우스 호버 상태 저장
		DKRenderTarget::DepthFormat				depthFormat;		// depth-buffer 포맷

		bool			loaded: 1;
		bool			hidden: 1;
		bool			enabled: 1;
		bool			enableVisibilityTest:1;
		mutable bool	drawSurface: 1;

		bool RenderInternal(void);											// 실제로 그려졌을때 true 를 리턴함.
		bool InsideFrameRect(bool* covered, const DKRect& rect, const DKMatrix3& tm) const;		// 부모 영역 안에 포함되는지 확인.
		bool ProcessKeyboardEvent(DKWindow::EventKeyboard type, int deviceId, DKVirtualKey key, const DKFoundation::DKString& text);
		bool ProcessMouseEvent(DKWindow::EventMouse type, int deviceId, int buttonId, const DKPoint& pos, const DKVector2& delta, bool propagate);
		bool ProcessMouseInOut(int deviceId, const DKPoint& pos, bool insideParent);
		void ReleaseMouseData(void);
	};
}

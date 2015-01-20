//
//  File: DKApplication.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKWindow.h"
#include "DKScreen.h"
#include "DKMatrix3.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKApplication
//
// 내부적으로 DKWindow 와 DKScreen 을 생성하고 초기화 한다.
// 이 객체를 사용하는것은 필수는 아니지만 크로스 플랫폼 환경을 맞춰주기 때문에 이 객체를 사용하는것을 추천함.
// 이 클래스를 사용하지 않으려면 DKWindow 와 DKScreen 을 직접 생성하고 초기화 해줘야 한다.
//
// 이 객체는 응용프로그램 프로세스 관련 이벤트만 핸들링 한다.
//
// Note:
// 이 객체는 하나의 App 에서는 단 하나의 인스턴스만 가질 수 있다.
// 프록시 윈도우를 사용하는 어플리케이션의 경우엔 이 클래스를 사용할 수 없다. (wxWidgets, MFC 등)
//
// Application 인스턴스 종료 (DKApplication::Terminate)
// 기본적으로 Terminate 함수는 이 인스턴스의 RunLoop 를 종료하지만,
// iOS 버전에서는 프로세스가 종료된다.
////////////////////////////////////////////////////////////////////////////////

#ifdef GetUserName
#undef GetUserName
#endif

namespace DKFramework
{
	class DKApplicationInterface;
	class DKLIB_API DKApplication
	{
		friend class DKApplicationInterface;

	public:
		enum SystemPath
		{
			SystemPathSystemRoot = 0,		// 시스템 루트 (윈도우즈는 부트 볼륨)
			SystemPathAppRoot,				// 실행파일의 루트 디렉토리
			SystemPathAppResource,			// 응용프로그램의 리소스 디렉토리
			SystemPathAppExecutable,		// 실행파일이 있는 디렉토리
			SystemPathAppData,				// 응용프로그램의 데이터 폴더 (공통)
			SystemPathUserHome,				// 해당 유저의 홈 폴더
			SystemPathUserDocuments,		// 해당 유저의 문서폴더
			SystemPathUserPreferences,		// 해당 유저의 설정 폴더 (세팅 파일 저장용)
			SystemPathUserCache,			// 해당 유저의 캐시 폴더
			SystemPathUserTemp,				// 해당 유저의 임시 폴더
		};


		DKApplication(void);
		DKApplication(int argc, char* argv[]);
		virtual ~DKApplication(void);

		void SetArgs(int argc, char* argv[]);

		// 어플리케이션 메인 루프
		int Run();
		// 인스턴스
		static DKApplication* Instance(void);
		// 어플리케이션 종료
		void Terminate(int exitCode);
		// 미리 정의된 경로 가져오기
		DKFoundation::DKString EnvironmentPath(SystemPath);
		// 실행파일 경로 가져오기
		DKFoundation::DKString ModulePath(void);

		// 어플리케이션 헬퍼 함수들
		DKFoundation::DKObject<DKFoundation::DKData> LoadResource(const DKFoundation::DKString& res, DKFoundation::DKAllocator& alloc = DKFoundation::DKAllocator::DefaultAllocator());		// read-writable
		DKFoundation::DKObject<DKFoundation::DKData> LoadStaticResource(const DKFoundation::DKString& res);	// read-only

		DKRect DisplayBounds(int displayId) const;			// 시스템 모니터의 해상도 가져온다. (참고용으로만 써야한다)
		DKRect ScreenContentBounds(int displayId) const;	// 윈도우 프레임이 표시될 수 있는 영역을 가져온다. (아이폰의 경우 이 크기로 생성하면 된다)

		// 어플리케이션 속성 함수들
		DKFoundation::DKString HostName(void) const;
		DKFoundation::DKString OSName(void) const;
		DKFoundation::DKString UserName(void) const;

	protected:
		virtual void OnHidden(void);					// 응용프로그램이 hide 되었다. (minimized 가 아님)
		virtual void OnRestore(void);					// 응용프로그램이 restore 되었다.
		virtual void OnActivated(void);					// 응용프로그램이 activated 되었다.
		virtual void OnDeactivated(void);				// 응용프로그램이 deactivated 되었다.
		virtual void OnInitialize(void);				// 응용프로그램이 초기화 되었다.
		virtual void OnTerminate(void);					// 응용프로그램이 종료되어야 한다.

	private:
		void Initialize();
		void Finalize();
		DKFoundation::DKDateTime			initializedAt;
		DKFoundation::DKArray<char*>		args;
		DKFoundation::DKMutex				mutex;		// 중복 실행을 막기 위한 뮤텍스 (Run 은 중복실행되면 안된다)
		DKApplicationInterface*				impl;
	};
}

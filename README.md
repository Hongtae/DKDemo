# README #

DKDemo 프로젝트는 DKLib 데모용 샘플 입니다.

이 데모의 로직은 모두 파이썬 스크립트로 되어있으며, 실행 파일을 빌드 할 수 있는 프로젝트 파일을 포함하고 있습니다. (Visual Studio 2013 / Xcode6 )

* `DemoApp` *데모 프로그램 소스코드 디렉토리*
* `DKLib` *엔진 디렉토리*
    * `DKLib/DK`  *코어 엔진 헤더파일 폴더 (C++)*
        * `DKLib/DK/DKFoundation` *템플릿, 파일, 쓰레딩, 데이터 구조 등, 기반 라이브러리*
        * `DKLib/DK/DKFramework` *그래픽, 오디오, 수학, 복셀 등, 어플리케이션 라이브러리*
    * `DKLib/PyDK` *파이썬 바인딩 모듈 헤더파일 폴더 (C++)*
    * `DKLib/Bin` *플랫폼 별 바이너리 폴더*
* `DKDemo.sln` *Visual Studio 2013 솔루션 파일*
* `DKDemo.xcworkspace` *Xcode 6 웍스페이스 파일*

## 윈도우즈용 실행파일(샘플) 다운로드.  (2015년 1월 22일 빌드)
https://bitbucket.org/tiff2766/dkdemo/downloads/DKDemoApp_Win32.zip

# About DKLib
크로스 플랫폼 게임 제작용 라이브러리 입니다. 개인 프로젝트로 혼자 작업하고 있으며, C++ 로 제작되었습니다. 데모 프로젝트에는 빌드에 필요한 헤더파일 및 DLL / framework 등을 포함하고 있으며, 컨텐츠는 모두 스크립트로 제작되었습니다.

## 주요 기능
* 3D 오디오
* 게임 물리
* 데이터 마샬링
* 애니메이션 리타게팅, 블렌딩.
* 모듈러 디자인
* 복셀 및 볼륨렌더링
* 파이썬 스크립팅 (PyDK 모듈)

## 실행 플랫폼
* 윈도우 비스타 이상 (32, 64비트)
* Mac OS X 10.8 이상
* iOS 4.0 이상
> 데모 버전에서는 용량 관계상 framework 형태로 배포 합니다. 따라서 framework 을 지원하는 iOS8 부터 실행이 가능합니다.
* Android 2.1 이상
* Linux

    **데모 버전에서는 Android 와 Linux 는 지원하지 않습니다.**

## 빌드 환경
* 윈도우
    * 비스타 이상
    * Visual Studio 2013
    > 모든 소스 파일이 UTF-8 로 인코딩 되어있기 때문에, 한글 윈도우에서 빌드시 UTF-8 로 빌드되는지 꼭 확인하세요. 한글 윈도우에서는 기본적으로 CP949 로 빌드됩니다. 영문 윈도우(Non-Unicode Locale 이 영어로 설정된) 는 상관 없습니다.
    > 
    > 프로젝트 저장소는 라인 엔딩이 LF 로 되어있습니다. 한글윈도우에서 UTF8 과 LF 를 같이 쓰면 빌드가 제대로 안되는것을 확인하였습니다. 압축 파일로 다운 받지 마시고, clone 하십시오.
* 맥
    * Mac OS X 10.8 이상
    * Xcode 6
    > 프레임웍에는 심볼릭 링크가 포함되어 있습니다. 윈도우에서 압축된 파일로 빌드하지 마세요.

## 하드웨어 요구 사항
* OpenGL 3.1 이상
* OpenGL ES 2.0 이상

## 제작
    Copyright (C) 2015, Hongtae Kim 김홍태

* 소스파일에 적힌 저작권 문구의 ICONDB.COM 은 제 개인 도메인입니다. (업체 아닙니다)
* 이 엔진은 아무나 마음대로 사용하실 수 있습니다만, 소스코드의 수정 및 코드 재판매는 불허합니다.
* tiff2766 골뱅이 gmail 닷컴

## 스크린샷 
### 캐릭터 애니메이션
![anim.png](https://bitbucket.org/repo/keLaoe/images/2465982228-anim.png)

* 애니메이션 제어 샘플
* 3개의 캐릭터, 3개의 애니메이션

### 자동차 물리
![car1.png](https://bitbucket.org/repo/keLaoe/images/1454161511-car1.png)
![car2.png](https://bitbucket.org/repo/keLaoe/images/736436422-car2.png)

* 키보드 방향키: 차량 조종, 마우스 좌클릭: 카메라 조정, 우클릭: 오브젝트 투척
* iOS 는 스크린 버튼으로 조종, 두손가락 터치시 오브젝트 투척

### 키네마틱 애니메이션
![kine1.png](https://bitbucket.org/repo/keLaoe/images/1827184730-kine1.png)
![kine2.png](https://bitbucket.org/repo/keLaoe/images/540183168-kine2.png)

* 마우스 좌클릭: 카메라 시점 조정, 우클릭: 오브젝트 투척
* iOS 에서는 두손가락 터치시 오브젝트 투척

### 쉐도우 맵 샘플
![sm1.png](https://bitbucket.org/repo/keLaoe/images/4172863041-sm1.png)
![sm2.png](https://bitbucket.org/repo/keLaoe/images/952550011-sm2.png)

* 스크립트로 제작된 LiSPSM 알고리즘
* 좌측 하단 슬라이더로 광원 조정.

### 텍스쳐 아틀라스 및 스프라이트
![sp.png](https://bitbucket.org/repo/keLaoe/images/605784757-sp.png)

* 2D 게임 제작용 스크립트 데모

### 게임용 UI 콘트롤
![ui.png](https://bitbucket.org/repo/keLaoe/images/3182995816-ui.png)

* 각종 UI 콘트롤 데모
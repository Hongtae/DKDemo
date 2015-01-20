# README #

DKDemo 프로젝트는 DKLib 데모용 샘플 입니다.

이 데모의 로직은 모두 파이썬 스크립트로 되어있으며, 실행 파일을 빌드 할 수 있는 프로젝트 파일을 포함하고 있습니다. (Visual Studio 2013 / Xcode6 )


# About DK
크로스 플랫폼 게임 제작용 라이브러리 입니다.
## 주요 기능
* 3D 오디오
* 게임 물리
* 데이터 마샬링
* 애니메이션 리타게팅, 블렌딩.
* 모듈러 디자인
* 파이썬 스크립팅 (PyDK 모듈)

## 빌드 환경
* 윈도우
    * 비스타 이상
    * Visual Studio 2013
    > 모든 소스 파일이 UTF-8 로 인코딩 되어있기 때문에, 한글 윈도우에서 빌드시 UTF-8 로 빌드되는지 꼭 확인하세요. 한글 윈도우에서는 기본적으로 CP949 로 빌드됩니다. 영문 윈도우(Non-Unicode Locale 이 영어로 설정된) 는 상관 없습니다.
* 맥
    * Mac OS X 10.8 이상
    * Xcode 6

## 실행 플랫폼
* 윈도우 비스타 이상 (32, 64비트)
* Mac OS X 10.8 이상
* iOS 4.0 이상
> 데모 버전에서는 용량 관계상 framework 형태로 배포 합니다. 따라서 framework 을 지원하는 iOS8 부터 실행이 가능합니다.
* Android 2.1 이상
* Linux

    **데모 버전에서는 Android 와 Linux 는 지원하지 않습니다.**

## 하드웨어 요구 사항
* OpenGL 3.1 이상
* OpenGL ES 2.0 이상

## 제작
    Copyright (C) 2015, Hongtae Kim
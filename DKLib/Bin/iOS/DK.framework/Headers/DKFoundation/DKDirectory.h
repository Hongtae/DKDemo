//
//  File: DKDirectory.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKinclude.h"
#include "DKObject.h"
#include "DKFile.h"
#include "DKString.h"
#include "DKArray.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKDirectory
//
// 디렉토리를 열고 하위 디렉토리 및 파일을 제어할 수 있는 객체
//
// Note: OpenDir(const DKString&) 으로 디렉토리를 열때는 절대경로여야 한다.
//
// Note: 실시간 모니터링은 지원하지 않음. (주기적으로 갱신해야한다)
//
// Note: 윈도우즈 버전에서의 '/' 디렉토리는 드라이브 레터를 포함하는 가상 디렉토리를 말한다.
// 즉, 'C:\' 는 '/C:\' 로도 표기할수 있다.
// SystemDirSystemRoot 는 시스템 부트 디스크 드라이브를 반환한다. (C:\)
// 다른 드라이브를 조하기 위해서는 '/' 디렉토리에서 시작하여야 한다.
// 따라서 윈도우 버전의 '/' 디렉토리는 무조건 읽기 전용이다.
//
// 맥 버전
//  '/' (root)
//   +---- 'Applications'
//   |      +--- 'Any Applications...'
//   +---- 'Users'
//   |      +--- 'Users Home Dirs...'
//   +---- 'Library'
//   ...
//
// 윈도우 버전
//  '/' (root)
//   +---- 'C:'
//   |      +--- 'Program Files'
//   |      +--- 'Windows'
//   |      \--- 'Any Sub Dirs in C:...'
//   +---- 'D:'
//   |      \--- 'Some Dirs in D:...'
//   +---- 'E:'
//   \---- 'F:'
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	class DKLIB_API DKDirectory
	{
	public:
		DKDirectory(void);
		~DKDirectory(void);

		static DKObject<DKDirectory> OpenDir(const DKString& path);
		static bool IsDirExist(const DKString& path);			// 절대경로 검사. (상대경로는 안됨)

		bool operator == (const DKDirectory& dir) const;
		bool operator != (const DKDirectory& dir) const;

		const DKString& AbsolutePath(void) const;									// 현재 경로 가져온다.
		DKString AbsolutePathOfFile(const DKString& name) const;					// 파일이 있으면 전체 경로 리턴
		DKString AbsolutePathOfSubdir(const DKString& name) const;					// 파일이 있으면 전체 경로 리턴
		DKObject<DKFile> OpenFile(const DKString& name, DKFile::ModeOpen mode, DKFile::ModeShare share) const;	// 파일을 연다. (해당 디렉토리에 있는 파일)
		DKObject<DKFile> OpenFile(unsigned int index, DKFile::ModeOpen mode, DKFile::ModeShare share) const;	// 파일 열기
		DKObject<DKData> MapFile(const DKString& name, size_t size, bool writable) const;		// 파일 매핑
		DKObject<DKData> MapFile(unsigned int index, size_t size, bool writable) const;			// 파일 매핑
		DKObject<DKDirectory> OpenSubdir(const DKString& name) const;		// 서브 디렉토리 오픈
		DKObject<DKDirectory> OpenSubdirAtIndex(unsigned int index) const;
		DKObject<DKDirectory> OpenParent(void) const;						// 상위 디렉토리 가져온다
		DKObject<DKDirectory> CreateDir(const DKString& name) const;		// 새 서브디렉토리 생성

		bool IsFileExist(const DKString& name) const;			// 특정 파일이 해당 디렉토리에 있는지 확인
		bool IsSubdirExist(const DKString& name) const;			// 특정 디렉토리가 해당 디렉토리에 있는지 확인
		bool IsValid(void) const;								// 디렉토리가 유효한지 확인
		bool IsReadable(void) const;							// 디렉토리가 읽기 가능한지 확인
		bool IsWritable(void) const;							// 디렉토리가 쓰기 가능한지 확인

		bool Reload(void) const;										// 디렉토리 목록을 갱신한다.

		size_t NumberOfFiles(void) const;								// 파일 갯수 가져오기 (갱신후 사용가능)
		size_t NumberOfSubdirs(void) const;								// 디렉토리 갯수 가져오기 (갱신후 사용가능)
		const DKString& FileNameAtIndex(unsigned int index) const;		// 파일명 순서대로 가져오기 (갱신후 사용가능)
		const DKString& SubdirNameAtIndex(unsigned int index) const;	// 디렉토리명 순서대로 가져오기 (갱신후 사용가능)

		bool DeleteSubDir(const DKString& name) const;				// 해당 디렉토리 제거

	private:
		typedef DKArray<DKString> StringList;
		void ReloadIfNeeded(void) const;

		DKString currentPath;		// 현재 경로 (절대경로여야 한다)
		StringList files;			// 현재 경로의 파일들 (update 해야 갱신)
		StringList directories;		// 현재 경로의 서브 디렉토리들 (update 해야 갱신)
		bool reloadRequired;
	};	
}


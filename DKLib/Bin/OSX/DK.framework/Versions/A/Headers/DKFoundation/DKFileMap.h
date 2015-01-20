//
//  File: DKFileMap.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKinclude.h"
#include "DKObject.h"
#include "DKSpinLock.h"
#include "DKString.h"
#include "DKData.h"
#include "DKStream.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKFileMap
//
// Random Access 가능하도록 메모리에 매핑한다.
// 큰 파일을 여러번 랜덤 억세스 할 경우에 효과가 좋다.
// 하지만, 처음 매핑할때 사이즈로 제한된다.
//
// 파일을 일부분 매핑하려면 DKFile::MapContentRange 를 사용할것.
//
// 다음과 같은 모드를 지원함.
// - 특정 파일 읽기 전용 (파일 사이즈 or 원하는 사이즈)
// - 특정 파일 읽고 쓰기 (파일 사이즈 or 원하는 사이즈)
// - 특정 파일 내용 삭제 후, 읽고 쓰기 (원하는 사이즈 생성)
// - 익명 파일 읽고 쓰기, 사용후 자동 삭제 (원하는 사이즈 생성)
//
// 주의:
// - 파일이 네트웍 드라이브나, 외장 디스크에 있을경우 사용하면 안됨.
// - 다른 프로세서와 공유할 수 없음.
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	class DKLIB_API DKFileMap : public DKData
	{
	public:
		DKFileMap(void);
		~DKFileMap(void);

		// Open: 존재하는 파일을 연다. (size=0 이면 파일 사이즈대로 연다)
		static DKObject<DKFileMap> Open(const DKString& file, size_t size, bool writable);
		// Create: 파일을 새로 생성한다. overwrite 가 false 이며, 파일이 존재하면 실패함
		static DKObject<DKFileMap> Create(const DKString& file, size_t size, bool overwrite);
		// Temporary: 임시 파일을 생성해서 매핑한다
		static DKObject<DKFileMap> Temporary(size_t size);
		// Virtual: 페이지 메모리를 사용한다.
		static DKObject<DKFileMap> Virtual(size_t size);

		bool IsReadable(void) const;
		bool IsWritable(void) const;
		bool IsExcutable(void) const;
		size_t Length(void) const;

	protected:
		void* LockContent(void);
		void UnlockContent(void);

	private:
		void* mapContext;

		DKFileMap(const DKFileMap&);
		DKFileMap& operator = (const DKFileMap&);
	};
}

//
//  File: DKFile.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKinclude.h"
#include "DKStream.h"
#include "DKString.h"
#include "DKBuffer.h"
#include "DKDateTime.h"

namespace DKFoundation
{
	class DKLIB_API DKFile : public DKStream
	{
	public:
		struct FileInfo
		{
			unsigned long long	size;
			DKDateTime			lastAccessed;
			DKDateTime			lastModified;
			DKDateTime			created;
			bool 				readable;
			bool 				writable;
		};
		enum ModeOpen
		{
			ModeOpenNew = 0,		// if file exist, truncate zero (create + truncate + read + write)
			ModeOpenCreate,			// create new, error if file exist (create + read + write)
			ModeOpenExisting,		// if file not exist error. (read + write)
			ModeOpenReadOnly,		// open exist file read-only (read)
			ModeOpenAlways,			// always open, even if not exist. (read + write)
		};
		enum ModeShare
		{
			ModeShareAll = 0,
			ModeShareRead,
			ModeShareExclusive,
		};

		DKFile(void);
		~DKFile(void);

		static bool GetInfo(const DKString& file, FileInfo& info);
		static bool Delete(const DKString& file);

		static DKObject<DKFile> Create(const DKString& file, ModeOpen mod, ModeShare share);
		static DKObject<DKFile> CreateTemporary(void);	// 임시파일 생성한다. 객체가 소멸되면 파일도 지워짐.

		Position SetPos(Position p);
		Position GetPos(void) const;
		Position RemainLength(void) const;
		Position TotalLength(void) const;

		DKObject<DKBuffer> Read(size_t s, DKAllocator& alloc = DKAllocator::DefaultAllocator()) const;		// 파일을 읽고 DKBuffer 반환
		size_t Read(void* p, size_t s) const;		// 버퍼 에 파일을 읽음.
		size_t Read(void* p, size_t s);				// DKStream::Read 재정의함수
		size_t Read(DKStream* p, size_t s) const;	// 스트림에 에 파일을 기록함.

		size_t Write(const void* p, size_t s);
		size_t Write(const DKData *p);
		size_t Write(DKStream* s);

		bool GetInfo(FileInfo& info) const;			// 이미 연 파일의 정보
		FileInfo GetInfo(void) const;

		bool SetLength(size_t len);

		bool IsReadable(void) const;
		bool IsWritable(void) const;
		bool IsSeekable(void) const;

		const DKString& Path(void) const;

		// file-mapped 데이터 
		DKObject<DKData> MapContentRange(size_t offset, size_t length);
	
	private:
		DKString	path;
		intptr_t	file;
		ModeOpen	modeOpen;
		ModeShare	modeShare;

		// 복사 막기 위해 사용함
		DKFile(const DKFile&);
		DKFile& operator = (const DKFile&);
	};
}

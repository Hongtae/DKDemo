//
//  File: DKBuffer.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKObject.h"
#include "DKSpinLock.h"
#include "DKString.h"
#include "DKData.h"
#include "DKStream.h"
#include "DKAllocator.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKBuffer
//
// URL 이나 메모리에서 데이터 객체를 생성한다. base64, 압축 기능등을 지원함
// thread-safe 함.
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	class DKLIB_API DKBuffer : public DKData
	{
	public:
		DKBuffer(DKAllocator& alloc = DKAllocator::DefaultAllocator());
		DKBuffer(const DKData* p, DKAllocator& alloc = DKAllocator::DefaultAllocator());
		DKBuffer(const void* p, size_t s, DKAllocator& alloc = DKAllocator::DefaultAllocator());	// p 가 NULL 이면 s 만큼 버퍼 생성함 (0 으로 초기화됨)
		DKBuffer(const DKBuffer&);
		DKBuffer(DKBuffer&&);
		~DKBuffer(void);
		
		size_t Length(void) const;
		size_t CopyContent(void* p, size_t offset, size_t length) const;

		DKObject<DKBuffer> Compress(DKAllocator& alloc = DKAllocator::DefaultAllocator()) const;
		DKObject<DKBuffer> Decompress(DKAllocator& alloc = DKAllocator::DefaultAllocator()) const;
		static DKObject<DKBuffer> Compress(const void* p, size_t len, DKAllocator& alloc = DKAllocator::DefaultAllocator());
		static DKObject<DKBuffer> Decompress(const void* p, size_t len, DKAllocator& alloc = DKAllocator::DefaultAllocator());

		bool Encode(DKStringU8& strOut) const;
		bool Encode(DKStringW& strOut) const;
		static DKObject<DKBuffer> Decode(const DKStringU8& str, DKAllocator& alloc = DKAllocator::DefaultAllocator());
		static DKObject<DKBuffer> Decode(const DKStringW& str, DKAllocator& alloc = DKAllocator::DefaultAllocator());

		bool CompressEncode(DKStringU8& strOut) const;					// 압축 후 base64 인코딩
		bool CompressEncode(DKStringW& strOut) const;					// 압축 후 base64 인코딩
		static DKObject<DKBuffer> DecodeDecompress(const DKStringU8& s, DKAllocator& alloc = DKAllocator::DefaultAllocator());	// base64 디코딩후 압축 해제
		static DKObject<DKBuffer> DecodeDecompress(const DKStringW& s, DKAllocator& alloc = DKAllocator::DefaultAllocator());	// base64 디코딩후 압축 해제
		
		// 파일이나 URL 을 통해 데이터를 읽어온다. (통째로 읽음)
		static DKObject<DKBuffer> Create(const DKString& url, DKAllocator& alloc = DKAllocator::DefaultAllocator());
		static DKObject<DKBuffer> Create(const void* p, size_t s, DKAllocator& alloc = DKAllocator::DefaultAllocator());
		static DKObject<DKBuffer> Create(const DKData* p, DKAllocator& alloc = DKAllocator::DefaultAllocator());
		static DKObject<DKBuffer> Create(DKStream* s, DKAllocator& alloc = DKAllocator::DefaultAllocator());
		
		size_t SetContent(const void* p, size_t s);		// p 가 NULL 이면 s 만큼 버퍼 생성함 (0 으로 초기화됨)
		size_t SetContent(const DKData* p);
		
		DKBuffer& operator = (const DKBuffer&);
		DKBuffer& operator = (DKBuffer&&);
		
		bool IsReadable(void) const		{return true;}
		bool IsWritable(void) const		{return true;}
		bool IsExcutable(void) const	{return false;}
	
		void SwitchAllocator(DKAllocator& alloc);
		DKAllocator& Allocator(void) const;

	protected:
		void* LockContent(void);
		void UnlockContent(void);

	private:
		void*	contentPtr;
		size_t	contentLength;
		DKAllocator* allocator;
	};
}

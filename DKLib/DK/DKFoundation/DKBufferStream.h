//
//  File: DKBufferStream.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKinclude.h"
#include "DKStream.h"
#include "DKBuffer.h"
#include "DKObject.h"
#include "DKDataStream.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKBufferStream
//
// 내부적으로 DKBuffer 를 사용하는 메모리 스트림
// 스트림 형태로 DKBuffer 에 데이터를 기록할수 있도록 한다.
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	class DKLIB_API DKBufferStream : public DKDataStream
	{
	public:
		DKBufferStream(void);
		DKBufferStream(DKBuffer&);
		DKBufferStream(DKBuffer*);
		~DKBufferStream(void);

		Position SetPos(Position p);
		Position GetPos(void) const;
		Position RemainLength(void) const;
		Position TotalLength(void) const;

		size_t Read(void* p, size_t s);
		size_t Write(const void* p, size_t s);

		bool IsReadable(void) const {return true;}
		bool IsSeekable(void) const {return true;}
		bool IsWritable(void) const {return true;}

		void ResetStream(const void* p, size_t s);
		void ResetStream(DKBuffer*);

		DKData* DataSource(void);					// 내부에서 사용하는 DKData 리턴
		const DKData* DataSource(void) const;		// 내부에서 사용하는 DKData 리턴
		DKBuffer* BufferObject(void);				// 내부에서 사용하는 DKBuffer 리턴
		const DKBuffer* BufferObject(void) const;	// 내부에서 사용하는 DKBuffer 리턴
	private:
		DKObject<DKBuffer> data;
		size_t offset;

		DKBufferStream(const DKBufferStream&);
		DKBufferStream& operator = (const DKBufferStream&);
	};
}

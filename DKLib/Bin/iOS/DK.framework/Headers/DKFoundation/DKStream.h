//
//  File: DKStream.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKinclude.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKStream (추상 클래스)
//
// 위치를 지정하여 순차적으로 읽고 쓸수 있는 객체
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	class DKStream
	{
	public:
		typedef long long Position;

		DKStream(void) {}
		virtual ~DKStream(void) {}

		virtual Position SetPos(Position p) = 0;
		virtual Position GetPos(void) const = 0;
		virtual Position RemainLength(void) const = 0;
		virtual Position TotalLength(void) const = 0;

		virtual size_t Read(void* p, size_t s) = 0;
		virtual size_t Write(const void* p, size_t s) = 0;
		
		virtual bool IsReadable(void) const = 0;
		virtual bool IsWritable(void) const = 0;
		virtual bool IsSeekable(void) const = 0;
	};
}

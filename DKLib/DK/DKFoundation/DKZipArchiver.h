//
//  File: DKZipArchiver.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKinclude.h"
#include "DKString.h"
#include "DKBuffer.h"
#include "DKDateTime.h"
#include "DKArray.h"
#include "DKStream.h"
#include "DKLock.h"

namespace DKFoundation
{
	class DKLIB_API DKZipArchiver
	{
	public:
		DKZipArchiver(void);
		~DKZipArchiver(void);

		static DKObject<DKZipArchiver> Create(const DKString& file, bool append);

		// Write : 압축파일을 추가한다. compressionLevel 은 0~9 까지, (0:무압축, 9:최대압축, 6 이 기본값)
		bool Write(const DKString& file, DKStream* stream, int compressionLevel, const char* password = NULL);
		bool Write(const DKString& file, const void* data, size_t len, int compressionLevel, const char* password = NULL);

		const DKString& GetArchiveName(void) const		{return filename;}
	private:
		DKString	filename;
		void*		zipHandle;
		DKLock		lock;
	};
}

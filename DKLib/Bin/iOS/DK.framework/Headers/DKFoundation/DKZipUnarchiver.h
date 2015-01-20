//
//  File: DKZipUnarchiver.h
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

namespace DKFoundation
{
	class DKLIB_API DKZipUnarchiver
	{
	public:
		enum Method
		{
			MethodUnknown,
			MethodStored,
			MethodDeflated,
			MethodBZip2ed,
		};
		struct FileInfo
		{
			size_t			uncompressedSize;
			size_t			compressedSize;
			int				compressLevel;
			DKDateTime		date;
			DKString		name;
			bool			directory;
			bool			crypted;
			Method			method;
			unsigned int	crc32;
		};

		DKZipUnarchiver(void);
		~DKZipUnarchiver(void);

		static DKObject<DKZipUnarchiver> Create(const DKString& file);

		const DKArray<FileInfo>& GetFileList(void) const	{return files;}
		const FileInfo* GetFileInfo(const DKString& file) const;

		DKObject<DKStream> OpenFileStream(const DKString& file, const char* password = NULL) const;

		const DKString& GetArchiveName(void) const		{return filename;}
	private:
		void*							zipHandle;
		DKArray<FileInfo>				files;
		DKString						filename;
	};
}

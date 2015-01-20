//
//  File: DKVoxel32Storage.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2009-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKVoxelVolume.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKVoxel32Storage
//
// 복셀 스토리지 인터페이스
// DKVoxel32 형식의 데이터를 기본 유닛으로 사용하며, 분할하여 저장하는 인터페이스를 제공함
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	union DKVoxel32
	{
		struct
		{
			unsigned char level;
			unsigned char data[3];
		};
		unsigned int uintValue;
	};

	class DKLIB_API DKVoxel32Storage
	{
	public:
		DKVoxel32Storage(void) {}
		virtual ~DKVoxel32Storage(void) {}

		typedef DKFoundation::DKUUID StorageId;
	
		virtual size_t MaxActiveUnits(void) const = 0;

		virtual DKVoxel32* Create(const StorageId&) = 0;
		virtual void Delete(const StorageId&) = 0;
		virtual DKVoxel32* Load(const StorageId&) = 0;
		virtual void Unload(const StorageId&) = 0;
	};
}

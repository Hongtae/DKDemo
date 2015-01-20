//
//  File: DKVoxel32FileStorage.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2009-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKVoxel32Storage.h"

namespace DKFramework
{
	class DKLIB_API DKVoxel32FileStorage : public DKVoxel32Storage
	{
	public:
		DKVoxel32FileStorage(void);
		~DKVoxel32FileStorage(void);

		DKVoxel32* Create(const StorageId&);
		void Delete(const StorageId&);
		DKVoxel32* Load(const StorageId&);
		void Unload(const StorageId&);

		size_t MaxActiveUnits(void) const {return maxLoadableUnits;}

	protected:
		size_t maxLoadableUnits;
	};
}

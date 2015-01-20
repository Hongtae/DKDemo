//
//  File: DKVoxel32SparseVolume.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2009-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKVoxelVolume.h"
#include "DKVoxel32Storage.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKVoxel32SparseVolume
//
// 복셀 스토리지 인터페이스
// DKVoxel32 형식의 데이터를 기본 유닛으로 사용하며, 분할하여 저장하는 인터페이스를 제공함
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKLIB_API DKVoxel32SparseVolume : public DKVoxelVolume<DKVoxel32>
	{
	public:
		typedef DKVoxel32Storage Storage;
		typedef Storage::StorageId StorageId;
		typedef DKFoundation::DKTimer::Tick TimeStamp;

		enum {UnitSize = 16};
		enum {UnitDimensions = UnitSize * UnitSize * UnitSize};
		enum {UnitBytes = UnitDimensions * sizeof(DKVoxel32)};

		DKVoxel32SparseVolume(Storage* stor = NULL);
		virtual ~DKVoxel32SparseVolume(void);

		bool GetVoxelAtLocation(unsigned int x, unsigned int y, unsigned int z, DKVoxel32& v);
		bool SetVoxelAtLocation(unsigned int x, unsigned int y, unsigned int z, const DKVoxel32& v);

		void GetDimensions(size_t* width, size_t* height, size_t* depth);
		bool SetDimensions(size_t width, size_t height, size_t depth);

		bool ResetContents(void);

		Storage* VolumeStorage(void);
		const Storage* VolumeStorage(void) const;

		void Compact(void);

	private:
		struct VolumetricBlock
		{
			DKFoundation::DKSpinLock lock;
			StorageId storageId;				// zero 이면 solid-block
			TimeStamp ts;
			union
			{
				DKVoxel32 solid;
				DKVoxel32* voxels;
			};
		};

		size_t width;
		size_t height;
		size_t depth;

		size_t blocksLoaded;				// 로딩된 블록들
		VolumetricBlock* volumeBlocks;		// 전체 블록들

		DKFoundation::DKObject<Storage> storage;
		DKFoundation::DKSharedLock volumeLock;	// volume 변경할때 사용하는 rwlock
		TimeStamp lastCompactedTS;

		void UnloadOldBlocks(size_t);
	};
}

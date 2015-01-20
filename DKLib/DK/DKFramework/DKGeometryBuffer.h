//
//  File: DKGeometryBuffer.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKResource.h"

namespace DKFramework
{
	class DKLIB_API DKGeometryBuffer : public DKResource
	{
	public:
		enum BufferType
		{
			BufferTypeVertexArray,
			BufferTypeElementArray,
		};
		enum MemoryLocation
		{
			MemoryLocationStatic,		// 한번기록후 계속 사용
			MemoryLocationDynamic,		// 불규칙한 기록 및 사용
			MemoryLocationStream,		// 매번 새로 기록 및 사용
		};
		enum BufferUsage
		{
			BufferUsageDraw,			// 그리기 전용 (App -> GL)
			BufferUsageRead,			// 데이터 복사 전용 (GL -> app)
			BufferUsageCopy,			// 그리기 및 복사 둘다 사용 (GL -> GL)
		};
		enum AccessMode
		{
			AccessModeReadOnly,			// 읽기 전용
			AccessModeWriteOnly,		// 쓰기 전용 (부분 덮어쓰기용)
			AccessModeReadWrite,		// 읽고 쓰기
		};

		virtual ~DKGeometryBuffer(void);
		bool Bind(void) const;

		// 버퍼를 잠그고 사용가능한 임시 포인터를 가져온다 (UpdateContent, UpdateSubContent 가 더 빠르다)
		void* Lock(AccessMode lock);
		// 잠금 해제
		void Unlock(void);
		bool IsLocked(AccessMode* lock) const;
		void* AcquireLockedBuffer(void) const;

		// 데이터를 복사해온다.
		DKFoundation::DKObject<DKFoundation::DKBuffer> CopyContent(void) const;
		// 내용 부분 업데이트
		bool UpdateSubContent(const void* data, size_t offset, size_t size);
		// 
		size_t			Size(void) const			{return resourceSize;}
		BufferType		Type(void) const			{return resourceType;}
		MemoryLocation	Location(void) const		{return resourceLocation;}
		BufferUsage		Usage(void) const			{return resourceUsage;}

		// 유효성 검사
		bool	IsValid(void) const;

		// DKResource::Serializer() override, 동기화용
		DKFoundation::DKObject<DKSerializer> Serializer(void);

		// DKResource::Validate() override
		bool Validate(void)					{return IsValid();}
	protected:
		DKGeometryBuffer(void);

		// 내용 새로 업데이트
		bool UpdateContent(BufferType t, MemoryLocation m, BufferUsage u, const DKFoundation::DKData* data);
		bool UpdateContent(BufferType t, MemoryLocation m, BufferUsage u, const void* data, size_t size);

	private:
		BufferType			resourceType;
		MemoryLocation		resourceLocation;
		BufferUsage			resourceUsage;
		unsigned int		resourceId;
		size_t				resourceSize;
	};
}

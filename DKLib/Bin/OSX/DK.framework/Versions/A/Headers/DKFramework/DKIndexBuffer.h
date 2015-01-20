//
//  File: DKIndexBuffer.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKGeometryBuffer.h"
#include "DKPrimitiveIndex.h"

namespace DKFramework
{
	class DKLIB_API DKIndexBuffer : public DKGeometryBuffer
	{
	public:
		enum Type
		{
			TypeUByte,
			TypeUShort,		// 가장 빠름
			TypeUInt,
		};

		DKIndexBuffer(void);
		~DKIndexBuffer(void);

		static DKFoundation::DKObject<DKIndexBuffer> Create(const unsigned char* indices, size_t count, DKPrimitive::Type p, MemoryLocation m, BufferUsage u);
		static DKFoundation::DKObject<DKIndexBuffer> Create(const unsigned short* indices, size_t count, DKPrimitive::Type p, MemoryLocation m, BufferUsage u);
		static DKFoundation::DKObject<DKIndexBuffer> Create(const unsigned int* indices, size_t count, DKPrimitive::Type p, MemoryLocation m, BufferUsage u);
		static DKFoundation::DKObject<DKIndexBuffer> Create(const void* buffer, Type indexType, size_t count, DKPrimitive::Type p, MemoryLocation m, BufferUsage u);
		
		DKPrimitive::Type	PrimitiveType(void) const;
		size_t				NumberOfIndices(void) const;
		Type				IndexType(void) const;
		bool				CopyIndices(DKFoundation::DKArray<unsigned int>& indices) const;

		DKFoundation::DKObject<DKSerializer> Serializer(void);
	private:
		DKPrimitive::Type	primitiveType;
		Type				indexType;
		size_t				indexCount;
	};
}

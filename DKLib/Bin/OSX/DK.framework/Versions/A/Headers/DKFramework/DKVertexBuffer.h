//
//  File: DKVertexBuffer.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKGeometryBuffer.h"
#include "DKVertexStream.h"

namespace DKFramework
{
	class DKLIB_API DKVertexBuffer : public DKGeometryBuffer
	{
	public:
		struct Decl
		{
			DKVertexStream::Stream	id;
			DKFoundation::DKString	name;		// id = user-defined 일때 사용됨
			DKVertexStream::Type	type;
			bool					normalize;	// 정수형 데이터를 실수형으로 변환할때 0.0~1.0 (unsigned) 또는 -1.0~1.0 (signed) 으로 변경한다
			size_t					offset;
		};

		DKVertexBuffer(void);
		~DKVertexBuffer(void);
		
		static DKFoundation::DKObject<DKVertexBuffer> Create(const Decl* decls, size_t declCount, const void* vertices, size_t size, size_t count, MemoryLocation m, BufferUsage u);
		template <typename T, typename U>
		static DKFoundation::DKObject<DKVertexBuffer> Create(const Decl* decls, size_t declCount, const DKFoundation::DKArray<T,U>& vertices, MemoryLocation m, BufferUsage u)
		{
			return Create(decls, declCount, (const T*)vertices, sizeof(T), vertices.Count(), m, u);
		}

		const Decl* DeclarationAtIndex(long index) const;
		const Decl* Declaration(DKVertexStream::Stream stream) const;			// StreamUserDefine 이 아닌것들 
		const Decl* Declaration(const DKFoundation::DKString& name) const;		// StreamUserDefine 인것중에 이름이 같은것들
		const Decl* Declaration(DKVertexStream::Stream stream, const DKFoundation::DKString& name) const;	// name 은 stream 이 DKVertexStream::StreamUserDefine 일때만 사용됨

		size_t NumberOfDeclarations(void) const;
		size_t VertexSize(void) const;
		size_t NumberOfVertices(void) const;

		DKFoundation::DKObject<DKFoundation::DKBuffer> CopyStream(DKVertexStream::Stream stream) const;
		DKFoundation::DKObject<DKFoundation::DKBuffer> CopyStream(const DKFoundation::DKString& name) const;
		DKFoundation::DKObject<DKFoundation::DKBuffer> CopyStream(DKVertexStream::Stream stream, const DKFoundation::DKString& name) const;

		bool IsValid(void) const;
		bool UpdateContent(const void* vertexData, size_t vertexCount, MemoryLocation m, BufferUsage u);

		DKFoundation::DKObject<DKSerializer> Serializer(void);

		bool BindStream(const DKVertexStream& stream) const;
	private:
		void UpdateDeclMap(void);
		typedef DKFoundation::DKMap<DKVertexStream::Stream, const Decl*> DeclMapById;
		typedef DKFoundation::DKMap<DKFoundation::DKString, const Decl*> DeclMapByName;

		DeclMapById declMapByStreamId;		// 검색용 맵
		DeclMapByName declMapByStreamName;

		DKFoundation::DKArray<Decl>			declarations;
		size_t								vertexCount;		// 버텍스 전체 갯수
		size_t								vertexSize;			// 버텍스 한개 크기
	};
}

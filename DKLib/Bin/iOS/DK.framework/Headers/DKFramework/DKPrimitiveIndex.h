//
//  File: DKPrimitiveIndex.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKTriangle.h"
#include "DKLine.h"
#include "DKVector3.h"

////////////////////////////////////////////////////////////////////////////////
//
// Primitive 간단한 설명
//
// Adjacency(근접점)은 geometry shader 에서 각각의 선분(또는 직선)을 제어하기 위해 사용되며
// geometry shader 를 사용하지 않을시 무시된다.
//
// TypePoints:
//		각각이 하나의 정점이 된다. (인덱스 버퍼가 성능향상에 전혀 도움을 주지는 않음)
// TypeLines:
//		각각의 두 점이 개별적인 직선으로 연결된다. (0,1)(2,3)(4,5)..
//		정점 개수는 2보다 크거나 같아야 하며 2의 배수여야 한다. (배수가 아닐경우 나머지는 무시된다)
// TypeLineStrip:
//		두개의 점이 직선으로 연결되고 다음 한점이 이전점의 끝점과 연결된다.
//		(0,1)(1,2)(2,3)(3,4)...
//		정점 개수는 2보다 크거나 같아야 한다.
// TypeLineLoop:
//		TypeLineStrip 과 동일하지만 맨 끝점이 첫 시작점과 연결된다. (항상 닫힌 도형을 만든다)
//		정점 개수는 2보다 크거나 같아야 한다.
// TypeTriangles:
//		각각 세개씩 하나의 삼각형을 이룬다 (0,1,2)(3,4,5)(6,7,8)..
//		정점 개수는 3보다 크거나 같아야하며 3의 배수여야 한다. (배수가 아닐경우 나머지 무시)
// TypeTriangleStrip:
//		한개씩 뒤로 이동하며 뒤의 두개와 함께 쌍을 이룬다
//		삼각형 순서가 홀수일때와 짝수일때가 나뉜다 (n>=0)
//		짝수번째: (n, n+1, n+2) ->  (0,1,2)(...)(2,3,4)(...)(4,5,6)....
//		홀수번째: (n+1, n, n+2) ->  (...)(2,1,3)(...)(4,3,5)(...)(6,5,7)....
//		정점 개수는 3보다 크거나 같아야한다.
// TypeTriangleFan:
//		맨 처음점과 다음 두개가 하나의 삼각형이 된다 (0,1,2)(0,2,3)(0,3,4)(0,4,5)..
//		(0, n, n+1) (0, n+2, n+3).. (n>0)
//		정점 개수는 3보다 크거나 같아야한다.
// TypeLinesAdjacency:
//		TypeLines 처럼 두 점이 개별적인 직선을 그리지만, 각 라인의 끝점은 근접점을 가진다.
//		근접점은 geometry shader 에서 라인을 제어하는데 사용된다. 근접점은 라인 정점의 전과 후에 위치한다.
//		직선: (1,2)(5,6)(9,10)..
//		근접점: (0,3)(4,7)(8,11)..
//		각 라인 인덱스의 내용 = [시작점에 대한 근접점][시작점][끝점][끝점에 대한 근접점]
//		정점스 개수는 4보다 크거나 같아야 하며, 직선 하나에 4개의 정점이 사용된다. (근접점-시작점-끝점-근접점) 
// TypeLineStripAdjacency:
//		TypeLineStrip 과 비슷하지만, 시작점에 대한 근접점과, 끝점에 대한 근접점을 가진다.
//		맨 처음 직선은 1 부터 시작하며 (0번째 정점이 시작점에 대한 근접점)
//		직선의 끝은 맨 마지막-1 까지 라인을 잇는다 (맨 마지막점은 끝점에 대한 근접점)
//		각 라인의 근접점은 라인 시작점 전의 정점과 라인 끝점 뒤의 정점이 된다. (TypeLinesAdjacency 와 같음)
//		정점의 개수는 4보다 크거나 같아야 하며, 인덱스 개수는 정점개수+3 (총 라인 개수는 정점개수-2) 이 된다. (근접점-라인시작-라인시작-...-라인끝-근접점)
// TypeTrianglesAdjacency:
//		Triangles 와 비슷하나 실제 정점은 0,2,4 가 된다
//		1,3,5 은 adjacency 정점으로 geometry shader 에서 콘트롤 포인트로 사용한다.
//		geometry shader 에서 adjacency 를 정의하지 않으면 근접점은 무시된다.
//		정점:(0,2,4)(6,8,10)(12,14,16)...
//		근접점:(1,3,5)(7,9,11)(13,15,17)..
//		삼각형의 각 선분 에 대한 근접점은 다음과 같다
//			선분(0,2)->1, 선분(2,4)->3, 선분(4,0)->5
//		정점 개수는 6보다 크거나 같아야하며 6의 배수여야 한다. (배수가 아닐경우 나머지 무시)
// TypeTriangleStripAdjacency:
//		TriangleStrips 와 비슷하지만 선분에 근접점(adjacency) 가 포함된다.
//		삼각형 단 한개만 그릴때의 정점 (0,2,4), 근접점 (1,5,3)
//		삼각형 두개 이상 그릴때 (n 은 0부터 시작)
//			첫번째 삼각형 (0,2,4), 근접점 (1,6,3)
//			중간 n이 홀수인 삼각형들 (2n+2, 2n, 2n+4), 근접점 (2n-2, 2n+3, 2n+6)
//			중간 n이 짝수인 삼각형들 (2n, 2n+2, 2n+4), 근접점 (2n-2, 2n+6, 2n+3)
//			마지막 n이 홀수인 삼각형 (2n+2, 2n, 2n+4), 근접점 (2n-2, 2n+3, 2n+5)
//			마지막 n이 짝수인 삼각형 (2n, 2n+2, 2n+4), 근접점 (2n-2, 2n+5, 2n+3)
//		정점 개수는 6보다 크거나 같아야하며 2(n+2)여야 한다. (n은 삼각형 개수)
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	struct DKPrimitive
	{
		enum Type
		{
			TypeUnknown = 0,
			TypePoints,
			TypeLines,
			TypeLineStrip,
			TypeLineLoop,
			TypeTriangles,
			TypeTriangleStrip,
			TypeTriangleFan,
			TypeLinesAdjacency,
			TypeLineStripAdjacency,
			TypeTrianglesAdjacency,
			TypeTriangleStripAdjacency,
		};
		static DKFoundation::DKString TypeToString(Type p);
		static Type StringToType(const DKFoundation::DKString& str);
	};

	class DKLIB_API DKPrimitiveIndex
	{
	public:
		DKPrimitiveIndex(void);
		~DKPrimitiveIndex(void);

		bool SetIndex(unsigned int* indices, size_t count, DKPrimitive::Type t);

		size_t NumberOfTriangles(void) const;
		size_t NumberOfLines(void) const;
		size_t NumberOfPoints(void) const;
		size_t MaxVertices(void) const;

		bool GetTriangleIndices(unsigned int triangle, unsigned int& index1, unsigned int& index2, unsigned int& index3) const;
		bool GetTriangleAdjacencyIndices(unsigned int triangle, unsigned int& index1, unsigned int& index2, unsigned int& index3, unsigned int& adj12, unsigned int& adj23, unsigned int& adj31) const;
		bool GetLineIndices(unsigned int line, unsigned int& index1, unsigned int& index2) const;
		bool GetLineAdjacencyIndices(unsigned int line, unsigned int& index1, unsigned int& index2, unsigned int& adj1, unsigned int& adj2) const;
		bool GetPointIndex(unsigned int point, unsigned int& index) const;

		bool IsTriangleBased(void) const;
		bool IsLineBased(void) const;
		bool HasAdjacency(void) const;

		DKPrimitive::Type GetType(void) const		{return type;}
		size_t GetIndexCount(void) const			{return indices.Count();}

		size_t GetTriangles(const DKVector3* positions, size_t start, size_t count, DKFoundation::DKArray<DKTriangle>& output) const;
		size_t GetLines(const DKVector3* positions, size_t start, size_t count, DKFoundation::DKArray<DKLine>& output) const;
		size_t GetPoints(const DKVector3* positions, size_t start, size_t count, DKFoundation::DKArray<DKVector3>& output) const;
	private:
		DKFoundation::DKArray<unsigned int>	indices;
		DKPrimitive::Type					type;
		size_t								maxVertexIndex;
	};
}

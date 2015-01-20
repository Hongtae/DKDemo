//
//  File: DKVoxelPolygonizer.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2009-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKVector3.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKVoxelPolygonizer
//
// Marching Cubes 알고리즘을 이용하여 복셀을 폴리곤화 함.
// 큐브의 각 정점이 비었으면 (또는 동위표면보다 작으면) CubicBitMask 를 세팅한다.
// 
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKLIB_API DKVoxelPolygonizer
	{
	public:
		enum CubeIndex
		{
			CI_X0_Y0_Z0 = 0,	// 좌측 후면 하단 (0,0,0)
			CI_X1_Y0_Z0,		// 우측 후면 하단 (1,0,0)
			CI_X1_Y0_Z1,		// 우측 전면 하단	(1,0,1)
			CI_X0_Y0_Z1,		// 좌측 전면 하단	(0,0,1)
			CI_X0_Y1_Z0,		// 좌측 후면 상단	(0,1,0)
			CI_X1_Y1_Z0,		// 우측 후면 상단	(1,1,0)
			CI_X1_Y1_Z1,		// 우측 전면 상단	(1,1,1)
			CI_X0_Y1_Z1,		// 좌측 전면 상단	(0,1,1)
		};
		enum CubicBitMask
		{
			CB_X0_Y0_Z0 = 1 << CI_X0_Y0_Z0,
			CB_X1_Y0_Z0 = 1 << CI_X1_Y0_Z0,
			CB_X1_Y0_Z1 = 1 << CI_X1_Y0_Z1,
			CB_X0_Y0_Z1 = 1 << CI_X0_Y0_Z1,
			CB_X0_Y1_Z0 = 1 << CI_X0_Y1_Z0,
			CB_X1_Y1_Z0 = 1 << CI_X1_Y1_Z0,
			CB_X1_Y1_Z1 = 1 << CI_X1_Y1_Z1,
			CB_X0_Y1_Z1 = 1 << CI_X0_Y1_Z1,
		};
		struct Vertex
		{
			DKVector3 pos;		// 0.0 ~ 1.0 사이의 값이 된다. 전체 스케일에서 위치를 보정해야 함.
			CubeIndex idx1;		// 영향을 받는 복셀 위치 (인덱스)
			CubeIndex idx2;		// 영향을 받는 복셀 위치 (인덱스)
		};

		DKVoxelPolygonizer(void);
		virtual ~DKVoxelPolygonizer(void);

		void PolygonizeSurface(int cubicBits);	// cubicBits: 빈 정점에 CubicBitMask 를 세팅
		virtual void GenerateTriangle(Vertex&, Vertex&, Vertex&) = 0;	// polygonized result
		virtual DKVector3 Interpolate(const DKVector3& p1, const DKVector3& p2, CubeIndex c1, CubeIndex c2) = 0;
	};
}

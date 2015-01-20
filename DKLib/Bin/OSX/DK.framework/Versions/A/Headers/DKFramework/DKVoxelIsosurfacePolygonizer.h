//
//  File: DKVoxelIsosurfacePolygonizer.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2009-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKVoxelPolygonizer.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKVoxelIsosurfacePolygonizer
//
// Marching Cubes 알고리즘을 이용하여 폴리곤을 생성한다.
// isosurface-level 을 기준으로 표면 정점을 보간함.
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKLIB_API DKVoxelIsosurfacePolygonizer : public DKVoxelPolygonizer
	{
	public:
		struct Cube
		{
			float levels[8];	// 해당 복셀의 surface-level
		};
		DKVoxelIsosurfacePolygonizer(void);
		virtual ~DKVoxelIsosurfacePolygonizer(void);

		float isoLevel;		// iso-surface level
		void PolygonizeSurface(const Cube& cube);
	protected:
		DKVector3 Interpolate(const DKVector3& p1, const DKVector3& p2, CubeIndex c1, CubeIndex c2);
		const float* levels;
	};
}

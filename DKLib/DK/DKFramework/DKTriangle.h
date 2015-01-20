//
//  File: DKTriangle.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKVector3.h"
#include "DKMatrix4.h"
#include "DKQuaternion.h"
#include "DKLine.h"
#include "DKSphere.h"
#include "DKCylinderShape.h"

namespace DKFramework
{
	class DKLIB_API DKTriangle
	{
	public:
		enum class Front
		{
			CCW,		// 반시계 방향을 윗면으로 함 (앞면:position1, position2, position3 순서)
			CW,			// 시계방향 (앞면: position3, position2, position1 순서)
			Both,		// 둘다
		};

		DKTriangle(void);
		DKTriangle(const DKVector3& p1, const DKVector3& p2, const DKVector3& p3);
		~DKTriangle(void);

		DKVector3	position1;
		DKVector3	position2;
		DKVector3	position3;

		bool Intersect(const DKLine& line, Front front = Front::Both, DKVector3* p = NULL) const;
	};
}

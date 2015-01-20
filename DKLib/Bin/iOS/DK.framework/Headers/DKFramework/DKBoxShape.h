//
//  File: DKBoxShape.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2012-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKVector3.h"
#include "DKMatrix3.h"
#include "DKQuaternion.h"
#include "DKPolyhedralConvexShape.h"

namespace DKFramework
{
	class DKLIB_API DKBoxShape : public DKPolyhedralConvexShape
	{
	public:
		DKBoxShape(float u, float v, float w);
		DKBoxShape(const DKVector3& halfExtents);
		~DKBoxShape(void);

		DKVector3 HalfExtents(void) const;
		DKVector3 HalfExtentsWithMargin(void) const;

		DKVector3 ScaledHalfExtents(void) const;
		DKVector3 ScaledHalfExtentsWithMargin(void) const;
	};
}

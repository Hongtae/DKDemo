//
//  File: DKCapsuleShape.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2012-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKVector3.h"
#include "DKConvexShape.h"

namespace DKFramework
{
	class DKLIB_API DKCapsuleShape : public DKConvexShape
	{
	public:
		DKCapsuleShape(float radius, float height, UpAxis up = UpAxis::Top);
		~DKCapsuleShape(void);

		float Radius(void) const;
		float HalfHeight(void) const;

		float ScaledRadius(void) const;
		float ScaledHalfHeight(void) const;

		UpAxis BaseAxis(void) const;
	};
}

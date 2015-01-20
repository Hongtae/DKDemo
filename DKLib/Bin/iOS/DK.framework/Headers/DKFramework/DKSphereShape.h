//
//  File: DKSphereShape.h
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
	class DKLIB_API DKSphereShape : public DKConvexShape
	{
	public:
		DKSphereShape(float radius);
		~DKSphereShape(void);

		float Radius(void) const;
		float ScaledRadius(void) const;
	};
}

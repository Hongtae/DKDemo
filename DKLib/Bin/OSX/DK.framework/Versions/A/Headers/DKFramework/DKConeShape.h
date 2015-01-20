//
//  File: DKConeShape.h
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
	class DKLIB_API DKConeShape : public DKConvexShape
	{
	public:
		DKConeShape(float radius, float height, UpAxis up = UpAxis::Top);
		~DKConeShape(void);

		float Radius(void) const;
		float Height(void) const;

		float ScaledRadius(void) const;
		float ScaledHeight(void) const;

		UpAxis BaseAxis(void) const;
	};
}

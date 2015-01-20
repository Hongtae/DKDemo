//
//  File: DKStaticPlaneShape.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2012-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKConcaveShape.h"

namespace DKFramework
{
	class DKLIB_API DKStaticPlaneShape : public DKConcaveShape
	{
	public:
		DKStaticPlaneShape(const DKVector3& planeNormal, float planeConstant);
		~DKStaticPlaneShape(void);

		DKVector3 PlaneNormal(void) const;
		float PlaneConstant(void) const;
	};
}

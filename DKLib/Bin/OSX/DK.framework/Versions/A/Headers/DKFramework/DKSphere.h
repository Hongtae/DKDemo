//
//  File: DKSphere.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKVector3.h"

namespace DKFramework
{
	class DKLine;
	class DKLIB_API DKSphere
	{
	public:
		DKSphere(void);
		DKSphere(const DKVector3& c, float r);

		static DKSphere Union(const DKSphere& s1, const DKSphere& s2);					// s1 과 s2 를 merge 하는 sphere 생성
		static DKSphere Intersection(const DKSphere&s1, const DKSphere&s2);				// s1 과 s2 사이에 교차지점에 있는 작은 원 생성

		bool IsValid(void) const;
		bool IsPointInside(const DKVector3& pos) const;
		float Volume(void) const;

		bool Intersect(const DKLine& line, DKVector3* p = NULL) const;

		DKVector3	center;
		float		radius;
	};
}

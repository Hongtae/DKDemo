//
//  File: DKAABox.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.


#pragma once
#include "../DKInclude.h"
#include "DKVector3.h"
#include "DKQuaternion.h"

namespace DKFramework
{
	class DKLine;
	class DKBox;
	// Axis Aligned (Bounding) Box
	class DKLIB_API DKAABox
	{
	public:
		DKAABox(void);
		DKAABox(const DKVector3& posMin, const DKVector3& posMax);

		static DKAABox Intersection(const DKAABox& b1, const DKAABox& b2);	// b1 과 b2 의 교차하는 박스
		static DKAABox Union(const DKAABox& b1, const DKAABox& b2);			// b1 과 b2 를 합친 바운딩 박스

		bool IsValid(void) const;
		bool IsPointInside(const DKVector3& pos) const;
		DKVector3 Center(void) const;
		float Volume(void) const;
		DKBox Box(void) const;

		bool Intersect(const DKLine& line, DKVector3* p = NULL) const;
		bool Intersect(const DKAABox& box) const;

		DKVector3	positionMax;
		DKVector3	positionMin;
	};
}

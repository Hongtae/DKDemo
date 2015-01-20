//
//  File: DKBox.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKVector3.h"
#include "DKQuaternion.h"

namespace DKFramework
{
	class DKLine;
	// Oriented (Bounding) Box
	class DKLIB_API DKBox
	{
	public:
		DKBox(void);
		// c,x,y,z 는 절대 위치값
		DKBox(const DKVector3& c, const DKVector3& x, const DKVector3& y, const DKVector3& z);
		// c:중심점의 절대 위치, u,v,w: 각 방향의 단위벡터, h: 중심으로부터 각각의 면 까지의 거리
		DKBox(const DKVector3& c, const DKVector3& u, const DKVector3& v, float hu, float hv, float hw);
		DKBox(const DKVector3& c, const DKVector3& u, const DKVector3& v, const DKVector3& w, float hu, float hv, float hw);
		// 박스 좌표계 행렬
		DKBox(const DKMatrix4& m);

		static DKBox Intersection(const DKBox& b1, const DKBox& b2);	// b1 과 b2 의 교차하는 박스
		static DKBox Union(const DKBox& b1, const DKBox& b2);			// b1 과 b2 를 합친 바운딩 박스

		bool IsValid(void) const;
		bool IsPointInside(const DKVector3& pos) const;

		float Volume(void) const;
		DKMatrix4 LocalTransform(void) const;	// 이 박스의 좌표계로 변환할 행렬
		DKMatrix4 AffineTransform(void) const;	// 박스의 아핀 행렬
		DKQuaternion Orientation(void) const;	// 회전값만 구함
		DKBox& RotateOrientation(const DKQuaternion& q);

		bool Intersect(const DKLine& line, DKVector3* p = NULL) const;

		DKVector3 center;
		DKVector3 u,v,w;		// center 로부터의 각 축에 해당되는 방향 (단위) 벡터
		float hu, hv, hw;
	};
}

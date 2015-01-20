//
//  File: DKRect.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKPoint.h"
#include "DKSize.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKRect
//
// 2D 상 직사각형을 표시하기 위한 클래스이며 왼쪽 하단이 origin
// 우측 상단이 origin + size 가 된다.
// size 가 음수가 되면 유효하지 않음.
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKLIB_API DKRect
	{
	public:
		DKRect(void);
		DKRect(const DKPoint& p, const DKSize& s);
		DKRect(float x, float y, float width, float height);

		DKPoint Center(void) const;
		DKRect& SetCenter(const DKPoint& p);
		DKRect& SetCenter(float x, float y);
		
		bool IsPointInside(const DKPoint& pt) const;
		bool IsValid(void) const;
		bool Intersect(const DKRect& rc) const;
		bool IntersectLine(const DKPoint& pt1, const DKPoint& pt2) const;
		bool IntersectTriangle(const DKPoint& pt1, const DKPoint& pt2, const DKPoint& pt3) const;
		bool IntersectRect(const DKRect& rc, const DKMatrix3& tm) const;

		bool operator == (const DKRect& rc) const;
		bool operator != (const DKRect& rc) const;

		static DKRect Intersection(const DKRect& rc1, const DKRect& rc2);
		static DKRect Union(const DKRect& rc1, const DKRect& rc2);

		DKPoint origin;
		DKSize size;

		static const DKRect zero;
	};
}

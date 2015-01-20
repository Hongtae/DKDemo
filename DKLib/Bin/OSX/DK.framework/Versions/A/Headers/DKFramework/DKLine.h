//
//  File: DKLine.h
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

namespace DKFramework
{
	class DKLIB_API DKLine
	{
	public:
		DKLine(void);
		DKLine(const DKVector3& b, const DKVector3& e);

		DKLine operator * (const DKMatrix4& m) const;
		DKLine operator * (const DKQuaternion& q) const;

		DKVector3 Direction(void) const;
		float Length(void) const;
		bool IsPointInside(const DKVector3& pos) const;

		bool Intersect(const DKLine& line, DKVector3* p = NULL) const;

		// 다른 라인과 연결되는 최단거리 라인을 구함. (s.begin 은 이 라인에 위치, s.end 는 다른라인(other) 에 위치)
		static DKLine LineBetween(const DKLine& line1, const DKLine& line2);

		DKVector3	begin;
		DKVector3	end;
	};
}

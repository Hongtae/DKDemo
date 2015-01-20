//
//  File: DKPlane.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"

namespace DKFramework
{
	class DKVector3;
	class DKVector4;
	class DKLine;
	class DKLIB_API DKPlane
	{
	public:
		DKPlane(void);
		DKPlane(const DKVector3& v1, const DKVector3& v2, const DKVector3& v3);		// 삼각형으로부터 평면 구함
		DKPlane(const DKVector3& n, const DKVector3& p);							// 한 점과 노멀벡터 로부터 평면 구함
		~DKPlane(void);

		float Dot(const DKVector3& v) const;					// 한 점과 평면과의 거리 (0 보다 작으면 평면 아래에 있음)
		float Dot(const DKVector4& v) const;

		DKVector3 Normal(void) const;

		bool Intersect(const DKLine& line, DKVector3* p = NULL) const;

		union
		{
			struct
			{
				float a, b, c, d;
			};
			float val[4];
		};
	};
}



//
//  File: DKVector4.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"

namespace DKFramework
{
	class DKMatrix4;
	
	class DKLIB_API DKVector4
	{
	public:
		DKVector4(void);
		DKVector4(float x_, float y_, float z_, float w_);

		static DKVector4 Cross(const DKVector4&v1, const DKVector4& v2, const DKVector4& v3);// cross product, 벡터 곱, 외적
		static float Dot(const DKVector4& v1, const DKVector4& v2);				// dot product, 스칼라 곱, 내적

		bool operator == (const DKVector4& v) const;
		bool operator != (const DKVector4& v) const;

		DKVector4 operator + (const DKVector4& v) const;
		DKVector4 operator - (const DKVector4& v) const;
		DKVector4 operator - (void) const;
		DKVector4 operator * (float f) const;
		DKVector4 operator * (const DKVector4& v) const;	// 각 원소들끼리의 곱
		DKVector4 operator * (const DKMatrix4& m) const;
		DKVector4 operator / (float f) const;
		DKVector4 operator / (const DKVector4& v) const;	// 각 원소들끼리 나누기

		DKVector4& operator += (const DKVector4& v);
		DKVector4& operator -= (const DKVector4& v);
		DKVector4& operator *= (float f);
		DKVector4& operator *= (const DKVector4& v);
		DKVector4& operator *= (const DKMatrix4& m);
		DKVector4& operator /= (float f);
		DKVector4& operator /= (const DKVector4& v);

		float Length(void) const;							// 길이
		float LengthSq(void) const;							// 길이 제곱

		DKVector4& Transform(const DKMatrix4& m);
		DKVector4& Normalize(void);

		operator float* (void)				{return val;}		// 형변환
		operator const float* (void) const	{return val;}

		union
		{
			struct
			{
				float x, y, z, w;
			};
			float val[4];
		};

		static const DKVector4 zero;
	};
}

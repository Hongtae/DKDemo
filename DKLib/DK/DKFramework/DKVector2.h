//
//  File: DKVector2.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"

namespace DKFramework
{
	class DKMatrix2;
	class DKMatrix3;

	class DKLIB_API DKVector2
	{
	public:
		DKVector2(void);
		DKVector2(float x_, float y_);

		static float Dot(const DKVector2& v1, const DKVector2& v2);				// dot product, 스칼라 곱, 내적

		bool operator == (const DKVector2& v) const;
		bool operator != (const DKVector2& v) const;

		DKVector2 operator + (const DKVector2& v) const;
		DKVector2 operator - (const DKVector2& v) const;		// binary
		DKVector2 operator - (void) const;						// unary
		DKVector2 operator * (float f) const;
		DKVector2 operator * (const DKVector2& v) const;
		DKVector2 operator * (const DKMatrix2& m) const;
		DKVector2 operator * (const DKMatrix3& m) const;
		DKVector2 operator / (float f) const;
		DKVector2 operator / (const DKVector2& v) const;

		DKVector2& operator += (const DKVector2& v);
		DKVector2& operator -= (const DKVector2& v);
		DKVector2& operator *= (float f);
		DKVector2& operator *= (const DKVector2& v);
		DKVector2& operator *= (const DKMatrix2& m);
		DKVector2& operator *= (const DKMatrix3& m);
		DKVector2& operator /= (float f);
		DKVector2& operator /= (const DKVector2& v);

		float Length(void) const;
		float LengthSq(void) const;			// 길이 제곱

		DKVector2& Transform(const DKMatrix2& m);			// Linear Transform
		DKVector2& Transform(const DKMatrix3& m);			// Homogeneous Transform
		DKVector2& Normalize(void);

		operator float* (void)				{return val;}		// 형변환
		operator const float* (void) const	{return val;}

		union
		{
			struct
			{
				float x, y;
			};
			float val[2];
		};

		static const DKVector2 zero;
	};
}

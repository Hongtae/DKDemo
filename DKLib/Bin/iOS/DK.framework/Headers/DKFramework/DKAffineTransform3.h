//
//  File: DKAffineTransform3.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKVector3.h"
#include "DKMatrix3.h"
#include "DKMatrix4.h"
#include "DKLinearTransform3.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKAffineTransform3
// 4x3 행렬
// 
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKLIB_API DKAffineTransform3
	{
	public:
		DKAffineTransform3(const DKVector3& translate = DKVector3::zero);
		DKAffineTransform3(const DKLinearTransform3& linear, const DKVector3& translate = DKVector3::zero);
		DKAffineTransform3(const DKVector3& left, const DKVector3& up, const DKVector3& forward, const DKVector3& translate = DKVector3::zero);
		DKAffineTransform3(float x, float y, float z);
		DKAffineTransform3(const DKMatrix4& m);

		DKAffineTransform3& Identity(void);
		bool IsIdentity(void) const;
		bool IsDiagonal(void) const;

		// 트랜스폼 연산
		DKAffineTransform3& Translate(float x, float y, float z);
		DKAffineTransform3& Translate(const DKVector3& v);
		// 행렬 연산
		DKAffineTransform3& Inverse(void);
		DKAffineTransform3& Multiply(const DKLinearTransform3& t);
		DKAffineTransform3& Multiply(const DKAffineTransform3& t);

		DKAffineTransform3 operator * (const DKLinearTransform3& t) const;
		DKAffineTransform3 operator * (const DKAffineTransform3& t) const;
		DKAffineTransform3& operator *= (const DKLinearTransform3& t);
		DKAffineTransform3& operator *= (const DKAffineTransform3& t);

		bool operator == (const DKAffineTransform3& t) const;
		bool operator != (const DKAffineTransform3& t) const;

		DKMatrix4 Matrix4(void) const;
		const DKMatrix3& Matrix3(void) const		{return matrix3;}

		DKMatrix3	matrix3;
		DKVector3	translation;
	};
	inline DKVector3 operator * (const DKVector3& v, const DKAffineTransform3& t)
	{
		return v * t.Matrix3() + t.translation;
	}
	inline DKVector3& operator *= (DKVector3& v, const DKAffineTransform3& t)
	{
		v *= t.Matrix3();
		v += t.translation;
		return v;
	}
}

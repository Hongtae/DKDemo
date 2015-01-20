//
//  File: DKLinearTransform3.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKVector3.h"
#include "DKQuaternion.h"
#include "DKMatrix3.h"
#include "DKMatrix4.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKLinearTransform3
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKLIB_API DKLinearTransform3
	{
	public:
		DKLinearTransform3(void);
		DKLinearTransform3(const DKQuaternion& q);
		DKLinearTransform3(float scaleX, float scaleY, float scaleZ);
		DKLinearTransform3(const DKVector3& left, const DKVector3& up, const DKVector3& forward);
		DKLinearTransform3(const DKMatrix3& m);

		DKLinearTransform3& Identity(void);
		bool IsIdentity(void) const;
		bool IsDiagonal(void) const;

		// 트랜스폼 연산
		DKLinearTransform3& Scale(const DKVector3& v);
		DKLinearTransform3& Scale(float x, float y, float z);
		DKLinearTransform3& Scale(float s);
		DKLinearTransform3& RotateX(float r);
		DKLinearTransform3& RotateY(float r);
		DKLinearTransform3& RotateZ(float r);
		DKLinearTransform3& Rotate(const DKVector3& axis, float r);
		DKLinearTransform3& Rotate(const DKQuaternion& q);
		// 행렬 연산
		DKLinearTransform3& Inverse(void);
		DKLinearTransform3& Multiply(const DKMatrix3& m);
		DKLinearTransform3& Multiply(const DKLinearTransform3& t);

		DKLinearTransform3 operator * (const DKLinearTransform3& t) const;
		DKLinearTransform3& operator *= (const DKLinearTransform3& t);

		bool operator == (const DKLinearTransform3& t) const;
		bool operator != (const DKLinearTransform3& t) const;

		// Decompose: scale * rotate 순서의 행렬로 분해함
		bool Decompose(DKVector3& scale, DKQuaternion& rotate) const;
		DKQuaternion Rotation(void) const;

		DKMatrix3& Matrix3(void)				{return matrix3;}
		const DKMatrix3& Matrix3(void) const	{return matrix3;}

		DKMatrix3 matrix3;
	};
	inline DKVector3 operator * (const DKVector3& v, const DKLinearTransform3& t)
	{
		return v * t.Matrix3();
	}
	inline DKVector3& operator *= (DKVector3& v, const DKLinearTransform3& t)
	{
		v *= t.Matrix3();
		return v;
	}
}

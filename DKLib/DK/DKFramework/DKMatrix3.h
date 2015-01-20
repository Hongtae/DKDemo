//
//  File: DKMatrix3.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKMatrix3
//
// V' = V * Mat
//
// Row-Major 행렬을 사용하지만, GLSL 로 들어갈땐 Transpose 되서 들어간다!
//
// Vector2 와의 곱은 Affine Transform 행렬이 된다.
// Vector3 와의 곱은 Linear Transform 행렬이 된다.
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKVector3;

	class DKLIB_API DKMatrix3
	{
	public:
		DKMatrix3(void);
		DKMatrix3(const DKVector3& row1, const DKVector3& row2, const DKVector3& row3);
		DKMatrix3(float e11, float e12, float e13, float e21, float e22, float e23, float e31, float e32, float e33);

		DKMatrix3& Zero(void);
		DKMatrix3& Identity(void);
		bool IsIdentity(void) const;
		bool IsDiagonal(void) const;

		DKMatrix3& Inverse(void);
		DKMatrix3& Transpose(void);
		DKMatrix3& Multiply(const DKMatrix3& m);

		float Determinant(void) const;
		bool GetInverseMatrix(DKMatrix3& matOut, float *pDeterminant) const;

		DKMatrix3 operator * (const DKMatrix3& m) const;
		DKMatrix3 operator + (const DKMatrix3& m) const;
		DKMatrix3 operator - (const DKMatrix3& m) const;
		DKMatrix3 operator * (float f) const;
		DKMatrix3 operator / (float f) const;

		DKMatrix3& operator *= (const DKMatrix3& m);
		DKMatrix3& operator += (const DKMatrix3& m);
		DKMatrix3& operator -= (const DKMatrix3& m);
		DKMatrix3& operator *= (float f);
		DKMatrix3& operator /= (float f);

		bool operator == (const DKMatrix3& m) const;
		bool operator != (const DKMatrix3& m) const;

		DKVector3 Row1(void) const;
		DKVector3 Row2(void) const;
		DKVector3 Row3(void) const;
		DKVector3 Column1(void) const;
		DKVector3 Column2(void) const;
		DKVector3 Column3(void) const;

		union
		{
			struct
			{
				float _11, _12, _13;
				float _21, _22, _23;
				float _31, _32, _33;
			};
			struct
			{
				float m[3][3];
			};
			float val[9];
		};

		static const DKMatrix3 identity;		// 단위행렬
	};
}

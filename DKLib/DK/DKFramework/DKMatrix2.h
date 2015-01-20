//
//  File: DKMatrix2.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKMatrix2
//
// V' = V * Mat2
//
// Row-Major 행렬을 사용하지만, GLSL 로 들어갈땐 Transpose 되서 들어간다!
//
// Vector2 와의 곱은 Linear Transform 행렬이 된다.
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKVector2;

	class DKLIB_API DKMatrix2
	{
	public:
		DKMatrix2(void);
		DKMatrix2(const DKVector2& row1, const DKVector2& row2);
		DKMatrix2(float e11, float e12, float e21, float e22);

		DKMatrix2& Zero(void);
		DKMatrix2& Identity(void);
		bool IsIdentity(void) const;
		bool IsDiagonal(void) const;

		DKMatrix2& Inverse(void);
		DKMatrix2& Transpose(void);
		DKMatrix2& Multiply(const DKMatrix2& m);

		float Determinant(void) const;
		bool GetInverseMatrix(DKMatrix2& matOut, float *pDeterminant) const;

		DKMatrix2 operator * (const DKMatrix2& m) const;
		DKMatrix2 operator + (const DKMatrix2& m) const;
		DKMatrix2 operator - (const DKMatrix2& m) const;
		DKMatrix2 operator * (float f) const;
		DKMatrix2 operator / (float f) const;

		DKMatrix2& operator *= (const DKMatrix2& m);
		DKMatrix2& operator += (const DKMatrix2& m);
		DKMatrix2& operator -= (const DKMatrix2& m);
		DKMatrix2& operator *= (float f);
		DKMatrix2& operator /= (float f);

		bool operator == (const DKMatrix2& m) const;
		bool operator != (const DKMatrix2& m) const;

		DKVector2 Row1(void) const;
		DKVector2 Row2(void) const;
		DKVector2 Column1(void) const;
		DKVector2 Column2(void) const;

		union
		{
			struct
			{
				float _11, _12;
				float _21, _22;
			};
			struct
			{
				float m[2][2];
			};
			float val[4];
		};

		static const DKMatrix2 identity;		// 단위행렬
	};
}

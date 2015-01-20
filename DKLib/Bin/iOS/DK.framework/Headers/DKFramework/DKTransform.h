//
//  File: DKTransform.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKVector3.h"
#include "DKQuaternion.h"
#include "DKMatrix3.h"
#include "DKMatrix4.h"


////////////////////////////////////////////////////////////////////////////////
//
// DKTransformUnit
// affine 변환용 유닛들
// 행렬로 변환하여 계산하여야 한다.
//
// Non-uniform scale 트랜스폼은 DKAffineTransform3 를 이용하면 된다.
//
////////////////////////////////////////////////////////////////////////////////
//
// DKUSTransform
// Uniform-Scale transform
// 그래픽 매시 변환에 사용
// 
////////////////////////////////////////////////////////////////////////////////
//
// DKNSTransform
// Non-Scale transform
// 회전과 위치값만 가지고 있다. 스케일 값은 없음. 물리와 충돌처리용 객체에서 사용됨.
// 
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	struct DKLIB_API DKTransformUnit
	{
		DKTransformUnit(void);
		DKTransformUnit(const DKVector3& s, const DKQuaternion& r, const DKVector3& t);

		DKMatrix3			Matrix3(void) const;	// scale * rotation
		DKMatrix4			Matrix4(void) const;	// scale * rotation * translation

		DKTransformUnit			Interpolate(const DKTransformUnit& target, float t) const;	// 보간
		DKTransformUnit&		Identity(void);

		bool operator == (const DKTransformUnit& t) const;
		bool operator != (const DKTransformUnit& t) const;

		DKVector3		scale;
		DKQuaternion	rotation;
		DKVector3		translation;
		static const DKTransformUnit identity;
	};
	inline DKVector3 operator * (const DKVector3& v, const DKTransformUnit& t)
	{
		return (v * t.scale) * t.rotation + t.translation;
	}
	inline DKVector3& operator *= (DKVector3& v, const DKTransformUnit& t)
	{
		v = (v * t.scale) * t.rotation + t.translation;
		return v;
	}

	// Uniform-Scale Affine Transform
	struct DKLIB_API DKUSTransform
	{
		DKUSTransform(void);
		DKUSTransform(float s, const DKQuaternion& r, const DKVector3& t);

		DKMatrix3			Matrix3(void) const;
		DKMatrix4			Matrix4(void) const;

		DKUSTransform		Interpolate(const DKUSTransform& target, float t) const;	// 보간
		DKUSTransform&		Identity(void);
		DKUSTransform&		Inverse(void);

		DKUSTransform operator * (const DKUSTransform& t) const;
		DKUSTransform& operator *= (const DKUSTransform& t);

		bool operator == (const DKUSTransform& t) const;
		bool operator != (const DKUSTransform& t) const;

		float			scale;
		DKQuaternion	orientation;
		DKVector3		position;
		static const DKUSTransform identity;
	};
	inline DKVector3 operator * (const DKVector3& v, const DKUSTransform& t)
	{
		return (v * t.scale) * t.orientation + t.position;
	}
	inline DKVector3& operator *= (DKVector3& v, const DKUSTransform& t)
	{
		v = (v * t.scale) * t.orientation + t.position;
		return v;
	}

	// Non-Scale Affine Transform (4x3 matrix)
	struct DKLIB_API DKNSTransform
	{
		DKNSTransform(const DKQuaternion& r, const DKVector3& t = DKVector3::zero);
		DKNSTransform(const DKMatrix3& r, const DKVector3& t = DKVector3::zero);
		DKNSTransform(const DKVector3& t = DKVector3::zero);

		DKNSTransform& Identity(void);
		DKNSTransform& Inverse(void);

		DKNSTransform Interpolate(const DKNSTransform& target, float t) const;	// 보간

		DKMatrix3 Matrix3(void) const;
		DKMatrix4 Matrix4(void) const;

		DKNSTransform operator * (const DKNSTransform& t) const;
		DKNSTransform& operator *= (const DKNSTransform& t);

		bool operator == (const DKNSTransform& t) const;
		bool operator != (const DKNSTransform& t) const;

		DKQuaternion	orientation;
		DKVector3		position;
		static const DKNSTransform identity;
	};
	inline DKVector3 operator * (const DKVector3& v, const DKNSTransform& t)
	{
		return (v * t.orientation) + t.position;
	}
	inline DKVector3& operator *= (DKVector3& v, const DKNSTransform& t)
	{
		v = (v * t.orientation) + t.position;
		return v;
	}
}

//
//  File: DKCollisionShape.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2012-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKVector3.h"
#include "DKMatrix3.h"
#include "DKMatrix4.h"
#include "DKQuaternion.h"
#include "DKLine.h"
#include "DKResource.h"
#include "DKTransform.h"
#include "DKAABox.h"
#include "DKSphere.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKCollisionShape
//
// 충돌 처리 도형 (collision-shape)
//
// 객체는 별도의 좌표를 가지지 않으며 모두 원점을 기준으로 위치한다.
//
// 리소스의 Serialize/Deserialize 는 객체내에서 직접 하지 않고, 내부의 SerializeHelper 를 이용한다.
// DKCollisionShape (를 상속받는) 객체들은 한번 생성하면 수정할수 없으므로 Serialize 할수 없음.
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKLIB_API DKCollisionShape
	{
	public:
		struct SerializeHelper : public DKResource
		{
			DKFoundation::DKObject<DKSerializer> Serializer(void) override;
			DKFoundation::DKObject<DKCollisionShape> shape;
		};

		enum class UpAxis
		{
			Left = 0,
			Top = 1,
			Forward = 2,
		};
		enum class ShapeType
		{
			Custom = 0,
			Empty,
			Compound,
			Box,
			Capsule,
			Cylinder,
			Cone,
			Sphere,
			MultiSphere,
			ConvexHull,
			StaticPlane,
			StaticTriangleMesh,
		};

		const ShapeType type;

		virtual ~DKCollisionShape(void);

		DKAABox AABB(const DKNSTransform& transform) const;
		DKSphere BoundingSphere(void) const;

		void AABB(const DKNSTransform& transform, DKVector3& aabbMin, DKVector3& aabbMax) const;

		void SetMargin(float m);
		float Margin(void) const;

		void SetLocalScale(const DKVector3& s);
		DKVector3 LocalScale(void) const;

		DKVector3 CalculateLocalInertia(float mass) const;

		bool IsCompund(void) const;
		bool IsConvex(void) const;
		bool IsConcave(void) const;
		bool IsPolyhedral(void) const;

	protected:
		class btCollisionShape* impl;
		DKCollisionShape(ShapeType t, class btCollisionShape* context);
		
	private:
		DKCollisionShape(const DKCollisionShape&);
		DKCollisionShape& operator = (const DKCollisionShape&);
	};
}

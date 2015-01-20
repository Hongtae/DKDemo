//
//  File: DKPoint2PointConstraint.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2012-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKConstraint.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKPoint2PointConstraint
// 
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKLIB_API DKPoint2PointConstraint : public DKConstraint
	{
	public:
		DKPoint2PointConstraint(DKRigidBody* bodyA, DKRigidBody* bodyB, const DKVector3& pivotInA, const DKVector3& pivotInB);
		DKPoint2PointConstraint(DKRigidBody* bodyA, const DKVector3& pivotInA);
		DKPoint2PointConstraint(void);
		~DKPoint2PointConstraint(void);

		DKVector3 PivotInA(void) const;
		DKVector3 PivotInB(void) const;
		void SetPivotInA(const DKVector3& v);
		void SetPivotInB(const DKVector3& v);

		bool IsValidParam(ParamType type, ParamAxis axis) const override;
		bool HasParam(ParamType type, ParamAxis axis) const override;

		DKFoundation::DKObject<DKSerializer> Serializer(void) override;

	protected:
		void ResetContext(void) override;
		void ResetContextImpl(void) override;

		DKFoundation::DKObject<DKModel> Clone(UUIDObjectMap&) const override;
		DKPoint2PointConstraint* Copy(UUIDObjectMap&, const DKPoint2PointConstraint*);
	};
}

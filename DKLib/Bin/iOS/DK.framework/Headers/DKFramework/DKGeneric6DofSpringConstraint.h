//
//  File: DKGeneric6DofSpringConstraint.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2012-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKGeneric6DofConstraint.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKGeneric6DofConstraint
// 
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKLIB_API DKGeneric6DofSpringConstraint : public DKGeneric6DofConstraint
	{
	public:
		// DKNSTransform 은 해당 객체(DKRigidBody) 상의 로컬 트랜스폼이다.
		DKGeneric6DofSpringConstraint(DKRigidBody* bodyA, DKRigidBody* bodyB, const DKNSTransform& frameA, const DKNSTransform& frameB);
		DKGeneric6DofSpringConstraint(DKRigidBody* bodyB, const DKNSTransform& frameB);
		DKGeneric6DofSpringConstraint(void);
		~DKGeneric6DofSpringConstraint(void);

		void EnableSpring(ParamAxis axis, bool enable);
		bool IsSpringEnabled(ParamAxis axis) const;

		void SetStiffness(ParamAxis axis, float stiffness);
		float Stiffness(ParamAxis axis) const;

		void SetDamping(ParamAxis axis, float damping);
		float Damping(ParamAxis axis) const;

		// set the current constraint position/orientation as an equilibrium point for all DOF
		void SetEquilibriumPoint(void);
		// set the current constraint position/orientation as an equilibrium point for given DOF
		void SetEquilibriumPoint(ParamAxis axis);
		void SetEquilibriumPoint(ParamAxis axis, float val);
		float EquilibriumPoint(ParamAxis axis) const;

		DKFoundation::DKObject<DKSerializer> Serializer(void) override;

	protected:
		void ResetContext(void) override;
		void ResetContextImpl(void) override;

		DKFoundation::DKObject<DKModel> Clone(UUIDObjectMap&) const override;
		DKGeneric6DofSpringConstraint* Copy(UUIDObjectMap&, const DKGeneric6DofSpringConstraint*);
	};
}

//
//  File: DKFixedConstraint.h
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
// DKFixedConstraint
// 
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKLIB_API DKFixedConstraint : public DKConstraint
	{
	public:
		DKFixedConstraint(DKRigidBody* bodyA, DKRigidBody* bodyB, const DKNSTransform& frameA, const DKNSTransform& frameB);
		DKFixedConstraint(DKRigidBody* bodyA, const DKNSTransform& frameA);
		DKFixedConstraint(void);
		~DKFixedConstraint(void);

		DKNSTransform FrameA(void) const;
		DKNSTransform FrameB(void) const;
		void SetFrameA(const DKNSTransform& t);
		void SetFrameB(const DKNSTransform& t);
		void SetFrames(const DKNSTransform& ta, const DKNSTransform& tb);

		DKFoundation::DKObject<DKSerializer> Serializer(void) override;

	protected:
		void ResetContext(void) override;
		void ResetContextImpl(void) override;

		DKFoundation::DKObject<DKModel> Clone(UUIDObjectMap&) const override;
		DKFixedConstraint* Copy(UUIDObjectMap&, const DKFixedConstraint*);
	};
}

//
//  File: DKDynamicsScene.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2012-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKScene.h"
#include "DKRigidBody.h"
#include "DKSoftBody.h"
#include "DKConstraint.h"
#include "DKActionController.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKDynamicsScene
//
// 충돌처리와 물리가 적용된 scene
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKLIB_API DKDynamicsScene : public DKScene
	{
	public:
		DKDynamicsScene(void);
		virtual ~DKDynamicsScene(void);

		void SetGravity(const DKVector3& g);
		DKVector3 Gravity(void) const;

		void Update(double tickDelta, DKFoundation::DKTimeTick tick) override;

		// FixedFrameRate 가 0 보다 크면 물리엔진을 고정 프레임으로 작동시킨다.
		// 네트웍이나 다른 기기간 동기화 해야할때 꼭 필요함. (determinism)
		void SetFixedFrameRate(double fps);
		double FixedFrameRate(void) const;

	protected:
		bool AddSingleObject(DKModel* obj) override;
		void RemoveSingleObject(DKModel* obj) override;
		void RemoveAllObjects(void) override;

		typedef DKFoundation::DKSet<DKRigidBody*> RigidBodySet;
		typedef DKFoundation::DKSet<DKSoftBody*> SoftBodySet;
		typedef DKFoundation::DKSet<DKConstraint*> ConstraintSet;
		typedef DKFoundation::DKSet<DKActionController*> ActionSet;

		RigidBodySet rigidBodies;
		SoftBodySet softBodies;
		ConstraintSet constraints;
		ActionSet actions;

		virtual void UpdateActions(double tickDelta);

		virtual bool NeedCollision(DKCollisionObject* objA, DKCollisionObject* objB);
		virtual bool NeedResponse(DKCollisionObject* objA, DKCollisionObject* objB);

	private:
		double dynamicsFixedFPS;	// 물리엔진 determinism, fixed time stepping 단위
		static void PreTickCallback(void*, float);
		static void PostTickCallback(void*, float);
		class btActionInterface* actionInterface;

		friend class DKActionController;
		friend class DKConstraint;
	};
}

//
//  File: DKConstraint.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2012-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKModel.h"
#include "DKRigidBody.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKConstraint
// 
// ERP : error reduction parameter
// CFM : constraint force mixing
//
// CFM = 0 (기본값) 일 경우, constraint 가 hard 해짐.
// CFM > 0 이면, constraint 가 좀더 유연해짐, (값이 클수록 부드러워짐)
// CFM < 0 이면, 불안정해진다. 사용하지 말것.
//
// ERP = 0 이면 joint error 발생시 수정하지 않으며, 시뮬레이션시 미끄러질 수 있다.
// ERP = 1 이면 시뮬레이션에 발생하는 오류를 모두 수정함.
// 하지만 다양한 경우에서 완전히 수정되지 않기 때문에, 1 을 사용하는것은 추천하지 않음.
// 0.1 부터 0.8 까지의 값을 사용할것, (기본값 = 0.2)
//
//
// Note:
//  1. Serializer() 를 이용하여 객체를 복원할 때는, bodyA, bodyB 정보는
//     uuid 를 이용하여 복원한다. 같은 트리내의 객체가 아닐경우 복원되지 않는다.
//     복원되지 않을 경우, Retarget 을 호출하기 전까지 복원정보를 가지고 있게 된다.
//     원본의 uuid 를 가진 객체가 나중에 추가되는 경우 복원될 수 있다.
//     (OnUpdateTreeReferences 에서 복원함)
//
//  2. Clone() 을 이용하여 객체를 복제할 경우, bodyA, bodyB 는 복제되지 않는다.
//     트리의 복제가 완료된 후, 복제된 트리내에 bodyA, bodyB 의 uuid 가 있을경우
//     새로운 bodyA, bodyB 를 사용하게 된다. (bodyA, bodyB 도 복제된 경우)
//     (UpdateCopiedReferenceUUIDs 에서 복원함)
//
//  3. bodyA, bodyB 는 DKConstraint 객체의 부모객체여선 안된다.
//     DKObject<DKRigidBody> 로 reference 를 가지고 있기 때문에, 서로 삭제되지 않는다.
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKLIB_API DKConstraint : public DKModel
	{
	public:
		enum class ParamType
		{
			ERP = 0,
			STOP_ERP,
			CFM,
			STOP_CFM,
		};
		enum class ParamAxis
		{
			Default = 0,
			LinearX,
			LinearY,
			LinearZ,
			AngularX,
			AngularY,
			AngularZ,
		};
		enum class LinkType
		{
			Custom = 0,
			Fixed,
			Point2Point,
			Hinge,
			ConeTwist,
			Generic6Dof,
			Generic6DofSpring,
			Slider,
			Gear,
		};
		const LinkType type;

		virtual ~DKConstraint(void);

		bool disableCollisionsBetweenLinkedBodies;

		virtual bool IsValidParam(ParamType type, ParamAxis axis) const		{return false;}
		virtual bool HasParam(ParamType type, ParamAxis axis) const			{return false;}
		virtual void SetParam(ParamType type, ParamAxis axis, float value);
		virtual float GetParam(ParamType type, ParamAxis axis);

		void SetBreakingImpulseThreshold(float threshold);
		float BreakingImpulseThreshold(void) const;

		bool IsEnabled(void) const;
		void SetEnabled(bool e);

		void Enable(void)				{SetEnabled(true);}
		void Disable(void)				{SetEnabled(false);}

		DKRigidBody* BodyA(void)				{ return bodyA; }
		DKRigidBody* BodyB(void)				{ return bodyB; }
		const DKRigidBody* BodyA(void) const	{ return bodyA; }
		const DKRigidBody* BodyB(void) const	{ return bodyB; }

		virtual bool Retarget(DKRigidBody* a, DKRigidBody* b);

		virtual DKFoundation::DKObject<DKSerializer> Serializer(void) override;

	protected:
		DKConstraint(LinkType type, DKRigidBody* rbA, DKRigidBody* rbB, class btTypedConstraint* p);

		void ResetObject(void);
		// ResetContext, ResetContextImpl 은 꼭 재정의 해야함. (ResetObject 에서 사용함)
		virtual void ResetContext(void);
		virtual void ResetContextImpl(void) = 0;

		DKFoundation::DKObject<DKRigidBody> bodyA;
		DKFoundation::DKObject<DKRigidBody> bodyB;

		// 객체가 복원되었을때 호출됨.
		void OnUpdateTreeReferences(NamedObjectMap&, UUIDObjectMap&) override;
	
		// 객체가 복제되었을때 호출됨.
		void UpdateCopiedReferenceUUIDs(UUIDObjectMap&) override;
		DKConstraint* Copy(UUIDObjectMap&, const DKConstraint*);

		// 다른 객체의 자식으로 추가될때, bodyA, bodyB 의 자식노드인지 확인함.
		bool CanAcceptObjectAsParent(DKModel*) const override;

		class btTypedConstraint* impl;

	private:
		DKConstraint(const DKConstraint&);
		DKConstraint& operator = (const DKConstraint&);

		struct TargetRestoreInfo
		{
			DKFoundation::DKUUID bodyA;
			DKFoundation::DKUUID bodyB;
		};
		DKFoundation::DKObject<TargetRestoreInfo> restoreInfo;		// 복구용 정보
		void RestoreTargets(UUIDObjectMap&);
	};
}

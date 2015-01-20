//
//  File: DKModel.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKResource.h"
#include "DKTransform.h"
#include "DKAnimationController.h"

///////////////////////////////////////////////////////////////////////////////
//
// DKModel
//
// DKScene 에 들어가는 모든 형식을 정의한다.
// 계층 구조로 이루어져 있다.
//
// Note:
//  Serialize 시에는 worldTransform 은 저장하지 않는다. (localTransform 만 저장함)
//  특수한 경우 자식객체로 받아들이지 못할 수도 있다. (DKConstraint 의 타겟)
//  복제되거나 복원된 경우 레퍼런스가 바로 복원되지 않을 수 있음.
//  상속받아서 사용시 Clone(), Serializer() 는 꼭 재정의 해야한다.
//
///////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKScene;
	class DKLIB_API DKModel : public DKResource
	{
	public:
		enum Type
		{
			TypeCustom = 0,
			TypeMesh,
			TypeCollision,
			TypeConstraint,
			TypeAction,
		};
		using NamedObjectMap = DKFoundation::DKMap<DKFoundation::DKString, DKModel*>;
		using UUIDObjectMap = DKFoundation::DKMap<DKFoundation::DKUUID, DKModel*>;

		using Enumerator = DKFoundation::DKFunctionSignature<bool(DKModel*)>;
		using EnumeratorLoop = DKFoundation::DKFunctionSignature<void(DKModel*)>;
		using ConstEnumerator = DKFoundation::DKFunctionSignature<bool(const DKModel*)>;
		using ConstEnumeratorLoop = DKFoundation::DKFunctionSignature<void(const DKModel*)>;

		const Type type;

		DKModel(void);
		virtual ~DKModel(void);

		DKScene* Scene(void)					{ return scene; }
		const DKScene* Scene(void) const		{ return scene; }
		virtual void RemoveFromScene(void);

		bool AddChild(DKModel*);
		void RemoveFromParent(void);

		DKModel* RootObject(void);
		const DKModel* RootObject(void) const;

		DKModel* Parent(void)				{ return parent; }
		const DKModel* Parent(void) const	{ return parent; }

		bool IsDescendantOf(const DKModel* obj) const;
		size_t NumberOfDescendants(void) const;
		DKModel* FindDescendant(const DKFoundation::DKString&);
		const DKModel* FindDescendant(const DKFoundation::DKString&) const;
		DKModel* FindCommonAncestor(DKModel*, DKModel*, Type t = TypeCustom);

		DKModel* ChildAtIndex(unsigned int i)					{ return children.Value(i); }
		const DKModel* ChildAtIndex(unsigned int i) const		{ return children.Value(i); }
		size_t NumberOfChildren(void) const						{ return children.Count(); }

		void Enumerate(Enumerator* e)					{ EnumerateInternal(e); }
		void Enumerate(EnumeratorLoop* e)				{ EnumerateInternal(e); }
		void Enumerate(ConstEnumerator* e) const		{ EnumerateInternal(e); }
		void Enumerate(ConstEnumeratorLoop* e) const	{ EnumerateInternal(e); }

		DKAnimatedTransform* Animation(void)				{ return animation; }
		const DKAnimatedTransform* Animation(void) const	{ return animation; }
		void SetAnimation(DKAnimatedTransform*, bool recursive = true);

		virtual void SetWorldTransform(const DKNSTransform&);
		virtual void SetLocalTransform(const DKNSTransform&);
		const DKNSTransform& WorldTransform(void) const		{ return worldTransform; }
		const DKNSTransform& LocalTransform(void) const		{ return localTransform; }

		void CreateNamedObjectMap(NamedObjectMap&);		// 객체 이름으로 검색용 맵 생성
		void CreateUUIDObjectMap(UUIDObjectMap&);		// UUID 로 검색용 맵 생성

		// UpdateKinematic : before simulate
		// UpdateSceneState : after simulate, before rendering
		void UpdateKinematic(double timeDelta, DKFoundation::DKTimeTick tick);
		void UpdateSceneState(const DKNSTransform&);

		// UpdateLocalTransform: 월드 트랜스폼으로 로컬 트랜스폼 계산, 내부에서 SetLocalTransform 사용
		// UpdateWorldTransform: 로컬 트랜스폼으로 월드 트랜스폼 계산, 내부에서 SetWorldTransform 사용
		void UpdateLocalTransform(bool recursive = true);
		void UpdateWorldTransform(bool recursive = true);

		// 객체 복사 (내부 리소스는 공유함)
		DKFoundation::DKObject<DKModel> Clone(void) const;
		// serializer
		DKFoundation::DKObject<DKSerializer> Serializer(void) override;

	protected:
		virtual void OnAddedToScene(void) {}
		virtual void OnRemovedFromScene(void) {}
		virtual void OnAddedToParent(void) {}
		virtual void OnRemovedFromParent(void) {}
		virtual void OnSetAnimation(DKAnimatedTransform*) {}

		// OnUpdateTreeReferences: 트리가 변경되거나 객체가 복원되었을때 호출된다.
		virtual void OnUpdateTreeReferences(NamedObjectMap&, UUIDObjectMap&) {}

		// OnUpdateKinematic: 시뮬레이션 전에 호출된다.
		// OnUpdateSceneState: 시뮬레이션 후, 렌더링 전에 호출된다.
		virtual void OnUpdateKinematic(double timeDelta, DKFoundation::DKTimeTick tick);
		virtual void OnUpdateSceneState(const DKNSTransform& parentWorldTransform);

		// ResolveTree: 자식노드들을 갱신함. (OnUpdateTreeReferences 호출함)
		void ResolveTree(bool force = false);

		// ReloadSceneContext: scene 에 들어가있는 context 를 다시 로딩함.
		// op 에 리셋하는 루틴을 넣어야 한다.
		void ReloadSceneContext(DKFoundation::DKOperation* op);	// scene 에서 객체를 다시 로딩함.

		DKModel(Type t);

		// Clone: 트리 복사 (UUIDObjectMap 에 원본의 UUID 를 기록함)
		virtual DKFoundation::DKObject<DKModel> Clone(UUIDObjectMap&) const;
		DKModel* Copy(UUIDObjectMap&, const DKModel*);

		// UpdateCopiedReferenceUUIDs: 객체가 복제되었을때(UUID변경되었을때) 호출된다.
		// 객체가 복사되면 UUID 가 변경되므로, 복제되기 전(원본)의 UUID 를 이용하여
		// 복제된 객체를 찾는 용도로 사용한다.
		// 주의: UUIDObjectMap 의 key 는 원본의 UUID 이며, value 는 복제된 객체이다.
		virtual void UpdateCopiedReferenceUUIDs(UUIDObjectMap&) {}

		// AddChild 에서 호출한다. true 를 리턴하면 부모 자식 관계가 된다.
		// 부모 객체의 reference 를 가진 경우, 부모자식 관계가 안될 수도 있다.(cross-reference)
		virtual bool CanAcceptObjectAsParent(DKModel*) const;

		DKNSTransform localTransform;
		DKNSTransform worldTransform;

	private:
		DKModel* parent;
		DKScene* scene;
		DKFoundation::DKArray<DKFoundation::DKObject<DKModel>> children;
		DKFoundation::DKObject<DKAnimatedTransform> animation;

		bool needResolveTree;		// true 로 설정하면 다음번 업데이트에 OnUpdateTreeReferences 호출함.

		bool EnumerateInternal(Enumerator* e);
		void EnumerateInternal(EnumeratorLoop* e);
		bool EnumerateInternal(ConstEnumerator* e) const;
		void EnumerateInternal(ConstEnumeratorLoop* e) const;

		DKModel& operator = (const DKModel&);

		friend class DKScene;
	};
}

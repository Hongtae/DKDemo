//
//  File: DKSkinMesh.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKStaticMesh.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKSkinMesh
//
// 뼈대에 바인딩 가능한 매쉬
// Static 매쉬 기반의 데이터를 사용하며, 쉐이더로 넘어갈 뼈대 행렬정보를 바인딩 해준다.
// 
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKLIB_API DKSkinMesh : public DKStaticMesh
	{
	public:
		typedef DKFoundation::DKString NodeId;
		struct Bone
		{
			NodeId id;
			DKMatrix4 tm;		// 뼈대 기본 world-tm
		};
		typedef DKFoundation::DKArray<Bone> BoneArray;
		typedef DKFoundation::DKArray<DKModel*> NodeArray;
		typedef DKFoundation::DKMap<NodeId, DKModel*> NodeMap;

		DKSkinMesh(void);
		~DKSkinMesh(void);

		void SetBones(const BoneArray& bones);
		size_t NumberOfBones(void) const;
		Bone& BoneAtIndex(unsigned int index);
		const Bone& BoneAtIndex(unsigned int index) const;

		void ClearTransformNodes(void);
		bool ResolveTransformNodes(void);
		bool ResolveTransformNodes(const NamedObjectMap&);

		bool NodeResolved(void) const	{ return transformNodeResolved; }

		DKFoundation::DKObject<DKSerializer> Serializer(void) override;

	protected:
		void OnUpdateTreeReferences(NamedObjectMap&, UUIDObjectMap&) override;
		void OnUpdateSceneState(const DKNSTransform&) override;

		bool BindTransform(DKSceneState&) const override;

		DKFoundation::DKObject<DKModel> Clone(UUIDObjectMap&) const;
		DKSkinMesh* Copy(UUIDObjectMap&, const DKSkinMesh*);

	private:
		struct TransformData
		{
			Bone bone;
			const DKModel* node;

			DKMatrix4 initInvTM;	// initial-tm 의 역행렬
			DKMatrix4 nodeTM;
		};
		typedef DKFoundation::DKArray<TransformData> TransformDataArray;
		TransformDataArray transformData;
		bool transformNodeResolved;
	};
}

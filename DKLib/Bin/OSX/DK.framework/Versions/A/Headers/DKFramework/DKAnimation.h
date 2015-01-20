//
//  File: DKAnimation.h
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

////////////////////////////////////////////////////////////////////////////////
//
// DKAnimation
//
// DKTransformUnit 데이터들을 가지고 있으며, 스키닝 애니메이션을 위해 노드정보를 가지고 있다.
// 각 노드는 프레임 데이터를 DKTransformUnit 타입으로 가지고 있다.
// 
// 이 객체는 애니메이션의 프레임 정보만 가지고 있으며, 재생하고 매쉬에 적용하기 위해서는
// DKController 를 사용해야 한다.
//
// 주의: 이 객체는 Affine-transform 만 다룬다.
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKLIB_API DKAnimation : public DKResource
	{
	public:
		typedef long NodeIndex;
		static const NodeIndex invalidNodeIndex = -1;

		struct Node
		{
			enum NodeType
			{
				NodeTypeSampling,
				NodeTypeKeyframe,
			};
			DKFoundation::DKString	name;
			const NodeType	type;

			Node(NodeType t) : type(t) {}
			virtual ~Node(void) {}
			virtual bool IsEmpty(void) const = 0;
		};
		struct SamplingNode : public Node
		{
			DKFoundation::DKArray<DKTransformUnit>	frames;
			SamplingNode(void) : Node(NodeTypeSampling) {}
			bool IsEmpty(void) const			{return frames.IsEmpty();}
		};
		struct KeyframeNode : public Node
		{
			template <typename KEY_TYPE> struct Key
			{
				float		time;
				KEY_TYPE	key;
			};
			typedef Key<DKVector3>		ScaleKey;
			typedef Key<DKQuaternion>	RotationKey;
			typedef Key<DKVector3>		TranslationKey;

			DKFoundation::DKArray<ScaleKey>			scaleKeys;
			DKFoundation::DKArray<RotationKey>		rotationKeys;
			DKFoundation::DKArray<TranslationKey>	translationKeys;

			KeyframeNode(void) : Node(NodeTypeKeyframe) {}
			bool IsEmpty(void) const		{return translationKeys.IsEmpty() && rotationKeys.IsEmpty() && scaleKeys.IsEmpty();}
		};
		struct NodeSnapshot
		{
			DKFoundation::DKString	name;
			DKTransformUnit			transform;
		};

		DKAnimation(void);
		~DKAnimation(void);

		// 노드 추가
		bool		AddNode(const Node* node);
		bool		AddSamplingNode(const DKFoundation::DKString& name, const DKTransformUnit* frames, size_t numFrames);
		bool		AddKeyframeNode(const DKFoundation::DKString& name,
									const KeyframeNode::ScaleKey* scaleKeys, size_t numSk,
									const KeyframeNode::RotationKey* rotationKeys, size_t numRk,
									const KeyframeNode::TranslationKey* translationKeys, size_t numTk);

		void		RemoveNode(const DKFoundation::DKString& name);
		void		RemoveAllNodes(void);
		size_t		NodeCount(void) const;
		NodeIndex	IndexOfNode(const DKFoundation::DKString& name) const;
		const Node*	NodeAtIndex(NodeIndex index) const;

		// 해당 시간(0 <= t <= 1)의 트랜스폼 가져오기
		bool GetNodeTransform(NodeIndex index, float t, DKTransformUnit& output) const;
		bool GetNodeTransform(const DKFoundation::DKString& name, float t, DKTransformUnit& output) const;

		// 스냅샷
		DKFoundation::DKArray<NodeSnapshot> CreateSnapshot(float t) const;

		// 스냅샷으로 애니메이션 생성 (두 애니메이션 사이를 보간하는 용도로 쓸 짧은 애니메이션 생성)
		static DKFoundation::DKObject<DKAnimation> Create(DKFoundation::DKArray<SamplingNode>* samples, DKFoundation::DKArray<KeyframeNode>* keyframes, float duration);
		static DKFoundation::DKObject<DKAnimation> Create(DKFoundation::DKArray<NodeSnapshot>* begin, DKFoundation::DKArray<NodeSnapshot>* end, float duration);

		// 노드의 affine-trasform 데이터를 가져온다.
		static DKTransformUnit GetTransform(const Node& node, float time);
		// 노드 변환 (tick = 1.0/(frames-1) 단위로 샘플링 한다. 첫프레임:0, 마지막프레임:frames-1)
		static bool ResampleNode(const Node& source, unsigned int frames, KeyframeNode& output, float threshold = 0.000001f);
		static bool ResampleNode(const Node& source, unsigned int frames, SamplingNode& output);

		// 재생 길이 설정
		void	SetDuration(float d);
		float	Duration(void) const;

		DKFoundation::DKObject<DKAnimationController> CreateLoopController(void);

		// 데이터 serialization
		DKFoundation::DKObject<DKSerializer> Serializer(void);
	private:
		float	duration;		// 재생시간

		DKFoundation::DKMap<DKFoundation::DKString, size_t>		nodeIndexMap;				// 노드 검색용 인덱스맵
		DKFoundation::DKArray<Node*>	nodes;
	};
}

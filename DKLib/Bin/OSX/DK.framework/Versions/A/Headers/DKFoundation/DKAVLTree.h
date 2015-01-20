//
//  File: DKAVLTree.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include <new>
#include "../DKInclude.h"
#include "DKTypeTraits.h"
#include "DKFunction.h"

////////////////////////////////////////////////////////////////////////////////
// DKAVLTree
//
// AVL Tree 구조로 제작된 트리
// 높이 레벨 차가 2이상 되면 밸런싱 작업을 한다.
//
// 쓰레드 동기화: 기본적으로 추가,삭제,검색 에 대해서 락을 사용한다.
// 검색결과에 대한 값을 수정하기 위해서는 사전에 락을 걸어놓아야 한다.
//
// VALUE: 기본적으로 저장되는 자료형
// SEARCH: 검색용 자료형
// CMPV: VALUE 와 VALUE 를 비교하기 위한 객체
// CMPS: VALUE 와 SEARCH 를 비교하기 위한 객체, 검색용으로만 사용한다.
// COPY: VALUE 간의 복사에 사용 (Update 함수에서만 사용한다. Update 를 사용하지 않으면 명시하지 않아도 된다)
//
// 밸런싱 작업을 해도 값이 저장되는 포인터의 주소는 바뀌지 않음.
////////////////////////////////////////////////////////////////////////////////


namespace DKFoundation
{
	template <typename VALUE, typename SEARCH> struct DKTreeComparison
	{
		int operator () (const VALUE& lhs, const SEARCH& rhs) const
		{
			if (lhs > rhs)				return 1;
			else if (lhs < rhs)			return -1;
			return 0;
		}
	};
	template <typename VALUE> struct DKTreeValueCopy
	{
		void operator () (VALUE& dst, const VALUE& src) const
		{
			dst = src;
		}
	};

	template <
		typename VALUE,											// 기본 자료 타입
		typename SEARCH,										// 검색용 자료 타입
		typename CMPV = DKTreeComparison<VALUE, VALUE>,			// 기본 자료타입 끼리 비교
		typename CMPS = DKTreeComparison<VALUE, SEARCH>,		// 검색용 자료 타입, 기본 자료타입 비교 객체
		typename COPY = DKTreeValueCopy<VALUE>,					// 값 복사
		typename ALLOC = DKMemoryDefaultAllocator				// 메모리 할당
	>
	class DKAVLTree
	{
	public:
		typedef VALUE					Value;
		typedef SEARCH					Search;
		typedef CMPV					ValueCompare;
		typedef CMPS					SearchCompare;
		typedef COPY					ValueCopy;
		typedef ALLOC					Allocator;

		typedef DKTypeTraits<Value>		ValueTraits;
		typedef DKTypeTraits<Search>	SearchTratis;

		DKAVLTree(void)
			: rootNode(NULL), count(0)
		{
		}
		DKAVLTree(DKAVLTree&& tree)
			: rootNode(NULL), count(0)
		{
			rootNode = tree.rootNode;
			count = tree.count;
			tree.rootNode = NULL;
			tree.count = 0;
		}
		DKAVLTree(const DKAVLTree& s)		// copy constructor 에서는 같은 형식의 DKAVLTree 만 받는다. (템플릿으로 만들면 사용할수 없게됨)
			: rootNode(NULL), count(0)
		{
			if (s.rootNode)
				rootNode = s.rootNode->Duplicate();
			count = s.count;
		}
		~DKAVLTree(void)
		{
			Clear();
		}
		// Update: 존재하지 않으면 추가, 존재하면 갱신
		const Value* Update(const Value& v)
		{
			bool created = false;
			Node* node = SetNode(v, &created);
			if (!created)
				valueCopyFunc(node->value, v);
			return &node->value;
		}
		// Insert: 존재하지 않는 아이템만 추가 (이미 존재하면 NULL 리턴)
		const Value* Insert(const Value& v)
		{
			bool created = false;
			Node* node = SetNode(v, &created);
			if (created && node)
				return &node->value;
			return NULL;
		}
		void Remove(const SEARCH& s)
		{
			Node* node = GetNode(s);
			if (node == NULL)
				return;

			Node* retrace = NULL;		// 밸런싱 작업을 시작할 노드

			if (node->left && node->right)
			{
				Node* replace = NULL;
				// 자식 노드중 왼쪽에서 가장 큰것 또는 오른쪽에서 가장 작은것을 찾아서 위치변경후 제거한다.
				// 제거후 좀전에 찾았던 노드의 부모부터 밸런싱 시작.
				// 해당 노드의 부모가 제거될 노드이면 별도로 처리해야한다.
				if (node->leftHeight > node->rightHeight)
				{
					for (replace = node->left; replace->right; replace = replace->right);	// 왼쪽에서 젤 큰거 찾아서 바꾼다
					if (replace == node->left)		// 대신할 자식(replace)가 바로 아래 노드임
					{
						retrace = replace;
					}
					else
					{
						retrace = replace->parent;
						// replace 노드의 자식(left)노드를 retrace(부모) 의 자식(right)으로 설정한다.
						retrace->right = replace->left;
						if (retrace->right)
							retrace->right->parent = retrace;
						replace->left = node->left;
						replace->left->parent = replace;
					}
					replace->right = node->right;
					replace->right->parent = replace;
				}
				else
				{
					for (replace = node->right; replace->left; replace = replace->left);	// 오른쪽에서 젤 작은거 찾아서 바꾼다
					if (replace == node->right)		// 대신할 자식(replace)이 바로 아래 노드임
					{
						retrace = replace;
					}
					else
					{
						retrace = replace->parent;
						// replace 의 자식(right)노드를 retrace(부모)의 자식(left)로 설정한다.
						retrace->left = replace->right;
						if (retrace->left)
							retrace->left->parent = retrace;
						replace->right = node->right;
						replace->right->parent = replace;
					}
					replace->left = node->left;
					replace->left->parent = replace;
				}
				if (node->parent)			// node 의 부모에게 replace 를 자식으로 설정
				{
					if (node->parent->left == node)
						node->parent->left = replace;
					else
						node->parent->right = replace;
				}
				replace->parent = node->parent;
			}
			else
			{
				retrace = node->parent;

				if (retrace)
				{
					if (retrace->left == node)
					{
						if (node->left)
						{
							node->left->parent = retrace;
							retrace->left = node->left;
						}
						else if (node->right)
						{
							node->right->parent = retrace;
							retrace->left = node->right;
						}
						else
							retrace->left = NULL;
					}
					else
					{
						if (node->left)
						{
							node->left->parent = retrace;
							retrace->right = node->left;
						}
						else if (node->right)
						{
							node->right->parent = retrace;
							retrace->right = node->right;
						}
						else
							retrace->right = NULL;
					}
				}
				else
				{
					if (node->left)
						rootNode = node->left;
					else
						rootNode = node->right;
					if (rootNode)
						rootNode->parent = NULL;
				}
			}

			node->left = NULL;
			node->right = NULL;
			DeleteNode(node);
			if (retrace)
				Balancing(retrace);
		}
		void Clear(void)
		{
			if (rootNode)
				DeleteNode(rootNode);
			rootNode = NULL;
			count = 0;
		}
		const VALUE* Find(const SEARCH& v) const
		{
			const Node* node = GetNode(v);
			if (node)
				return &node->value;
			return NULL;
		}
		size_t Count(void) const
		{
			return count;
		}
		DKAVLTree& operator = (DKAVLTree&& tree)
		{
			if (this != &tree)
			{
				Clear();

				rootNode = tree.rootNode;
				count = tree.count;
				tree.rootNode = NULL;
				tree.count = 0;
			}
			return *this;
		}
		DKAVLTree& operator = (const DKAVLTree& s)
		{
			if (this == &s)	return *this;

			Clear();

			if (s.rootNode)
				rootNode = s.rootNode->Duplicate();
			count = s.count;
			return *this;
		}
		// lambda enumerator (VALUE&, bool*)
		template <typename T> void EnumerateForward(T&& enumerator)
		{
			static_assert(DKFunctionType<T&&>::Signature::template CanInvokeWithParameterTypes<VALUE&, bool*>(),
						  "enumerator's parameter is not compatible with (VALUE&, bool*)");

			if (count > 0)
			{
				bool stop = false;
				auto func = [=, &enumerator](VALUE& v) mutable -> bool {enumerator(v, &stop); return stop;};
				rootNode->EnumerateForward(func);
			}
		}
		template <typename T> void EnumerateBackward(T&& enumerator)
		{
			static_assert(DKFunctionType<T&&>::Signature::template CanInvokeWithParameterTypes<VALUE&, bool*>(),
						  "enumerator's parameter is not compatible with (VALUE&, bool*)");

			if (count > 0)
			{
				bool stop = false;
				auto func = [=, &enumerator](VALUE& v) mutable -> bool {enumerator(v, &stop); return stop;};
				rootNode->EnumerateBackward(func);
			}
		}
		// lambda enumerator bool (const VALUE&, bool*)
		template <typename T> void EnumerateForward(T&& enumerator) const
		{
			static_assert(DKFunctionType<T&&>::Signature::template CanInvokeWithParameterTypes<const VALUE&, bool*>(),
						  "enumerator's parameter is not compatible with (const VALUE&, bool*)");

			if (count > 0)
			{
				bool stop = false;
				auto func = [=, &enumerator](const VALUE& v) mutable -> bool {enumerator(v, &stop); return stop;};
				rootNode->EnumerateForward(func);
			}
		}
		template <typename T> void EnumerateBackward(T&& enumerator) const
		{
			static_assert(DKFunctionType<T&&>::Signature::template CanInvokeWithParameterTypes<const VALUE&, bool*>(),
						  "enumerator's parameter is not compatible with (const VALUE&, bool*)");

			if (count > 0)
			{
				bool stop = false;
				auto func = [=, &enumerator](const VALUE& v) mutable -> bool {enumerator(v, &stop); return stop;};
				rootNode->EnumerateBackward(func);
			}
		}
	private:
		class Node
		{
		public:
			Node(const VALUE& v, Node* parentNode)
				: value(v), left(NULL), right(NULL), parent(parentNode), nodeHeight(1), leftHeight(0), rightHeight(0)
			{
			}
			VALUE		value;
			Node*		left;
			Node*		right;
			Node*		parent;
			int			leftHeight;		// 왼쪽 높이
			int			rightHeight;	// 오른쪽 높이
			int			nodeHeight;		// 왼쪽과 오른쪽것중 큰것 + 1 (자기것)
			Node* Duplicate(void) const
			{
				Node* node = new(Allocator::Alloc(sizeof(Node))) Node(value, NULL);
				if (left)
				{
					node->left = left->Duplicate();
					node->left->parent = node;
				}
				if (right)
				{
					node->right = right->Duplicate();
					node->right->parent = node;
				}
				node->leftHeight = leftHeight;
				node->rightHeight = rightHeight;
				node->nodeHeight = nodeHeight;
				return node;
			}
			template <typename R> bool EnumerateForward(R&& enumerator)
			{
				if (left && left->EnumerateForward(std::forward<R>(enumerator)))	return true;
				if (enumerator(value))												return true;
				if (right && right->EnumerateForward(std::forward<R>(enumerator)))	return true;
				return false;
			}
			template <typename R> bool EnumerateBackward(R&& enumerator)
			{
				if (right && right->EnumerateBackward(std::forward<R>(enumerator)))	return true;
				if (enumerator(value))												return true;
				if (left && left->EnumerateBackward(std::forward<R>(enumerator)))	return true;
				return false;
			}
			template <typename R> bool EnumerateForward(R&& enumerator) const
			{
				if (left && left->EnumerateForward(std::forward<R>(enumerator)))	return true;
				if (enumerator(value))												return true;
				if (right && right->EnumerateForward(std::forward<R>(enumerator)))	return true;
				return false;
			}
			template <typename R> bool EnumerateBackward(R&& enumerator) const
			{
				if (right && right->EnumerateBackward(std::forward<R>(enumerator)))	return true;
				if (enumerator(value))												return true;
				if (left && left->EnumerateBackward(std::forward<R>(enumerator)))	return true;
				return false;
			}
		};
		void DeleteNode(Node* node)
		{
			if (node->right)
				DeleteNode(node->right);
			if (node->left)
				DeleteNode(node->left);

			count--;

			(*node).~Node();
			Allocator::Free(node);
		}
		void LeftRotation(Node* pivot)
		{
			Node* parent = pivot->parent;

			if (parent->parent)
			{
				if (parent->parent->left == parent)
					parent->parent->left = pivot;
				else
					parent->parent->right = pivot;
			}
			pivot->parent = parent->parent;
			parent->parent = pivot;
			parent->right = pivot->left;
			if (parent->right)
				parent->right->parent = parent;
			pivot->left = parent;
		}
		void RightRotation(Node* pivot)
		{
			Node* parent = pivot->parent;

			if (parent->parent)
			{
				if (parent->parent->left == parent)
					parent->parent->left = pivot;
				else
					parent->parent->right = pivot;
			}
			pivot->parent = parent->parent;
			parent->parent = pivot;
			parent->left = pivot->right;
			if (parent->left)
				parent->left->parent = parent;
			pivot->right = parent;
		}
		void UpdateHeight(Node* node)
		{
			node->leftHeight = node->left ? node->left->nodeHeight : 0;
			node->rightHeight = node->right ? node->right->nodeHeight : 0;
			node->nodeHeight = node->leftHeight > node->rightHeight ? node->leftHeight + 1 : node->rightHeight + 1;
		}
		// 트리의 밸런싱을 맞춘다.
		void Balancing(Node* node)
		{
			int left = node->left ? node->left->nodeHeight : 0;
			int right = node->right ? node->right->nodeHeight : 0;

			if (left - right > 1)		// 왼쪽 노드가 2이상 클때
			{
				if (node->left->rightHeight > 0 && node->left->rightHeight > node->left->leftHeight)
				{
					// node->left 를 왼쪽 회전후 재귀하여 오른쪽 회전
					LeftRotation(node->left->right);
					UpdateHeight(node->left->left);
				}
				// node->left 를 오른쪽 회전
				RightRotation(node->left);
			}
			else if (right - left > 1)	// 오른쪽 노드가 2이상 클때
			{
				if (node->right->leftHeight > 0 && node->right->leftHeight > node->right->rightHeight)
				{
					// node->right 를 오른쪽 회전후 재귀하여 왼쪽 회전
					RightRotation(node->right->left);
					UpdateHeight(node->right->right);
				}
				// node->right 를 왼쪽 회전
				LeftRotation(node->right);
			}

			UpdateHeight(node);

			if (node->parent)
				return Balancing(node->parent);
			else
				rootNode = node;
		}
		// 키k 의 노드를 있으면 리턴하고 없으면 만든다.
		Node* SetNode(const Value& v, bool* created)
		{
			if (rootNode == NULL)
			{
				*created = true;

				count++;
				rootNode = new(Allocator::Alloc(sizeof(Node))) Node(v, NULL);
				return rootNode;
			}
			Node* node = rootNode;
			while (node)
			{
				int cmp = valueCompareFunc(node->value, v);
				if (cmp > 0)
				{
					if (node->left)
						node = node->left;
					else
					{
						*created = true;
						count++;
						Node* ret = new(Allocator::Alloc(sizeof(Node))) Node(v, node);
						node->left = ret;
						Balancing(node);
						return ret;
					}
				}
				else if (cmp < 0)
				{
					if (node->right)
						node = node->right;
					else
					{
						*created = true;
						count++;
						Node* ret = new(Allocator::Alloc(sizeof(Node))) Node(v, node);
						node->right = ret;
						Balancing(node);
						return ret;
					}
				}
				else
				{
					*created = false;
					return node;
				}
			}
			return NULL;
		}
		// 키 k 의 노드를 찾아서 리턴한다. 없으면 NULL 리턴
		Node* GetNode(const Search& s)
		{
			return const_cast<Node*>(static_cast<const DKAVLTree&>(*this).GetNode(s));
		}
		const Node* GetNode(const Search& s) const
		{
			Node* node = rootNode;
			while (node)
			{
				int cmp = searchCompareFunc(node->value, s);
				if (cmp > 0)
					node = node->left;
				else if (cmp < 0)
					node = node->right;
				else
					return node;
			}
			return NULL;
		}

		// 데이터 멤버들
		Node*		rootNode;
		size_t		count;
		ValueCompare		valueCompareFunc;
		SearchCompare		searchCompareFunc;
		ValueCopy			valueCopyFunc;
	};
}


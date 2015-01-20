//
//  File: DKSharedInstance.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKObject.h"
#include "DKSpinLock.h"
#include "DKCondition.h"
#include "DKCriticalSection.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKSharedInstance
//
// 공유하는 인스턴스를 생성한다. 객체는 개별 생성할수도 있고 공유할수도 있다.
// 공유하는 인스턴스는 모든 참조가 사라지면 소멸되고, SharedInstance() 를 호출시 재 생성된다.
// 공유하는 인스턴스는 재생성시 기존 객체가 완전히 제거된 후에 생성된다.
// (직접 생성시 해당 없음)
//
// 상속받거나 typedef 로 정의하여 할 수 있다. 상속을 권장
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	template <class TYPE> class DKSharedInstance
	{
	public:
		static DKObject<TYPE> SharedInstance(void)
		{
			DKCriticalSection<DKSpinLock> cs(creatorLock);	// 이 함수는 한번에 하나의 쓰레드만 억세스해야함.

			DKObject<TYPE> obj = NULL;
			if (obj == NULL)
			{
				DKCriticalSection<DKSpinLock> guard(sharedRefLock);
				obj = sharedRef;
				if (obj.IsManaged() == false)	// 객체가 소거되는중이면 IsManaged() 가 false 를 리턴함.
					obj = NULL;
			}
			if (obj == NULL)		// 객체가 소거중이거나 없다
			{
				creatorCond.Lock();
				while (creator != NULL)
					creatorCond.Wait();		// 이전 객체가 완전히 파기될때까지 기다린다.

				DKCriticalSection<DKSpinLock> guard(sharedRefLock);

				DKASSERT_DEBUG(creator == NULL);

				obj = DKObject<TYPE>::New();
				sharedRef = obj;
				creator = obj.template SafeCast<DKSharedInstance>();

				creatorCond.Unlock();

				DKASSERT_DESC_DEBUG(creator != NULL, "Object creation failure!");
			}
			DKASSERT_DESC_DEBUG(obj != NULL, "Object creation failure!");
			return obj;
		}
		static DKObject<TYPE> GetSharedInstanceIfExist(void)
		{
			DKCriticalSection<DKSpinLock> guard(sharedRefLock);
			if (creator)
			{
				DKObject<TYPE> obj = sharedRef;
				if (obj.IsManaged())
					return obj;
			}
			return NULL;
		}
	protected:
		DKSharedInstance(void)
		{
		}
		virtual ~DKSharedInstance(void)
		{
			DKCriticalSection<DKSpinLock> guard(sharedRefLock);
			if (creator == this)
			{
				creatorCond.Lock();
				creator = NULL;
				sharedRef = ObjectRef();
				creatorCond.Signal();
				creatorCond.Unlock();
			}
		}
	private:
		typedef typename DKObject<TYPE>::Ref	ObjectRef;
		static ObjectRef			sharedRef;				// 공유되는 객체의 Ref 형식
		static DKSharedInstance*	creator;				// 객체가 파기되는동안은 더이상 dynamic 않으므로 원형으로 보관한다.
		static DKSpinLock			sharedRefLock;			// sharedInstance 에 억세스 하기 위한 락
		static DKSpinLock			creatorLock;			// 객체 생성 부분을 크리티컬 섹션으로 만들기 위한 락
		static DKCondition			creatorCond;			// 객체가 완전히 파기될때까지 보장하는 락
	};

	template <typename TYPE> typename DKSharedInstance<TYPE>::ObjectRef DKSharedInstance<TYPE>::sharedRef;
	template <typename TYPE> DKSharedInstance<TYPE>* DKSharedInstance<TYPE>::creator = NULL;
	template <typename TYPE> DKSpinLock DKSharedInstance<TYPE>::sharedRefLock;
	template <typename TYPE> DKSpinLock DKSharedInstance<TYPE>::creatorLock;
	template <typename TYPE> DKCondition DKSharedInstance<TYPE>::creatorCond;
}

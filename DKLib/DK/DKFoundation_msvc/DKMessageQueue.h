//
//  File: DKMessageQueue.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKinclude.h"
#include "DKQueue.h"
#include "DKCondition.h"
#include "DKTimer.h"
#include "DKSpinLock.h"
#include "DKObject.h"
#include "DKTimer.h"
#include "DKThread.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKMessageQueue (메시지 큐)
//
// 여러 쓰레드가 하나의 큐를 사용하여 메시지를 주고받을 수 있으며
// 다수의 쓰레드가 하나의 큐에서 메시지를 차례대로 꺼내서 사용할 수 있다.
//	
// 메시지큐를 사용하기 위해서는 Result 와 Content 을 정의하면 된다.
// Result 와 Value 는 기본타입을 사용하거나 다른 클래스를 사용하면 되는데
// 클래스를 사용할 경우, 기본생성자, 복사생성자, 오퍼레이터 (=) 가 모두 구현되어야 한다.
// Result 는 void 를 사용할 수 있다.
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	template <typename Result, typename MessageContent>
	class DKMessageQueue
	{
	public:
		////////////////////////////////////////////////////////////////////////////////
		// 큐에 저장되는 타입
		// 동기식 처리를 위해서, 뮤텍스 객체를 포함한 QReceiver 객체를 사용한다.
		template <typename R, typename U> class QReceiver
		{
		public:
			QReceiver(void) : cond(NULL) {}
			QReceiver(const QReceiver& r) : object(r.object), cond(r.cond), result(r.result) {}
			QReceiver(const U& obj, DKCondition* c) : object(obj), cond(c) {}
			~QReceiver(void) {}
			void SetResult(const R& r)
			{
				if (cond)
				{
					cond->Lock();
					result = r;
					cond->Broadcast();
					cond->Unlock();
					cond = NULL;
				}
				else
				{
					result = r;
				}
			}
			U& Content(void)							{return object;}
			const U& Content(void) const				{return object;}
			QReceiver& operator = (const QReceiver& r)
			{
				object = r.object;
				cond = r.cond;
				result = r.result;
				return *this;
			}
			R GetResult(void) const	{return result;}
		private:
			U object;
			R result;
			DKCondition* cond;
		};
		// result 가 없는(void) 형식에 대한 템플릿 특화 버전
		template <typename U> class QReceiver<void, U>
		{
		public:
			QReceiver(void) : cond(NULL) {}
			QReceiver(const QReceiver& r) : object(r.object), cond(r.cond) {}
			QReceiver(const U& obj, DKCondition* c) : object(obj), cond(c) {}
			~QReceiver(void) {}
			void SetResult(void)
			{
				if (cond)
				{
					cond->Lock();
					cond->Broadcast();
					cond->Unlock();
					cond = NULL;
				}
			}
			U& Content(void)							{return object;}
			const U& Content(void) const				{return object;}
			QReceiver& operator = (const QReceiver& r)
			{
				object = r.object;
				cond = r.cond;
				return *this;
			}
			void GetResult(void) const {}
		private:
			U object;
			DKCondition* cond;
		};
		typedef QReceiver<Result, MessageContent> Receiver;

		////////////////////////////////////////////////////////////////////////////////
		// 생성자
		DKMessageQueue(void)
		{
		}
		~DKMessageQueue(void)
		{
		}
		////////////////////////////////////////////////////////////////////////////////
		// POST (external)
		// PostMessage: 메시지를 보내고 바로 리턴한다.
		void PostMessage(const MessageContent& mesg)
		{
			DKObject<Receiver> rp = DKOBJECT_NEW Receiver(mesg, NULL);
			queueCond.Lock();
			messageQueue.PushFront(rp);
			queueCond.Broadcast();
			queueCond.Unlock();
		}
		// ProcessMessage: 메시지를 보내고 완료될때까지 기다린다.
		Result ProcessMessage(const MessageContent& mesg)
		{
			DKCondition cond;
			DKObject<Receiver> r = DKOBJECT_NEW Receiver(mesg, &cond);

			queueCond.Lock();
			messageQueue.PushFront(r);
			queueCond.Broadcast();
			queueCond.Unlock();

			cond.Lock();
			cond.Wait();		// 처리되면 signal 상태가 된다. (다른 쓰레드에서 SetResult 를 호출해야함)
			cond.Unlock();
			return r->GetResult();
		}
		// GetMessageTimeOut, timeout 동안 메시지를 가져오지 못하면 NULL 리턴
		DKObject<Receiver> GetMessageTimeOut(double timeout)
		{
			DKObject<Receiver> ret = NULL;
			DKTimer timer;
			timer.Reset();

			queueCond.Lock();
			while (messageQueue.PopBack(ret) == false)
			{
				double dt = timeout - timer.Elapsed();
				if (dt > 0)
				{
					if (queueCond.WaitTimeout(dt))
						continue;
					else
						break;
				}
				else
				{
					break;
				}
			}
			if (messageQueue.Count() > 0)
				queueCond.Signal();		// 다른 쓰레드를 위해 시그널 상태로 만듦
			queueCond.Unlock();
			return ret;
		}
		// GetMessage 메시지 바로 가져오기 (메시지가 없고, wait=false 이면 NULL 리턴), 가져온 메시지는 큐에서 제거됨
		DKObject<Receiver> GetMessage(bool wait)
		{
			DKObject<Receiver> ret = NULL;

			queueCond.Lock();
			while (messageQueue.PopBack(ret) == false && wait)
			{
				queueCond.Wait();
			}
			if (messageQueue.Count() > 0)
			{
				DKASSERT_DEBUG(ret != NULL);
				queueCond.Signal();		// 다른 쓰레드를 위해 시그널 상태로 만듦
			}
			queueCond.Unlock();
			return ret;
		}
		// PeekMessage: 메시지 복사해서 가져오기 (메시지가 없으면 false, 큐에 메시지 남아있음)
		bool PeekMessage(MessageContent& mesg) const
		{
			DKObject<Receiver> rp = NULL;
			bool ret = false;
			queueCond.Lock();
			if (messageQueue.Back(rp))
			{
				mesg = rp->Content();
				ret = true;
			}
			queueCond.Unlock();
			return ret;
		}
		size_t PendingMessageCount(void) const
		{
			return messageQueue.Count();
		}
		
	private:
		DKCondition						queueCond;
		DKQueue<DKObject<Receiver>>		messageQueue;
	};
}

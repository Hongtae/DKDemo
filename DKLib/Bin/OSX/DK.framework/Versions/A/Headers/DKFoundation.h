#pragma once

#ifdef _MSC_VER
#include "DKFoundation_msvc.h"
#else

#include "DKInclude.h"

// 기본 객체 및 메모리 관리
#include "DKFoundation/DKMemory.h"
#include "DKFoundation/DKObject.h"
#include "DKFoundation/DKAllocator.h"
#include "DKFoundation/DKTypeInfo.h"
#include "DKFoundation/DKTypeList.h"
#include "DKFoundation/DKTypeTraits.h"
#include "DKFoundation/DKSingleton.h"
#include "DKFoundation/DKSharedInstance.h"

// 스트링
#include "DKFoundation/DKString.h"
#include "DKFoundation/DKStringU8.h"

// 데이터 콜렉션
#include "DKFoundation/DKArray.h"
#include "DKFoundation/DKCircularQueue.h"
#include "DKFoundation/DKList.h"
#include "DKFoundation/DKMap.h"
#include "DKFoundation/DKOrderedArray.h"
#include "DKFoundation/DKSet.h"
#include "DKFoundation/DKStack.h"
#include "DKFoundation/DKStaticArray.h"
#include "DKFoundation/DKTuple.h"
#include "DKFoundation/DKQueue.h"

// 해쉬, UUID
#include "DKFoundation/DKHash.h"
#include "DKFoundation/DKUUID.h"

// 쓰레드, 뮤텍스, 동기화 객체
#include "DKFoundation/DKAtomicNumber32.h"
#include "DKFoundation/DKAtomicNumber64.h"
#include "DKFoundation/DKCriticalSection.h"
#include "DKFoundation/DKDummyLock.h"
#include "DKFoundation/DKFence.h"
#include "DKFoundation/DKLock.h"
#include "DKFoundation/DKMutex.h"
#include "DKFoundation/DKSharedLock.h"
#include "DKFoundation/DKSpinLock.h"
#include "DKFoundation/DKThread.h"
#include "DKFoundation/DKCondition.h"

// 스트림, 파일, 데이터, 디렉토리
#include "DKFoundation/DKData.h"
#include "DKFoundation/DKStream.h"
#include "DKFoundation/DKDataStream.h"
#include "DKFoundation/DKBuffer.h"
#include "DKFoundation/DKBufferStream.h"
#include "DKFoundation/DKDirectory.h"
#include "DKFoundation/DKFile.h"
#include "DKFoundation/DKFileMap.h"
#include "DKFoundation/DKZipArchiver.h"
#include "DKFoundation/DKZipUnarchiver.h"

// XML
#include "DKFoundation/DKXMLParser.h"
#include "DKFoundation/DKXMLDocument.h"

// 날짜 시간, 타이머
#include "DKFoundation/DKTimer.h"
#include "DKFoundation/DKDateTime.h"
#include "DKFoundation/DKRational.h"

// 오퍼레이션, 인보케이션
#include "DKFoundation/DKFunction.h"
#include "DKFoundation/DKInvocation.h"
#include "DKFoundation/DKCallback.h"
#include "DKFoundation/DKOperation.h"
#include "DKFoundation/DKValue.h"

// 런루프, 오퍼레이션 큐, 메시지 핸들러
#include "DKFoundation/DKMessageQueue.h"
#include "DKFoundation/DKOperationQueue.h"
#include "DKFoundation/DKRunLoop.h"
#include "DKFoundation/DKRunLoopTimer.h"

// 기타
#include "DKFoundation/DKEndianness.h"
#include "DKFoundation/DKError.h"
#include "DKFoundation/DKLog.h"
#include "DKFoundation/DKUtils.h"

#endif //ifdef _MSC_VER

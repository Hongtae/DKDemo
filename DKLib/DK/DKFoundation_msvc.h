#pragma once

#ifndef _MSC_VER
#error "Use this header only with Microsoft Visual C++ compilers!"  
#endif

#include "DKInclude.h"

// 기본 객체 및 메모리 관리
#include "DKFoundation_msvc/DKMemory.h"
#include "DKFoundation_msvc/DKObject.h"
#include "DKFoundation_msvc/DKAllocator.h"
#include "DKFoundation_msvc/DKTypeInfo.h"
#include "DKFoundation_msvc/DKTypeList.h"
#include "DKFoundation_msvc/DKTypeTraits.h"
#include "DKFoundation_msvc/DKSingleton.h"
#include "DKFoundation_msvc/DKSharedInstance.h"

// 스트링
#include "DKFoundation_msvc/DKString.h"
#include "DKFoundation_msvc/DKStringU8.h"

// 데이터 콜렉션
#include "DKFoundation_msvc/DKArray.h"
#include "DKFoundation_msvc/DKCircularQueue.h"
#include "DKFoundation_msvc/DKList.h"
#include "DKFoundation_msvc/DKMap.h"
#include "DKFoundation_msvc/DKOrderedArray.h"
#include "DKFoundation_msvc/DKSet.h"
#include "DKFoundation_msvc/DKStack.h"
#include "DKFoundation_msvc/DKStaticArray.h"
#include "DKFoundation_msvc/DKTuple.h"
#include "DKFoundation_msvc/DKQueue.h"

// 해쉬, UUID
#include "DKFoundation_msvc/DKHash.h"
#include "DKFoundation_msvc/DKUUID.h"

// 쓰레드, 뮤텍스, 동기화 객체
#include "DKFoundation_msvc/DKAtomicNumber32.h"
#include "DKFoundation_msvc/DKAtomicNumber64.h"
#include "DKFoundation_msvc/DKCriticalSection.h"
#include "DKFoundation_msvc/DKDummyLock.h"
#include "DKFoundation_msvc/DKFence.h"
#include "DKFoundation_msvc/DKLock.h"
#include "DKFoundation_msvc/DKMutex.h"
#include "DKFoundation_msvc/DKSharedLock.h"
#include "DKFoundation_msvc/DKSpinLock.h"
#include "DKFoundation_msvc/DKThread.h"
#include "DKFoundation_msvc/DKCondition.h"

// 스트림, 파일, 데이터, 디렉토리
#include "DKFoundation_msvc/DKData.h"
#include "DKFoundation_msvc/DKStream.h"
#include "DKFoundation_msvc/DKDataStream.h"
#include "DKFoundation_msvc/DKBuffer.h"
#include "DKFoundation_msvc/DKBufferStream.h"
#include "DKFoundation_msvc/DKDirectory.h"
#include "DKFoundation_msvc/DKFile.h"
#include "DKFoundation_msvc/DKFileMap.h"
#include "DKFoundation_msvc/DKZipArchiver.h"
#include "DKFoundation_msvc/DKZipUnarchiver.h"

// XML
#include "DKFoundation_msvc/DKXMLParser.h"
#include "DKFoundation_msvc/DKXMLDocument.h"

// 날짜 시간, 타이머
#include "DKFoundation_msvc/DKTimer.h"
#include "DKFoundation_msvc/DKDateTime.h"
#include "DKFoundation_msvc/DKRational.h"

// 오퍼레이션, 인보케이션
#include "DKFoundation_msvc/DKFunction.h"
#include "DKFoundation_msvc/DKInvocation.h"
#include "DKFoundation_msvc/DKCallback.h"
#include "DKFoundation_msvc/DKOperation.h"
#include "DKFoundation_msvc/DKValue.h"

// 런루프, 오퍼레이션 큐, 메시지 핸들러
#include "DKFoundation_msvc/DKMessageQueue.h"
#include "DKFoundation_msvc/DKOperationQueue.h"
#include "DKFoundation_msvc/DKRunLoop.h"
#include "DKFoundation_msvc/DKRunLoopTimer.h"

// 기타
#include "DKFoundation_msvc/DKEndianness.h"
#include "DKFoundation_msvc/DKError.h"
#include "DKFoundation_msvc/DKLog.h"
#include "DKFoundation_msvc/DKUtils.h"


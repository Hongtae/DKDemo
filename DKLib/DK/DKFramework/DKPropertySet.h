//
//  File: DKPropertySet.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKVariant.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKPropertySet
//
// Key-Value Pair 로 값이 저장되며, URI 에 저장된 속성들을 import 할수 있고
// 파일로 export 할 수 있다. value 는 DKVariant 가 사용된다.
//
// 값의 추가,변경,제거를 모니터링 할 수 있다.
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKLIB_API DKPropertySet
	{
	public:
		static DKPropertySet& DefaultSet(void);

		typedef void InsertionFunc(DKFoundation::DKString, DKVariant);					// key, new-value
		typedef void ModificationFunc(DKFoundation::DKString, DKVariant, DKVariant);	// key, old-value, new-value
		typedef void DeletionFunc(DKFoundation::DKString, DKVariant);					// key, old-value

		typedef DKFoundation::DKFunctionSignature<InsertionFunc>		InsertionCallback;
		typedef DKFoundation::DKFunctionSignature<ModificationFunc>		ModificationCallback;
		typedef DKFoundation::DKFunctionSignature<DeletionFunc>			DeletionCallback;
		typedef DKFoundation::DKFunctionSignature<void (const DKFoundation::DKString&, const DKVariant&)> Enumerator;

		DKPropertySet(void);
		~DKPropertySet(void);

		// 파일 또는 URL 에서 임포트. 임포트한 property 개수 리턴. (올바르지 않은 파일이면 -1 리턴)
		int Import(const DKFoundation::DKString& url, bool overwrite);
		int Import(const DKPropertySet& prop, bool overwrite);
		int Import(const DKFoundation::DKXMLElement* e, bool overwrite);
		int Import(DKFoundation::DKStream* stream, bool overwrite);
		int Export(const DKFoundation::DKString& file, bool exportXML) const;
		int Export(DKFoundation::DKStream* stream, bool exportXML) const;
		DKFoundation::DKObject<DKFoundation::DKXMLElement> Export(bool exportXML, int* numExported) const;

		// SetInitialValue, 값이 없으면 value 로 설정한다. 값이 있으면 false 리턴함
		bool SetInitialValue(const DKFoundation::DKString& key, const DKVariant& value);
		void SetValue(const DKFoundation::DKString& key, const DKVariant& value);
		const DKVariant& Value(const DKFoundation::DKString& key) const;
		bool HasValue(const DKFoundation::DKString& key) const;
		void Remove(const DKFoundation::DKString& key);
		size_t NumberOfEntries(void) const;

		// 관찰자 추가, 제거
		void SetCallback(const DKFoundation::DKString& key, InsertionCallback* insertion, ModificationCallback* modification, DeletionCallback* deletion, DKFoundation::DKRunLoop* runLoop, void* context);
		void RemoveCallback(const DKFoundation::DKString& key, void* context);
		void RemoveCallback(void* context);

		// 읽기 전용으로 key,value 열거함
		void EnumerateForward(const Enumerator* e) const;
		void EnumerateBackward(const Enumerator* e) const;
				
	private:
		DKFoundation::DKSpinLock lock;
		typedef DKFoundation::DKMap<DKFoundation::DKString, DKVariant> PropertyMap;
		PropertyMap properties;

		typedef DKFoundation::DKMap<DKFoundation::DKString, DKFoundation::DKCallback<InsertionFunc, void*, DKFoundation::DKSpinLock>>		InsertionCallbackMap;
		typedef DKFoundation::DKMap<DKFoundation::DKString, DKFoundation::DKCallback<ModificationFunc, void*, DKFoundation::DKSpinLock>>	ModificationCallbackMap;
		typedef DKFoundation::DKMap<DKFoundation::DKString, DKFoundation::DKCallback<DeletionFunc, void*, DKFoundation::DKSpinLock>>		DeletionCallbackMap;

		InsertionCallbackMap		insertionCallbacks;
		ModificationCallbackMap		modificationCallbacks;
		DeletionCallbackMap			deletionCallbacks;
	};
}

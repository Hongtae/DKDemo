//
//  File: DKResourcePool.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKResource.h"
#include "DKResourceLoader.h"

#ifdef FindResource
#undef FindResource
#endif

////////////////////////////////////////////////////////////////////////////////
//
// DKResourcePool
//
// DKFramework 에서 사용되는 리소스들을 로딩하고 관리하게 된다.
// 리소스 디렉토리는 DKDirectory 와 DKZipUnarchiver 로 추가가 된다.
// 리소스를 읽는 방식은 해당 디렉토리의 루트에 있으면 파일명을 넘기며
// url 를 사용하면 네트워크로 읽어올 수 있다. 또는 디렉토리명을 명시하여 하위 디렉토리도 가져올 수 있다.
//
// 주의 1:
// LoadResource(DKString&) 함수는 실제 프레임워크 객체를 읽어오지만
// LoadResourceData(DKString&) 함수를 호출하면 버퍼만 읽어들인다. (DKData 형태로 보관한다)
// 주의 2:
// 데이터를 보관할 필요가 없는것은 OpenResourceStream 으로 읽어야 한다.
//
// 리소스는 DKResource 객체이며, DKResource 객체가 아닌것은 LoadResourceData 로 읽음.
//
// SetMemoryLocation 함수를 사용하여 힙 또는 가상메모리 영역에 메모리를 할당할 수 있다.
//
// 예:
//  DKResourcePool pool;
//  pool.AddSearchPath("/data/dir");                    /data/dir 디렉토리 추가
//  pool.AddSearchPath("/data/dir/file.zip");           file.zip 추가
//  pool.AddSearchPath("/data/dir/file.zip/prefix");	file.zip 의 prefix 포함
//
//  pool.LoadResource("MyFile.dat");      MyFile.dat 파일 로딩.
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKLIB_API DKResourcePool : public DKResourceLoader
	{
	public:
		struct Locator
		{
			virtual ~Locator(void) {}
			virtual DKFoundation::DKString FindSystemPath(const DKFoundation::DKString&) const = 0;
			virtual DKFoundation::DKObject<DKFoundation::DKStream> OpenStream(const DKFoundation::DKString&) const = 0;
		};

		DKResourcePool(void);
		~DKResourcePool(void);

		bool AddLocator(Locator*, const DKFoundation::DKString& name);
		Locator* AddLocatorForPath(const DKFoundation::DKString& path);
		Locator* FindLocator(const DKFoundation::DKString& name);
		void RemoveLocator(const DKFoundation::DKString& name);
		void RemoveAllLocators(void);
		DKFoundation::DKString::StringArray AllLocatorNames(void) const;

		bool AddSearchPath(const DKFoundation::DKString& path)
		{
			return AddLocatorForPath(path) != NULL;
		}

		DKFoundation::DKObject<DKResource> FindResource(const DKFoundation::DKString& name) const;		// 풀에서 리소스 찾기
		DKFoundation::DKObject<DKFoundation::DKData> FindResourceData(const DKFoundation::DKString& name) const;
		DKFoundation::DKObject<DKResource> LoadResource(const DKFoundation::DKString& name);			// 리소스를 읽어온다
		DKFoundation::DKObject<DKFoundation::DKData> LoadResourceData(const DKFoundation::DKString& name, bool mapFileIfPossible = true);

		void AddResource(const DKFoundation::DKString& name, DKResource* res);					// 리소스 추가
		void AddResourceData(const DKFoundation::DKString& name, DKFoundation::DKData* data);	// 데이터 추가
		void RemoveResource(const DKFoundation::DKString& name);								// 리소스 제거
		void RemoveResourceData(const DKFoundation::DKString& name);
		void RemoveAllResourceData(void);									// 캐쉬된 데이터 제거
		void RemoveAllResources(void);										// 캐쉬된 리소스 제거
		void RemoveAll(void);

		DKFoundation::DKString ResourceFilePath(const DKFoundation::DKString& name) const;								// 리소스 디렉토리에서 파일 검색 (디렉토리만 가능)
		DKFoundation::DKObject<DKFoundation::DKStream> OpenResourceStream(const DKFoundation::DKString& name) const;	// 리소스 찾아서 스트림으로 연다

		DKFoundation::DKObject<DKResourcePool> Clone(void) const;

		void SetAllocator(DKFoundation::DKMemoryLocation loc);	// 기본 할당자 사용
		void SetAllocator(DKFoundation::DKAllocator* alloc);	// 기본 할당자 또는 사용자 정의 사용
		DKFoundation::DKAllocator& Allocator(void) const;

	private:
		struct NamedLocator
		{
			DKFoundation::DKString name;
			DKFoundation::DKObject<Locator> locator;
		};
		DKFoundation::DKArray<NamedLocator> locators;

		typedef DKFoundation::DKMap<DKFoundation::DKString, DKFoundation::DKObject<DKResource>>						ResourceMap;
		typedef DKFoundation::DKMap<DKFoundation::DKString, DKFoundation::DKObject<DKFoundation::DKData>>			DataMap;
		ResourceMap			resources;
		DataMap				resourceData;

		DKFoundation::DKSpinLock lock;
		mutable DKFoundation::DKAllocator* allocator;
	};
}

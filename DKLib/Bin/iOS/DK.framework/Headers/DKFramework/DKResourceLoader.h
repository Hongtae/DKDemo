//
//  File: DKResourceLoader.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"

///////////////////////////////////////////////////////////////////////////////
//
// DKResourceLoader
//
// DKResource 객체를 생성하고 파일에서 읽어들인다. (서브클래싱 하여 사용함)
// XML 또는 바이너리의 DKSerializer 파일을 이용하여 마샬링 하며,
// 파일 확장자를 등록하여 사용할 수 있다.
//
// 자동화된 리소스 마샬링 두가지 방법
// 1. Serializer() 함수 재정의. 고유의 DKSerializer 객체를 생성한다. (이경우 Serialize / Deserialize 재정의 필요없음)
// 2. Serialize() / Deserialize() 재정의. (이경우 Serializer() 재정의 필요없음. 직접 XML 읽고 쓰기 해야한다)
//
// 도큐먼트 로딩 (XML 파일 또는 일부분-element, file, http, ftp, zip 가능)
//  RegisterAllocator 사용하여 도큐먼트(XML Element) 를 등록한다.
//  XML 형식의 도큐먼트를 사용할 수 있다. Serializer() 를 재정의 하거나 Serialize / Deserialize 재정의함
//
// 파일 사용하여 로딩 (XML 이 아닌 다른 형식의 파일, file, zip 가능)
//  RegisterFileExtension 을 사용하여 파일 확장자 등록. 해당 파일이 로딩될때 등록된 Loader 가 호출된다.
//  (이미지 파일들 jpg,png 등의 일반 바이너리 포맷에서 사용가능함)
//  Loader 는 해당 객체에서 직접 구현해야 한다. (실패시 NULL 리턴가능함)
//
// DKSerializer 를 사용하여 DKResource 를 상속받지 않아도 마샬링이 가능함 (단, 해당 클래스는 외부 리소스로는 사용 불가능)
//
// DKLIB_STATIC_RESOURCE_CLASS, DKLIB_STATIC_RESOURCE_CLASS_URI 매크로틀 이용하여 등록하는
// 클래스ID 와 URI 는 XML 문서일 경우 사용되며, DKSerializer 를 사용할경우에도 클래스 정보로 사용된다.
//
// DKSerializer 를 사용하지 않을 경우엔 Serialize 또는 SerializeXML 와 Deserialize 를 구현해줘야 한다.
// XML 을 사용할 경우엔 XMLElement 에 등록한 클래스ID와 URI를 사용하여야 한다.
//
///////////////////////////////////////////////////////////////////////////////


namespace DKFramework
{
	class DKResource;
	class DKLIB_API DKResourceLoader
	{
	public:
		DKResourceLoader(void);
		virtual ~DKResourceLoader(void);

		DKFoundation::DKObject<DKResource> ResourceFromXML(const DKFoundation::DKXMLElement* element);
		DKFoundation::DKObject<DKResource> ResourceFromData(const DKFoundation::DKData* data, const DKFoundation::DKString& name);
		DKFoundation::DKObject<DKResource> ResourceFromStream(DKFoundation::DKStream* stream, const DKFoundation::DKString& name);
		DKFoundation::DKObject<DKResource> ResourceFromFile(const DKFoundation::DKString& path, const DKFoundation::DKString& name);

		// resource-pool
		virtual void AddResource(const DKFoundation::DKString& name, DKResource* res) = 0;
		virtual DKFoundation::DKObject<DKResource> FindResource(const DKFoundation::DKString& name) const = 0;
		virtual DKFoundation::DKObject<DKResource> LoadResource(const DKFoundation::DKString& name) = 0;
		virtual DKFoundation::DKObject<DKFoundation::DKStream> OpenResourceStream(const DKFoundation::DKString& name) const = 0;

		// resource memory location (Memory-Allocator)
		virtual DKFoundation::DKAllocator& Allocator(void) const;

		// ResourceAllocator : DKResource 객체를 생성함
		typedef DKFoundation::DKFunctionSignature<DKFoundation::DKObject<DKResource>(DKFoundation::DKAllocator&)> ResourceAllocator;
		// ResourceLoader : 스트림으로부터 객체를 복원함.
		typedef DKFoundation::DKFunctionSignature<DKFoundation::DKObject<DKResource>(DKFoundation::DKStream*, DKFoundation::DKAllocator&)> ResourceLoader;

		static void SetResourceAllocator(const DKFoundation::DKString& URI, const DKFoundation::DKString& classId, ResourceAllocator* allocator);
		static void SetResourceFileExtension(const DKFoundation::DKString& ext, ResourceLoader* loader);
	};

	// DKResourceAlloc 함수는 리소스 객체를 생성할때 사용되며, DKResourcePool 에서 호출된다. (alloc 은 DKResourceLoader 에서 넘겨줌)
	// 이 함수는 재정의 가능하며, 어플리케이션에서 직접 제공할 수 있다. (DKResource::RegisterAllocator 사용하여 등록함)
	template <typename T> static DKFoundation::DKObject<DKResource> DKResourceAlloc(DKFoundation::DKAllocator& alloc)
	{
		return DKFoundation::DKObject<T>(new (alloc)T()).template SafeCast<DKResource>();
	}
}

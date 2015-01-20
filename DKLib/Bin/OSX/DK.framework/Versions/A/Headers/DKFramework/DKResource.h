//
//  File: DKResource.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKSerializer.h"
#include "DKVariant.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKResource
//
// 시스템에서 사용되는 리소스이며, 파일 또는 네트워크로 전송이 가능한 객체이다.
// DKResourceLoader 를 이용하여 객체를 복원하거나, Serializer 를 이용하여 복원 할 수 있다.
//
////////////////////////////////////////////////////////////////////////////////


namespace DKFramework
{
	class DKResourceLoader;
	class DKLIB_API DKResource
	{
	public:
		DKResource(void);
		virtual ~DKResource(void);

		virtual void SetName(const DKFoundation::DKString& name);
		const DKFoundation::DKString& Name(void) const;
		virtual void SetUUID(const DKFoundation::DKUUID& uuid);
		const DKFoundation::DKUUID& UUID(void) const;

		virtual DKFoundation::DKObject<DKSerializer> Serializer(void);
		virtual DKFoundation::DKObject<DKFoundation::DKData> Serialize(DKSerializer::SerializeForm) const;
		virtual DKFoundation::DKObject<DKFoundation::DKXMLElement> SerializeXML(DKSerializer::SerializeForm) const;
		virtual bool Deserialize(const DKFoundation::DKData*, DKResourceLoader*);
		virtual bool Deserialize(const DKFoundation::DKXMLElement*, DKResourceLoader*);

		// 리소스 Validation
		virtual bool Validate(void);

		DKVariant::VPairs metadata;

	protected:
		DKFoundation::DKAllocator& Allocator(void);

	private:
		DKFoundation::DKString objectName;
		DKFoundation::DKUUID objectUUID;
		DKFoundation::DKAllocator* allocator;

		DKResource(const DKResource&);
		DKResource& operator = (const DKResource&);

		friend class DKResourceLoader;
	};
}

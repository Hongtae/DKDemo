//
//  File: DKTypeInfo.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include <typeinfo>
#include "DKString.h"

namespace DKFoundation
{
	class DKLIB_API DKTypeInfo
	{
	public:
		DKTypeInfo(void);
		DKTypeInfo(const DKTypeInfo& ti);
		DKTypeInfo(const std::type_info&);
		~DKTypeInfo(void);

		bool IsValid(void) const;

		DKString Name(void) const;
		bool Before(const DKTypeInfo& rhs) const;
		operator const std::type_info& (void) const;

		DKTypeInfo& operator = (const DKTypeInfo& ti);

		bool operator == (const DKTypeInfo& rhs) const;
		bool operator != (const DKTypeInfo& rhs) const;
		bool operator < (const DKTypeInfo& rhs) const;
		bool operator > (const DKTypeInfo& rhs) const;
		bool operator <= (const DKTypeInfo& rhs) const;
		bool operator >= (const DKTypeInfo& rhs) const;

	private:
		const std::type_info* info;
	};
}

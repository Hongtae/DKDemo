//
//  File: DKString.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKStringUE.h"
#include "DKStringU8.h"
#include "DKStringW.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKString
//
// 윈도우(win32) 에서는 utf-8 이 아직 기본적으로 안되기 때문에 wchar_t 를 기본으로 사용한다.
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	typedef DKStringW DKString;
}

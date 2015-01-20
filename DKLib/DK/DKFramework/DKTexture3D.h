//
//  File: DKTexture3D.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKSize.h"
#include "DKRect.h"
#include "DKResource.h"
#include "DKColor.h"
#include "DKTexture.h"

namespace DKFramework
{
	class DKLIB_API DKTexture3D : public DKTexture
	{
	public:
		DKTexture3D(void);
		~DKTexture3D(void);


		DKFoundation::DKObject<DKSerializer> Serializer(void);
	};
}

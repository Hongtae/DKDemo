//
//  File: DKShader.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2006-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKShader
//
// GLSL 데이터를 받아서 컴파일 하는 객체.
// 하나의 쉐이더가 하나의 객체가 되며, DKShaderProgram 을 통하여, 여러개의 쉐이더가
// 하나의 모듈로 합쳐지고 렌더링시 DCU 로 전송된다.
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKLIB_API DKShader
	{
	public:
		enum Type
		{
			TypeUnknown = 0,
			TypeVertexShader,
			TypeFragmentShader,
			TypeGeometryShader,
		};

		DKShader(void);
		~DKShader(void);

		unsigned int GetId(void) const	{return objectId;}

		bool IsValid(void) const;
		Type GetType(void) const;

		// 일반 쉐이더 객체 생성
		static DKFoundation::DKObject<DKShader> Create(const DKFoundation::DKString& source, Type t);
		static DKFoundation::DKObject<DKShader> Create(const DKFoundation::DKString& source, Type t, DKFoundation::DKString& err);

		DKFoundation::DKString GetSource(void) const;

	private:
		unsigned int objectId;
	};
}

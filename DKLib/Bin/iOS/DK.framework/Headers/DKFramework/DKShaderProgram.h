//
//  File: DKShaderProgram.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2006-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKShader.h"
#include "DKVertexStream.h"
#include "DKShaderConstant.h"

////////////////////////////////////////////////////////////////////////////////
//
//	DKShaderProgram
//
// 컴파일된 쉐이더(DKShader) 객체를 모아서 링크한다. 실제 DCU 로 올라가는 프로그램을
// 모듈화 한다.
//
// Uniform 값은 버텍스 쉐이더와 프래그먼트 쉐이더에서 읽을 수 있는 변수이다.
// Attribute 는 배열값으로 버텍스의 크기만큼 넣어야 하며, 버텍스 쉐이더에서
// 버텍스를 처리할때마다 순서대로 하나씩 읽혀진다
// Uniform 과 Attribute 는 한번 설정해두면 Destroy 할때까지 유지된다.
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKLIB_API DKShaderProgram
	{
	public:
		typedef DKFoundation::DKArray<DKShaderConstant>		UniformArray;
		typedef DKFoundation::DKArray<DKVertexStream>		AttributeArray;

		AttributeArray		attributes;
		UniformArray		uniforms;

		DKShaderProgram(void);
		~DKShaderProgram(void);

		void Bind(void) const;

		static DKFoundation::DKObject<DKShaderProgram> Create(const DKFoundation::DKArray<DKFoundation::DKObject<DKShader>>& shaders, DKFoundation::DKString& err);
		static DKFoundation::DKObject<DKShaderProgram> Create(const DKFoundation::DKArray<DKShader*>& shaders, DKFoundation::DKString& err);

		unsigned int GetId(void) const				{return programId;}
		bool IsValid(void) const;

		int	GetUniformComponents(const DKFoundation::DKString& name) const;
		int GetAttribComponents(const DKFoundation::DKString& name) const;

	private:
		unsigned int programId;
	};
}

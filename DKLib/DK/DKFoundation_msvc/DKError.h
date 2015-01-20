//
//  File: DKError.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKString.h"
#include "DKFunction.h"
#include "DKStream.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKError 는 예외처리를 위한 객체이며, 콜스택 정보를 조회하거나 파일로 저장할 수 있다.
//
// DKError 를 사용한 예외 처리
// SetDefaultDescriptor() 를 호출하여, 에러 발생시 관련 정보를 어떻게 처리할지 정할 수 있다.
// (예를들면, 서버로 보내거나, 다른 프로그램을 띄우거나 할수 있음)
//
//	try
//	{
//		.... 어딘가에서 예외 발생함 (DKERROR_THROW 사용)
//	}
//	catch (DKError& e)
//	{
//		// 복구 시도함..
//
//		e.PrintDescriptionWithStackFrames();	// 콘솔에 콜스택 출력
//		e.WriteToDefaultDescriptor();			// 사용자 정의 에러 파일에 정보 기록함
//		e.WriteToFile(myLogFile);				// myLogFile 에 정보 기록함
//	}
//
////////////////////////////////////////////////////////////////////////////////

#define DKERROR_DEFAULT_CALLSTACK_TRACE_DEPTH	1024

namespace DKFoundation
{
	typedef DKFunctionSignature<void (class DKError&)> DKCriticalErrorHandler;

	DKLIB_API bool IsDebuggerPresent(void);
	DKLIB_API bool IsDebugBuild(void);
	DKLIB_API void SetCriticalErrorHandler(DKCriticalErrorHandler*);	// 일부 플랫폼에서는 작동 안함
	DKLIB_API void DKErrorRaiseException(const char*, const char*, unsigned int, const char*);

	namespace Private { struct UnexpectedError; }

	class DKLIB_API DKError
	{
	public:
		static int DefaultCallstackTraceDepth(void) {return 1024;}

		struct StackFrame
		{
			DKString	module;				// module file
			void*		baseAddress;		// module base address
			DKString	function;			// function name
			void*		address;			// frame address
			size_t		offset;				// function offset
			DKString	filename;			// source-file
			size_t		line;				// source-file line
		};
		typedef DKFunctionSignature<void (const void*, size_t)> Descriptor;
		typedef DKFunctionSignature<void (const DKString&)> StringOutput;

		DKError(void);
		DKError(const DKString& desc);
		DKError(int errorCode, const DKString& desc);
		DKError(DKError&&);
		DKError(const DKError&);
		~DKError(void);

		DKError& operator = (DKError&&);
		DKError& operator = (const DKError&);

		static void RaiseException(const DKString& func, const DKString& file, unsigned int line, const DKString& desc);
		static void RaiseException(const DKError& e);
		static void RaiseException(int errorCode, const DKString& desc);
		static void RaiseException(const DKString& desc);

		int Code(void) const;
		const DKString& Function(void) const;
		const DKString& File(void) const;
		int Line(void) const;
		const DKString& Description(void) const;

		size_t NumberOfStackFrames(void) const;
		const StackFrame* StackFrameAtIndex(unsigned int index) const;
		size_t CopyStackFrames(StackFrame* s, size_t maxCount) const;

		// 콜스택 데이터 재생성
		size_t RetraceStackFrames(size_t skip = 0, size_t maxDepth = DKERROR_DEFAULT_CALLSTACK_TRACE_DEPTH);

		// 에러 정보 기록
		void PrintDescription(void) const;
		void PrintDescription(const StringOutput*) const;
		void PrintStackFrames(void) const;
		void PrintStackFrames(const StringOutput*) const;
		void PrintDescriptionWithStackFrames(void) const;
		void PrintDescriptionWithStackFrames(const StringOutput*) const;
		// 파일 또는 스트림에 정보 기록
		void WriteToDescriptor(const Descriptor*) const;
		void WriteToDefaultDescriptor(void) const;
		bool WriteToFile(const DKString& file) const;
		bool WriteToStream(DKStream* stream) const;

		// 기본 에러 저장 설정. SetDefaultDescriptor(데이터, 크기) 가 호출된다.
		static void SetDefaultDescriptor(const Descriptor* output);

	private:
		int errorCode;
		DKString functionName;
		DKString fileName;
		unsigned int lineNo;
		DKString description;

		// 콜스택
		size_t numFrames;
		StackFrame* stackFrames;
		void* threadId;
		friend struct Private::UnexpectedError;
		static void DumpUnexpectedError(Private::UnexpectedError*);
	};
}

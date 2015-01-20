//
//  File: DKStringW.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include <stdarg.h>		// for va_list
#include "DKObject.h"
#include "DKSet.h"
#include "DKArray.h"
#include "DKStringUE.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKStringW
//
// 유니코드는 시스템의 DKUniCharW(wchar_t) 를 사용하게 되고
// 유니코드가 아닐 경우 멀티바이트는 처리하지 않는다.!! CP367 (ASCII) 만 사용가능하다.
// 예외로 다른 로케일로 인코딩 된 경우엔 유니코드로 인코딩 가능하다.
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	class DKData;
	class DKLIB_API DKStringW
	{
	public:
		typedef DKSet<DKUniCharW, DKDummyLock, DKSetComparison<DKUniCharW>> CharacterSet;
		typedef DKArray<long long, DKDummyLock> IntegerArray;
		typedef DKArray<unsigned long long, DKDummyLock> UnsignedIntegerArray;
		typedef DKArray<double, DKDummyLock> RealNumberArray;
		typedef DKArray<DKStringW, DKDummyLock> StringArray;

		static const DKStringW& EmptyString();
		static DKStringEncoding SystemEncoding(void);		// DKUniCharW* 인코딩

		// DKStringW class
		DKStringW(void);
		DKStringW(DKStringW&& str);
		DKStringW(const DKStringW& str);
		DKStringW(const DKUniCharW* str, size_t len = (size_t)-1);
		DKStringW(const DKUniChar8* str, size_t len = (size_t)-1);
		explicit DKStringW(const void* str, size_t len, DKStringEncoding e);
		explicit DKStringW(DKUniCharW c);
		explicit DKStringW(DKUniChar8 c);
		~DKStringW(void);

		DKObject<DKData> Encode(DKStringEncoding e) const;
		static DKStringW Format(const DKUniChar8* fmt, ...);		// (char=%s, wchar_t=%ls), CP367 (ASCII) ONLY!! (대문자S 사용금지)
		static DKStringW Format(const DKUniCharW* fmt, ...);		// %ls 로 유니코드만 사용해야한다. 멀티바이트는 vc 와 gcc 가 서로 다르므로 사용하지 말자
		static DKStringW FormatV(const DKUniChar8* fmt, va_list v);
		static DKStringW FormatV(const DKUniCharW* fmt, va_list v);

		size_t Length(void) const;
		size_t Bytes(void) const;

		long Find(DKUniCharW c, long begin = 0) const;
		long Find(const DKUniCharW* str, long begin = 0) const;
		long Find(const DKStringW& str, long begin = 0) const;
		long FindWhitespaceCharacter(long begin = 0) const;
		long FindAnyCharactersInSet(const CharacterSet& cs, long begin = 0) const;

		DKStringW Right(long index) const;				// index 를 기준으로 오른쪽꺼 복사
		DKStringW Left(size_t count) const;				// 0 ~ nCount 까지 복사
		DKStringW Mid(long index, size_t count) const;	// index 부터 count 만큼 복사
		DKStringW LowercaseString(void) const;
		DKStringW UppercaseString(void) const;
				
		int Compare(const DKUniCharW* str) const;
		int Compare(const DKStringW& str) const;
		int CompareNoCase(const DKUniCharW* str) const;
		int CompareNoCase(const DKStringW& str) const;
		
		int Replace(const DKUniCharW c1, const DKUniCharW c2);
		int Replace(const DKUniCharW* strOld, const DKUniCharW* strNew);
		int Replace(const DKStringW& src, const DKStringW& dst); 
		DKStringW& Insert(long index, const DKUniCharW* str);
		DKStringW& Insert(long index, DKUniCharW ch);

		bool HasPrefix(const DKStringW& str) const;
		bool HasSuffix(const DKStringW& str) const;
		DKStringW& RemovePrefix(const DKStringW& str);
		DKStringW& RemoveSuffix(const DKStringW& str);

		// 디렉토리 경로 관련
		DKStringW FilePathString(void) const;
		DKStringW FilePathStringByAppendingPath(const DKStringW& path) const;
		DKStringW LastPathComponent(void) const;
		StringArray PathComponents(void) const;

		// 공백 관련
		bool IsWhitespaceCharacterAtIndex(long index) const;
		DKStringW& TrimWhitespaces(void);									// 스트링 앞뒤의 공백 제거 (공백과 escape sequences들 제거함)
		DKStringW& RemoveWhitespaces(long begin = 0, long count = -1);		// 공백 제거 (공백과 escape sequence 들만 제거함)

		// append, set
		DKStringW& Append(const DKStringW& str);			// 스트링 더함
		DKStringW& Append(const DKUniCharW* str, size_t len = (size_t)-1);
		DKStringW& Append(const DKUniChar8* str, size_t len = (size_t)-1);
		DKStringW& Append(const void* str, size_t bytes, DKStringEncoding e);
		DKStringW& SetValue(const DKStringW& str);
		DKStringW& SetValue(const DKUniCharW* str, size_t len = (size_t)-1);
		DKStringW& SetValue(const DKUniChar8* str, size_t len = (size_t)-1);
		DKStringW& SetValue(const void* str, size_t bytes, DKStringEncoding e);

		// assignment operators
		DKStringW& operator = (DKStringW&& str);
		DKStringW& operator = (const DKStringW& str);
		DKStringW& operator = (const DKUniCharW* str);
		DKStringW& operator = (const DKUniChar8* str);
		DKStringW& operator = (DKUniCharW ch);
		DKStringW& operator = (DKUniChar8 ch);

		// conversion operators
		operator const DKUniCharW* (void) const;

		// concatenation operators
		DKStringW& operator += (const DKStringW& str);
		DKStringW& operator += (const DKUniCharW* str);
		DKStringW& operator += (const DKUniChar8* str);
		DKStringW& operator += (DKUniCharW ch);
		DKStringW& operator += (DKUniChar8 ch);
		DKStringW operator + (const DKStringW& str) const;
		DKStringW operator + (const DKUniCharW* str) const;
		DKStringW operator + (const DKUniChar8* str) const;
		DKStringW operator + (DKUniCharW c) const;
		DKStringW operator + (DKUniChar8 c) const;

		// comparison operators
		bool operator > (const DKStringW& str) const			{return Compare(str) > 0;}
		bool operator > (const DKUniCharW* str) const			{return Compare(str) > 0;}
		bool operator < (const DKStringW& str) const			{return Compare(str) < 0;}
		bool operator < (const DKUniCharW* str) const			{return Compare(str) < 0;}
		bool operator >= (const DKStringW& str) const			{return Compare(str) >= 0;}
		bool operator >= (const DKUniCharW* str) const			{return Compare(str) >= 0;}
		bool operator <= (const DKStringW& str) const			{return Compare(str) <= 0;}
		bool operator <= (const DKUniCharW* str) const			{return Compare(str) <= 0;}
		bool operator == (const DKStringW& str) const			{return Compare(str) == 0;}
		bool operator == (const DKUniCharW* str) const			{return Compare(str) == 0;}
		bool operator != (const DKStringW& str) const			{return Compare(str) != 0;}
		bool operator != (const DKUniCharW* str) const			{return Compare(str) != 0;}

		// 숫자형 타입으로 변환
		long long ToInteger(void) const;
		unsigned long long ToUnsignedInteger(void) const;
		double ToRealNumber(void) const;

		IntegerArray ToIntegerArray(const DKStringW& delimiter, bool ignoreEmptyString = false) const;
		UnsignedIntegerArray ToUnsignedIntegerArray(const DKStringW& delimiter, bool ignoreEmptyString = false) const;
		RealNumberArray ToRealNumberArray(const DKStringW& delimiter, bool ignoreEmptyString = false) const;

		// 스트링 쪼개기
		StringArray Split(const DKStringW& delimiter, bool ignoreEmptyString = false) const;
		StringArray SplitByCharactersInSet(const CharacterSet& cs, bool ignoreEmptyString = false) const;
		StringArray SplitByWhitespace(void) const;

	private:
		DKUniCharW* stringData;
	};
}

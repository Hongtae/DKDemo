//
//  File: DKDateTime.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKString.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKDateTime
//
// UTC 를 기준으로 시간을 계산함.
// UNIX Epoch 는 1970-01-01 00:00:00 UTC (1970-01-01T00:00:00Z ISO 8601)
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	class DKLIB_API DKDateTime
	{
	public:
		enum DateFormat
		{
			DateFormatNone = 0,
			DateFormatNormal,
			DateFormatWithWeekday,
			DateFormatWithWeekdayShort,
		};
		enum TimeFormat
		{
			TimeFormatNone = 0,
			TimeFormat12Hour,
			TimeFormat12HourWithMillisecond,
			TimeFormat12HourWithMicrosecond,
			TimeFormat12HourWithTimezone,
			TimeFormat12HourWithMillisecondTimezone,
			TimeFormat12HourWithMicrosecondTimezone,
			TimeFormat24Hour,
			TimeFormat24HourWithMillisecond,
			TimeFormat24HourWithMicrosecond,
			TimeFormat24HourWithTimezone,
			TimeFormat24HourWithMillisecondTimezone,
			TimeFormat24HourWithMicrosecondTimezone,
		};
		struct Component
		{
			unsigned short year;
			unsigned char month;
			unsigned char day;
			unsigned char hour;
			unsigned char minute;
			unsigned char second;
			unsigned int microsecond;
			unsigned char dayOfWeek;
			unsigned int dayOfYear;
		};

		static DKDateTime Now(void);
		explicit DKDateTime(double d);
		explicit DKDateTime(long long seconds, int microseconds);			// UTC 기준임
		explicit DKDateTime(int year, int month, int day, int hour, int min, int sec, int msec, bool utc = false);
		explicit DKDateTime(int year, int month, int day, int hour, int min, int sec, int msec, int timezone);	// timezone 은 초단위
		explicit DKDateTime(const DKString& iso8601);
		DKDateTime(const DKDateTime& time);
		DKDateTime(void);
		~DKDateTime(void);

		void GetComponent(Component& c) const;			// UTC
		void GetLocalComponent(Component& c) const;		// local-time

		int Year(void) const;
		int Month(void) const;
		int Day(void) const;
		int DayOfYear(void) const;
		int DayOfWeek(void) const;
		int Hour(void) const;
		int Minute(void) const;
		int Second(void) const;
		int Microsecond(void) const;

		long long DaysSinceEpoch(void) const;
		long long HoursSinceEpoch(void) const;
		long long MinutesSinceEpoch(void) const;
		long long SecondsSinceEpoch(void) const;
		static long TimezoneOffset(void);				// GMT 와의 시간차이를 초단위로 리턴함

		DKDateTime operator + (double d) const;
		DKDateTime& operator += (double d);
		DKDateTime operator - (double d) const;
		DKDateTime& operator -= (double d);
		DKDateTime& operator = (const DKDateTime& t);

		double Interval(const DKDateTime& t) const;		// 시간차 계산함 , t 가 더 크면 음수 리턴. (정밀한 타이머는 DKTimer 사용할것!)
		double IntervalSinceEpoch(void) const;

		DKString Format(const DKString& fmt) const;		// strftime (wcsftime) 과 같음. (플랫폼에 따라 다를수 있다)
		DKString Format(DateFormat df, TimeFormat tf) const;
		DKString FormatISO8601(bool utc = false) const;
		static bool GetDateTime(DKDateTime& dtOut, int year, int month, int day, int hour, int min, int sec, int msec, bool utc);
		static bool GetDateTime(DKDateTime& dtOut, int year, int month, int day, int hour, int min, int sec, int msec, int timezone);	// timezone 은 초단위
		static bool GetDateTime(DKDateTime& dtOut, int year, int dayOfYear, int hour, int min, int sec, int msec, bool utc);
		static bool GetDateTime(DKDateTime& dtOut, const DKString& iso8601str);		// ISO-8601 스트링에서 DKDateTime 을 읽어들인다.

		int Compare(const DKDateTime& t) const;
		int CompareDate(const DKDateTime& t) const;
		int CompareDateTimeWithoutSeconds(const DKDateTime& t) const;
		int CompareDateTimeWithoutMicroseconds(const DKDateTime& t) const;

		bool operator > (const DKDateTime& t) const;
		bool operator >= (const DKDateTime& t) const;
		bool operator < (const DKDateTime& t) const;
		bool operator <= (const DKDateTime& t) const;
		bool operator == (const DKDateTime& t) const;
		bool operator != (const DKDateTime& t) const;

	private:
		long long seconds;		// 초단위 (32비트일 경우 68년 주기를 가짐)
		int microseconds;			// 1~1,000,000 까지의 마이크로초 단위
	};
}

﻿//
//  File: DKRational.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"

namespace DKFoundation
{
	class DKLIB_API DKRational
	{
	public:
		typedef int64_t Integer;

		DKRational(void);
		DKRational(Integer num, Integer den = 1);

		bool IsInfinity(void) const;
		bool IsPositiveInfinity(void) const;
		bool IsNegativeInfinity(void) const;
		bool IsIndeterminate(void) const;
		bool IsNumeric(void) const;

		Integer Numerator(void) const;
		Integer Denominator(void) const;

		DKRational operator + (const DKRational&) const;
		DKRational operator - (const DKRational&) const;
		DKRational operator * (const DKRational&) const;
		DKRational operator / (const DKRational&) const;
		DKRational& operator += (const DKRational&);
		DKRational& operator -= (const DKRational&);
		DKRational& operator *= (const DKRational&);
		DKRational& operator /= (const DKRational&);

		DKRational& operator = (const DKRational&);

		bool operator == (const DKRational&) const;
		bool operator != (const DKRational&) const;
		bool operator < (const DKRational&) const;
		bool operator <= (const DKRational&) const;
		bool operator > (const DKRational&) const;
		bool operator >= (const DKRational&) const;

		DKRational& Inverse(void);
		double RealNumber(void) const;

	private:
		void Normalize(void);
		Integer numerator;
		Integer denominator;
	};
}

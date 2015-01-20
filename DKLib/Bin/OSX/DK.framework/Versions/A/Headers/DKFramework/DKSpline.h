//
//  File: DKSpline.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKVector2.h"
#include "DKVector3.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKSpline
//
// CatmullRom: 두 점을 지나는 곡선을 구함.
//		point1 - point2 사이의 곡선을 구한다.
//		point0 : point1 이전의 점 (point1 이 시작점일경우 point1 를 사용함)
//		point3 : point2 이후의 점 (point2 가 끝점일 경우 point2 를 사용함)
//
// UniformCubic: 모든 점을 거치지 않고 기울기에 영향을 받는 곡선을 구함.
//		point1 - point2 사이의 곡선을 구한다.
//		point0 : point1 이전의 점 (point1 이 시작점일경우 point1 를 사용함)
//		point3 : point2 이후의 점 (point2 가 끝점일 경우 point2 를 사용함)
//		생성된 곡선은 모든 점들로 이루어진 볼록 다면체(convex-hull) 안에 항상 포함된다.
//
// Hermite: 두 점 사이의 곡선을 탄젠트 벡터를 이용해서 구함.
//		point0 : 곡선 시작점
//		point1 : 곡선 끝점
//		point2 : point0의 탄젠트 벡터. (시작점에서의  곡선의 방향 및 속도 - 곡선이 어떻게 시작할지 정의)
//		point3 : point1의 탄젠트 벡터. (끝점에서 곡선의 방향 및 속도 - 곡선이 어떻게 끝날지 정의)
//		point2, point3 을 이용하여 곡선의 기울기와 구간의 속도를 제어할 수 있다.
//
// Bezier: 3차 베지어 곡선, 4개의 점으로 하나의 곡선을 구함. (연속되는 곡선은 지원하지 않음)
//		point0 - point3 사이의 곡선을 구한다.
//		point1, point2 는 제어점으로 사용함
//		생성된 곡선은 위의 4개 점으로 이루어진 볼록 사각형 안에 항상 포함된다.
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKLIB_API DKSpline
	{
	public:
		enum Type
		{
			CatmullRom = 0,
			UniformCubic,
			Hermite,
			Bezier,
		};

		DKSpline(void);
		DKSpline(float p0, float p1, float p2, float p3);

		float Interpolate(float t, Type c) const;

		union
		{
			struct
			{
				float point0;
				float point1;
				float point2;
				float point3;
			};
			float points[4];
		};
	};

	class DKSpline2
	{
	public:
		DKSpline2(void)
			:point0(0, 0), point1(0, 0), point2(0, 0), point3(0, 0)
		{
		}
		DKSpline2(const DKVector2& p0, const DKVector2& p1, const DKVector2& p2, const DKVector2& p3)
			: point0(p0), point1(p1), point2(p2), point3(p3)
		{
		}

		DKVector2 Interpolate(float t, DKSpline::Type c) const
		{
			return DKVector2(
				DKSpline(point0.x, point1.x, point2.x, point3.x).Interpolate(t, c),
				DKSpline(point0.y, point1.y, point2.y, point3.y).Interpolate(t, c)
				);
		}

		DKVector2 point0;
		DKVector2 point1;
		DKVector2 point2;
		DKVector2 point3;
	};

	class DKSpline3
	{
	public:
		DKSpline3(void)
			:point0(0, 0, 0), point1(0, 0, 0), point2(0, 0, 0), point3(0, 0, 0)
		{
		}
		DKSpline3(const DKVector3& p0, const DKVector3& p1, const DKVector3& p2, const DKVector3& p3)
			: point0(p0), point1(p1), point2(p2), point3(p3)
		{
		}

		DKVector3 Interpolate(float t, DKSpline::Type c) const
		{
			return DKVector3(
				DKSpline(point0.x, point1.x, point2.x, point3.x).Interpolate(t, c),
				DKSpline(point0.y, point1.y, point2.y, point3.y).Interpolate(t, c),
				DKSpline(point0.z, point1.z, point2.z, point3.z).Interpolate(t, c)
				);
		}

		DKVector3 point0;
		DKVector3 point1;
		DKVector3 point2;
		DKVector3 point3;
	};
}

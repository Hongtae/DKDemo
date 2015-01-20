//
//  File: DKCamera.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKVector3.h"
#include "DKVector4.h"
#include "DKMatrix4.h"
#include "DKQuaternion.h"
#include "DKPlane.h"
#include "DKSphere.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKCamera
//
// Note: 실제 프러스텀은 +z 가 화면 안쪽 방향이지만 (-1:화면, +1:화면 안쪽)
// 이 클래스에서는 화면 안쪽이 -z 로 설정한다. (오른손 좌표계)
//
// 변환 행렬을 거치고 나면 아래와 같은 오른손 좌표계로 변형된다.
// 따라서 반시계방향(CCW)이 front-face 가 된다.
//
//          +Y
//           |
//           |
//           |_______ +X
//           /
//          /
//         /
//        +Z 
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKLIB_API DKCamera
	{
	public:
		DKCamera(void);
		~DKCamera(void);

		// view
		void SetView(const DKVector3& pos, const DKVector3& dir, const DKVector3& up);
		DKVector3 ViewPosition(void) const;
		DKVector3 ViewDirection(void) const;
		DKVector3 ViewUp(void) const;

		// projection
		void SetPerspective(float fov, float aspect, float nearDistance, float farDistance);
		void SetOrthographic(float width, float height, float nearDistance, float farDistance);
		bool IsPerspective(void) const;
		bool IsOrthographic(void) const;

		// frustum intersection test
		bool IsPointInside(const DKVector3& point) const;					// 점이 시야에 포함되는지 확인
		bool IsSphereInside(const DKVector3& center, float radius) const;	// 구가 시야에 포함되는지 확인
		bool IsSphereInside(const DKSphere& s) const;

		const DKPlane& TopFrustumPlane(void) const			{return frustumTop;}
		const DKPlane& BottomFrustumPlane(void) const		{return frustumBottom;}
		const DKPlane& LeftFrustumPlane(void) const			{return frustumLeft;}
		const DKPlane& RightFrustumPlane(void) const		{return frustumRight;}
		const DKPlane& NearFrustumPlane(void) const			{return frustumNear;}
		const DKPlane& FarFrustumPlane(void) const			{return frustumFar;}

		// matrix
		void SetView(const DKMatrix4& m);
		void SetProjection(const DKMatrix4& m);
		void SetViewProjection(const DKMatrix4& view, const DKMatrix4& proj);
		const DKMatrix4& ViewMatrix(void) const				{return viewMatrix;}
		const DKMatrix4& ProjectionMatrix(void) const		{return projectionMatrix;}
		DKMatrix4 ViewProjectionMatrix(void) const			{return viewMatrix * projectionMatrix;}

	private:
		void UpdateFrustum(void);	// 평면, 매트릭스 업데이트

		// 절두체 컬링용 평면들
		DKPlane	frustumNear;
		DKPlane	frustumFar;
		DKPlane	frustumLeft;
		DKPlane	frustumRight;
		DKPlane	frustumTop;
		DKPlane frustumBottom;

		DKMatrix4 viewMatrix;
		DKMatrix4 projectionMatrix;
	};
}

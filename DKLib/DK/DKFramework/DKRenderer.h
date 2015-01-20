//
//  File: DKRenderer.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKRenderTarget.h"
#include "DKPrimitiveIndex.h"
#include "DKVector2.h"
#include "DKVector3.h"
#include "DKPoint.h"
#include "DKColor.h"
#include "DKCamera.h"
#include "DKBlendState.h"
#include "DKMaterial.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKRenderer
//
// 렌더타겟(텍스쳐)에 렌더링을 한다. 2D/3D 렌더링 기능을 제공함.
// 3D 는 단순한 프리미티브 드로잉을 제공한다. 쉐이더를 사용하려면
// DKModel, DKMesh 와 DKScene 을 이용하여 장면구성을 해야한다.
//
// DKScreen 에 의해 관리되는 스크린에 렌더링을 해주는 객체. 
// 파라메터로 사용되는 DKMatrix3 는 아핀변환 행렬까지만 허용한다.
//
// 여기 적용되는 행렬은 픽셀단위로 적용되지 않으며, 각 정점 위치 계산에만 사용된다.
// 따라서 삼각형은 여전히 삼각형이 되고, 사각형은 사각형이 된다. 
// 즉, 행렬은 Affine-Transform 만 적용된다.
//
// 주의:
//  - 2D 는 스크린 좌표계를 사용한다. 왼쪽하단이 (0,0), 우측 상단이 (width,height)
//  - 3D 는 3D 좌표계를 사용한다. x,y,z 는 -1.0~1.0 사이의 값
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKTexture;
	class DKTextureSampler;
	class DKFont;
	class DKMesh;
	class DKScene;
	class DKSceneState;
	class DKRenderState;
	class DKCollisionObject;

	class DKLIB_API DKRenderer
	{
	public:
		static const float minimumScaleFactor;
		
		DKRenderer(DKRenderTarget*);
		virtual ~DKRenderer(void);

		struct Vertex2D
		{
			Vertex2D(const DKPoint& p, const DKPoint& t) : position(p), texcoord(t) {}
			DKPoint position;		// 스크린 좌표계
			DKPoint texcoord;
		};
		struct Vertex3D
		{
			Vertex3D(const DKVector3& p, const DKPoint& t, const DKColor& c) : position(p), texcoord(t), color(c) {}
			DKVector3 position;		// 3D 공간 좌표계
			DKPoint texcoord;
			DKColor color;
		};
		struct Vertex3DColored
		{
			Vertex3DColored(const DKVector3& p, const DKColor& c) : position(p), color(c) {}
			DKVector3 position;		// 3D 공간 좌표계
			DKColor color;
		};
		struct Vertex3DTextured
		{
			Vertex3DTextured(const DKVector3& p, const DKPoint& t) : position(p), texcoord(t) {}
			DKVector3 position;		// 3D 공간 좌표계
			DKPoint texcoord;
		};

		const DKRect& Viewport(void) const;
		void SetViewport(const DKRect& rc);

		const DKRect& ContentBounds(void) const;
		void SetContentBounds(const DKRect&);

		const DKMatrix3& ContentTransform(void) const;
		void SetContentTransform(const DKMatrix3& tm);

		void SetPolygonOffset(float factor, float units);		// 0, 0 이면 disable 된다.
		void PolygonOffset(float*) const;

		DKRenderTarget* RenderTarget(void);
		const DKRenderTarget* RenderTarget(void) const;

		void Clear(const DKColor& color) const;
		void ClearColorBuffer(const DKColor& color) const;
		void ClearDepthBuffer(void) const;

		void RenderPrimitive(DKPrimitive::Type p, const Vertex2D* vertices, size_t count, const DKTexture* texture, const DKTextureSampler* sampler, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderPrimitive(DKPrimitive::Type p, const Vertex3D* vertices, size_t count, const DKMatrix4& tm, const DKTexture* texture, const DKTextureSampler* sampler, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		
		// 2D 단색 렌더링
		void RenderSolidRect(const DKRect& rect, const DKMatrix3& transform, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderSolidRects(const DKRect* rects, const DKMatrix3* transforms, size_t numRects, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderSolidQuad(const DKPoint& lb, const DKPoint& lt, const DKPoint& rt, const DKPoint& rb, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderSolidQuads(const DKPoint* points, size_t numPoints, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderSolidTriangle(const DKPoint& pos1, const DKPoint& pos2, const DKPoint& pos3, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderSolidTriangles(const DKPoint* points, size_t numPoints, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderSolidTriangleStrip(const DKPoint* points, size_t numPoints, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderSolidTriangleFan(const DKPoint* points, size_t numPoints, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderSolidLine(const DKPoint& pos1, const DKPoint& pos2, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderSolidLines(const DKPoint* points, size_t numPoints, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderSolidLineStrip(const DKPoint* points, size_t numPoints, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderSolidPoint(const DKPoint& point, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderSolidPoints(const DKPoint* points, size_t numPoints, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderSolidEllipse(const DKRect& bounds, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;

		// 2D 텍스쳐 렌더링
		void RenderTexturedRect(const DKRect& posRect, const DKMatrix3& posTM, const DKRect& texRect, const DKMatrix3& texTM, const DKTexture* texture, const DKTextureSampler* sampler, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderTexturedRects(const DKRect* posRects, const DKMatrix3* posTMs, const DKRect* texRects, const DKMatrix3* texTMs, size_t numRects, const DKTexture* texture, const DKTextureSampler* sampler, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderTexturedQuad(const Vertex2D& lb, const Vertex2D& lt, const Vertex2D& rt, const Vertex2D& rb, const DKTexture* texture, const DKTextureSampler* sampler, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderTexturedQuads(const Vertex2D* vertices, size_t numVerts, const DKTexture* texture, const DKTextureSampler* sampler, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderTexturedTriangle(const Vertex2D& v1, const Vertex2D& v2, const Vertex2D& v3, const DKTexture* texture, const DKTextureSampler* sampler, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderTexturedTriangles(const Vertex2D* vertices, size_t numVerts, const DKTexture* texture, const DKTextureSampler* sampler, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderTexturedTriangleStrip(const Vertex2D* vertices, size_t numVerts, const DKTexture* texture, const DKTextureSampler* sampler, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderTexturedTriangleFan(const Vertex2D* vertices, size_t numVerts, const DKTexture* texture, const DKTextureSampler* sampler, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderTexturedLine(const Vertex2D& v1, const Vertex2D& v2, const DKTexture* texture, const DKTextureSampler* sampler, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderTexturedLines(const Vertex2D* vertices, size_t numVerts, const DKTexture* texture, const DKTextureSampler* sampler, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderTexturedLineStrip(const Vertex2D* vertices, size_t numVerts, const DKTexture* texture, const DKTextureSampler* sampler, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderTexturedPoint(const Vertex2D& vertex, const DKTexture* texture, const DKTextureSampler* sampler, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderTexturedPoints(const Vertex2D* vertices, size_t numVerts, const DKTexture* texture, const DKTextureSampler* sampler, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderTexturedEllipse(const DKRect& bounds, const DKRect& texBounds, const DKTexture* texture, const DKTextureSampler* sampler, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;

		// 3D 단색 렌더링
		void RenderSolidTriangles(const DKVector3* points, size_t numPoints, const DKMatrix4& tm, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderSolidTriangleStrip(const DKVector3* points, size_t numPoints, const DKMatrix4& tm, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderSolidTriangleFan(const DKVector3* points, size_t numPoints, const DKMatrix4& tm, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderSolidLines(const DKVector3* points, size_t numPoints, const DKMatrix4& tm, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderSolidLineStrip(const DKVector3* points, size_t numPoints, const DKMatrix4& tm, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderSolidPoints(const DKVector3* points, size_t numPoints, const DKMatrix4& tm, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;

		// 3D 여러색 렌더링
		void RenderColoredTriangles(const Vertex3DColored* vertices, size_t numVerts, const DKMatrix4& tm, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderColoredTriangleStrip(const Vertex3DColored* vertices, size_t numVerts, const DKMatrix4& tm, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderColoredTriangleFan(const Vertex3DColored* vertices, size_t numVerts, const DKMatrix4& tm, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderColoredLines(const Vertex3DColored* vertices, size_t numVerts, const DKMatrix4& tm, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderColoredLineStrip(const Vertex3DColored* vertices, size_t numVerts, const DKMatrix4& tm, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderColoredPoints(const Vertex3DColored* vertices, size_t numVerts, const DKMatrix4& tm, const DKBlendState& blend = DKBlendState::defaultOpaque) const;

		// 3D 텍스쳐 렌더링
		void RenderTexturedTriangles(const Vertex3DTextured* vertices, size_t numVerts, const DKMatrix4& tm, const DKTexture* texture, const DKTextureSampler* sampler, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderTexturedTriangleStrip(const Vertex3DTextured* vertices, size_t numVerts, const DKMatrix4& tm, const DKTexture* texture, const DKTextureSampler* sampler, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderTexturedTriangleFan(const Vertex3DTextured* vertices, size_t numVerts, const DKMatrix4& tm, const DKTexture* texture, const DKTextureSampler* sampler, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderTexturedLines(const Vertex3DTextured* vertices, size_t numVerts, const DKMatrix4& tm, const DKTexture* texture, const DKTextureSampler* sampler, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderTexturedLineStrip(const Vertex3DTextured* vertices, size_t numVerts, const DKMatrix4& tm, const DKTexture* texture, const DKTextureSampler* sampler, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderTexturedPoints(const Vertex3DTextured* vertices, size_t numVerts, const DKMatrix4& tm, const DKTexture* texture, const DKTextureSampler* sampler, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;

		// 3D 도형 렌더링
		void RenderSolidSphere(const DKVector3& center, float radius, int lats, int longs, const DKMatrix4& tm, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderWireSphere(const DKVector3& center, float radius, int lats, int longs, const DKMatrix4& tm, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderSolidAABB(const DKVector3& aabbMin, const DKVector3& aabbMax, const DKMatrix4& tm, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;
		void RenderWireAABB(const DKVector3& aabbMin, const DKVector3& aabbMax, const DKMatrix4& tm, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultOpaque) const;

		// DKMesh
		size_t RenderMesh(const DKMesh*, DKSceneState&, const DKBlendState* blend = NULL) const;

		// DKScene
		struct RenderSceneCallback
		{
			using MaterialCallback = DKMaterial::PropertyCallback;
			using ObjectColorCallback = DKFoundation::DKFunctionSignature<void (const DKCollisionObject*, DKColor&, DKColor&, DKColor&)>;
			using MeshFilter = DKFoundation::DKFunctionSignature<void (DKFoundation::DKArray<const DKMesh*>&)>;

			MaterialCallback* materialCallback = NULL;
			ObjectColorCallback* objectColors = NULL;
			MeshFilter* meshFilter = NULL;
		};
		void RenderScene(const DKScene*, const DKCamera& camera, int sceneIndex, bool enableCulling = true, RenderSceneCallback* sc = NULL) const;

		// 기타
		void RenderText(const DKRect& bounds, const DKMatrix3& transform, const DKFoundation::DKString& text, const DKFont* font, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultAlpha) const;
		void RenderText(const DKPoint& baselineBegin, const DKPoint& baselineEnd, const DKFoundation::DKString& text, const DKFont* font, const DKColor& color, const DKBlendState& blend = DKBlendState::defaultAlpha) const;

	private:
		class RendererContext;
		DKFoundation::DKObject<DKRenderTarget>			renderTarget;
		DKFoundation::DKObject<DKFoundation::DKUnknown>	context;
		DKRect											contentBounds;
		DKRect											viewport;
		DKMatrix3										contentTM;
		DKMatrix3										screenTM;		// 2D 스크린 좌표계용 TM

		struct
		{
			float factor;
			float units;
		} polygonOffset;

		void UpdateTransform(void);
		bool IsDrawable(void) const;
		DKRenderState* Bind(void) const;
		RendererContext* GetContext(void) const;
	};
}

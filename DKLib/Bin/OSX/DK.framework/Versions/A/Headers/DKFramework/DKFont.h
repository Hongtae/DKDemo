//
//  File: DKFont.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKPoint.h"
#include "DKSize.h"
#include "DKRect.h"


////////////////////////////////////////////////////////////////////////////////
//
// DKFont
//
// 폰트 파일이나 데이터(DKBuffer) 로부터 읽어들여서 텍스쳐 폰트를 생성한다.
// 데이터로 읽는 경우에는 입력 데이터(DKBuffer) 가 변경되면 안된다.
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKTexture2D;
	class DKLIB_API DKFont
	{
	public:
		struct GlyphData
		{
			DKFoundation::DKObject<DKTexture2D>		texture;
			DKPoint									position;
			DKSize									advance;
			DKRect									rect;
		};

		DKFont(void);
		~DKFont(void);

		static DKFoundation::DKObject<DKFont> Create(const DKFoundation::DKString& file);
		static DKFoundation::DKObject<DKFont> Create(void* data, size_t size);			// 데이터 사본을 메모리에 저장하게 된다.
		static DKFoundation::DKObject<DKFont> Create(DKFoundation::DKData* data);		// data 가 외부에서 변형되면 안된다.
		static DKFoundation::DKObject<DKFont> Create(DKFoundation::DKStream* stream);	// 필요에 따라 사본을 저장 할 수 있음

		const GlyphData* GetGlyphData(wchar_t c) const;
		float LineWidth(const DKFoundation::DKString& str) const;		// 원점 (baseline) 기준 너비 (픽셀, 아웃라인 영역 포함 안함)
		float LineHeight(void) const;									// 원점 기준 높이 (픽셀, 아웃라인 영역 포함 안함)
		float Baseline(void) const;										// baseline 높이 (픽셀, 아웃라인 영역은 포함하지 않음)
		DKRect Bounds(const DKFoundation::DKString& str) const;			// 바운딩 박스 (픽셀, 0,0이 원점-baseline)
		DKPoint	KernAdvance(wchar_t left, wchar_t right) const;

		int				PointSize(void) const							{ return pointSize; }
		const DKPoint&	Resolution(void) const							{ return resolution; }
		float			Outline(void) const								{ return outline; }
		float			Embolden(void) const							{ return embolden; }
		bool			KerningEnabled(void) const						{ return kerningEnabled; }
		bool			ForceBitmap(void) const							{ return forceBitmap; }

		DKFoundation::DKString FamilyName(void) const;
		DKFoundation::DKString StyleName(void) const;

		// point 와 embolden 은 point-size, outline 은 픽셀 사이즈임.
		bool SetStyle(int point, float embolden = 0, float outline = 0, DKPoint dpi = DKPoint(72,72), bool enableKerning = true, bool forceBitmap = false);
		bool IsValid(void) const;

		float Height(void) const;		// 폰트의 높이 (아웃라인 픽셀 포함)
		float Width(void) const;		// 폰트의 너비 (아웃라인 픽셀 포함)

	private:
		float		outline;			// 0 이면 아웃라인 아님
		float		embolden;			// 0 이면 보통
		int			pointSize;			// 해당dpi 기준 포인트 크기
		DKPoint		resolution;			// dpi 해상도
		bool		kerningEnabled;		// 텍스트 커닝 on/off
		bool		forceBitmap;		// 강제 비트맵 로딩

		struct SharedTextures
		{
			DKFoundation::DKObject<DKTexture2D>	texture;
			unsigned int freeSpaceWidth;
		};
		typedef DKFoundation::DKMap<wchar_t, GlyphData>		GlyphDataMap;
		typedef DKFoundation::DKMap<wchar_t, unsigned int>	CharIndexMap;
		
		mutable GlyphDataMap								glyphMap;
		mutable CharIndexMap								charIndexMap;
		mutable DKFoundation::DKArray<SharedTextures>		textures;
		mutable unsigned int								numGlyphsLoaded;

		void* ftFace;
		DKFoundation::DKSpinLock lock;
		DKFoundation::DKObject<DKFoundation::DKData> fontData;
		DKTexture2D* GetGlyphTexture(int width, int height, void* data, DKRect& rect) const;
	};
}

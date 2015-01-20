//
//  File: DKAudioStream.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKFoundation.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKAudioStream
// 
// 오디오 데이터를 디코딩 하는 객체, 디코딩하여 PCM 스트림으로 만든다.
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKLIB_API DKAudioStream
	{
	public:
		typedef long long Position;
		enum FileType
		{
			FileTypeUnknown = 0,
			FileTypeOggVorbis,
			FileTypeOggFLAC,
			FileTypeFLAC,
			FileTypeWave,
		};

		static DKFoundation::DKObject<DKAudioStream>	Create(DKFoundation::DKStream* stream);
		static DKFoundation::DKObject<DKAudioStream>	Create(const DKFoundation::DKString& file);

		DKAudioStream(FileType t);
		virtual ~DKAudioStream(void);

		// 미디어 정보
		bool IsSeekable(void) const				{return seekable;}		// 미디어가 검색이 가능한지..
		unsigned int Frequency(void) const		{return frequency;}		// 비트레이트
		unsigned int Channels(void) const		{return channels;}		// 사운드 채널 수
		unsigned int Bits(void) const			{return bits;}			// 16비트여야 된다.
		FileType MediaType(void) const			{return type;}

		////////////////////////////////////////////////////////////////////////////////
		// 오버라이드 하여 사용하는 함수
		// 데이터 디코딩
		virtual size_t Read(void*, size_t) = 0;
		// 미디어 검색 위치
		virtual Position SeekRaw(Position) = 0;			// 파일 위치로 검색
		virtual Position SeekPcm(Position) = 0;			// PCM 위치로 검색
		virtual double SeekTime(double) = 0;			// 시간으로 검색
		// 미디어 재생 위치
		virtual Position RawPos(void) const = 0;		// raw 파일 위치 리턴 (현재 파일상의 위치)
		virtual Position PcmPos(void) const = 0;		// 현재 pcm 위치 리턴 (사이즈가 매우크다)
		virtual double TimePos(void) const = 0;			// 초 단위로 리턴
		// 미디어 길이
		virtual Position RawTotal(void) const = 0;		// 전체 크기 (실제 파일크기)
		virtual Position PcmTotal(void) const = 0;		// 전체 크기 (pcm 바이트 크기)
		virtual double TimeTotal(void) const = 0;		// 전체 크기 (플레이 시간)
		////////////////////////////////////////////////////////////////////////////////

	protected:
		void SetSeekable(bool s)					{seekable = s;}
		void SetFrequency(unsigned int freq)		{frequency = freq;}
		void SetChannels(unsigned int c)			{channels = c;}
		void SetBits(unsigned int b)				{bits = b;}

	private:
		static FileType DetermineAudioType(char* data, size_t len);
		bool seekable;
		unsigned int frequency;
		unsigned int channels;
		unsigned int bits;
		const FileType type;
	};
}

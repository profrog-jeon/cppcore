#pragma once

#include <vector>

namespace core
{
#pragma pack(push, 1)
	struct ST_WAV_HEADER
	{
		char szMAGIC[4];
		DWORD dwPayloadSize;
		char szFormat[4];

		ST_WAV_HEADER(void)
			: szMAGIC { 'R', 'I', 'F', 'F' }
			, dwPayloadSize(0)
			, szFormat { 'W', 'A', 'V', 'E' }
		{}
	};

	struct ST_WAV_CHUNK
	{
		char szChunkID[4];
		DWORD dwChunkSize;
		ST_WAV_CHUNK(void)
			: szChunkID{ 0, }
			, dwChunkSize(0)
		{}
	};

	struct ST_WAV_CHUNK_FMT
	{
		char	szChunkID[4];	// "fmt "
		DWORD	dwChunkSize;
		WORD	wAudioFormat;	// 1: PCM
		WORD	wNumOfChannel;	// 1: MONO, 2: STEREO
		DWORD	dwSampleRate;	// 8000Hz ... 48000Hz
		DWORD	dwByteRate;	// Average Bytes Per Second
		WORD	wBlockAlign;	// Size of Sample Frame -> dwSampleRate * wBlockAlign == dwByteRate
		WORD	wBitPerSample;	// Bit Depth

		ST_WAV_CHUNK_FMT(void)
			: szChunkID{ 'f', 'm', 't', ' ' }
			, dwChunkSize(16)
			, wAudioFormat(0)
			, wNumOfChannel(0)
			, dwSampleRate(0)
			, dwByteRate(0)
			, wBlockAlign(0)
			, wBitPerSample(0)
		{}
	};

	struct ST_WAV_CHUNK_DATA
	{
		char szChunkID[4];	// "data"
		DWORD dwChunkSize;

		ST_WAV_CHUNK_DATA(void)
			: szChunkID{ 'd', 'a', 't', 'a' }
			, dwChunkSize(0)
		{}
	};

	struct ST_WAV_CHUNK_LIST
	{
		char szChunkID[4];	// "LIST"
		DWORD dwChunkSize;
		char szSubType[4];	// "INFO"


		ST_WAV_CHUNK_LIST(void)
			: szChunkID{ 'L', 'I', 'S', 'T' }
			, dwChunkSize(0)
			, szSubType{ 'I', 'N', 'F', 'O' }
		{}
	};

	struct ST_WAVE_FORMATEX
	{
		WORD        wFormatTag;         /* format type */
		WORD        nChannels;          /* number of channels (i.e. mono, stereo...) */
		DWORD       nSamplesPerSec;     /* sample rate */
		DWORD       nAvgBytesPerSec;    /* for buffer estimation */
		WORD        nBlockAlign;        /* block size of data */
		WORD        wBitsPerSample;     /* number of bits per sample of mono data */
		WORD        cbSize;             /* the count in bytes of the size of */
	};
#pragma pack(pop)

	ECODE ReadWavFile(std::string strWavFile, ST_WAVE_FORMATEX* outFormat, std::vector<unsigned char>& vecPCM);
	ECODE ReadWavFile(std::wstring strWavFile, ST_WAVE_FORMATEX* outFormat, std::vector<unsigned char>& vecPCM);

	ECODE WriteWavFile(std::tstring strWavFile, const ST_WAVE_FORMATEX& stFormat, const std::vector<unsigned char>& vecPCM);
}

#pragma once

#include <vector>

namespace core
{
	struct ST_WAV_HEADER
	{
		char szMAGIC[4];
		DWORD dwPayloadSize;
		char szFormat[4];
	};

	struct ST_WAV_CHUNK
	{
		char szChunkID[4];
		DWORD dwChunkSize;
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
	};

	struct ST_WAV_CHUNK_DATA
	{
		char szChunkID[4];	// "data"
		DWORD dwChunkSize;
	};

	struct ST_WAV_CHUNK_LIST
	{
		char szChunkID[4];	// "LIST"
		DWORD dwChunkSize;
		char szSubType[4];	// "INFO"
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
										/* extra information (after cbSize) */
	};

	int ReadWavFile(LPCTSTR pszWavFile, ST_WAVE_FORMATEX* outFormat, std::vector<unsigned char>& vecPCM);
}

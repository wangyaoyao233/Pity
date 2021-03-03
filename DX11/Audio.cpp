#include "main.h"
#include "Audio.h"

#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'

#pragma comment(lib,"Xaudio2.lib")
//1.Initialize Xaudio2

void CAudio::Init()
{
	//*******
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	//1. create an instance of the XAudio2 engine
	XAudio2Create(&m_XAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	//2. create a mastering voice
	m_XAudio2->CreateMasteringVoice(&m_MasterVoice);

	//
	m_SourceVoice.resize(15);
	wfx.resize(15);
	buffer.resize(15);
}

void CAudio::PlayOnce(int index)
{
	m_SourceVoice[index]->Stop();
	m_SourceVoice[index]->FlushSourceBuffers();

	//summit
	m_SourceVoice[index]->SubmitSourceBuffer(&buffer[index]);
	m_SourceVoice[index]->Start();
}

void CAudio::Loop(int index)
{
	buffer[index].LoopCount = 255;
	m_SourceVoice[index]->SubmitSourceBuffer(&buffer[index]);

	m_SourceVoice[index]->SetVolume(0.6f);
	m_SourceVoice[index]->Start();
}

void CAudio::Play(int index)
{
	//XAUDIO2_VOICE_STATE state;
	//m_SourceVoice[index]->GetState(&state);
	//if (state.BuffersQueued == NULL)
	//{
	//	m_SourceVoice[index]->SubmitSourceBuffer(&buffer[index]);
	//}
	m_SourceVoice[index]->Start();
}

void CAudio::Stop(int index)
{
	m_SourceVoice[index]->Stop();
}

HRESULT CAudio::LoadFile(const WCHAR* strFileName, int index)
{
	/// <summary>
	/// Declare WAVEFORMATEXTENSIBLE and XAUDIO2_BUFFER structures.
	/// </summary>

	//1.Declare WAVEFORMATEXTENSIBLE and XAUDIO2_BUFFER structures.

	//2.Open the audio file with CreateFile.
	HANDLE hFile = CreateFile(
		strFileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return HRESULT_FROM_WIN32(GetLastError());

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	//3.Locate the 'RIFF' chunk in the audio file, and check the file type.
	DWORD dwChunkSize;
	DWORD dwChunkPosition;
	//check the file type, should be fourccWAVE or 'XWMA'
	FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
	DWORD filetype;
	ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
	if (filetype != fourccWAVE)
		return S_FALSE;

	//4.Locate the 'fmt ' chunk, and copy its contents into a WAVEFORMATEXTENSIBLE structure.
	FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
	ReadChunkData(hFile, &wfx[index], dwChunkSize, dwChunkPosition);

	//5.Locate the 'data' chunk, and read its contents into a buffer.
	//fill out the audio data buffer with the contents of the fourccDATA chunk
	FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
	BYTE* pDataBuffer = new BYTE[dwChunkSize];
	ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

	//6.Populate an XAUDIO2_BUFFER structure.
	buffer[index].AudioBytes = dwChunkSize;  //buffer containing audio data
	buffer[index].pAudioData = pDataBuffer;  //size of the audio buffer in bytes
	buffer[index].Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer

	/// <summary>
	/// create a source voice
	/// </summary>

	HRESULT hr;
	//1.Create a source voice
	if (FAILED(hr = m_XAudio2->CreateSourceVoice(&m_SourceVoice[index], (WAVEFORMATEX*)&wfx[index]))) return hr;

	//2.submit an XAUDIO2_BUFFER
	if (FAILED(hr = m_SourceVoice[index]->SubmitSourceBuffer(&buffer[index])))
		return hr;
}

HRESULT CAudio::FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;

	while (hr == S_OK)
	{
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;

		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
				return HRESULT_FROM_WIN32(GetLastError());
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (bytesRead >= dwRIFFDataSize) return S_FALSE;
	}

	return S_OK;
}

HRESULT CAudio::ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
		hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}
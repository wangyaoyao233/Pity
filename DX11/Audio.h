#pragma
#include <xaudio2.h>
class CAudio
{
public:
	CAudio() {}
	~CAudio()
	{
		//m_SourceVoice->DestroyVoice();
		for (auto item : m_SourceVoice)
		{
			if (item)
			{
				item->DestroyVoice();
			}
		}

		for (auto item : buffer)
		{
			delete[] item.pAudioData;
		}

		m_MasterVoice->DestroyVoice();
		m_XAudio2->Release();
	}
	/// <summary>
	/// 初始化
	/// </summary>
	void Init();
	/// <summary>
	/// Loop
	/// </summary>
	/// <param name="index">第几号</param>
	void Loop(int index = 0);
	/// <summary>
	/// Stop
	/// </summary>
	/// <param name="index">第几号</param>
	void Stop(int index = 0);
	/// <summary>
	/// 播放
	/// </summary>
	/// <param name="index">第几号</param>
	void Play(int index = 0);
	/// <summary>
	/// 只播放一次, 后续可再次播放
	/// </summary>
	/// <param name="index">第几号</param>
	void PlayOnce(int index = 0);
	/// <summary>
	/// 加载文件
	/// </summary>
	/// <param name="strFileName">文件名</param>
	/// <param name="index">第几号</param>
	/// <returns></returns>
	HRESULT LoadFile(const WCHAR* strFileName, int index);

private:
	HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);

	HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);

	IXAudio2* m_XAudio2;
	IXAudio2MasteringVoice* m_MasterVoice;
	//IXAudio2SourceVoice* m_SourceVoice;
	std::vector<IXAudio2SourceVoice*> m_SourceVoice;

	std::vector<WAVEFORMATEXTENSIBLE> wfx;
	std::vector<XAUDIO2_BUFFER> buffer;
};

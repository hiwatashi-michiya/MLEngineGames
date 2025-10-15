#include "AudioManager.h"
#include <fstream>
#include <cassert>
#include "Convert.h"

#pragma comment(lib, "xaudio2.lib")
#pragma comment(lib, "Mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "Mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")

using namespace MLEngine::Core;

uint32_t AudioManager::audioCount_ = 0;

AudioManager* AudioManager::GetInstance() {
	static AudioManager instance;
	return &instance;
}

void AudioManager::Initialize() {

	HRESULT result;

	result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);

	result = xAudio2_->CreateMasteringVoice(&masterVoice_);

	//Media Foundationの初期化
	MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);

}

void AudioManager::Finalize() {

	//XAudio2解放
	xAudio2_.Reset();

	for (Audio audio : audios_) {

		audio.pSourceVoice = nullptr;

		/*if (audio.soundData.pBuffer) {
			delete[] audio.soundData.pBuffer;
		}

		audio.soundData.pBuffer = 0;
		audio.soundData.bufferSize = 0;*/
		audio.wfex = {};

		//ポインタが存在していたら開放する
		if (audio.pMFMediaType) {
			audio.pMFMediaType->Release();
		}

		if (audio.pMFSourceReader) {
			audio.pMFSourceReader->Release();
		}

	}

	//Media Foundationの終了
	MFShutdown();

}

uint32_t AudioManager::LoadInMF(const std::string& filename) {

	//既にロードした音源だったらそのまま返す
	if (intMap_.find(filename) != intMap_.end()) {

		return intMap_[filename];

	}

	//最大ロード数を超えないようにする
	assert(audioCount_ < kMaxAudioCount_);

	Audio tmpAudio{};

	std::wstring path = ConvertString(filename);

	//ソースリーダー作成
	MFCreateSourceReaderFromURL(path.c_str(), NULL, &tmpAudio.pMFSourceReader);

	//メディアタイプの取得
	MFCreateMediaType(&tmpAudio.pMFMediaType);
	tmpAudio.pMFMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	tmpAudio.pMFMediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
	tmpAudio.pMFSourceReader->SetCurrentMediaType(DWORD(MF_SOURCE_READER_FIRST_AUDIO_STREAM), nullptr, tmpAudio.pMFMediaType);

	tmpAudio.pMFMediaType->Release();
	tmpAudio.pMFMediaType = nullptr;
	tmpAudio.pMFSourceReader->GetCurrentMediaType(DWORD(MF_SOURCE_READER_FIRST_AUDIO_STREAM), &tmpAudio.pMFMediaType);

	//オーディオデータ形式の作成
	MFCreateWaveFormatExFromMFMediaType(tmpAudio.pMFMediaType, &tmpAudio.wfex, nullptr);

	//データの読み込み

	while (true)
	{

		IMFSample* pMFSample{ nullptr };
		DWORD dwStreamFlags{ 0 };
		tmpAudio.pMFSourceReader->ReadSample(DWORD(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, nullptr, &dwStreamFlags, nullptr, &pMFSample);

		if (dwStreamFlags & MF_SOURCE_READERF_ENDOFSTREAM) {
			break;
		}

		IMFMediaBuffer* pMFMediaBuffer{ nullptr };
		pMFSample->ConvertToContiguousBuffer(&pMFMediaBuffer);

		BYTE* pBuffer{ nullptr };
		DWORD cbCurrentLength{ 0 };
		pMFMediaBuffer->Lock(&pBuffer, nullptr, &cbCurrentLength);

		tmpAudio.mediaData.resize(tmpAudio.mediaData.size() + cbCurrentLength);
		memcpy(tmpAudio.mediaData.data() + tmpAudio.mediaData.size() - cbCurrentLength, pBuffer, cbCurrentLength);

		pMFMediaBuffer->Unlock();

		pMFMediaBuffer->Release();
		pMFSample->Release();

	}

	//ファイル名と値を関連付け、配列に格納
	intMap_[filename] = audioCount_;
	audios_.push_back(tmpAudio);

	//戻り値を設定
	uint32_t number = audioCount_;

	//使用カウントを上昇
	audioCount_++;

	return number;

}

void AudioManager::Play(uint32_t handle, float volume, bool isLoop) {

	//ハンドルの値がカウントより大きい場合処理をしない
	if (handle >= audioCount_) {
		return;
	}

	HRESULT result;

	//波形フォーマットを基にSoundVoiceの生成
	result = xAudio2_->CreateSourceVoice(&audios_[handle].pSourceVoice, audios_[0].wfex);
	assert(SUCCEEDED(result));

	//再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = audios_[handle].mediaData.data();
	buf.AudioBytes = sizeof(BYTE) * static_cast<UINT32>(audios_[handle].mediaData.size());
	buf.Flags = XAUDIO2_END_OF_STREAM;

	//波形データの再生
	if (isLoop) {
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	audios_[handle].pSourceVoice->SetVolume(volume);
	result = audios_[handle].pSourceVoice->SubmitSourceBuffer(&buf);
	result = audios_[handle].pSourceVoice->Start();

}

void AudioManager::Stop(uint32_t handle) {

	//ハンドルの値がカウントより大きい場合処理をしない
	if (handle >= audioCount_) {
		return;
	}

	if (audios_[handle].pSourceVoice) {
		audios_[handle].pSourceVoice->Stop();
		audios_[handle].pSourceVoice->DestroyVoice();
		audios_[handle].pSourceVoice = nullptr;
	}

}

void AudioManager::Pause(uint32_t handle) {

	//ハンドルの値がカウントより大きい場合処理をしない
	if (handle >= audioCount_) {
		return;
	}

	if (audios_[handle].pSourceVoice) {
		audios_[handle].isPause = true;
		audios_[handle].pSourceVoice->Stop();
	}

}

void AudioManager::ReStart(uint32_t handle) {

	//ハンドルの値がカウントより大きい場合処理をしない
	if (handle >= audioCount_) {
		return;
	}

	if (audios_[handle].pSourceVoice) {
		audios_[handle].isPause = false;
		audios_[handle].pSourceVoice->Start();
	}

}

void AudioManager::SetVolume(uint32_t handle, float volume) {

	//ハンドルの値がカウントより大きい場合処理をしない
	if (handle >= audioCount_) {
		return;
	}

	if (audios_[handle].pSourceVoice) {
		audios_[handle].pSourceVoice->SetVolume(volume);
	}

}

bool AudioManager::IsPlaying(uint32_t handle) {

	//ハンドルの値がカウントより大きい場合処理をせず、falseを返す
	if (handle >= audioCount_) {
		return false;
	}

	if (audios_[handle].pSourceVoice) {
		
		XAUDIO2_VOICE_STATE voiceState;

		audios_[handle].pSourceVoice->GetState(&voiceState);

		return (voiceState.BuffersQueued > 0); //バッファがキューにあれば再生中とみなす
		

	}

	return false;

}

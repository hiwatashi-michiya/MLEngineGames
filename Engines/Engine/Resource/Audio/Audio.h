#pragma once
#include <stdint.h>
#include <string>

namespace MLEngine::Resource {

	/// <summary>
	/// オーディオ再生クラス
	/// </summary>
	class Audio
	{
	public:
		Audio() = default;
		~Audio() = default;

		//オーディオファイル読み込み
		void Load(const std::string& filename);

		/// <summary>
		/// 音声再生
		/// </summary>
		/// <param name="handle">ハンドル</param>
		/// <param name="volume">音量。0.0f～1.0f</param>
		/// <param name="isLoop">ループするかどうか</param>
		void Play(float volume, bool isLoop = false);

		/// <summary>
		/// 音声停止
		/// </summary>
		/// <param name="handle">ハンドル</param>
		void Stop();

		/// <summary>
		/// 一時停止
		/// </summary>
		/// <param name="handle">ハンドル</param>
		void Pause();

		/// <summary>
		/// 停止したところから再生
		/// </summary>
		/// <param name="handle">ハンドル</param>
		void ReStart();

		/// <summary>
		/// ボリューム調整
		/// </summary>
		/// <param name="handle">ハンドル</param>
		/// <param name="volume">音量</param>
		void SetVolume(float volume);

		/// <summary>
		/// 再生中かどうか
		/// </summary>
		/// <param name="handle">ハンドル</param>
		/// <returns>再生中だったらtrue</returns>
		bool IsPlaying();

	private:

		//オーディオハンドル
		uint32_t handle_;

	};

}

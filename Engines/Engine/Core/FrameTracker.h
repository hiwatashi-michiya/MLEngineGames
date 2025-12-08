#pragma once
#include <stdint.h>
#include <chrono>
#include <thread>

namespace MLEngine::Core {

	/// <summary>
	/// デルタタイム計測
	/// </summary>
	class FrameTracker
	{
	public:
		
		static FrameTracker* GetInstance();
		//FPS制限
		static const uint32_t kFixedFPS = 60;

		//初期化
		void Initialize();
		//デルタタイム取得
		double GetDeltaTime();
		//float型でデルタタイム取得
		float GetDeltaTimeF();

		//FPS更新
		void Update();
		//FPS表示(デバッグ)
		void Debug();

	private:

		//記録時間(FPS固定用)
		std::chrono::steady_clock::time_point reference_;
		//前回参照時間
		std::chrono::steady_clock::time_point preReference_;
		//固定時間
		std::chrono::microseconds minTime_;
		//固定時間より少し短い時間
		std::chrono::microseconds minCheckTime_;
		//差分時間
		std::chrono::microseconds elapsed_;

	private:

		FrameTracker() = default;
		~FrameTracker() = default;
		FrameTracker(const FrameTracker&) = delete;
		const FrameTracker& operator=(const FrameTracker&) = delete;

	};

}

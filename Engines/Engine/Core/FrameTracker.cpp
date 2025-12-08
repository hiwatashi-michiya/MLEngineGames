#include "FrameTracker.h"
#include <Windows.h>
#include "ImGuiManager.h"

#pragma comment(lib, "winmm.lib")

using namespace MLEngine::Core;

FrameTracker* FrameTracker::GetInstance() {
	static FrameTracker instance;
	return &instance;
}

void FrameTracker::Initialize() {

	//システムタイマーの分解能を上げる
	timeBeginPeriod(1);

	//1/60秒ぴったりの時間
	minTime_ = static_cast<std::chrono::microseconds>(uint64_t(1000000.0f / float(kFixedFPS)));
	//1/60秒よりわずかに短い時間
	minCheckTime_ = static_cast<std::chrono::microseconds>(uint64_t(1000000.0f / float(kFixedFPS + 5)));

	//現在時間を記録する
	reference_ = std::chrono::steady_clock::now();
	preReference_ = reference_;

}

double FrameTracker::GetDeltaTime()
{
	return static_cast<double>(elapsed_.count() / 1000000.0f);
}

float FrameTracker::GetDeltaTimeF()
{
	return static_cast<float>(GetDeltaTime());
}

//FPS固定更新
void FrameTracker::Update() {

	//現在時間を取得する
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	//前回記録からの経過時間を取得する
	 elapsed_ = std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	//1/60秒よりわずかに短い時間が経っていない場合
	if (elapsed_ < minCheckTime_) {
		//1/60秒経過するまで微小なスリーブを繰り返す
		while (std::chrono::steady_clock::now() - reference_ < minTime_) {
			//1マイクロ秒スリーブ
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}

		elapsed_ = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - reference_);

	}
	//現在の時間を記録する
	preReference_ = reference_;
	reference_ = std::chrono::steady_clock::now();

}

void FrameTracker::Debug() {

#ifdef _DEBUG
	ImGui::Text("FPS : %1.5f", 1.0f / GetDeltaTime());
#endif // _DEBUG

}

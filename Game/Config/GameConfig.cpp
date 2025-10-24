#include "GameConfig.h"
#include"Externals/imgui/imgui.h"

void GameConfig::Initialize() {
	centerLane_ = maxLane_ / 2;//ちょうど半分になるように

	/*Json等で外部に出している場合ここで読み込む*/
}

void GameConfig::Update() {
	centerLane_ = maxLane_ / 2;//ちょうど半分になるように
}

void GameConfig::Debug() {
	ImGui::Begin("ゲームの設定");
	ImGui::DragInt("レーンの最大値", &maxLane_, 2, 1, 15);
	ImGui::DragInt("レーンの幅", &laneDistance_, 1, 50, 1000);
	ImGui::End();
}
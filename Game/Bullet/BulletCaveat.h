#pragma once

#include <vector>
#include <list>

#include <Engine/Resource/Sprite/Sprite2D.h>
#include ".vs/../Engine/Tool/GlobalVariables.h"

class Player;

class BulletCaveat {
	struct EffectElement {
		float time = 0.0f;
		float switchTime = 0.1f;
		float totalTime = 1.0f;
		int lane = -1;
		int prevSwitchIndex = 0;
	};

public:
	void Initialize(Player* player);
	void Update();
	void DebugUI();
	void Warn(int lane);
	void Continuous();
	void SetPlayer(Player* player) { player_ = player; }
private:
	GlobalVariables* global_ = nullptr;

	Player* player_;

	// 注意表示用スプライト
	std::vector<std::unique_ptr<MLEngine::Resource::Sprite2D>>  caveatSprites_;
	// スケール
	MLEngine::Math::Vector2 size_ = { 0.2f, 0.2f };
	// 平行移動
	MLEngine::Math::Vector2 position_ = { 0.0f, -0.18f };
	// 間隔
	float distance_ = 1.7f;

	// スプライト表示用のタイマー　
	std::list<EffectElement> effectElements_;
	float switchTime_ = 0.1f;
	float totalTime_ = 1.0f;

	std::unique_ptr<MLEngine::Resource::Sprite2D> continuousSprite_;
	EffectElement effectElement_;
	bool isContinuous_;
	MLEngine::Math::Vector2 continuousPosition_;
	MLEngine::Math::Vector2 continuousSize_;
	float continuousTotalTime_ = 3.0f;
	


};
#pragma once

#include <Engine/Resource/Sprite/Sprite.h>

#include ".vs/../Engine/Tool/GlobalVariables.h"

class Enemy;

class EnemyUI
{
public:
	void Initialize(Enemy* enemy);
	void Update();

private:
	GlobalVariables* global_ = nullptr;

	// 敵情報
	Enemy* enemy_ = nullptr;

	// テクスチャ
	MLEngine::Resource::Texture texture_;

	// 体力ゲージ
	std::unique_ptr<MLEngine::Resource::Sprite> hpBar_;
	// 体力ゲージ調整項目
	MLEngine::Math::Vector2 barPosition_{};
	MLEngine::Math::Vector2 barSize_{};
	MLEngine::Math::Vector4 barColor_ = { 1.0f,0.0f,0.0f,1.0f };

	// フレーム
	std::unique_ptr<MLEngine::Resource::Sprite> frame_;
	// フレーム調整項目
	MLEngine::Math::Vector2 framePosition_{};
	MLEngine::Math::Vector2 frameSize_{};
	MLEngine::Math::Vector4 frameColor_ = { 0.0f,0.0f,0.0f,1.0f };

};
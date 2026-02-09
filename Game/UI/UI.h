#pragma once
#include "Sprite2D.h"

/// <summary>
/// SPrite2Dを使ったUIクラス
/// </summary>
class UI
{
public:
	UI();
	~UI();

	//UI作成
	void Initialize(MLEngine::Resource::Texture texture, MLEngine::Math::Vector2 pos, MLEngine::Math::Vector4 col = { 1.0f,1.0f,1.0f,1.0f });
	//更新
	void Update();
	//イージングの開始
	void Start() { 
		isStartEasing_ = true;
		isEndEasing_ = false;
	}
	//イージング中断
	void Pause() { isStartEasing_ = false; }
	//イージングを停止してリセット
	void Stop();
	//最初から再スタート
	void ReStart() {
		Stop();
		Start();
	}
	//表示するかどうか
	void SetIsActive(bool flag) { sprite_->isActive = flag; }
	//イージング開始取得
	bool GetIsStartEasing() const { return isStartEasing_; }
	//イージング終了取得
	bool GetIsEndEasing() const { return isEndEasing_; }

	void SetUVScale(const MLEngine::Math::Vector2 vec) { sprite_->uvScale = vec; }

	void SetUVTrans(const MLEngine::Math::Vector2 vec) { sprite_->uvTranslate = vec; }

	void SetColor(const MLEngine::Math::Vector4 color) { sprite_->color = color; }

	void SetRotate(float rotate) { sprite_->rotate = rotate; }

	//スプライト取得
	MLEngine::Resource::Sprite2D* GetSprite() { return sprite_.get(); }
	//開始座標
	MLEngine::Math::Vector2 startPosition{};
	//中間座標
	MLEngine::Math::Vector2 middlePosition{};
	//終了座標
	MLEngine::Math::Vector2 endPosition{};
	//開始スケール
	MLEngine::Math::Vector2 startScale{1.0f,1.0f};
	//中間スケール
	MLEngine::Math::Vector2 middleScale{1.0f,1.0f};
	//終了スケール
	MLEngine::Math::Vector2 endScale{1.0f,1.0f};
	//総イージング時間
	float easingTime = 1.0f;
	//中間座標までの時間
	float startToMiddleTime = 0.25f;
	//中間座標で止まる時間
	float stayMiddleTime = 0.5f;

private:

	//イージングを開始しているかどうか
	bool isStartEasing_ = false;
	//イージングが終了したかどうか
	bool isEndEasing_ = false;
	//現在のイージングカウント
	float currentEasingTime_ = 0.0f;
	//画像
	std::unique_ptr<MLEngine::Resource::Sprite2D> sprite_;

};



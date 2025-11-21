#pragma once
#include<Manager/PlayerManager.h>

/*敵やプレイヤーの体力UIを管理するクラス*/
class LifeUI{
public:
	LifeUI(Player* player);

	void Initialize();

	void Update();

	void Draw();

private:
	//デバック描画(ラインとか)
	void DebugDraw();

	void SizeCulc();

private:
	Player* player_ = nullptr;

	float PLyMax_ = 370.0f;

	MLEngine::Resource::Texture texture_;

	std::unique_ptr<MLEngine::Resource::Sprite> meter_;

	MLEngine::Math::Vector2 plMeterPos_{};

	MLEngine::Math::Vector2 plMeterSize_{};


	std::unique_ptr<MLEngine::Resource::Sprite> frame_;

	MLEngine::Math::Vector2 plFramePos_{};

	MLEngine::Math::Vector2 plFrameSize_{};



	MLEngine::Math::Vector4 plColor_ = { 0.0f,1.0f,0.0f,1.0f };
};


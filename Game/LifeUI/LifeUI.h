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
	std::unique_ptr<MLEngine::Resource::Sprite> sprite_;

	MLEngine::Resource::Texture texture_;

	Player* player_ = nullptr;

	MLEngine::Math::Vector2 plPos_{};

	MLEngine::Math::Vector4 plColor_{};
};


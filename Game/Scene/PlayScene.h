#pragma once
#include "Scene/BaseScene.h"
#include "Model/RigidModel.h"
#include "Object/Camera.h"
#include "Sprite/Sprite.h"
#include "Audio/Audio.h"
#include "Input/Input.h"
#include "Particle/Particle3D.h"
#include"Manager/GameManager.h"
#include"Config/GameConfig.h"
#include"Manager/PlayerManager.h"
#include <memory>
#include "Enemy/Enemy.h"
#include "Bullet/BulletManager.h"

class PlayScene : public BaseScene
{
public:
	PlayScene();
	~PlayScene();

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

private:
	void DrawImgui();

private:

	//入力デバイス
	MLEngine::Input::Manager* input_ = nullptr;

	//カメラ
	//MLEngine::Object::Camera camera_;

	GameManager* gameManager_ = nullptr;
	GameConfig* config_ = nullptr;
	
	// プレイヤーマネージャー
	std::unique_ptr<PlayerManager> playerManager_;

	// 敵
	std::unique_ptr<Enemy> enemy_;

	// 弾マネージャー
	std::unique_ptr<BulletManager> bulletManager_;

};


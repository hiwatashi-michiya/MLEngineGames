#pragma once
#include "Scene/BaseScene.h"
#include "Model/RigidModel.h"
#include "Object/Camera.h"
#include "Sprite/Sprite2D.h"
#include "Audio/Audio.h"
#include "Input/Input.h"
#include "Particle/Particle3D.h"
#include"Manager/GameManager.h"
#include"Config/GameConfig.h"
#include"Manager/PlayerManager.h"
#include"LifeUI/LifeUI.h"
#include"Score/ScoreNumber.h"
#include <memory>
#include "Enemy/Enemy.h"
#include "Bullet/BulletManager.h"
#include "Enemy/EnemyAttackTurnController.h"

class PlayScene : public BaseScene
{
public:
	struct GameStatePacket {
		NetworkManager::PacketHeader header;
		GameManager::GameState gameState;
	};

	struct EnemyFlugPacket {
		NetworkManager::PacketHeader header;
		bool isEnemyDead;
	};


public:
	PlayScene();
	~PlayScene();

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

private:
	//外部化クラスに登録
	void GlobalSetValue();
	//外部化クラスから取得
	void GlobalGetValue();

	void DrawImgui();

private:

	//入力デバイス
	MLEngine::Input::Manager* input_ = nullptr;

	//カメラ
	//MLEngine::Object::Camera camera_;

	GameManager* gameManager_ = nullptr;
	GameConfig* config_ = nullptr;
	
	GameManager::GameState state_ = GameManager::GameState::Title;

	// プレイヤーマネージャー
	std::unique_ptr<PlayerManager> playerManager_;

	// 敵
	std::unique_ptr<Enemy> enemy_;

	// 弾マネージャー
	std::unique_ptr<BulletManager> bulletManager_;

	std::unique_ptr<LifeUI> lifeUI_;

	std::unique_ptr<ScoreNumber> scoreUI_;



	MLEngine::Math::Vector3 rotate_ = { 0.0f, 0.0f, 0.0f };

	// スケール・平行移動
	MLEngine::Math::Vector3 scale_ = { 2.0f, 1.0f, 1.0f };
	MLEngine::Math::Vector3 translate_ = { 0.0f, 0.0f,0.0f };

	MLEngine::Resource::Texture titleTexture_;
	MLEngine::Resource::Texture gameClearTexture_;
	MLEngine::Resource::Texture gameOverTexture_;

	std::string tutorialMoveTexture_;
	std::string tutorialTurnTexture_;

	std::unique_ptr<MLEngine::Object::Transform> tutorialTransform_;
	MLEngine::Resource::Sprite3D tutorialSprite_;
	std::unique_ptr<MLEngine::Resource::Sprite2D> titleSprite_;
	std::unique_ptr<MLEngine::Resource::Sprite2D> resultSprite_;


	MLEngine::Math::Vector2 titlePos_;
	MLEngine::Math::Vector2 titleScale_;

	MLEngine::Math::Vector2 tutorialAnchor_;
	MLEngine::Math::Vector3 tutorialPos_;
	MLEngine::Math::Vector3 tutorialRotate_;
	MLEngine::Math::Vector3 tutorialScale_;

	MLEngine::Math::Vector2 resultPos_;
	MLEngine::Math::Vector2 resultScale_;

	MLEngine::Math::Vector4 titleColor_;
	MLEngine::Math::Vector4 tutorialColor_;
	MLEngine::Math::Vector4 resultColor_;

	std::unique_ptr<MLEngine::Object::Transform> planeTransform_;

	//床のテクスチャ
	MLEngine::Resource::Sprite3D groundPlane_;
	MLEngine::Resource::Sprite3D lanePlane_;

	std::string groundTexture_;
	std::string laneTexture_;

	bool isClientEnemyDead_ = false;

};


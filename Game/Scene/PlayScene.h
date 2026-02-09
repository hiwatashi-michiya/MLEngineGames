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
#include "Render/PostEffect/PostEffectDrawer.h"
#include "MLMath.h"
#include "../UI/UI.h"
#include "Enemy/EnemyStateController.h"

class PlayScene : public BaseScene
{
public:
	

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

	//ビネット演出の設定
	struct VignetteConfig {
		//色
		MLEngine::Math::Vector3 color = { 0.75f, 0.2f, 0.0f };
		//パワーの値の範囲最小
		float minPowerRange = 0.05f;
		//パワーの値の範囲最大
		float maxPowerRange = 0.1f;
		//パワーの値最小値
		float minPower = 0.2f;
		//パワーの値最大値
		float maxPower = 0.45f;
		//ビネットの拡縮時間最小
		float scalingMinTime = 0.5f;
		//ビネットの拡縮時間最長
		float scalingMaxTime = 1.5f;
		//現在の経過時間
		float currentTime = 0.0f;
		//ビネットがかかり始めるプレイヤーの体力比率
		float startRatio = 0.4f;
		//ビネットの強さが最大になるプレイヤーの体力比率
		float endRatio = 0.2f;

	};

	//入力デバイス
	MLEngine::Input::Manager* input_ = nullptr;

	//ポストエフェクトの描画管理
	MLEngine::Core::Render::PostEffect::PostEffectDrawer* postEffect_ = nullptr;

	//ビネット演出設定
	VignetteConfig vignetteConfig_;

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

	//天球
	MLEngine::Resource::RigidModel skydome_;
	MLEngine::Object::Transform skydomeTransform_;

	static const int32_t kMaxStone_ = 3;

	//縁石
	std::array<MLEngine::Resource::RigidModel, kMaxStone_> stoneLeft_;
	std::array<MLEngine::Resource::RigidModel, kMaxStone_> stoneRight_;
	std::array<MLEngine::Object::Transform, kMaxStone_> stoneLeftTF_;
	std::array<MLEngine::Object::Transform, kMaxStone_> stoneRightTF_;

	std::string groundTexture_;
	std::string laneTexture_;

	bool isClientEnemyDead_ = false;

	//UI関係
	MLEngine::Resource::Texture ingameStartTex_;
	MLEngine::Resource::Texture ingameGameoverTex_;
	MLEngine::Resource::Texture ingameFinishTex_;
	MLEngine::Resource::Texture resultScoreBackTex_;
	MLEngine::Resource::Texture numTex_;

	UI ingameStartUI_;
	UI ingameGameoverUI_;
	UI ingameFinishUI_;
	UI resultScoreBackUI_;
	std::array<UI, 2> resultScoreUIs_;

};


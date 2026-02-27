#pragma once

#include "Object/Transform.h"
#include "Math/Rand.h"
#include "EnemyState.h"
#include "Bullet/BulletManager.h"
#include "RigidModel.h"
#include ".vs/../Engine/Tool/GlobalVariables.h"
#include "Enemy/EnemyUI.h"
#include "Enemy/EnemyMotionState.h"
#include "Sprite3D.h"
#include "Enemy/EnemyHand.h"
#include "Audio/Audio.h"
#include "Particle/HitParticle.h"

class Enemy
{
public:
	const char* states[4] = {
	"Normal",
	"Down",
	"Berserk",
	"GreatAttack"
	};

	enum class Mode {
		kNormal,
		kAngry,
		kAttack,
		kCry
	};

public:
	Enemy() {};
	~Enemy() {};
	void Initialize();
	void TutorialInitialize();
	void Update();
	void TutorialUpdate();
	void DebugUI();

	// 状態変更
	void ChangeState(std::unique_ptr<EnemyState> newState);

	// モーション状態変更
	void ChangeMotionState(std::unique_ptr<EnemyMotionState> newMotionState);

	// 衝突処理
	void OnCollision(MLEngine::Math::Vector3 position, int damege);

	void ChangeTexture(Mode mode);

	void ParantTransform();

	void AddGrateAttackTime(float time) {
		greatAttackTime_ += time;


	}

	void AddAngryTime(float time) {
		angryTime_ += time;
	}


	// ゲット関数
	MLEngine::Object::Camera* GetCamera() { return camera_; }
	BulletManager* GetBulletManager() { return bulletManager_; }
	EnemyHand* GetLeftHand() { return leftHand_.get(); }
	EnemyHand* GetRightHand() { return rightHand_.get(); }
	MLEngine::Resource::Sprite3D* GetFrontSprite() { return &frontPlane_; }
	MLEngine::Math::Vector3 GetRotate() { return rotate_; }
	int GetHp() const { return hp_; }
	int GetMaxHp() const { return maxHp_; }
	float GetHealthRate()const { return healthRate_; }
	bool GetIsDead() const {
		if (hp_ <= 0) {
			return true;
		}
		else {
			return false;
		}
	}
	float GetGreatAttackTime() const { return greatAttackTime_; }
	float GetAngryTime() const { return angryTime_; }

	bool GetIsActive() const { return frontPlane_.isActive; }

	bool GetHitMomentFlug()const { return hitMomentFlug_; }

	// セット関数
	// 弾マネージャー取得
	void SetBulletManager(BulletManager* bulletManager) {
		bulletManager_ = bulletManager;
	}
	void SetCamera(MLEngine::Object::Camera* camera) {
		camera_ = camera;
	}
	void SetRotate(MLEngine::Math::Vector3 rotate) {
		rotate_ = rotate;
	}
	void SetIsActive(const bool isActive) {
		frontPlane_.isActive = isActive;
		backPlane_.isActive = isActive;
		enemyUI_->SetIsActive(isActive);
		bulletManager_->SetIsModelActive(isActive);
		leftHand_->SetIsActive(isActive);
		rightHand_->SetIsActive(isActive);
	}
	void ResetGreatAttackTime() {
		greatAttackTime_ = 0.0f;
	}
	void ResetAngryTime() {
		angryTime_ = 0.0f;
	}


private:

	MLEngine::Object::Camera* camera_;

	GlobalVariables* global_;

	std::unique_ptr<EnemyUI> enemyUI_;

	// 現在の状態
	std::unique_ptr<EnemyState> currentState_;

	// 現在のモーション状態
	std::unique_ptr<EnemyMotionState> motionState_;

	// 原点モデル
	MLEngine::Resource::RigidModel model_;

	// 敵3Dスプライト
	MLEngine::Resource::Sprite3D frontPlane_;
	MLEngine::Resource::Sprite3D backPlane_;

	std::string normalTexture_;
	std::string angryTexture_;
	std::string attackTexture_;
	std::string backTextrue_;
	std::string cryTesture_;

	// 3Dスプライトのトランスフォーム
	std::unique_ptr<MLEngine::Object::Transform> transform_;

	// 3Dスプライトの親トランスフォーム
	std::unique_ptr<MLEngine::Object::Transform> offsetTransform_;
	MLEngine::Math::Vector3 rotate_ = { 0.0f, 0.0f, 0.0f };

	// スケール・平行移動
	MLEngine::Math::Vector3 scale_ = { 2.0f, 1.0f, 1.0f };
	MLEngine::Math::Vector3 translate_ = { 0.0f, 1.0f,0.0f };

	// 弾マネージャー
	BulletManager* bulletManager_;

	// 左手
	std::unique_ptr<EnemyHand> leftHand_;
	// 右手
	std::unique_ptr<EnemyHand> rightHand_;
	// 
	std::unique_ptr<HitParticle> hitParticle_;
	// 体力
	int maxHp_ = 500;
	int hp_ = 0;

	float healthRate_ = 0.0f;

	// ダウンカウント
	int maxDownCount_ = 10;
	int downCount_ = 0;

	// 大技用
	float greatAttackTime_ = 0.0f;
	float maxGrateAttackTime_ = 20.0f;
	// 怒り用
	float angryTime_ = 0.0f;
	float maxAngryTime_ = 6.0f;

	// ImGui用状態選択インデックス
	int stateIndex = 0;

	//攻撃が当たった瞬間
	bool hitMomentFlug_ = false;

	//SE
	MLEngine::Resource::Audio enemyDamageSE_;
	MLEngine::Resource::Audio enemyAngrySE_;
	MLEngine::Resource::Audio enemyDownSE_;

	bool isEasing_ = true;
	float easingTime_ = 0.0f;


};


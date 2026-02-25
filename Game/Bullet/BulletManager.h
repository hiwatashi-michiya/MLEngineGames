#pragma once

#include <list>
#include <vector>

#include "Object/Camera.h"
#include "Bullet.h"
#include ".vs/../Engine/Tool/GlobalVariables.h"
#include "Player/Player.h"
#include "BulletCaveat.h"
#include "Particle/SmokeParticle.h"
#include "Particle/WeakHitParticle.h"

class Enemy;
//class BulletCaveat;

class BulletManager {
	public:
		BulletManager() {};
		~BulletManager() {};
		void Initialize(Player* player, Enemy* enemy);
		void Update(bool isTutorial);

		// 弾を生成する
		void SpawnBullet(int laneNumber, float time, bool isTutorial);

		void SpawnReflectBullet(int laneNumber, float time, Bullet::BulletType type);

		Bullet::BulletType GetBulletType(int normalNum, int normalDen,int weakNum, int weakDen);

		BulletCaveat* GetBulletCaveat() { return bulletCaveat_.get(); }

		bool GetIsReceive() const { return isReceive_; }
		bool GetIsReflect() const { return isReflect_; }

		void SetPlayer(Player* player) { player_ = player; }
		void SetEnemy(Enemy* enemy) { enemy_ = enemy; }

		void SetIsModelActive(bool isActive) {
			isModelActive_ = isActive;
		}
private:
	// 弾のリスト
	std::list<std::unique_ptr<Bullet>> bullets_;

	GlobalVariables* global_;

	// プレイヤー情報
	Player* player_ = nullptr;

	// 敵情報
	Enemy* enemy_ = nullptr;

	std::unique_ptr<BulletCaveat> bulletCaveat_;

	// モデル
	std::vector<std::unique_ptr<MLEngine::Resource::RigidModel>> startModels_;
	std::vector<std::unique_ptr<MLEngine::Resource::Sprite3D>> startSprite3D_;

	std::vector<std::unique_ptr<MLEngine::Resource::RigidModel>> endModels_;
	std::vector<std::unique_ptr<MLEngine::Resource::Sprite3D>> endSprite3D_;
	// スケール
	MLEngine::Math::Vector3 startScale_ = { 0.2f, 0.2f, 1.0f };
	MLEngine::Math::Vector3 endScale_ = { 0.04f, 0.04f, 1.0f };
	// 平行移動
	MLEngine::Math::Vector3 startTranslate_ = { 0.0f, 0.0f,0.0f };
	MLEngine::Math::Vector3 endTranslate_ = { 0.0f, -0.18f, -9.0f };
	float startDistance_ = 1.7f;
	float endDistance_ = 0.28f;
	// 弾のダメージ
	int bulletDamege_ = 10;
	// モデルのアクティブ状態
	bool isModelActive_ = true;
	// 弾を受け取ったかどうか
	bool isReceive_ = false;
	int receiveCount_ = 0;
	// 反射したかどうか
	bool isReflect_ = false;
	// 反射した弾の速度
	float reflectSpeed_ = 0.5f;
	// 煙エフェクト
	std::unique_ptr<SmokeParticle> smokeParticle_;
	// 弱攻撃ヒットエフェクト
	std::unique_ptr<WeakHitParticle> weakHitParticle_;

	int normalBumerator_ = 2;
	int normalDenominator_ = 3;
	int weakNumerator_ = 1;
	int weakDenominator_ = 3;
};
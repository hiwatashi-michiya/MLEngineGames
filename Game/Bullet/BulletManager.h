#pragma once

#include <list>
#include <vector>
#include "Object/Camera.h"
#include "Bullet.h"

class BulletManager {
	public:
		BulletManager() {};
		~BulletManager() {};
		void Initialize();
		void Update();
		void Draw(MLEngine::Object::Camera* camera);

        //void SetLaunchPosition(MLEngine::Math::Vector2 position) { launchPosition_ = position; }
		// 弾を生成する
		void SpawnBullet(const MLEngine::Math::Vector2& position, const MLEngine::Math::Vector2& direction, float speed);
		void SpawnBullet(const MLEngine::Math::Vector2& position, int laneNumber, float time);
		void SpawnBullet(int laneNumber, float time);
private:
	// 弾のリスト
	std::list<std::unique_ptr<Bullet>> bullets_;

	//// 始点スプライト
	//std::vector<std::unique_ptr<MLEngine::Resource::Sprite>> startSprites_;
	//// 終点スプライト
	//std::vector<std::unique_ptr<MLEngine::Resource::Sprite>> targetSprites_;

	//const std::vector <MLEngine::Math::Vector2> targetPositions_ = {
	//	{ 200.0f, 640.0f},
	//	{ 640.0f, 640.0f },
	//	{ 1080.0f, 640.0f },
	//};
	//// 発射位置
	//MLEngine::Math::Vector2 launchPosition_;
	//// 始点間隔
	//float startDistance_ = 100.0f;
	//// 終点間隔
	//float endDistance_ = 440.0f;
	//// 終点ライン
	//float endLine_ = 640.0f;

	//// 最小サイズ
	//float minSize_ = 32.0f;
	//// 最大サイズ
	//float maxSize_ = 128.0f;

	// モデル
	std::vector<std::unique_ptr<MLEngine::Resource::RigidModel>> startModels_;

	std::vector<std::unique_ptr<MLEngine::Resource::RigidModel>> endModels_;
	// スケール
	MLEngine::Math::Vector3 startScale_ = { 0.2f, 0.2f, 1.0f };
	MLEngine::Math::Vector3 endScale_ = { 0.04f, 0.04f, 1.0f };
	// 平行移動
	MLEngine::Math::Vector3 startTranslate_ = { 0.0f, 0.0f,0.0f };
	MLEngine::Math::Vector3 endTranslate_ = { 0.0f, -0.18f, -9.0f };
	float startD_ = 1.7f;
	float endD_ = 0.28f;
};
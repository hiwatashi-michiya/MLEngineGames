#pragma once

#include <list>

#include "Object/Camera.h"
#include "Bullet.h"

class BulletManager {
	public:
		BulletManager() {};
		~BulletManager() {};
		void Initialize();
		void Update();
		void Draw(MLEngine::Object::Camera* camera);
		// 弾を生成する
		void SpawnBullet(const MLEngine::Math::Vector2& position, const MLEngine::Math::Vector2& direction, float speed);
		void SpawnBullet(const MLEngine::Math::Vector2& position, int laneNumber, float time);
private:
	std::list<std::unique_ptr<Bullet>> bullets_;
	const std::vector <MLEngine::Math::Vector2> targetPositions_ = {
		{ 200.0f, 640.0f},
		{ 640.0f, 640.0f },
		{ 1080.0f, 640.0f },
	};
};
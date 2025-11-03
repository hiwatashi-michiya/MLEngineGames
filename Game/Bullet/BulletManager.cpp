#include "BulletManager.h"
#include "Input/Input.h"

void BulletManager::Initialize()
{
	bullets_.clear();
}

void BulletManager::Update()
{
	MLEngine::Input::Manager* input = MLEngine::Input::Manager::GetInstance();

	for(auto& bullet : bullets_)
	{
		bullet->Update();

		/*if (input->GetKeyboard()->TriggerKey(DIK_R)) {
			bullet->Reverse();
		}*/

	}

	bullets_.remove_if([](const std::unique_ptr<Bullet>& bullet) {
		return bullet->IsDead();
	});
}

void BulletManager::Draw(MLEngine::Object::Camera* camera)
{
	for(auto& bullet : bullets_)
	{
		bullet->Draw(camera);
	}
}

void BulletManager::SpawnBullet(const MLEngine::Math::Vector2& position, const MLEngine::Math::Vector2& direction, float speed)
{
	std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
	newBullet->Initialize();
	newBullet->SetPosition(position);
	newBullet->SetVelocity(direction, speed);
	bullets_.push_back(std::move(newBullet));


}

void BulletManager::SpawnBullet(const MLEngine::Math::Vector2& position, int laneNumber, float time)
{
	std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
	newBullet->Initialize();
	newBullet->SetPosition(position);
	newBullet->SetTargetPosition(targetPositions_[laneNumber]);
	newBullet->SetTravelTime(time);
	bullets_.push_back(std::move(newBullet));
}


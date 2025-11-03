#include "Enemy.h"

#include "Input/Input.h"

void Enemy::Initialize()
{
	position_ = { 640.0f, 200.0f };

	MLEngine::Resource::Texture texture;
	texture.Load("./Resources/EngineResources/defaultMask.png");
	sprite_.reset(MLEngine::Resource::Sprite::Create(texture, initialPosition_, { 1.0f,1.0f,1.0f,1.0f }));
	sprite_->anchorPoint_ = { 0.5f,0.5f };
	sprite_->size_ = { 320.0f,160.0f };


	ChangeState(std::make_unique<EnemyNormalState>());

	bulletManager_ = std::make_unique<BulletManager>();
	bulletManager_->Initialize();

	for (int i = 0; i < 3; ++i) {
		bulletManager_->SpawnBullet({ LaunchPosition().x + distance_ * (i - 1), LaunchPosition().y}, {(i - 1) * 0.8f , 1.0f}, 4.0f);
	}

	hp_ = maxHp_;

}

void Enemy::Update()
{
	if (!dynamic_cast<EnemyBerserkState*>(currentState_.get())) {
		if(hp_ <= maxHp_ * 0.3f) {
			ChangeState(std::make_unique<EnemyBerserkState>());
		}
	}

	MLEngine::Input::Manager* input = MLEngine::Input::Manager::GetInstance();

	if (input->GetKeyboard()->TriggerKey(DIK_H)) {
		hp_ = maxHp_ * 0.3f;
	}
	if (input->GetKeyboard()->TriggerKey(DIK_J)) {
		ChangeState(std::make_unique<EnemyDownState>());
	}

	currentState_->Update(this);
	bulletManager_->Update();


}

void Enemy::Draw(MLEngine::Object::Camera* camera)
{
	sprite_->Draw();
	bulletManager_->Draw(camera);
}

void Enemy::ChangeState(std::unique_ptr<EnemyState> newState)
{
	if (currentState_) {
		currentState_->Exit(this);
	}

	currentState_ = std::move(newState);
	currentState_->Enter(this);
}

MLEngine::Math::Vector2 Enemy::LaunchPosition()
{
	MLEngine::Math::Vector2 launchPosition = initialPosition_;
	launchPosition.y += sprite_->size_.y / 2.0f;
	return launchPosition;
}

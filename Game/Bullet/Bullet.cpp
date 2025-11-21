#include "Bullet.h"

#include "Resource/Texture/Texture.h"

void Bullet::Initialize()
{
	model_.Initialize("./Resources/model/plane/plane.obj");
	model_.worldMatrix = MLEngine::Math::MakeAffineMatrix(scale_, { 0.0f, 0.0f, 0.0f, 1.0f }, position_);

	/*MLEngine::Resource::Texture texture;
	texture.Load("./Resources/EngineResources/blockMask.png");
	sprite_.reset(MLEngine::Resource::Sprite::Create(texture, position_, { 1.0f,1.0f,1.0f,1.0f }));
	sprite_->anchorPoint = { 0.5f,0.5f };*/
}

void Bullet::Update()
{
	//position_ = MLEngine::Math::Lerp(startPosition_, targetPosition_, elapsedTime_ / travelTime_);
	//sprite_->SetPosition(position_);

	//float t = (position_.y - startPosition_.y) / (endLine_ - startPosition_.y);

	//float size =/* t * multiplier_*/minSize_ + t * (maxSize_ - minSize_);
	//sprite_->size = {size, size };

	position_ = MLEngine::Math::Lerp(startPosition_, targetPosition_, elapsedTime_ / travelTime_);
	scale_ = MLEngine::Math::Lerp(startScale_, endScale_, elapsedTime_ / travelTime_);
	model_.worldMatrix = MLEngine::Math::MakeAffineMatrix(scale_, { 0.0f, 0.0f, 0.0f, 1.0f }, position_);

	if (elapsedTime_ > travelTime_)
	{
		isActive_ = false;
	}
	else {
		elapsedTime_ += 1.0f / 60.0f;
	}

	

	/*position_.x += velocity_.x;
	position_.y += velocity_.y;

	float t = position_.y / bottomLine_;
	float scale = t * multiplier_;

	elapsedTime_ += 1.0f / 60.0f;
		
	sprite_->size_ = { minSize_ * scale, minSize_ * scale };
	sprite_->SetPosition(position_);

	if (position_.y >= bottomLine_ )
	{
		isActive_ = false;
	}*/
}

void Bullet::Draw(MLEngine::Object::Camera* camera)
{
	
}

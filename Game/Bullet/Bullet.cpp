#include "Bullet.h"

#include "Resource/Texture/Texture.h"

void Bullet::Initialize()
{
	/*model_.Initialize("./Resources/model/plane/plane.obj");
	model_.worldMatrix = MLEngine::Math::MakeAffineMatrix(scale_, { 0.0f, 0.0f, 0.0f, 1.0f }, position_);*/
	sprite3D_.Initialize("./Resources/Texture/enemy1_hand_attack.png", 1);
	sprite3D_.transform.scale = scale_;
	sprite3D_.transform.translate = position_;
}

void Bullet::Update()
{

	position_ = MLEngine::Math::Lerp(startPosition_, targetPosition_, elapsedTime_ / travelTime_);
	scale_ = MLEngine::Math::Lerp(startScale_, endScale_, elapsedTime_ / travelTime_);
	//model_.worldMatrix = MLEngine::Math::MakeAffineMatrix(scale_, { 0.0f, 0.0f, 0.0f, 1.0f }, position_);
	sprite3D_.transform.scale = scale_;
	sprite3D_.transform.translate = position_;
	sprite3D_.transform.UpdateMatrix();

	if (elapsedTime_ > travelTime_)
	{
		isActive_ = false;
	}
	else {
		elapsedTime_ += 1.0f / 60.0f;
	}
}

void Bullet::Draw(MLEngine::Object::Camera* camera)
{
	
}

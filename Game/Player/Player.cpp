#include "Player.h"
#include"Externals/imgui/imgui.h"

using namespace MLEngine::Math;

Player::Player(){
	//必須となる情報の読み込み
	texture_.Load("./Resources/white.png");

	sprite_ = MLEngine::Resource::Sprite::Create(texture_, MLEngine::Math::Vector2(pos_.x, pos_.y), color_);
}

Player::~Player(){

}

void Player::Initialize(){
	life_ = lifeMax_;
	pos_ = Vector3(640.0f, 650.0f, 0.0f);
	color_ = Vector4(1.0f, 1.0f, 0.0f, 1.0f);
}

void Player::Finalize(){

}

void Player::Update(float deltaTime){
	DebugDraw();

	sprite_->position_ = Vector2(pos_.x, pos_.y);
	sprite_->color_ = color_;
	sprite_->size_ = Vector2(128.0f, 128.0f);
}

void Player::Draw(){
	sprite_->Draw();
}

void Player::DebugDraw(){
#ifdef _DEBUG
	ImGui::Begin("プレイヤー");
	ImGui::DragFloat2("座標", &pos_.x, 1.0f);
	ImGui::End();
#endif // _DEBUG

}

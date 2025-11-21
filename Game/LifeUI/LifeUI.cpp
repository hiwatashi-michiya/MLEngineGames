#include "LifeUI.h"

LifeUI::LifeUI(Player* player){
	player_ = player;

	//必須となる情報の読み込み
	texture_.Load("./Resources/white.png");

	sprite_.reset(MLEngine::Resource::Sprite::Create(texture_,plPos_, plColor_));
}

void LifeUI::Initialize() {

}


void LifeUI::Update() {

}


void LifeUI::Draw() {

}

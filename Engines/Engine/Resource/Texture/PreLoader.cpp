#include "PreLoader.h"

using namespace MLEngine::Resource;

PreLoader* PreLoader::GetInstance() {

	static PreLoader instance;
	return &instance;

}

void PreLoader::Initialize() {

	Texture tex;
	tex.Load("./Resources/white.png");

	sprite_.reset(Sprite2D::Create(tex, {200.0f,200.0f}));

}

void PreLoader::Update()
{
	isEndLoad_ = true;
	/*if (not textures_.empty()) {
		isEndLoad_ = false;
		sprite_->SetTexture(textures_[0]);
		textures_.erase(textures_.begin());
		sprite_->isActive = true;
	}
	else {
		isEndLoad_ = true;
		sprite_->isActive = false;
	}*/

}

void PreLoader::AddTexture(MLEngine::Resource::Texture tex)
{

	//読み込み済のテクスチャを追加しない
	if (texMap_.find(tex.GetIndex()) != texMap_.end()) {
		return;
	}

	texMap_[tex.GetIndex()] = tex;
	textures_.push_back(tex);

}
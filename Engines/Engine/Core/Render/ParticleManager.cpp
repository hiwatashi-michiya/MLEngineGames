#include "ParticleManager.h"

using namespace MLEngine::Core::Render::Particle;
using namespace MLEngine::Object;

Manager* Manager::GetInstance() {
	static Manager instance;
	return &instance;
}

void Manager::Initialize() {

	for (int32_t i = 0; i < kMaxParticle_; i++) {

		if (not particles_[i]) {
			particles_[i] = std::make_unique<MLEngine::Resource::Particle>();
			particles_[i]->Initialize();
		}
		particles_[i]->SetDefault();
		isUsed_[i] = false;
	}

}

void Manager::Finalize()
{

	for (int32_t i = 0; i < kMaxParticle_; i++) {

		if (particles_[i]) {
			particles_[i].reset();
		}
	}

}

void Manager::Update() {
	
	//パーティクル更新
	for (int32_t i = 0; i < kMaxParticle_; i++) {

		particles_[i]->Update();
		//パーティクルが終了した時に使用フラグを降ろす
		if (particles_[i]->IsEnd()) {
			isUsed_[i] = false;
		}

	}

}

void Manager::Draw(Camera* camera) {

	for (int32_t i = 0; i < kMaxParticle_; i++) {

		if (isUsed_[i]) {
			particles_[i]->Draw(camera);
		}

	}

}

void Manager::Reset()
{

	for (int32_t i = 0; i < kMaxParticle_; i++) {

		particles_[i]->SetDefault();
		isUsed_[i] = false;

	}

}

bool Manager::GetUnUsedParticle(MLEngine::Resource::Particle*& particle)
{

	//全てのパーティクルから検索し、未使用のパーティクルのポインタを渡す
	for (int32_t i = 0; i < kMaxParticle_; i++) {

		if (not isUsed_[i]) {
			particle = particles_[i].get();
			isUsed_[i] = true;
			//取得出来たらtrueを返す
			return true;
		}

	}

	//取得できなかったのでnullptrを渡す
	particle = nullptr;
	//取得できなかったらfalseを返す
	return false;
}

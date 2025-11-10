#include "ResourceManager.h"
#include "Model/InstancingModel.h"
#include "SceneManager.h"

using namespace MLEngine::Resource;

Manager* Manager::GetInstance() {
	static Manager instance;
	return &instance;
}

void Manager::Initialize() {



}

void Manager::Update() {

	camera_ = MLEngine::Scene::Manager::GetInstance()->GetMainCamera();

	//モデルの描画
	for (RigidModel* model : rigidModels_) {

		//モデルがアクティブなら
		if (model->isActive) {
			//ワールドビュープロジェクション更新
			model->worldViewProjectionMatrix = model->localMatrix * model->worldMatrix * camera_->matViewProjection_;
			//モデルのデータを追加
			model->Regist();
		}

	}

	//スプライト描画
	for (Sprite* sprite : sprites_) {

		if (sprite->isActive) {

			sprite->Draw();

		}

	}

	//パーティクル描画
	for (Particle3D* particle : particle3ds_) {

		if (particle->isActive) {

			particle->Draw(camera_);

		}

	}

	//ライン描画
	for (Line* line : lines_) {

		if (line->isActive) {
			line->Draw(camera_);
		}

	}

}

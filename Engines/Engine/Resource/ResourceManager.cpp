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

		//モデルを表示するなら
		if (model->isActive) {
			//ワールドビュープロジェクション更新
			model->worldViewProjectionMatrix = model->localMatrix * model->worldMatrix * camera_->matViewProjection_;
			//モデルのデータを追加
			model->Regist();
		}

	}

	for (Sprite3D* sprite3d : sprite3Ds_) {

		if (sprite3d->isActive) {
			//ワールドビュープロジェクション更新
			sprite3d->worldViewProjectionMatrix = sprite3d->localMatrix * sprite3d->transform.worldMatrix * camera_->matViewProjection_;
			//モデルのデータを追加
			sprite3d->Regist();
		}

	}

	//スプライト描画
	for (Sprite2D* sprite : sprite2Ds_) {

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

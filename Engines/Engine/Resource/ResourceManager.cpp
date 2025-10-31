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
		if (model->isActive_) {
			//ワールドビュープロジェクション更新
			model->worldViewProjectionMatrix_ = model->localMatrix_ * model->worldMatrix_ * camera_->matViewProjection_;
			//モデルのデータを追加
			model->GetInstancingModel()->AddModelData(model);
		}

	}

	//スプライト更新
	for (Sprite* sprite : sprites_) {

		if (sprite->isActive_) {



		}

	}

}

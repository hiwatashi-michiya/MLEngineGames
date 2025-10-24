#include "SceneManager.h"
#include <cassert>
#include "Core/DirectXSetter.h"
#include "Model/Model.h"
#include "Particle/Particle3D.h"
#include "Sprite/Sprite.h"
#include "Core/Render/PostEffect/PostEffectDrawer.h"
#include "Model/Skinning/SkinningModel.h"
#include "Line/LineDrawer.h"
#include "Core/Render/RenderManager.h"
#include "Core/Render/ParticleManager.h"
#include "BaseScene.h"

using namespace MLEngine::Core::Render;
using namespace MLEngine::Core;
using namespace MLEngine;

Scene::Manager* Scene::Manager::GetInstance() {
	static Scene::Manager instance;
	return &instance;
}

void Scene::Manager::Update() {

	if (nextScene_) {

		//旧シーンの終了
		if (scene_) {
			//全パーティクルリセット
			Particle::Manager::GetInstance()->Reset();
			scene_->Finalize();
			scene_.reset();
		}

		//シーン切り替え
		scene_.reset(nextScene_.release());

		scene_->SetSceneManager(this);
		scene_->SetCamera();
		scene_->Initialize();

	}
	//シーン更新
	scene_->Update();

}

void Scene::Manager::Finalize()
{

	scene_->Finalize();

}

void Scene::Manager::Draw() {

	scene_->Draw();

}

void Scene::Manager::Render()
{

	Render::Manager::GetInstance()->Render();

}

void Scene::Manager::ChangeScene(BaseScene* nextScene) {

	assert(nextScene_ == nullptr);

	nextScene_.reset(nextScene);

}

void MLEngine::Scene::Manager::ChangeScene(const std::string& sceneName)
{

	assert(sceneFactory_);

	assert(nextScene_ == nullptr);

	nextScene_.reset(sceneFactory_->CreateScene(sceneName));

}

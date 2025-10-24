#include "MLEngine.h"
#pragma comment(lib, "winmm.lib")

#ifdef _DEBUG

#include "ImGuiManager.h"

#endif // _DEBUG

using namespace MLEngine;
using namespace MLEngine::Resource;
using namespace MLEngine::Core;

//template<class BaseScene>
//void MLEngine::Run(const char* title) {
//
//	//エンジンの生成
//	Engine* engine = new Engine();
//
//	engine->Initialize(title, 1280, 720);
//
//	engine->Run(new BaseScene());
//
//	engine->Finalize();
//
//	//エンジンの開放
//	delete engine;
//
//}

void Engine::Initialize(const char* title, int width, int height) {

	HRESULT hr;

	hr = CoInitializeEx(0, COINIT_MULTITHREADED);

	assert(SUCCEEDED(hr));

	//システムタイマーの分解能を上げる
	timeBeginPeriod(1);

	//乱数生成
	MLEngine::Math::SetRandom();

	//ウィンドウ作成
	auto&& titleString = ConvertString(title);
	windowManager_ = Core::Window::Manager::GetInstance();
	windowManager_->CreateGameWindow(
		titleString.c_str(), width, height);

	//初期化
	//インスタンス取得
	//ここから諸々の初期化処理
	dxSetter_ = Core::DirectXSetter::GetInstance();
	dxSetter_->Initialize(windowManager_, width, height);

#ifdef _DEBUG

	ImGuiManager::GetInstance()->Initialize();

#endif // _DEBUG

	textureManager_ = Core::TextureManager::GetInstance();
	shaderManager_ = Core::Render::Shader::Manager::GetInstance();
	pipelineManager_ = Core::Render::Pipeline::Manager::GetInstance();
	rootSignatureManager_ = Core::Render::RootSignature::Manager::GetInstance();
	modelManager_ = Core::Render::Model::Manager::GetInstance();
	collisionManager_ = CollisionManager::GetInstance();

	textureManager_->Initialize(dxSetter_->GetSrvHeap()->Get());
	shaderManager_->Initialize();
	pipelineManager_->Initialize(dxSetter_->GetDevice());
	rootSignatureManager_->Initialize(dxSetter_->GetDevice());

	AudioManager::GetInstance()->Initialize();
	Sprite::StaticInitialize(dxSetter_->GetDevice());
	modelManager_->Initialize();
	Graphics::Mesh::StaticInitialize(dxSetter_->GetDevice());
	Graphics::Material::StaticInitialize(dxSetter_->GetDevice());
	SkinningModel::StaticInitialize(dxSetter_->GetDevice());
	Skybox::Initialize();
	Particle3D::StaticInitialize(dxSetter_->GetDevice());
	Line::Initialize(dxSetter_->GetDevice());
	Render::PostEffect::PostEffectDrawer::GetInstance()->Initialize();

	Input::Manager::GetInstance()->Initialize();
	VirtualController::GetInstance().Initialize();

	//Engineクラスでインスタンス生成をしておく
	collisionManager_->Initialize();
	Render::Particle::Manager::GetInstance()->Initialize();
	Render::Manager::GetInstance()->Clear();

	sceneManager_ = Scene::Manager::GetInstance();

}

void Engine::Run(BaseScene* startScene, BaseSceneFactory* sceneFactory) {
	

	sceneManager_->SetSceneFactory(sceneFactory);
	sceneManager_->ChangeScene(startScene);

	while (true)
	{

		//フレーム開始
		BeginFrame();

		//ゲームシーン更新
		sceneManager_->Update();
		//当たり判定チェック
		collisionManager_->CheckAllCollisions();

		//エスケープキーが押されるか、ウィンドウのxボタンが押されたら終了する
		if (Input::Manager::GetInstance()->GetKeyboard()->Trigger(DIK_ESCAPE) or ProcessMessage() != 0) {
			sceneManager_->Finalize();
			break;
		}

		//ゲームシーン描画
		sceneManager_->Draw();
		sceneManager_->Render();

		//フレーム終了
		EndFrame();

	}

}

int Engine::ProcessMessage() {
	return windowManager_->ProcessMessage();
}

void Engine::Finalize() {

	textureManager_->Finalize();

#ifdef _DEBUG

	ImGuiManager::GetInstance()->Finalize();

#endif // _DEBUG

	Particle3D::Finalize();
	Render::Particle::Manager::GetInstance()->Finalize();
	AudioManager::GetInstance()->Finalize();
	SkinningModel::Finalize();
	Sprite::Finalize();
	dxSetter_->Finalize();
	dxSetter_ = nullptr;
	CoUninitialize();
	//ウィンドウの破棄
	windowManager_->TerminateGameWindow();
}

void Engine::BeginFrame() {

#ifdef _DEBUG

	ImGuiManager::GetInstance()->BeginFrame();

#endif // _DEBUG

	Input::Manager::GetInstance()->Update();

}

void Engine::EndFrame() {

	dxSetter_->Execute();

}

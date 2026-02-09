#include "MLEngine.h"

#ifdef _DEBUG

#include "ImGuiManager.h"

#endif // _DEBUG

using namespace MLEngine;
using namespace MLEngine::Resource;
using namespace MLEngine::Core;

void Engine::Initialize(const char* title, int width, int height) {

	HRESULT hr;

	hr = CoInitializeEx(0, COINIT_MULTITHREADED);

	assert(SUCCEEDED(hr));

	//乱数生成
	MLEngine::Math::SetRandom();

	//ウィンドウ作成
	auto&& titleString = ConvertString(title);
	windowManager_ = Core::Window::Manager::GetInstance();
	windowManager_->CreateGameWindow(
		titleString.c_str(), width, height);

	//FPS計測クラス初期化
	FrameTracker::GetInstance()->Initialize();

	//デバイス初期化
	device_ = DXDevice::GetInstance();
	device_->Initialize();

	//初期化
	//インスタンス取得
	//ここから諸々の初期化処理
	dxSetter_ = Core::DirectXSetter::GetInstance();
	dxSetter_->Initialize(windowManager_, width, height);

#ifdef _DEBUG

	ImGuiManager::GetInstance()->Initialize();
	
#endif // _DEBUG
	GlobalVariables::GetInstance()->LoadFiles();

	textureManager_ = Core::TextureManager::GetInstance();
	shaderManager_ = Core::Render::Shader::Manager::GetInstance();
	pipelineManager_ = Core::Render::Pipeline::Manager::GetInstance();
	rootSignatureManager_ = Core::Render::RootSignature::Manager::GetInstance();
	modelManager_ = Core::Render::Model::Manager::GetInstance();
	collisionManager_ = CollisionManager::GetInstance();

	textureManager_->Initialize(dxSetter_->GetSrvHeap()->Get());
	shaderManager_->Initialize();
	pipelineManager_->Initialize(device_->GetDevice());
	rootSignatureManager_->Initialize(device_->GetDevice());

	AudioManager::GetInstance()->Initialize();
	Sprite2D::StaticInitialize(device_->GetDevice());
	modelManager_->Initialize();
	Graphics::Mesh::StaticInitialize(device_->GetDevice());
	Graphics::Material::StaticInitialize(device_->GetDevice());
	SkinningModel::StaticInitialize(device_->GetDevice());
	Skybox::Initialize();
	Particle3D::StaticInitialize(device_->GetDevice());
	Line::Initialize(device_->GetDevice());
	Render::PostEffect::PostEffectDrawer::GetInstance()->Initialize();

	Input::Manager::GetInstance()->Initialize();
	VirtualController::GetInstance().Initialize();

	//Engineクラスでインスタンス生成をしておく
	collisionManager_->Initialize();
	Render::Particle::Manager::GetInstance()->Initialize();
	Render::Manager::GetInstance()->Initialize();

	resourceManager_ = Resource::Manager::GetInstance();
	resourceManager_->Initialize();
	sceneManager_ = Scene::Manager::GetInstance();

#ifdef _DEBUG
	
#else
	//windowManager_->SetFullScreenMode();
#endif 


}

void Engine::Run(BaseScene* startScene, BaseSceneFactory* sceneFactory) {
	

	sceneManager_->SetSceneFactory(sceneFactory);
	sceneManager_->ChangeScene(startScene);

	while (true)
	{

		//フレーム開始
		BeginFrame();

#ifdef _DEBUG
		GlobalVariables::GetInstance()->Update();
#endif //DEBUG
		//ゲームシーン更新
		sceneManager_->Update();

		//当たり判定チェック
		collisionManager_->CheckAllCollisions();

		//エスケープキーが押されるか、ウィンドウのxボタンが押されたら終了する
		if (Input::Manager::GetInstance()->GetKeyboard()->Trigger(DIK_ESCAPE) or ProcessMessage() != 0) {
			sceneManager_->Finalize();
			break;
		}

		//altキー押しながらEnterでフルスクリーン切り替え
		if (Input::Manager::GetInstance()->GetKeyboard()->Push(DIK_LALT) and
			Input::Manager::GetInstance()->GetKeyboard()->Trigger(DIK_RETURN)) {

			if (windowManager_->IsFullScreen()) {
				windowManager_->SetWindowMode();
			}
			else {
				windowManager_->SetFullScreenMode();
			}

		}

		//ゲームシーン描画
		resourceManager_->Update();
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
	Sprite2D::Finalize();
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
	FrameTracker::GetInstance()->Update();

}

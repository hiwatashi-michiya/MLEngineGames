#pragma once
#include "Sprite/Sprite.h"
#include "Model/RigidModel.h"
#include "Particle/Particle3D.h"
#include "AudioManager.h"
#include "Core/DirectXSetter.h"
#include "Core/SafeDelete.h"
#include "Core/WindowManager.h"
#include "Convert.h"
#include "Line/LineDrawer.h"
#include "PipelineManager.h"
#include "PostEffect/PostEffectDrawer.h"
#include "RootSignatureManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "Input.h"
#include "Rand.h"
#include "Model/Skinning/SkinningModel.h"
#include "Tool/GlobalVariables.h"
#include <cassert>
#include <string>
#include <Windows.h>
#include "CollisionManager.h"
#include "Model/Skybox.h"
#include "ParticleManager.h"
#include "RenderManager.h"
#include "ModelManager.h"
#include "Scene/SceneManager.h"
#include <memory>
#include "Resource/ResourceManager.h"

namespace MLEngine {

	/// <summary>
	/// ゲームエンジン
	/// </summary>
	class Engine
	{
	public:

		/// <summary>
		/// 初期化処理
		/// </summary>
		/// <param name="title">ウィンドウタイトル</param>
		/// <param name="width">ウィンドウ横幅</param>
		/// <param name="height">ウィンドウ縦幅</param>
		void Initialize(const char* title, int width, int height);

		/// <summary>
		/// ループ開始(一度のみ)
		/// </summary>
		void Run(BaseScene* startScene, BaseSceneFactory* sceneFactory);

		/// <summary>
		/// ウィンドウの×ボタンが押されたかどうか
		/// </summary>
		/// <returns></returns>
		int ProcessMessage();

		/// <summary>
		/// 全体終了
		/// </summary>
		void Finalize();

		/// <summary>
		/// フレームの開始
		/// </summary>
		void BeginFrame();

		/// <summary>
		/// フレームの終了
		/// </summary>
		void EndFrame();

	private:

		MLEngine::Core::Window::Manager* windowManager_ = nullptr;
		MLEngine::Core::DirectXSetter* dxSetter_ = nullptr;
		MLEngine::Core::Render::Manager* renderManager_ = nullptr;
		MLEngine::Core::Render::Model::Manager* modelManager_ = nullptr;
		MLEngine::Core::TextureManager* textureManager_ = nullptr;
		MLEngine::Core::Render::Shader::Manager* shaderManager_ = nullptr;
		MLEngine::Core::Render::Pipeline::Manager* pipelineManager_ = nullptr;
		MLEngine::Core::Render::RootSignature::Manager* rootSignatureManager_ = nullptr;
		MLEngine::Scene::Manager* sceneManager_ = nullptr;
		MLEngine::Core::CollisionManager* collisionManager_ = nullptr;
		MLEngine::Resource::Manager* resourceManager_ = nullptr;

	};

	//開始時の処理(一度だけ)
	template<class BaseScene, class Factory>
	void Run(const char* title) {

		//エンジンの生成
		Engine* engine = new Engine();
		//シーンファクトリー生成
		std::unique_ptr<BaseSceneFactory> factory = std::make_unique<Factory>();

		engine->Initialize(title, 1280, 720);

		engine->Run(new BaseScene(), factory.get());

		engine->Finalize();

		//エンジンの開放
		delete engine;

	}

}


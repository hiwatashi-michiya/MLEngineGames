#pragma once
#include "Model/RigidModel.h"
#include "Model/Skinning/SkinningModel.h"
#include "Particle/Particle3D.h"
#include "Sprite/Sprite.h"
#include "Line/LineDrawer.h"
#include <vector>
#include "Lighting/DirectionalLight.h"
#include "Camera.h"

namespace MLEngine::Core::Render {

	/// <summary>
	/// 描画コマンドを集めて実際の描画を行うクラス
	/// </summary>
	class Manager
	{
	public:

		static Manager* GetInstance();
		//全てを一旦クリアする
		void Clear();
		//描画
		void Render();
		//モデルの追加
		void AddModel(MLEngine::Resource::RigidModel* model) { models_.push_back(model); }
		//スキニングモデルの追加
		void AddSkinningModel(MLEngine::Resource::SkinningModel* skinningModel) { skinningModels_.push_back(skinningModel); }
		//パーティクルの追加
		void AddParticle3D(MLEngine::Resource::Particle3D* particle) { particles_.push_back(particle); }
		//スプライトの追加
		void AddSprite(MLEngine::Resource::Sprite* sprite) { sprites_.push_back(sprite); }
		//ラインの追加
		void AddLine(MLEngine::Resource::Line* line) { lines_.push_back(line); }
		//平行光源ポインタセット
		void SetLight(MLEngine::Lighting::DirectionalLight* light) { dLight_ = light; }
		//カメラポインタセット
		void SetCamera(MLEngine::Object::Camera* camera) { camera_ = camera; }

	private:

		//描画する対象のポインタを集めたリスト
		std::vector<MLEngine::Resource::RigidModel*> models_;
		std::vector<MLEngine::Resource::SkinningModel*> skinningModels_;
		std::vector<MLEngine::Resource::Particle3D*> particles_;
		std::vector<MLEngine::Resource::Sprite*> sprites_;
		std::vector<MLEngine::Resource::Line*> lines_;
		//平行光源ポインタ
		MLEngine::Lighting::DirectionalLight* dLight_;
		//カメラポインタ
		MLEngine::Object::Camera* camera_;

	private:
		//シングルトン化
		Manager() = default;
		~Manager() = default;
		Manager(const Manager&) = delete;
		const Manager& operator=(const Manager&) = delete;

	};

}


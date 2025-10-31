#pragma once
#include <list>
#include "Model/RigidModel.h"
#include "Sprite/Sprite.h"
#include "Camera.h"

namespace MLEngine::Resource {

	/// <summary>
	/// リソース管理マネージャー
	/// </summary>
	class Manager
	{
	public:
		//インスタンス取得
		static Manager* GetInstance();
		//初期化
		void Initialize();
		//更新
		void Update();
		//カメラセット
		void SetCamera(MLEngine::Object::Camera* camera) { camera_ = camera; }
		//剛体モデル追加
		void AddRigidModel(RigidModel* model) { rigidModels_.push_back(model); }
		//剛体モデル削除(重くなったらイテレータを利用してeraseに書き換える)
		void RemoveRigidModel(RigidModel* model) { rigidModels_.remove(model); }
		//スプライト追加
		void AddSprite(Sprite* sprite) { sprites_.push_back(sprite); }
		//スプライト追加
		void RemoveSprite(Sprite* sprite) { sprites_.remove(sprite); }

	private:
		//モデルのポインタリスト
		std::list<RigidModel*> rigidModels_;
		//スプライトのポインタリスト
		std::list<Sprite*> sprites_;
		//メインカメラのポインタ
		MLEngine::Object::Camera* camera_;

	private:

		//シングルトン化
		Manager() = default;
		~Manager() = default;
		Manager(const Manager&) = delete;
		const Manager& operator=(const Manager&) = delete;

	};

}

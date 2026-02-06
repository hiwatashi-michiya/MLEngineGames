#pragma once
#include <list>
#include "Model/RigidModel.h"
#include "Sprite/Sprite2D.h"
#include "Sprite/Sprite3D.h"
#include "Particle/Particle3D.h"
#include "Line/LineDrawer.h"
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
		void AddSprite2D(Sprite2D* sprite) { 
			sprite2Ds_.push_back(sprite);
			SortSprite2D();
		}
		//スプライト削除
		void RemoveSprite2D(Sprite2D* sprite) { 
			sprite2Ds_.remove(sprite);
			SortSprite2D();
		}
		//スプライト追加
		void AddSprite3D(Sprite3D* sprite) { sprite3Ds_.push_back(sprite); }
		//スプライト削除
		void RemoveSprite3D(Sprite3D* sprite) { sprite3Ds_.remove(sprite); }
		//スプライト追加
		void AddParticle3D(Particle3D* particle) { particle3ds_.push_back(particle); }
		//スプライト削除
		void RemoveParticle3D(Particle3D* particle) { particle3ds_.remove(particle); }
		//ライン追加
		void AddLine(Line* line) { lines_.push_back(line); }
		//ライン削除
		void RemoveLine(Line* line) { lines_.remove(line); }
		//スプライト2Dのソート
		void SortSprite2D();

	private:
		//モデルのポインタリスト
		std::list<RigidModel*> rigidModels_;
		//スプライト2Dのポインタリスト
		std::list<Sprite2D*> sprite2Ds_;
		//スプライト3Dのポインタリスト
		std::list<Sprite3D*> sprite3Ds_;
		//パーティクルのポインタリスト
		std::list<Particle3D*> particle3ds_;
		//ラインポインタのリスト
		std::list<Line*> lines_;
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

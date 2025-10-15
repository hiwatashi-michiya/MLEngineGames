#pragma once
#include "Object/Collision/Collider.h"
#include <list>
#include <mutex>

namespace MLEngine::Core {

	/// <summary>
	/// コライダーの当たり判定を管理するクラス
	/// </summary>
	class CollisionManager {
	public:
		//インスタンス取得
		static CollisionManager* GetInstance();
		//初期化
		void Initialize() { colliders_.clear(); }

		//コライダー追加
		void PushCollider(MLEngine::Object::Collision::Collider* collider);

		//コライダー削除
		void PopCollider(MLEngine::Object::Collision::Collider* collider);
		//リストクリア
		void ClearList();
		//全当たり判定チェック
		void CheckAllCollisions();

	private:

		// コライダーリスト
		std::list<MLEngine::Object::Collision::Collider*> colliders_;

		std::mutex collidersMutex_;

		/// <summary>
		/// コライダーの2つの衝突判定と応答
		/// </summary>
		/// <param name="colliderA">コライダーA</param>
		/// <param name="colliderB">コライダーB</param>
		void CheckCollisionPair(MLEngine::Object::Collision::Collider* colliderA, MLEngine::Object::Collision::Collider* colliderB);

	private:
		//シングルトン化
		CollisionManager() = default;
		~CollisionManager() = default;
		CollisionManager(const CollisionManager&) = delete;
		const CollisionManager& operator=(const CollisionManager&) = delete;

	};


}

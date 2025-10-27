#include "CollisionManager.h"
#include <Windows.h>
#include <cassert>

using namespace MLEngine::Object::Collision;
using namespace MLEngine::Core;

CollisionManager* CollisionManager::GetInstance() {
	static CollisionManager instance;
	return &instance;
}

void CollisionManager::Initialize() {
	
	colliders_.clear();

	isUsed_.resize(kMaxCount_);

}

void CollisionManager::PushCollider(Collider* collider) {
	std::lock_guard<std::mutex> lock(collidersMutex_);
	colliders_.push_back(collider);
}

void CollisionManager::PopCollider(Collider* collider) {

	std::lock_guard<std::mutex> lock(collidersMutex_);

	auto it = std::find(colliders_.begin(), colliders_.end(), collider);

	if (it != colliders_.end()) {
		colliders_.erase(it);
	}

}

void CollisionManager::ClearList() { colliders_.clear(); }

void CollisionManager::CheckAllCollisions() {

	// リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {

		// イテレータAからコライダーAを取得
		Collider* colliderA = *itrA;

		//非アクティブ状態ならスキップ
		if (not colliderA->GetIsActive()) {
			continue;
		}

		// イテレータBはイテレータAの次の要素から回す(重複判定回避)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			// イテレータBからコライダーBを取得
			Collider* colliderB = *itrB;

			//非アクティブ状態ならスキップ
			if (not colliderB->GetIsActive()) {
				continue;
			}

			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {

	// 衝突フィルタリング
	if ((colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) == 0 or
	    (colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()) == 0) {
		return;
	}

	// 交差判定
	if (colliderA->CollideWith(colliderB)) {
		// コライダーAの衝突時コールバックを呼び出す
		colliderA->Hit(colliderB);
		// コライダーBの衝突時コールバックを呼び出す
		colliderB->Hit(colliderA);
		return;
	}

	// コライダーAの衝突していない時のコールバックを呼び出す
	colliderA->NoHit(colliderB);
	// コライダーBの衝突していない時のコールバックを呼び出す
	colliderB->NoHit(colliderA);

}

uint32_t CollisionManager::GetUnUsedIndex() {

	for (uint32_t i = 0; i < kMaxCount_; i++) {

		//使用されていない場合、そのインデックスを返す
		if (not isUsed_[i]) {
			isUsed_[i] = true;
			return i;
		}

	}

	//使用制限を超えているのでassert
	assert(false);

	return 0;

}

void CollisionManager::SetIndexUnUsed(uint32_t index)
{

	assert(index < kMaxCount_);

	if (isUsed_[index]) {
		isUsed_[index] = false;
	}

}

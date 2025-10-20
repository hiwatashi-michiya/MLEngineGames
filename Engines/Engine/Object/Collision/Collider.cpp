#include "Collider.h"
#include "CollisionManager.h"
#include "ImguiManager.h"

using namespace MLEngine::Object::Collision;
using namespace MLEngine::Core;

Collider::Collider() { 
	id_ = CollisionManager::GetInstance()->GetUnUsedIndex();
	CollisionManager::GetInstance()->PushCollider(this);
}

Collider::~Collider() { 
	CollisionManager::GetInstance()->SetIndexUnUsed(id_);
	CollisionManager::GetInstance()->PopCollider(this);
}

void MLEngine::Object::Collision::Collider::Hit(Collider* collider)
{

	if (idMap_[collider->id_] <= 0) {

		if (enterFunc_) {
			enterFunc_(collider);
		}

	}
	else {

		if (stayFunc_) {
			stayFunc_(collider);
		}

	}

	idMap_[collider->id_]++;

}

void MLEngine::Object::Collision::Collider::NoHit(Collider* collider)
{

	if (idMap_[collider->id_] > 0) {

		if (exitFunc_) {
			exitFunc_(collider);
		}

	}

	idMap_[collider->id_] = 0;

}

bool BoxCollider::CollideWith(Collider* other) { return other->CollideWithBox(this); }
bool BoxCollider::CollideWithBox(BoxCollider* box) { return IsCollision(this->collider_, box->collider_); }
bool BoxCollider::CollideWithSphere(SphereCollider* sphere) { return IsCollision(this->collider_, sphere->collider_); }

void BoxCollider::Debug()
{

	ImGui::DragFloat3("center", &collider_.center.x);
	ImGui::DragFloat3("size", &collider_.size.x);

}

void SphereCollider::Debug()
{

	ImGui::DragFloat3("center", &collider_.center.x);
	ImGui::DragFloat("size", &collider_.radius);

}

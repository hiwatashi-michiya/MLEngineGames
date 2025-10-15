#include "Collider.h"
#include "CollisionManager.h"

using namespace MLEngine::Object::Collision;
using namespace MLEngine::Core;

Collider::Collider() { CollisionManager::GetInstance()->PushCollider(this); }

Collider::~Collider() { CollisionManager::GetInstance()->PopCollider(this); }

bool BoxCollider::CollideWith(Collider* other) { return other->CollideWithBox(this); }
bool BoxCollider::CollideWithBox(BoxCollider* box) { return IsCollision(this->collider_, box->collider_); }
bool BoxCollider::CollideWithSphere(SphereCollider* sphere) { return IsCollision(this->collider_, sphere->collider_); }

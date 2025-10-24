#pragma once
#include "Vector3.h"

namespace MLEngine::Object::Collision {

	//球と球の当たり判定
	bool IsCollision(const MLEngine::Math::Sphere& s1, const MLEngine::Math::Sphere& s2);

	//球と平面の当たり判定
	bool IsCollision(const MLEngine::Math::Sphere& sphere, const MLEngine::Math::Plane& plane);

	//線分と平面の当たり判定
	bool IsCollision(const MLEngine::Math::Segment& segment, const MLEngine::Math::Plane& plane);

	//線分と三角形の当たり判定
	bool IsCollision(const MLEngine::Math::Segment& segment, const MLEngine::Math::Triangle& triangle);

	//AABBの当たり判定
	bool IsCollision(const MLEngine::Math::AABB& aabb1, const MLEngine::Math::AABB& aabb2);

	//球とAABBの当たり判定
	bool IsCollision(const MLEngine::Math::AABB& aabb, const MLEngine::Math::Sphere& sphere);

	//AABBと線分の当たり判定
	bool IsCollision(const MLEngine::Math::AABB& aabb, const MLEngine::Math::Segment& segment);

	//OBBと球の当たり判定
	bool IsCollision(const MLEngine::Math::OBB& obb, const MLEngine::Math::Sphere& sphere);
	bool IsCollision(const MLEngine::Math::Sphere& sphere, const MLEngine::Math::OBB& obb);

	//OBBと線分の当たり判定
	bool IsCollision(const MLEngine::Math::OBB& obb, const MLEngine::Math::Segment& segment);
	bool IsCollision(const MLEngine::Math::Segment& segment, const MLEngine::Math::OBB& obb);

	//OBB同士の当たり判定
	bool IsCollision(const MLEngine::Math::OBB& obb1, const MLEngine::Math::OBB& obb2);

	//OBBと点の当たり判定
	bool IsCollision(const MLEngine::Math::OBB& obb, const MLEngine::Math::Vector3& point);

	//OBBが完全にもう一方のOBBに内包しているか
	bool IsWrapped(const MLEngine::Math::OBB& outSide, const MLEngine::Math::OBB& inside);

}


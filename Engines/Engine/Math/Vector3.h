#pragma once

namespace MLEngine::Math {

	/// <summary>
	/// 3次元ベクトル
	/// </summary>
	class Vector3 {
	public:

		float x;
		float y;
		float z;

		//全ての要素を0.0fで返す
		static const Vector3 Zero() { return Vector3{ 0.0f,0.0f,0.0f }; }

		//全ての要素を1.0fで返す
		static const Vector3 Identity() { return Vector3{ 1.0f,1.0f,1.0f }; }

		//X軸
		static const Vector3 AxisX() { return Vector3{ 1.0f,0.0f,0.0f }; }
		//Y軸
		static const Vector3 AxisY() { return Vector3{ 0.0f,1.0f,0.0f }; }
		//Z軸
		static const Vector3 AxisZ() { return Vector3{ 0.0f,0.0f,1.0f }; }

		Vector3& operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
		Vector3& operator-=(const Vector3& v) { x -= v.x;  y -= v.y; z -= v.z; return *this; }
		Vector3& operator+=(const Vector3& v) { x += v.x;  y += v.y; z += v.z; return *this; }
		Vector3& operator/=(float s) { x /= s; y /= s; z /= s; return *this; }

	};
	//パーティクル用のトランスフォーム
	struct TransformForParticle {
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};
	//パーティクルに使う値
	struct ParticleData {
		TransformForParticle transform;
		Vector3 velocity;
	};

	// 直線
	struct StraightLine {
		Vector3 origin; // 始点
		Vector3 diff;   // 終点への差分ベクトル
	};

	// 半直線
	struct Ray {
		Vector3 origin; // 始点
		Vector3 diff;   // 終点への差分ベクトル
	};

	// 線分
	struct Segment {
		Vector3 origin; // 始点
		Vector3 diff;   // 終点への差分ベクトル。始点の座標分マイナスしておく
	};

	// 三角形
	struct Triangle {
		Vector3 vertices[3]; // 頂点
	};

	// AABB(箱)
	struct AABB {
		Vector3 min; // 最小点
		Vector3 max; // 最大点
	};

	// OBB(箱)
	struct OBB {
		// 中心点
		Vector3 center;
		// 座標軸。正規化・直交必須
		Vector3 orientations[3] = {
			{1.0f,0.0f,0.0f},
			{0.0f,1.0f,0.0f},
			{0.0f,0.0f,1.0f}
		};
		// 座標軸方向の長さの半分。中心から面までの距離
		Vector3 size;
	};

	//バネ構造体
	struct Spring {
		//アンカー。固定された端の位置
		Vector3 anchor;
		float naturalLength; //自然長
		float stiffness; //剛性。バネ定数k
		float dampingCoefficient; //減衰係数
	};

	//ボール構造体(3次元)
	struct Ball {
		Vector3 position; //位置
		Vector3 velocity; //速度
		Vector3 acceleration; //加速度
		float mass; //ボールの質量
		float radius; // ボールの半径
		unsigned int color; //色
	};

	//振り子構造体
	struct Pendulum {
		Vector3 anchor; //アンカー。固定された端の位置
		float length; //紐の長さ
		float angle; //現在の角度
		float angularVelocity; //角速度ω
		float angularAcceleration; //角加速度
	};

	//円錐振り子構造体
	struct ConicalPendulum {
		Vector3 anchor; // アンカー。固定された端の位置
		float length; // 紐の長さ
		float halfApexAngle; //円錐の頂角の半分
		float angle; // 現在の角度
		float angularVelocity; // 角速度ω
	};

	//カプセル構造体
	struct Capsule {
		Segment segment;
		float radius;
	};

	struct Vector2;

	class Matrix4x4;

	class Quaternion;
	//加算
	Vector3 Add(const Vector3& v1, const Vector3& v2);
	//減算
	Vector3 Subtract(const Vector3& v1, const Vector3& v2);
	//乗算
	Vector3 Multiply(float scalar, const Vector3& v1);
	//内積
	float Dot(const Vector3& v1, const Vector3& v2);
	//長さ
	float Length(const Vector3& v);
	//正規化
	Vector3 Normalize(const Vector3& v);

	//座標変換
	Vector3 CoordTransform(const Vector3& vector, const Matrix4x4& matrix);

	//スクリーン座標からワールド座標への変換
	Vector3 ScreenToWorld(const Vector2& position, float zValue, const Matrix4x4& matView, const Matrix4x4& matProjection);

	//ベクトル変換(スケールと回転)
	Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

	// クロス積
	Vector3 Cross(const Vector3& v1, const Vector3& v2);

	// 線形補間
	Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

	// 球面線形補間
	Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t);
	//Catmull-Rom直線
	Vector3 CatmullRomPoint(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t);

	//射影
	Vector3 Project(const Vector3& v1, const Vector3& v2);
	//線と点の最近接点
	Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

	// 垂直なベクトルを求める関数
	Vector3 Perpendicular(const Vector3& vector);

	//反射ベクトル
	Vector3 Reflect(const Vector3& input, const Vector3& normal);

	//Z軸中心でベクトルを回転
	Vector3 RotateOnZAxis(const Vector3& vec, float angle);

	//Y軸中心でベクトルを回転
	Vector3 RotateOnYAxis(const Vector3& vec, float angle);
	//AABBを収める
	void ClampAABB(AABB& aabb);

	// OBB回転
	void RotateOBB(OBB& obb, const Vector3& rotate);

	//二項演算子
	Vector3 operator+(const Vector3& v1, const Vector3& v2);
	Vector3 operator-(const Vector3& v1, const Vector3& v2);
	Vector3 operator*(float s, const Vector3& v);
	Vector3 operator*(const Vector3& v, float s);
	Vector3 operator/(const Vector3& v, float s);

	//単項演算子
	Vector3 operator-(const Vector3& v);
	Vector3 operator+(const Vector3& v);

}



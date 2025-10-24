#pragma once
#include "Model/Model.h"
#include "Sprite/Sprite.h"
#include "Audio/Audio.h"
#include"Config/GameConfig.h"
#include "Input/Input.h"
//自機や敵の元となる基底クラス
class BaseCharacter{
public:
	//初期化
	virtual void Initialize() = 0;
	//終了処理
	virtual void Finalize() = 0;
	//更新
	virtual void Update(float deltaTime) = 0;
	//描画
	virtual void Draw() = 0;
	//デバック描画(ラインとか)
	virtual void DebugDraw() = 0;

	virtual ~BaseCharacter() = default;

protected:
	//体力
	int16_t life_;
	//座標
	MLEngine::Math::Vector3 pos_;
	//大きさ
	MLEngine::Math::Vector3 scale_;
	//回転
	MLEngine::Math::Vector3 rotate_;
	//回転
	MLEngine::Math::Vector4 color_;

	//行列
	MLEngine::Math::Matrix4x4 worldMat_;
	//死亡フラグ
	bool isAlive_ = true;
	//有効化フラグ
	bool isActive_ = true;

};


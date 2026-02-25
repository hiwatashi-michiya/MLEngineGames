#pragma once
#include<../Network/NetworkManager.h>
#include "Sprite/Sprite3D.h"
#include <array>

/// <summary>
/// 背景の動きを制御するクラス
/// </summary>
class BackgroundMove
{
public:
	BackgroundMove();
	~BackgroundMove();

	void Initialize();

	void Update();

	void Debug();

private:

	//通信相手の情報を取得
	void SyncFromNetwork();

private:

	//雲最大数
	static const uint32_t kMaxClouds_ = 5;
	//一反木綿最大数
	static const uint32_t kMaxIttanCottons_ = 2;
	//送受信する情報
	NetworkManager::BackgroundState bgState_{};
	//雲の開始座標
	float startCloudPositionX_ = 20.0f;
	//雲の終了座標
	float endCloudPositionX_ = -40.0f;
	//雲全体を動かす座標
	float cloudPositionX_ = 0.0f;
	//次の画面に移動する閾値
	float moveNextScreenX_ = -11.1f;
	//雲の最大移動時間
	float maxCloudMoveTime_ = 40.0f;
	//雲の現在の時間
	float currentCloudMoveTime_ = 0.0f;

	//雲画像名前
	std::string cloudTextureName_;
	//雲全体のオフセット
	MLEngine::Math::Vector3 defaultCloudOffset_{};
	//雲それぞれのオフセット
	std::array<MLEngine::Math::Vector3, kMaxClouds_> cloudOffset_;
	//雲のトランスフォーム
	std::array<MLEngine::Object::Transform, kMaxClouds_> cloudTransforms_;
	//雲画像
	std::array<MLEngine::Resource::Sprite3D, kMaxClouds_> clouds_;
	//一反木綿画像
	std::array<MLEngine::Resource::Sprite3D, kMaxIttanCottons_> ittanCottons_;
	//雲の動きを開始するかどうか
	bool isStartMoveClouds_ = false;

	bool needSend_ = false;

};



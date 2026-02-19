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

private:

	//通信相手の情報を取得
	void SyncFromNetwork();

private:

	//雲最大数
	static const uint32_t kMaxClouds_ = 5;
	//一反木綿最大数
	static const uint32_t kMaxIttanCottons_ = 2;
	//送受信する情報
	NetworkManager::BackgroundState plState_{};

	//雲画像
	std::array<MLEngine::Resource::Sprite3D, kMaxClouds_> clouds_;
	//一反木綿画像
	std::array<MLEngine::Resource::Sprite3D, kMaxIttanCottons_> ittanCottons_;


};



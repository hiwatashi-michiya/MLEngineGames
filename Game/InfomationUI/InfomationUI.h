#pragma once
#include<Manager/PlayerManager.h>
#include <UI/UI.h>

/*お知らせ系統のUIを制御するクラス*/
class InfomationUI {
public:
	InfomationUI();

	void Initialize();

	void Update();

	void InfoEase() {
		startEase_ = true;

		isReverse_ = false;
		scaleTime_ = 0.0f;
		hpBar_.ReStart();
		hpFrame_.ReStart();
		speech_Bubble_.ReStart();
		enemyTex_.ReStart();
	}

	void SetIsActive(bool flag);

private:
	void GlobalSetValues();

	void GlobalGetValues();
	//テクスチャに変数を代入しているのをまとめる
	void SetPosSize();
	//反対に代入し動きを反転
	void SetPosSizeReverse();

	//デバック描画(ラインとか)
	void DebugDraw();

	//スコアによる表示の挙動
	void NumberDrawControl();

	Vector2 AddStarthalf(Vector2 end) const;

private:

	MLEngine::Resource::Texture texture_;
	//吹き出し
	UI speech_Bubble_;

	MLEngine::Math::Vector2  speech_BubblePos_{};
	MLEngine::Math::Vector2  speech_BubbleSize_{};
	//敵の画像
	UI enemyTex_;

	MLEngine::Math::Vector2 enemyTexPos_{};
	MLEngine::Math::Vector2 enemyTexSize_{};
	//HPのフレーム
	UI hpFrame_;

	MLEngine::Math::Vector2 hpFramePos_{};
	MLEngine::Math::Vector2 hpFrameSize_{};

	//HPのバー
	UI hpBar_;

	MLEngine::Math::Vector2 hpBarPos_{};
	MLEngine::Math::Vector2 hpBarSize_{};

	//拡大率
	float magnification_ = 1.2f;

	//時間制限
	float scaleTimeRimit_ = 1.5f;
	//時間
	float scaleTime_ = 0.0f;
	//hpバーの最大の長さ
	float maxLangth_ = 140.0f;
	//反対に動くときのフラグ
	bool isReverse_ = false;

	bool startEase_ = false;

	Vector2 infoStartPos_ = { 1920.0f,700.0f };

	//基本色
	MLEngine::Math::Vector4 baseColor_ = { 1.0f,1.0f,1.0f,1.0f };
	//HPUIの色
	MLEngine::Math::Vector4 hpColor_ = { 1.0f,0.0f,0.0f,1.0f };

};


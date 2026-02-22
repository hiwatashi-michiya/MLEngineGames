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
		hpBar_.ReStart();
		hpFrame_.ReStart();
		speech_Bubble_.ReStart();
		enemyTex_.ReStart();
	}

	void SetIsActive(bool flag);

private:
	void GlobalSetValues();

	void GlobalGetValues();

	//デバック描画(ラインとか)
	void DebugDraw();

	//スコアによる表示の挙動
	void NumberDrawControl();

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

	//コンボなどの傾き
	float comboRotate_ = 0.0f;


	//基本色
	MLEngine::Math::Vector4 baseColor_ = { 1.0f,1.0f,1.0f,1.0f };
	//コンボUIの色
	MLEngine::Math::Vector4 hpColor_ = { 1.0f,1.0f,1.0f,1.0f };

};


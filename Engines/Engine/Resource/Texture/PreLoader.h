#pragma once
#include "Texture.h"
#include <vector>
#include "Sprite/Sprite2D.h"
#include <memory>
#include <unordered_map>
#include <string>

namespace MLEngine::Resource {

	/// <summary>
	/// 画像を事前読み込みしてくれるクラス
	/// </summary>
	class PreLoader
	{
	public:
		
		static PreLoader* GetInstance();

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// テクスチャ事前描画
		/// </summary>
		void Update();

		/// <summary>
		/// 読み込むテクスチャ追加
		/// </summary>
		/// <param name="tex"></param>
		void AddTexture(MLEngine::Resource::Texture tex);

		/// <summary>
		/// ロード完了フラグ
		/// </summary>
		/// <returns></returns>
		bool GetIsEndLoad() const { return isEndLoad_; }

	private:

		//全てのロードが終わったかどうか
		bool isEndLoad_ = false;

		//読み込んだテクスチャを確認するためのマップ
		std::unordered_map<int32_t, MLEngine::Resource::Texture> texMap_;
		//読み込むテクスチャ
		std::vector<MLEngine::Resource::Texture> textures_;
		//表示させる用のsprite
		std::unique_ptr<MLEngine::Resource::Sprite2D> sprite_;

	private:

		PreLoader() = default;
		~PreLoader() = default;
		PreLoader(const PreLoader&) = delete;
		const PreLoader& operator=(const PreLoader&) = delete;

	};

}

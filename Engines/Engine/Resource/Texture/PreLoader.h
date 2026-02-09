#pragma once
#include "Texture.h"
#include <vector>
#include "Sprite/Sprite2D.h"
#include <memory>

namespace MLEngine::Resource {

	/// <summary>
	/// 画像を事前読み込みしてくれるクラス
	/// </summary>
	class PreLoader
	{
	public:
		
		static PreLoader* GetInstance();

		void DrawAllTextures();

	private:

		//全てのロードが終わったかどうか
		bool isEndLoad_ = false;

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

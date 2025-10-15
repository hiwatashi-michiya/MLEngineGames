#pragma once
#include "Graphics/Mesh.h"
#include <string>
#include <unordered_map>

namespace MLEngine::Core::Render::Mesh {

	/// <summary>
	/// メッシュの一元管理マネージャー
	/// </summary>
	class Manager
	{
	public:
		//インスタンス取得
		static Manager* GetInstance();

		/// <summary>
		/// メッシュの作成
		/// </summary>
		/// <param name="fileName">作成に使用するファイル</param>
		void CreateMesh(const std::string& fileName);

		/// <summary>
		/// メッシュを取得する
		/// </summary>
		/// <param name="fileName">取得するファイル名</param>
		/// <returns></returns>
		MLEngine::Graphics::Mesh* GetMesh(const std::string& fileName) { return meshes_[fileName].get(); }

		/// <summary>
		/// メッシュを既に作成しているか
		/// </summary>
		/// <param name="fileName">確認するファイルの名前</param>
		/// <returns></returns>
		bool IsExistMesh(const std::string& fileName) { return meshes_.find(fileName) != meshes_.end(); }

	private:
		//メッシュと名前を紐づけたマップ
		std::unordered_map<std::string, std::shared_ptr<MLEngine::Graphics::Mesh>> meshes_;

	private:
		//シングルトン化
		Manager() = default;
		~Manager() = default;
		Manager(const Manager&) = delete;
		const Manager& operator=(const Manager&) = delete;

	};


}


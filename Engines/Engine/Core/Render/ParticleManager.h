#pragma once
#include "Particle/Particle3D.h"
#include "Vector3.h"
#include <memory>
#include <list>
#include "Particle/Particle.h"
#include <array>

namespace MLEngine::Core::Render {

	namespace Particle {

		/// <summary>
		/// パーティクルの管理を行うクラス
		/// </summary>
		class Manager
		{
		public:
			//インスタンス取得
			static Manager* GetInstance();
			//初期化
			void Initialize();
			//終了処理
			void Finalize();
			//更新
			void Update();
			//描画
			void Draw(MLEngine::Object::Camera* camera);
			//全パーティクルのリセット
			void Reset();
			//未使用のパーティクル取得
			bool GetUnUsedParticle(MLEngine::Resource::Particle*& particle);
			//パーティクル最大数
			static const int32_t kMaxParticle_ = 256;

		private:
			//管理しているパーティクルのリスト
			std::array<std::unique_ptr<MLEngine::Resource::Particle>, kMaxParticle_> particles_;
			//どのパーティクルが現在使われているか
			std::array<bool, kMaxParticle_> isUsed_;

		private:
			//シングルトン化
			Manager() = default;
			~Manager() = default;
			Manager(const Manager&) = delete;
			const Manager& operator=(const Manager&) = delete;

		};


	}

}



#pragma once
#include "Quaternion.h"
#include "Vector3.h"

namespace MLEngine::Resource {

	/// <summary>
	/// キーフレーム構造体
	/// </summary>
	/// <typeparam name="tValue"></typeparam>
	template <typename tValue>
	struct Keyframe {
		float time;
		tValue value;
	};

	//Vector3とQuaternionのキーフレーム定義
	using KeyframeVector3 = Keyframe<MLEngine::Math::Vector3>;
	using KeyframeQuaternion = Keyframe<MLEngine::Math::Quaternion>;

}


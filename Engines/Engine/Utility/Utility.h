#pragma once
#include <string>
#include <vector>
#include <cmath>
#include <filesystem>
#include <windows.h>
#include "Vector4.h"
#include "Vector3.h"
#include <stdint.h>

/// <summary>
/// 便利な機能まとめ
/// </summary>
namespace MLEngine::Utility {

	/// <summary>
	/// リソースファイルの検索
	/// </summary>
	/// <param name="name"></param>
	/// <param name="extension">拡張子名</param>
	/// <returns></returns>
	std::string SearchResourceFile(const std::string& name, const std::string& extension);

}



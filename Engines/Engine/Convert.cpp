#include "Convert.h"
#include <Windows.h>

// 簡易 UTF-8 判定
bool IsUTF8(const std::string& str) {
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(str.c_str());
    size_t i = 0;
    while (i < str.size()) {
        if (bytes[i] <= 0x7F) { // ASCII
            i++;
        }
        else if ((bytes[i] & 0xE0) == 0xC0) { // 2バイト
            if (i + 1 >= str.size() || (bytes[i + 1] & 0xC0) != 0x80) return false;
            i += 2;
        }
        else if ((bytes[i] & 0xF0) == 0xE0) { // 3バイト
            if (i + 2 >= str.size() ||
                (bytes[i + 1] & 0xC0) != 0x80 ||
                (bytes[i + 2] & 0xC0) != 0x80) return false;
            i += 3;
        }
        else if ((bytes[i] & 0xF8) == 0xF0) { // 4バイト
            if (i + 3 >= str.size() ||
                (bytes[i + 1] & 0xC0) != 0x80 ||
                (bytes[i + 2] & 0xC0) != 0x80 ||
                (bytes[i + 3] & 0xC0) != 0x80) return false;
            i += 4;
        }
        else {
            return false;
        }
    }
    return true;
}

std::wstring ConvertString(const std::string& str) {
    if (str.empty()) return std::wstring();

    UINT codePage = IsUTF8(str) ? CP_UTF8 : 932; // UTF-8かShift-JISを判定

    int sizeNeeded = MultiByteToWideChar(codePage, 0, str.c_str(), -1, nullptr, 0);
    if (sizeNeeded == 0) return std::wstring();

    std::wstring result(sizeNeeded, 0);
    MultiByteToWideChar(codePage, 0, str.c_str(), -1, &result[0], sizeNeeded);

    if (!result.empty() && result.back() == L'\0') result.pop_back();

    return result;
}

std::string ConvertString(const std::wstring& str) {
	if (str.empty()) {
		return std::string();
	}

	auto sizeNeeded = WideCharToMultiByte(CP_UTF8, 0,
		str.data(), static_cast<int>(str.size()), NULL, 0, NULL, NULL);
	if (sizeNeeded == 0) {
		return std::string();
	}
	std::string result(sizeNeeded, 0);
	WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>
		(str.size()), result.data(), sizeNeeded, NULL, NULL);
	return result;

}

void Log(const std::string& message) {
	OutputDebugStringA(message.c_str());
}

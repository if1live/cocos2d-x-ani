// Ŭnicode please
#pragma once

#if SR_USE_PCH == 0
#include <string>
#include <vector>
#endif

namespace sora {;

class FileHelper {
public:
    //cocos2d-x 2.0에서 리소스 패스 정책이 달라져서 더이상 필요잇을지 모르겟다
	static std::string RemoveSuffix(const std::string &orig);

	static std::string ReadFile(const std::string &file);
	static std::vector<unsigned char> ReadByteFile(const std::string &file);
};
}
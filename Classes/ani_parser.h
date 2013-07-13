// Ŭnicode please
#pragma once

#include <glm/glm.hpp>
#include "ani.h"

namespace sora {;
class AniPrototype;
//데이터만 들어있는거
struct AniData;

class AniParser {
public:
	enum {
		kXmlParser,
		kByteParser
	};

public:
	//ani parser의 실제 구현체는 내부로 숨김
	//적절히 new로 생성된거 받아서 밖에서는 쓰자
	static AniParser CreateByteAniParser(AniDeviceType dev_type) {
		return AniParser(dev_type, kByteParser);
	}
	static AniParser CreateXmlAniParser(AniDeviceType dev_type) {
		return AniParser(dev_type, kXmlParser);
	}

public:
	//input은 규격 수정없이 xml에 잇는것을 그대로 갖다쓴다
	static glm::mat3 Mat3ToMat4(const glm::mat3 &input, AniDeviceType dev_type);
	static void ReverseYDirection(glm::mat3 &m);

	//static void ConvertToFlashMatrix(qb::mat4 &mat);

private:
	AniParser(AniDeviceType dev_type, int type) : dev_type_(dev_type), parser_type_(type) {}
public:
	virtual ~AniParser() {}

	AniDeviceType dev_type() const { return dev_type_; }

	AniPrototype LoadFile(const std::string &file, bool *retcode);
	AniPrototype LoadData(const std::vector<unsigned char> &data, bool *retcode) const;

private:
	AniPrototype LoadByteData(const std::vector<unsigned char> &data, bool *retcode) const;
	AniPrototype LoadXmlData(const std::vector<unsigned char> &data, bool *retcode) const;
	AniPrototype LoadByteFile(const std::string &file, bool *retcode) const;
	AniPrototype LoadXmlFile(const std::string &file, bool *retcode) const;

protected:
	void Convert(AniPrototype *ani, const AniData &data) const;
	std::string ConvertFilename(const std::string &orig) const;
private:
	int parser_type_;
	AniDeviceType dev_type_;
};

}
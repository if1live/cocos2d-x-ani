// Ŭnicode please
#pragma once

#include <glm/glm.hpp>
#include "ani_globals.h"

#if SR_USE_PCH == 0
#include "tinyxml/tinyxml.h"
#include <vector>
#include <string>
#endif

namespace sora {;

struct AniData;
struct AniFrameData;

class AniReader {
public:
	AniReader() {}
	virtual ~AniReader() {}
	virtual AniData Read(const std::vector<unsigned char> &data) = 0;
    
    static AniReader *ByteVersion();
    static AniReader *XmlVersion();
};

// private class같은 느낌으로 구현 세부사항을 숨겨버리느 방법도 있지만
// unittest때문에 일단 까놓음

//byte ani data 규격
//header
//magic name : 2byte. 'qb'로 시작. "qb"로 라이브러리에서 처리가능한 파일인지 구분
//version : 2byte,  숫자를 떄려박아서 버전정보 표기
//frame_file_name : 1byte + nbyte : 1byte로 파일길이. "ch_main-hd.plist" 같은식
//texture_file_name : 1byte+nbyte : 1byte로 파일길이 표시. "ch_main-hd.png" 같은식
//fps : 2byte, 굳이 int로 박을 필요 없어보이니까
//num_frame : 2byte, 굳이 int로 박을 필요 없어보이니까
class ByteAniReader : public AniReader {
public:
	ByteAniReader() {}
	virtual ~ByteAniReader() {}

	virtual AniData Read(const std::vector<unsigned char> &data);

	int ReadHeader(const std::vector<unsigned char> &data, int offset, AniData *ani);
	int ReadMask(const std::vector<unsigned char> &data, int offset, AniData *ani);
	int ReadState(const std::vector<unsigned char> &data, int offset, AniData *ani);
	int ReadFrame(const std::vector<unsigned char> &data, int offset, AniData *ani);
	int ReadString(const std::vector<unsigned char> &data, int offset, std::string *value);
	int ReadChar(const std::vector<unsigned char> &data, int offset, unsigned char *value);
	int ReadShort(const std::vector<unsigned char> &data, int offset, short *value);
	int ReadFloat(const std::vector<unsigned char> &data, int offset, float *value);
};


class XmlAniReader : public AniReader {
public:
	XmlAniReader() {}
	virtual ~XmlAniReader() {}

	virtual AniData Read(const std::vector<unsigned char> &data);

	void ReadHeader(TiXmlElement *root, AniData &ani);
	void ReadFrameList(TiXmlElement *frame_root, AniData &ani);
	void ReadMaskList(TiXmlElement *mask_root, AniData &ani);
	void ReadStateList(TiXmlElement *state_root, AniData &ani);
	void ReadFrame(TiXmlElement *frame, AniFrameData &frame_data);

public:
    static std::vector<int> StrToIntList(const std::string &str);
	static void StrToColor(const std::string &str, AniColor4ub *mul_color, AniColor4ub *add_color);
	static void StrToMat3(const std::string &str, glm::mat3 *m);
};


}
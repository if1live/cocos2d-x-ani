// Ŭnicode please
#pragma once

#include <vector>
#include <string>

namespace sora {;

struct AniFrameCmd;
struct AniData;

class AniWriter {
public:
	AniWriter() {}
	virtual ~AniWriter() {}
	virtual std::vector<unsigned char> Write(const AniData &data) = 0;

    static AniWriter *ByteVersion();
};

class ByteAniWriter : public AniWriter {
public:
	ByteAniWriter() {} 
	virtual ~ByteAniWriter() {}

	std::vector<unsigned char> Write(const AniData &data);
	void WriteHeader(const AniData &ani, std::vector<unsigned char> *data);
	bool NeedWriteColor(const AniFrameCmd &cmd) const;
	void WriteFrame(const AniData &ani, std::vector<unsigned char> *data);
	void WriteMask(const AniData &ani, std::vector<unsigned char> *data);
	void WriteState(const AniData &ani, std::vector<unsigned char> *data);
	void WriteString(std::vector<unsigned char> *data, const std::string &value);
	void WriteChar(std::vector<unsigned char> *data, unsigned char value);
	void WriteShort(std::vector<unsigned char> *data, short value);
	void WriteFloat(std::vector<unsigned char> *data, float value);
};


}
// Ŭnicode please
#include "stdafx.h"
#include "ani_reader.h"
#include "ani_data.h"

#if SR_USE_PCH == 0
#include <array>
#include <set>
#endif

namespace sora {;

AniData ByteAniReader::Read(const std::vector<unsigned char> &data) {
	AniData ani;

	if(data.size() < 2) {
		//read fail
		return ani;
	}
	if(data[0] != 'q' || data[1] != 'b') {
		//read fail
		return ani;
	}

	int offset = 0;
	offset = ReadHeader(data, offset, &ani);
	offset = ReadFrame(data, offset, &ani);
	offset = ReadMask(data, offset, &ani);
	offset = ReadState(data, offset, &ani);

	return ani;
}
int ByteAniReader::ReadHeader(const std::vector<unsigned char> &data, int offset, AniData *ani) {
	offset = 2;	//qb는 건너뛴다
	short version = 0;
	short fps = 0;
	short num_frame = 0;
	offset = ReadShort(data, offset, &version);
	offset = ReadString(data, offset, &ani->frame_file_name);
	offset = ReadString(data, offset, &ani->texture_file_name);
	offset = ReadShort(data, offset, &fps);
	offset = ReadShort(data, offset, &num_frame);

	ani->version = version;
	ani->fps = fps;
	ani->num_frame = num_frame;
	return offset;
}

int ByteAniReader::ReadMask(const std::vector<unsigned char> &data, int offset, AniData *ani) {
	short mask_count = 0;
	offset = ReadShort(data, offset, &mask_count);
	ani->mask_list.reserve(mask_count);

	for(int i = 0 ; i < mask_count ; i++) {
		short mask_id = 0;
		short clip_count = 0;
		offset = ReadShort(data, offset, &mask_id);
		offset = ReadShort(data, offset, &clip_count);

		AniMaskData mask;
		mask.mask = mask_id;
		mask.clip_list.resize(clip_count);

		for(int j = 0 ; j < clip_count ; j++) {
			short clip = 0;
			offset = ReadShort(data, offset, &clip);
			mask.clip_list[j] = clip;
		}

		ani->mask_list.push_back(mask);
	}
	return offset;
}

int ByteAniReader::ReadState(const std::vector<unsigned char> &data, int offset, AniData *ani) {
	short state_count = 0;
	offset = ReadShort(data, offset, &state_count);
	ani->state_list.reserve(state_count);

	for(int i = 0 ; i < state_count ; i++) {
		short frame = 0;
		AniStateData state;
		offset = ReadShort(data, offset, &frame);
		offset = ReadString(data, offset, &state.name);
		state.frame = frame;
		ani->state_list.push_back(state);
	}
	return offset;
}
int ByteAniReader::ReadFrame(const std::vector<unsigned char> &data, int offset, AniData *ani) {
	short frame_count = 0;
	offset = ReadShort(data, offset, &frame_count);
	ani->frame_list.resize(frame_count);

	for(int i = 0 ; i < frame_count ; i++) {
		short number = 0;
		short cmd_count = 0;
		offset = ReadShort(data, offset, &number);
		offset = ReadShort(data, offset, &cmd_count);

		AniFrameData &frame = ani->frame_list[i];
		frame.number = number;
		frame.cmd_list.resize(cmd_count);

		for(int j = 0 ; j < cmd_count ; j++) {
			//cmd 공통 속성
			unsigned char cmd_type = 0;
			short res_id = 0;
			offset = ReadChar(data, offset, &cmd_type);
			offset = ReadShort(data, offset, &res_id);

			AniFrameCmd &cmd = frame.cmd_list[j];
			cmd.cmd_type = cmd_type;
			cmd.res_id = res_id;

			if(cmd_type == kFrameCmd_Show) {
				offset = ReadFloat(data, offset, &cmd.matrix[0][0]);
                offset = ReadFloat(data, offset, &cmd.matrix[1][0]);
                offset = ReadFloat(data, offset, &cmd.matrix[2][0]);
                offset = ReadFloat(data, offset, &cmd.matrix[0][1]);
                offset = ReadFloat(data, offset, &cmd.matrix[1][1]);
                offset = ReadFloat(data, offset, &cmd.matrix[2][1]);
                offset = ReadFloat(data, offset, &cmd.matrix[0][2]);
                offset = ReadFloat(data, offset, &cmd.matrix[1][2]);
                offset = ReadFloat(data, offset, &cmd.matrix[2][2]);

				unsigned char use_color = 0;
				offset = ReadChar(data, offset, &use_color);
				IUASSERT(use_color == 'y' || use_color == 'n');
				if(use_color == 'y') {
                    std::array<unsigned char, 8> color_array;
                    for(int color_idx = 0 ; color_idx < 8 ; color_idx++) {
                        offset = ReadChar(data, offset, &color_array[color_idx]);
                    }
                    cmd.mul_color = AniColor4ub(color_array[0], color_array[1], color_array[2], color_array[3]);
                    cmd.add_color = AniColor4ub(color_array[4], color_array[5], color_array[6], color_array[7]);
				}

				short z = 0;
				offset = ReadShort(data, offset, &z);
				cmd.z = z;
				offset = ReadString(data, offset, &cmd.res_name);

			} else if(cmd_type == kFrameCmd_Apply) {
                offset = ReadFloat(data, offset, &cmd.matrix[0][0]);
                offset = ReadFloat(data, offset, &cmd.matrix[1][0]);
                offset = ReadFloat(data, offset, &cmd.matrix[2][0]);
                offset = ReadFloat(data, offset, &cmd.matrix[0][1]);
                offset = ReadFloat(data, offset, &cmd.matrix[1][1]);
                offset = ReadFloat(data, offset, &cmd.matrix[2][1]);
                offset = ReadFloat(data, offset, &cmd.matrix[0][2]);
                offset = ReadFloat(data, offset, &cmd.matrix[1][2]);
                offset = ReadFloat(data, offset, &cmd.matrix[2][2]);

				unsigned char use_color = 0;
				offset = ReadChar(data, offset, &use_color);
				IUASSERT(use_color == 'y' || use_color == 'n');
				if(use_color == 'y') {
                    std::array<unsigned char, 8> color_array;
                    for(int color_idx = 0 ; color_idx < 8 ; color_idx++) {
                        offset = ReadChar(data, offset, &color_array[color_idx]);
                    }
                    cmd.mul_color = AniColor4ub(color_array[0], color_array[1], color_array[2], color_array[3]);
                    cmd.add_color = AniColor4ub(color_array[4], color_array[5], color_array[6], color_array[7]);
				}

			} else if(cmd_type == kFrameCmd_Hide) {
			}
		}
	}
	return offset;
}

int ByteAniReader::ReadString(const std::vector<unsigned char> &data, int offset, std::string *value) {
	short length = 0;
	int content_offset = ReadShort(data, offset, &length);

	value->reserve(length + 1);
	value->clear();
	for(int i = 0 ; i < length ; i++) {
		unsigned char ch = data[content_offset + i];
		value->push_back(ch);
	}

	return content_offset + length;
}
int ByteAniReader::ReadChar(const std::vector<unsigned char> &data, int offset, unsigned char *value) {
	*value = data[offset];
	return offset + 1;
}
int ByteAniReader::ReadShort(const std::vector<unsigned char> &data, int offset, short *value) {
	union {
		short s;
		struct { unsigned char uc[2]; };
	} tmp;
	tmp.uc[0] = data[offset];
	tmp.uc[1] = data[offset + 1];
	*value = tmp.s;

	return offset + 2;
}
int ByteAniReader::ReadFloat(const std::vector<unsigned char> &data, int offset, float *value) {
	union {
		float f;
		struct { unsigned char uc[2]; };
	} tmp;
	for(int i = 0 ; i < 4 ; i++) {
		tmp.uc[i] = data[offset + i];
	}
	*value = tmp.f;
	return offset + 4;
}

AniData XmlAniReader::Read(const std::vector<unsigned char> &data) {
	AniData ani;

	TiXmlDocument doc;
	doc.Parse((const char*)data.data());

	if(doc.Error()) {
		const char *error_msg = doc.ErrorDesc();
		int row = doc.ErrorRow();
		int col = doc.ErrorCol();
			
		char msg[512];
		sprintf(msg, "%d:%d %s", row, col, error_msg);
		IUASSERT(false && msg);
		return ani;
	}
	//성공적으로 파싱 성공한 경우, 세부 내용을 하나씩 처리하기
	TiXmlElement *root = doc.RootElement();
	ReadHeader(root, ani);
	for(TiXmlElement *child = root->FirstChildElement() ; child != NULL ; child = child->NextSiblingElement()) {
		std::string elem_name = child->Value();
		if(elem_name == "frames") {
			ReadFrameList(child, ani);
		} else if(elem_name == "masks") {
			ReadMaskList(child, ani);
		} else if(elem_name == "states") {
			ReadStateList(child, ani);
		}
	}

	return ani;
}

void XmlAniReader::ReadHeader(TiXmlElement *root, AniData &ani)
{
	IUASSERT(strcmp("animation", root->Value()) == 0);
	root->Attribute("fps", &ani.fps);
	ani.version = 1;
	ani.frame_file_name = root->Attribute("frameFileName");
	ani.texture_file_name = root->Attribute("textureFileName");

	for(TiXmlElement *child = root->FirstChildElement() ; child != NULL ; child = child->NextSiblingElement()) {
		std::string elem_name = child->Value();
		if(elem_name == "frames") {
			child->Attribute("numFrames", &ani.num_frame);
		}
	}
}
void XmlAniReader::ReadFrameList(TiXmlElement *frame_root, AniData &ani)
{
	for(TiXmlElement *child = frame_root->FirstChildElement() ; child != NULL ; child = child->NextSiblingElement()) {
		AniFrameData frame;
		child->Attribute("number", &frame.number);
		ReadFrame(child, frame);
			
		ani.frame_list.push_back(frame);
	}
}
void XmlAniReader::ReadMaskList(TiXmlElement *mask_root, AniData &ani)
{
	for(TiXmlElement *child = mask_root->FirstChildElement() ; child != NULL ; child = child->NextSiblingElement()) {
		AniMaskData mask;
		child->Attribute("mask", &mask.mask);
		std::string cliped_str = child->Attribute("cliped");

		std::vector<int> clip_raw_list = StrToIntList(cliped_str);
		std::set<int> clip_set;
		for(auto it = clip_raw_list.begin(), e = clip_raw_list.end() ; it != e ; ++it) {
			clip_set.insert(*it);
		}
			
		mask.clip_list.resize(clip_set.size());
		std::copy(clip_set.begin(), clip_set.end(), mask.clip_list.begin());

		ani.mask_list.push_back(mask);
	}
}
void XmlAniReader::ReadStateList(TiXmlElement *state_root, AniData &ani)
{
	for(TiXmlElement *child = state_root->FirstChildElement() ; child != NULL ; child = child->NextSiblingElement()) {
		AniStateData state;
		child->Attribute("frame", &state.frame);
		std::string name = child->Attribute("name");
		state.name = name;
		ani.state_list.push_back(state);
	}
}
void XmlAniReader::ReadFrame(TiXmlElement *frame, AniFrameData &frame_data)
{
	//커맨드 읽기
	for(TiXmlElement *child = frame->FirstChildElement() ; child != NULL ; child = child->NextSiblingElement()) {
		AniFrameCmd cmd;
		child->Attribute("id", &cmd.res_id);
		const char *color_str = child->Attribute("color");
		const char *matrix_str = child->Attribute("matrix");

		if(color_str != NULL) {
			std::string color = color_str;
			StrToColor(color, &cmd.mul_color, &cmd.add_color);
		}
		if(matrix_str != NULL) {
			std::string matrix = matrix_str;
			StrToMat3(matrix, &cmd.matrix);
		}			

		const std::string elem_name = child->Value();
		if(elem_name == "show") {
			cmd.cmd_type = kFrameCmd_Show;
				
			const char *name_str = child->Attribute("name");
			IUASSERT(name_str != NULL);
			cmd.res_name = name_str;

			child->Attribute("z", &cmd.z);

		} else if(elem_name == "apply") {
			cmd.cmd_type = kFrameCmd_Apply;


		} else if(elem_name == "hide") {
			cmd.cmd_type = kFrameCmd_Hide;

		}

		frame_data.cmd_list.push_back(cmd);
	}
}

std::vector<int> XmlAniReader::StrToIntList(const std::string &str) {
	//새로운 색 속성으로 설정해주기
	const int buffer_size = 512;
	char buffer[buffer_size];
	memset(buffer, 0, sizeof(buffer[0]) * buffer_size);
	memcpy(buffer, str.c_str(), str.length());

	int space_count = 0;
	for(int i = 0 ; i < buffer_size ; i++) {
		if(buffer[i] == ' ') {
			buffer[i] = '\0';
			space_count++;
		} else if(buffer[i] == '\0') {
			break;
		}
	}

	//공백갯수+1이 숫자 조각의 수
	std::vector<int> result;
	result.reserve(space_count + 1);

	int str_idx = 0;
	char prev_char = '\0';
	for(size_t i = 0 ; i < str.length() ; i++) {
		char curr_char = buffer[i];
		if(prev_char == '\0' && curr_char != '\0') {
			const char *str = &buffer[i];
			int ivalue = atoi(str);
			result.push_back(ivalue);
			str_idx++;
		} 

		if(str_idx > space_count) {
			break;
		}
		prev_char = curr_char;
	}

	return result;
}

void XmlAniReader::StrToColor(const std::string &str, AniColor4ub *mul_color, AniColor4ub *add_color) {
	//색깔 속성 얻기
	const static std::string normal_value = "1 1 1 1 0 0 0 0";
	if(str.empty() == true || str == normal_value) {
		//기본색
		memset(mul_color, 255, sizeof(unsigned char) * 4);
		memset(add_color, 0, sizeof(unsigned char) * 4);
	} else {
		//새로운 색 속성으로 설정해주기
		const int buffer_size = 512;
		char buffer[buffer_size];
		memset(buffer, 0, sizeof(buffer[0]) * buffer_size);
		memcpy(buffer, str.c_str(), str.length());

		for(int i = 0 ; i < buffer_size ; i++) {
			if(buffer[i] == ' ') {
				buffer[i] = '\0';
			} else if(buffer[i] == '\0') {
				break;
			}
		}

		std::array<char*, 8> num_str_list = { '\0' };
		int str_idx = 0;
		char prev_char = '\0';
		for(size_t i = 0 ; i < str.length() ; i++) {
			char curr_char = buffer[i];
			if(prev_char == '\0' && curr_char != '\0') {
				num_str_list[str_idx] = &buffer[i];
				str_idx++;
			} 

			if(str_idx >= 8) {
				break;
			}
			prev_char = curr_char;
		}

		unsigned char color_array[8] = { 1, 1, 1, 1, 0, 0, 0, 0 };
		for(int i = 0 ; i < 8 ; i++) {
            if(num_str_list[i] != NULL) {
			    float fvalue = (float)atof(num_str_list[i]);
			    unsigned char value = (unsigned char)(fvalue * 255.0f);
			    color_array[i] = value;
            }
		}
        memcpy(mul_color->data, &color_array[0], sizeof(unsigned char) * 4);
        memcpy(add_color->data, &color_array[4], sizeof(unsigned char) * 4);
	}
}

void XmlAniReader::StrToMat3(const std::string &str, glm::mat3 *m) {
    SR_ASSERT(m != NULL);
	//str을 복사한후, 공백을 널로 치환하고 로직을 적당히 돌리면 추가적인 복사없이
	//똑같은 작업을 할수 잇을듯하다
	//1.1<null>12.3
	//같이 널을 기준으로 전부 잘라버린다
	//그리고 atof로 돌릴때 포인터 위치를 적절히 
	const int buffer_size = 512;
	IUASSERT(str.length() + 1 < buffer_size && "Need more big buffer, resize buffer_size");
	std::array<char, buffer_size> buffer;
	memset(buffer.data(), 0, sizeof(buffer[0]) * buffer_size);
	memcpy(buffer.data(), str.c_str(), str.length());

	for(size_t i = 0 ; i < buffer_size ; i++) {
		if(buffer[i] == ' ') {
			buffer[i] = '\0';
		} else if(buffer[i] == '\0') {
			break;
		}
	}

	//공백직전것의 포인터 배열을 구성. 이것을 atof로 변환하면 적절히 나온다
	std::array<const char *, 9> num_value_ptr_list;
	int str_index = 0;
	char prev_ch = '\0';
	char curr_ch = '\0';
	for(size_t i = 0 ; i < buffer_size ; i++) {
		curr_ch = buffer[i];
		if(prev_ch == '\0') {
			num_value_ptr_list[str_index] = &buffer[i];
			str_index++;
		}

		if(str_index == 9) {
			break;
		}
		prev_ch = curr_ch;
	}

	//token_vec를 숫자로
    std::array<float, 9> fvalue_list;
	for(size_t i = 0 ; i < 9 ; i++){
		const char *s = num_value_ptr_list[i];
		float value = (float)atof(s);
        fvalue_list[i] = value;
	}

    *m = glm::mat3(
        fvalue_list[0], fvalue_list[3], fvalue_list[6],
        fvalue_list[1], fvalue_list[4], fvalue_list[7],
        fvalue_list[2], fvalue_list[5], fvalue_list[8]
        );
}

AniReader *AniReader::ByteVersion() {
    return new ByteAniReader();
}
AniReader *AniReader::XmlVersion() {
    return new XmlAniReader();
}

}

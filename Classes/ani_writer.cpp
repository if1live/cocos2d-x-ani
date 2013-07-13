// Ŭnicode please
#include "stdafx.h"
#include "ani_writer.h"
#include "ani_data.h"

#if SR_USE_PCH == 0
#include <array>
#endif

namespace sora {;

std::vector<unsigned char> ByteAniWriter::Write(const AniData &data) {
	std::vector<unsigned char> result;

	WriteHeader(data, &result);
	WriteFrame(data, &result);
	WriteMask(data, &result);
	WriteState(data, &result);

	return result;
}
void ByteAniWriter::WriteHeader(const AniData &ani, std::vector<unsigned char> *data) {
	//magic name
	WriteChar(data, 'q');
	WriteChar(data, 'b');
	WriteShort(data, 1);
	WriteString(data, ani.frame_file_name);
	WriteString(data, ani.texture_file_name);
	WriteShort(data, ani.fps);
	WriteShort(data, ani.num_frame);
}

bool ByteAniWriter::NeedWriteColor(const AniFrameCmd &cmd) const {
    const static AniColor4ub default_mul(255, 255, 255, 255);
    const static AniColor4ub default_add(0, 0, 0, 0);
    if(default_mul != cmd.mul_color) {
        return true;
	}
    if(default_add != cmd.add_color) {
		return true;
	}
	return false;
}

void ByteAniWriter::WriteFrame(const AniData &ani, std::vector<unsigned char> *data) {
	WriteShort(data, (short)ani.frame_list.size());

	for(size_t i = 0 ; i < ani.frame_list.size() ; i++) {
		const AniFrameData &frame = ani.frame_list[i];
		WriteShort(data, (short)frame.number);
		WriteShort(data, (short)frame.cmd_list.size());

		for(size_t j = 0 ; j < frame.cmd_list.size() ; j++) {
			const AniFrameCmd &cmd = frame.cmd_list[j];
			unsigned char cmd_type = (unsigned char)cmd.cmd_type;

			//cmd 공통
			WriteChar(data, cmd_type);
			WriteShort(data, (short)cmd.res_id);

			if(cmd.cmd_type == kFrameCmd_Show) {
                std::array<float, 9> mat_value_list;
                mat_value_list[0] = cmd.matrix[0][0];
                mat_value_list[1] = cmd.matrix[1][0];
                mat_value_list[2] = cmd.matrix[2][0];
                mat_value_list[3] = cmd.matrix[0][1];
                mat_value_list[4] = cmd.matrix[1][1];
                mat_value_list[5] = cmd.matrix[2][1];
                mat_value_list[6] = cmd.matrix[0][2];
                mat_value_list[7] = cmd.matrix[1][2];
                mat_value_list[8] = cmd.matrix[2][2];
				for(int i = 0 ; i < 9 ; i++) {
					WriteFloat(data, mat_value_list[i]);
				}

				//색깔 정보 기록이 필요하면 기록하기
				bool need_color = NeedWriteColor(cmd);
				if(need_color == false) {
					WriteChar(data, 'n');
				} else {
					WriteChar(data, 'y');

					for(int color_idx = 0 ; color_idx < 4 ; color_idx++) {
                        WriteChar(data, cmd.mul_color.data[color_idx]);
					}
					for(int color_idx = 0 ; color_idx < 4 ; color_idx++) {
						WriteChar(data, cmd.add_color.data[color_idx]);
					}
				}

				WriteShort(data, (short)cmd.z);
				WriteString(data, cmd.res_name);

			} if(cmd.cmd_type == kFrameCmd_Apply) {
				std::array<float, 9> mat_value_list;
                mat_value_list[0] = cmd.matrix[0][0];
                mat_value_list[1] = cmd.matrix[1][0];
                mat_value_list[2] = cmd.matrix[2][0];
                mat_value_list[3] = cmd.matrix[0][1];
                mat_value_list[4] = cmd.matrix[1][1];
                mat_value_list[5] = cmd.matrix[2][1];
                mat_value_list[6] = cmd.matrix[0][2];
                mat_value_list[7] = cmd.matrix[1][2];
                mat_value_list[8] = cmd.matrix[2][2];
				for(int i = 0 ; i < 9 ; i++) {
					WriteFloat(data, mat_value_list[i]);
				}

				//색깔 정보 기록이 필요하면 기록하기
				bool need_color = NeedWriteColor(cmd);
				if(need_color == false) {
					WriteChar(data, 'n');
				} else {
					WriteChar(data, 'y');

					for(int color_idx = 0 ; color_idx < 4 ; color_idx++) {
						WriteChar(data, cmd.mul_color.data[color_idx]);
					}
					for(int color_idx = 0 ; color_idx < 4 ; color_idx++) {
						WriteChar(data, cmd.add_color.data[color_idx]);
					}
				}
			} else if(cmd.cmd_type == kFrameCmd_Hide) {

			}
		}
	}
}

void ByteAniWriter::WriteMask(const AniData &ani, std::vector<unsigned char> *data) {
	short mask_count = ani.mask_list.size();
	WriteShort(data, mask_count);

	for(size_t i = 0 ; i < ani.mask_list.size() ; i++) {
		const AniMaskData &mask = ani.mask_list[i];

		WriteShort(data, mask.mask);
		WriteShort(data, mask.clip_list.size());
		for(size_t j = 0; j < mask.clip_list.size() ; j++) {
			WriteShort(data, mask.clip_list[j]);
		}
	}
}
void ByteAniWriter::WriteState(const AniData &ani, std::vector<unsigned char> *data) {
	short state_count = ani.state_list.size();
	WriteShort(data, state_count);

	for(size_t i = 0 ; i < ani.state_list.size() ; i++) {
		const AniStateData &state = ani.state_list[i];

		WriteShort(data, (short)state.frame);
		WriteString(data, state.name);
	}
}

void ByteAniWriter::WriteString(std::vector<unsigned char> *data, const std::string &value) {
	WriteShort(data, value.length());
	std::copy(value.begin(), value.end(), std::back_inserter(*data));
}
void ByteAniWriter::WriteChar(std::vector<unsigned char> *data, unsigned char value) {
	data->push_back(value);
}
void ByteAniWriter::WriteShort(std::vector<unsigned char> *data, short value) {
	union {
		short s;
		struct { unsigned char uc[2]; };
	} tmp;
	tmp.s = value;
	std::copy(tmp.uc, tmp.uc + 2, std::back_inserter(*data));
}
void ByteAniWriter::WriteFloat(std::vector<unsigned char> *data, float value) {
	union {
		float f;
		struct { unsigned char uc[4]; };
	} tmp;
	tmp.f = value;
	std::copy(tmp.uc, tmp.uc + 4, std::back_inserter(*data));
}

AniWriter *AniWriter::ByteVersion() {
    return new ByteAniWriter;
}

}

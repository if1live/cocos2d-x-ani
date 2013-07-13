// Ŭnicode please
#include "stdafx.h"
#include "ani_data.h"
#include "ani_globals.h"

namespace sora {;
bool AniData::operator==(const AniData &o) const {
    if(version != o.version) {
        return false;
    }
    if(frame_file_name != o.frame_file_name) {
        return false;
    }
    if(texture_file_name != o.texture_file_name) {
        return false;
    }
    if(fps != o.fps) {
        return false;
    }
    if(num_frame != o.num_frame) {
        return false;
    }	
    if(frame_list != o.frame_list) {
        return false;
    }
    if(mask_list != o.mask_list) {
        return false;
    }
    if(state_list != o.state_list) {
        return false;
    }
    return true;
}
bool AniData::operator!=(const AniData &o) const {
    return !(*this == o);
}

bool AniFrameData::operator==(const AniFrameData &o) const {
    if(number != o.number) {
        return false;
    }
    if(cmd_list != o.cmd_list) {
        return false;
    }
    return true;
}
bool AniFrameData::operator!=(const AniFrameData &o) const {
    return !(*this == o);
}

AniFrameCmd::AniFrameCmd()
    : cmd_type(kFrameCmd_None), res_id(0), z(0) {
        static_assert(sizeof(mul_color) == 4, "");
        mul_color = AniColor4ub(255, 255, 255, 255);
        add_color = AniColor4ub(0, 0, 0, 0);
        matrix = glm::mat3();
}

bool AniFrameCmd::operator==(const AniFrameCmd &o) const {
    if(cmd_type != o.cmd_type) {
        return false;
    }
    if(mul_color != o.mul_color) {
        return false;
    }
    if(add_color != o.add_color) {
        return false;
    }
    if(res_id != o.res_id) {
        return false;
    }
    if(matrix != o.matrix) {
        return false;
    }
    if(res_name != o.res_name) {
        return false;
    }
    return true;
}

bool AniFrameCmd::operator!=(const AniFrameCmd &o) const {
    return !(*this == o);
}

bool AniMaskData::operator==(const AniMaskData &o) const {
    if(mask != o.mask) {
        return false;
    }
    if(clip_list != o.clip_list) {
        return false;
    }
    return true;
}
bool AniMaskData::operator!=(const AniMaskData &o) const {
    return !(*this == o);
}

bool AniStateData::operator==(const AniStateData &o) const {
    if(frame != o.frame) {
        return false;
    }
    if(name != o.name) {
        return false;
    }
    return true;
}

bool AniStateData::operator!=(const AniStateData &o) const {
    return !(*this == o);
}
}   // namespace sora

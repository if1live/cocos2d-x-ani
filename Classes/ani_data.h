// Ŭnicode please
#pragma once

#include <vector>
#include <string>
#include "ani_globals.h"
#include <glm/glm.hpp>

namespace cocos2d {;
class CCSpriteFrame;
}

namespace sora {;

enum {
    kFrameCmd_None,
    kFrameCmd_Hide,
    kFrameCmd_Apply,
    kFrameCmd_Show,
};

struct AniData;
struct AniFrameData;
struct AniFrameCmd;
struct AniMaskData;
struct AniStateData;

struct AniData {
    int version;
    std::string frame_file_name;
    std::string texture_file_name;
    int fps;
    int num_frame;

    std::vector<AniFrameData> frame_list;
    std::vector<AniMaskData> mask_list;
    std::vector<AniStateData> state_list;

    AniData() : version(0), fps(0), num_frame(0) {}

    bool operator==(const AniData &o) const;
    bool operator!=(const AniData &o) const;
};

struct AniFrameData {
    int number;
    std::vector<AniFrameCmd> cmd_list;
    AniFrameData() : number(0) {}
    bool operator==(const AniFrameData &o) const;
    bool operator!=(const AniFrameData &o) const;
};

struct AniFrameCmd {
    int cmd_type;
    AniColor4ub mul_color;
    AniColor4ub add_color;

    int res_id;
    glm::mat3 matrix;

    //show에만 유효
    int z;
    std::string res_name;

    AniFrameCmd();
    bool operator==(const AniFrameCmd &o) const;
    bool operator!=(const AniFrameCmd &o) const;
};

struct AniMaskData {
    int mask;
    std::vector<int> clip_list;

    AniMaskData() : mask(0) {}

    bool operator==(const AniMaskData &o) const;
    bool operator!=(const AniMaskData &o) const;
};

struct AniStateData {
    int frame;
    std::string name;	//대체 인코딩이 뭔지 모르겟지만 일단 xml에 잇는걸 그대로 가져다쓰자

    AniStateData() : frame(0) {}

    bool operator==(const AniStateData &o) const;
    bool operator!=(const AniStateData &o) const;
};


}
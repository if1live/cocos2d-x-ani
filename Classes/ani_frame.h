// Ŭnicode please
#pragma once

#include "glm/glm.hpp"
#include <vector>
#include "ani_globals.h"

namespace sora {;

class Ani;

//command가 여러 종류된다고 상속을 넣으면 virtual질을 해야하는데
//메모리 관리나 그런게 영 귀찮아서 그냥 enum질로 구분함
class AniFrameCommand {
public:
    static AniFrameCommand Hide(int resid);
    static AniFrameCommand Show(int resid, const glm::mat3 &m);
    static AniFrameCommand Apply(int resid, const glm::mat3 &m);
    static AniFrameCommand Show(int resid, const glm::mat3 &m, const AniColor4ub &mul, const AniColor4ub &add);
    static AniFrameCommand Apply(int resid, const glm::mat3 &m, const AniColor4ub &mul, const AniColor4ub &add);

public:
    AniFrameCommand(int type, int resid, const glm::mat3 &m, const AniColor4ub &mul, const AniColor4ub &add);
    AniFrameCommand();
    ~AniFrameCommand();

    int type() const { return type_; }
    int resid() const { return resid_; }
    const glm::mat3 &mat() const { return mat_; }

    const AniColor4ub &mul_color() const { return mul_; }
    const AniColor4ub &add_color() const { return add_; }

    void Run(Ani *ani);

private:
    //써놓고 나니 사실상 대부분이 공용 속성이네
    int type_;
    int resid_;
    glm::mat3 mat_;	//hide에는 필요없다

    //프레임에 blend속성이 추가되면 쓴다
    AniColor4ub mul_;
    AniColor4ub add_;

private:
    void Run_Hide(Ani *ani);
    void Run_Show(Ani *ani);
    void Run_Apply(Ani *ani);
};

class AniFrame {
public:
    typedef std::vector<AniFrameCommand> CmdListType;
public:
    AniFrame() : frame_idx_(0) {}
    AniFrame(int frame_idx, int cmd_count) : frame_idx_(frame_idx) { cmd_list_.reserve(cmd_count); }
    ~AniFrame() {}

    void AddCmd(const AniFrameCommand &cmd) { cmd_list_.push_back(cmd); }
    void Run(Ani *ani);
    int frame_idx() const { return frame_idx_; }

private:
    int frame_idx_;
    CmdListType cmd_list_;
};
}

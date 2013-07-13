// Ŭnicode please
#include "stdafx.h"
#include "ani_frame.h"

#include "ani.h"
#include "ani_prototype.h"
#include "ani_data.h"
#include "ani_resource.h"

#if SR_USE_PCH == 0

#endif

using namespace glm;

namespace sora {;
AniFrameCommand AniFrameCommand::Hide(int resid) {
    mat3 identity;

    static AniColor4ub mul(0, 0, 0, 0);
    static AniColor4ub add(0, 0, 0, 0);
    return AniFrameCommand(kFrameCmd_Hide, resid, identity, mul, add);
}
AniFrameCommand AniFrameCommand::Show(int resid, const glm::mat3 &m) {
    static AniColor4ub mul;
    static AniColor4ub add;
    return AniFrameCommand(kFrameCmd_Show, resid, m, mul, add);
}
AniFrameCommand AniFrameCommand::Apply(int resid, const glm::mat3 &m) {
    static AniColor4ub mul;
    static AniColor4ub add;
    return AniFrameCommand(kFrameCmd_Apply, resid, m, mul, add);
}

AniFrameCommand AniFrameCommand::Show(int resid, const glm::mat3 &m, const AniColor4ub &mul, const AniColor4ub &add) {
    return AniFrameCommand(kFrameCmd_Show, resid, m, mul, add);
}

AniFrameCommand AniFrameCommand::Apply(int resid, const glm::mat3 &m, const AniColor4ub &mul, const AniColor4ub &add) {
    return AniFrameCommand(kFrameCmd_Apply, resid, m, mul, add);
}

AniFrameCommand::AniFrameCommand(int type, int resid, const glm::mat3 &m, const AniColor4ub &mul, const AniColor4ub &add)
    : type_(type), 
    resid_(resid), 
    mat_(m),
    mul_(mul),
    add_(add)
{
}
AniFrameCommand::AniFrameCommand()
    : type_(kFrameCmd_None), resid_(-1)
{
}
AniFrameCommand::~AniFrameCommand()
{
}

void AniFrameCommand::Run(Ani *ani) {
    switch(type_) {
    case kFrameCmd_Apply:
        Run_Apply(ani);
        break;
    case kFrameCmd_Hide:
        Run_Hide(ani);
        break;
    case kFrameCmd_Show:
        Run_Show(ani);
        break;
    default:
        SR_ASSERT(!"do not reach");
    }
}

void AniFrameCommand::Run_Hide(Ani *ani) {
    AniResource *res = ani->GetResource(resid_);
    if(res == NULL) {
        return;
    }

    res->set_visible(false);
}

void AniFrameCommand::Run_Show(Ani *ani) {
    AniResource *res = ani->GetResource(resid_);
    if(res == NULL) {
        return;
    }

    bool is_mask = ani->prototype()->IsMaskResId(resid_);
    if(is_mask == false) {
        res->set_visible(true);
    }
    res->set_matrix(mat_);

    int int_r = (int)mul_.r + (int)add_.r;
    int int_g = (int)mul_.g + (int)add_.g;
    int int_b = (int)mul_.b + (int)add_.b;
    int int_a = (int)mul_.a + (int)add_.a;

    if(int_r > 255) {
        int_r = 255;
    }
    if(int_g > 255) {
        int_g = 255;
    }
    if(int_b > 255) {
        int_b = 255;
    }
    if(int_a > 255) {
        int_a = 255;
    }

    unsigned char r = int_r;
    unsigned char g = int_g;
    unsigned char b = int_b;
    unsigned char a = int_a;
    AniColor4ub color(r, g, b, a);
    res->set_color(color);
}

void AniFrameCommand::Run_Apply(Ani *ani) {
    AniResource *res = ani->GetResource(resid_);
    if(res == NULL) {
        return;
    }

    res->set_matrix(mat_);

    int int_r = (int)mul_.r + (int)add_.r;
    int int_g = (int)mul_.g + (int)add_.g;
    int int_b = (int)mul_.b + (int)add_.b;
    int int_a = (int)mul_.a + (int)add_.a;

    if(int_r > 255) {
        int_r = 255;
    }
    if(int_g > 255) {
        int_g = 255;
    }
    if(int_b > 255) {
        int_b = 255;
    }
    if(int_a > 255) {
        int_a = 255;
    }

    unsigned char r = int_r;
    unsigned char g = int_g;
    unsigned char b = int_b;
    unsigned char a = int_a;
    AniColor4ub color(r, g, b, a);
    res->set_color(color);
}

/////////////////////////////////////////

void AniFrame::Run(Ani *ani) {
    auto it = cmd_list_.begin();
    auto endit = cmd_list_.end();
    for( ; it != endit; ++it) {
        it->Run(ani);
    }
}

}

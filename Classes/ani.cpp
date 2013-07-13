// Ŭnicode please
#include "stdafx.h"
#include "ani.h"
#include "ani_frame.h"
#include "ani_prototype.h"

#include <vector>

using namespace std;

namespace sora {;


Ani::Ani()
    : is_paused_(false),
    prototype_(NULL),
    curr_index_(0),
    remain_time_(0)
{
}

Ani::Ani(AniPrototype *prototype)
    : is_paused_(false),
    prototype_(prototype),
    curr_index_(0),
    remain_time_(0)
{
    res_dict_ = prototype->GetResourceDict();

    ForceGoTo(0);
}
Ani::~Ani()
{
}
void Ani::Update(float dt)
{
    if(is_paused_ == true) {
        return;
    }

    float speed = play_param_.speed;
    float fps = static_cast<float>(prototype_->fps());
    float frame_delta = 1.0f / fps / fabs(speed);
    remain_time_ += dt;

    if(speed == 0) {
        return;
    } else if(speed > 0) {
        while(remain_time_ > 0) {
            GoNextFrame();
            remain_time_ -= frame_delta;
        }
    } else {
        //역방향 재생. 모든 프레임을 역으로 재생해야한다 귀찮지만 답이 없다-_-
        //그래서 버림
        IUASSERT(!"do not support reverse ani play");
    }
}

void Ani::SetPlayParam(const AniPlayParam &param)
{
    play_param_ = param;
    //재생속성 설정후 추가로 작업할 내용이 상황에 따라서 생길수 잇을거같은데
    if(play_param_.from_index < 0) {
        play_param_.from_index = 0;
    }
    int num_frame = prototype_->num_frame();
    if(play_param_.to_index >= num_frame) {
        play_param_.to_index = num_frame - 1;
    }

    ForceGoTo(play_param_.from_index); 
}

void Ani::ForceGoTo(int index)
{
    int num_frame = prototype_->num_frame();
    int target_index = 0;
    if(index < 0) {
        target_index = 0;
    } else if(index >= num_frame) {
        target_index = num_frame - 1;
    } else {
        target_index = index;
    }
    if(target_index < play_param_.from_index) {
        target_index = play_param_.from_index;
    } else if(target_index > play_param_.to_index) {
        target_index = play_param_.to_index;
    }

    //0프레임 재생전에는 모든것이 not visible
    for(auto it = res_dict_.begin(), e = res_dict_.end() ; it != e ; ++it) {
        it->second.set_visible(false);
    }
    //mask로 선언된것은 안보여야한다
    vector<int> mask_res_list = prototype_->GetMaskResIdList();
    for(auto it = mask_res_list.begin(), e = mask_res_list.end() ; it != e ; ++it) {
        int res_id = *it;
        AniResource *res = GetResource(res_id);
        if(res != NULL) {
            res->set_visible(false);
        }
    }

    AniPrototype::FrameDictType::iterator frame_it = prototype_->FrameBegin();
    AniPrototype::FrameDictType::iterator frame_endit = prototype_->FrameEnd();
    for( ; frame_it != frame_endit ; frame_it++) {
        int frame_index = frame_it->first;
        AniFrame &frame = frame_it->second;
        if(frame_index <= target_index) {
            frame.Run(this);
        } else {
            break;
        }
    }
    curr_index_ = target_index;
}
void Ani::MoveFirstFrame()
{
    ForceGoTo(0);
}
void Ani::MoveLastFrame()
{
    int num_frame = prototype_->num_frame();
    ForceGoTo(num_frame - 1);
}

void Ani::GoNextFrame()
{
    if(IsEnd() == true) {
        return;
    }

    int num_frame = prototype_->num_frame();
    int next_index = curr_index_ + 1;
    if(next_index > play_param_.to_index || next_index >= num_frame) {
        //end occur!
        play_param_.pass_loop_count++;
        next_index = play_param_.from_index;
        //종료시에는 종료 프레임으로 냅두면 됨
        if(IsEnd() == true) {
            return;
        }
    }

    if(curr_index_ + 1 == next_index) {
        //바로 다음장이면 다음 프레임 정보를 재생하면 끝
        AniFrame *frame = prototype_->GetFrame(next_index);
        if(frame != NULL) {
            frame->Run(this);
        }
        curr_index_ = next_index;
    } else {
        //처음부터 next로 강제 이동
        ForceGoTo(next_index);
        curr_index_ = next_index;
    }
    //if(info_.num_frame < 20) { printf("%d\n", curr_index_); }
}

bool Ani::IsEnd() const
{
    if(play_param_.infinite_loop == true) {
        return false;
    }

    if(play_param_.pass_loop_count >= play_param_.total_loop_count) {
        return true;
    } else {
        return false;
    }
}

AniResource *Ani::GetResource(int resid)
{
    auto found = res_dict_.find(resid);
    if(found == res_dict_.end()) {
        return NULL;
    } else {
        return &found->second;
    }
}
const AniResource *Ani::GetResource(int resid) const
{
    auto found = res_dict_.find(resid);
    if(found == res_dict_.end()) {
        return NULL;
    } else {
        return &found->second;
    }
}

}

// Ŭnicode please
#pragma once

#include "ani_frame.h"
#include "ani_resource.h"

#if SR_USE_PCH == 0
#include <map>
#endif

namespace sora {;

struct AniPlayParam {
    AniPlayParam() { Reset(); }
    void Reset() {
        speed = 1;
        from_index = 0;
        to_index = 1000000;	//설마 100000장 이상으로 구성된게 있을까
        infinite_loop = true;
        total_loop_count = 1;
        pass_loop_count = 0;
    }

    float speed;
    //구간 반복에 필요한 값
    int from_index;
    int to_index;
    //무한 재생을 on/off
    bool infinite_loop;
    //무한재생을 하지 않을경우 몇회?
    //역재생으로 할 경우 1 더 넣어준다
    //1을 추가로 넣는 이유는
    //애니재생에서 최초 프레임을 따로 지정해주지 않는다면, 최초 프레임은 0프레임이고
    //여기에서 역재생하면 바로 마지막 프레임으로 이동해서 한번에 1카운터가 올가가기 때문이다
    int total_loop_count;
    int pass_loop_count;	//이미 재생된 횟수
};

//애니메이션용 인터페이스. 재생/정지/기타 잡탕을 정의
struct AniInterface {
public:
    AniInterface() {}
    virtual ~AniInterface() {}

public:
    //재생 통제
    virtual void Resume() = 0;
    virtual void Pause() = 0;
    virtual bool IsPaused() const = 0;

    virtual void Update(float dt) = 0;

    virtual void SetPlayParam(const AniPlayParam &param) = 0;
    virtual const AniPlayParam &GetPlayParam() const = 0;

    virtual void ForceGoTo(int index) = 0;
    virtual void MoveFirstFrame() = 0;
    virtual void MoveLastFrame() = 0;

    virtual void GoNextFrame() = 0;
    virtual bool IsEnd() const = 0;
};

class AniPrototype;

class Ani : public AniInterface {
public:
    typedef std::map<int, AniResource> ResDictType;

public:
    Ani();
    Ani(AniPrototype *prototype);
    virtual ~Ani();

    virtual void Resume() { is_paused_ = false; }
    virtual void Pause() { is_paused_ = true; }
    virtual bool IsPaused() const { return is_paused_; }

    virtual void Update(float dt);

    virtual void SetPlayParam(const AniPlayParam &param);
    virtual const AniPlayParam &GetPlayParam() const { return play_param_; }

    //애니메이션 내부 정보
    //특정 프레임으로 강제 이동. 처음부터 원하는 프레임까지 통째로 재생하면됨
    virtual void ForceGoTo(int index);
    virtual void MoveFirstFrame();
    virtual void MoveLastFrame();

    virtual void GoNextFrame();
    virtual bool IsEnd() const;

public:
    AniResource *GetResource(int resid);
    const AniResource *GetResource(int resid) const;
    const ResDictType &GetResourceDict() const { return res_dict_; }
    ResDictType &GetResourceDict() { return res_dict_; }
    AniPrototype *prototype() { return prototype_; }

private:
    //res는 각각이 고유한 id를 가진다
    //자동생성되는 id는 아마도 1부터 순서대로 증가할테니까
    //적절히 배열화해서 벡터에 넣으면 O(1)로 접근할수 잇으니
    //적절할듯하다
    ResDictType res_dict_;
    AniPrototype *prototype_;

    int curr_index_;

    //애니메이션 재생에 필요한것들
private:
    bool is_paused_;
    AniPlayParam play_param_;

    float remain_time_;	//다음프레임 갱신까지 남은 시간(sec). 이것으로 고정프레임 흉내내자
};
}
// Ŭnicode please
#pragma once

#include "cocos2d.h"
#include "ani.h"

#if SR_USE_PCH == 0
#include <memory>
#endif

namespace sora {;

typedef enum {
	kVertexLeftBottom,
	kVertexRightBottom,
	kVertexRightTop,
	kVertexLeftTop,
} VertexDirectionType;

struct AniVertex {
	float x, y;
	float s, t;
	unsigned char r, g, b, a;
	void SetVertex(float x, float y, cocos2d::CCSpriteFrame *frame, VertexDirectionType dir);
};

struct AniQuad {
	//3 2
	//0 1
	AniVertex vert[4];
};


class SimpleAniNode : public AniInterface, public cocos2d::CCNodeRGBA {
public:
    SimpleAniNode();
    virtual ~SimpleAniNode();

	bool initWithPrototype(AniPrototype *prototype);

private:
    virtual void update(float dt);
    virtual void draw();

	//ani 구현부
public:
	virtual void Resume() { ani_->Resume(); }
	virtual void Pause() { ani_->Pause(); }
	virtual bool IsPaused() const { return ani_->IsPaused(); }

	virtual void Update(float dt) { ani_->Update(dt); }
	
	virtual void SetPlayParam(const AniPlayParam &param) { ani_->SetPlayParam(param); }
	virtual const AniPlayParam &GetPlayParam() const { return ani_->GetPlayParam(); }

	virtual void ForceGoTo(int index) { ani_->ForceGoTo(index); }
	virtual void MoveFirstFrame() { ani_->MoveFirstFrame(); }
	virtual void MoveLastFrame() { ani_->MoveLastFrame(); }

	virtual void GoNextFrame() { ani_->GoNextFrame(); }
	virtual bool IsEnd() const { return ani_->IsEnd(); }

	AniPrototype *prototype();

protected:
	Ani *ani() { return ani_.get(); }
	void set_ani(Ani *ani) { ani_.reset(ani); }

private:
	std::unique_ptr<Ani> ani_;
};

}	// namespace sora
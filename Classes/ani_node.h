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


class AniNode : public cocos2d::CCNode, public AniInterface, public cocos2d::CCRGBAProtocol {
public:
    AniNode();
    virtual ~AniNode();

    bool initWithPrototype(AniPrototype *prototype, int tex_w, int tex_h);
	bool initWithPrototype(AniPrototype *prototype);
	virtual void Init(int tex_w, int tex_h) = 0;

private:
    virtual void update(float dt);
    virtual void draw();

    //rgba 구현부
public:
	virtual void setOpacityModifyRGB(bool bValue) {}
	virtual bool isOpacityModifyRGB() { return false; }
	
	virtual void setOpacity(GLubyte opacity) { alpha_ = opacity; }
	virtual GLubyte getOpacity(void) { return alpha_; }
	virtual void setColor(const cocos2d::ccColor3B& color) { color_ = color; }
	virtual const cocos2d::ccColor3B& color(void) { return color_; }

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

	virtual void DrawOnMainRT() = 0;

private:
	cocos2d::ccColor3B color_;
	unsigned char alpha_;
	std::unique_ptr<Ani> ani_;

	//알패를 적용하기 위해서 어쨋든 rt는 거쳐야한다
	cocos2d::CCRenderTexture *rt_;
};


class SimpleAniNode : public AniNode {
public:
	SimpleAniNode();
	virtual ~SimpleAniNode();

	static void SimpleDraw(Ani *ani);

	//node 구현부
public:
	virtual void Init(int tex_w, int tex_h);
private:
	virtual void DrawOnMainRT();
};

}

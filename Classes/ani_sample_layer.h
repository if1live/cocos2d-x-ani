// Ŭnicode please 
#pragma once

namespace sora {;
class SimpleAniNode;
}

class AniSampleLayer : public cocos2d::CCLayer {
public:
	virtual bool init();

	static cocos2d::CCScene *scene();
	void update(float dt);

	CREATE_FUNC(AniSampleLayer);

private:
	enum {
		kNormalAni,
		kLoopCountAni,
		kPauseAni,
		kFrameLimitAni,
		kSpeedAni,
		kAniCount,
	};
	std::array<sora::SimpleAniNode*, kAniCount> ani_list_;
};
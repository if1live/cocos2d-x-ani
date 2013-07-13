// Ŭnicode please 
#pragma once

namespace sora {;
struct AniInterface;
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
		kRGBAAni,
		kAniCount,
	};
	std::array<sora::AniInterface*, kAniCount> ani_list_;
};
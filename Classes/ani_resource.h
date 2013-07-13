// Ŭnicode please
#pragma once

#include "glm/glm.hpp"
#include "ani_globals.h"

#if SR_USE_PCH == 0
#include "cocos2d.h"
#endif

namespace sora {;
//이미지 1장을 의미. 마스킹용으로 쓰일수도 잇고 그냥 렌더링용으로 쓰일수도 잇다
//TODO frame을 포인터 대신 내용 자체로 보관하는거 검토하기
class AniResource {
public:
	AniResource(int id, int z, cocos2d::CCSpriteFrame *frame)
		: id_(id), z_(z), frame_(frame), visible_(true) {}
	AniResource() : id_(-1), z_(0), frame_(NULL), visible_(false) {}
	~AniResource() {}

	int id() const { return id_; }
	int z() const { return z_; }
	bool visible() const { return visible_; }
	void set_visible(bool b) { visible_ = b; }
	void set_matrix(const glm::mat3 &m) { mat_ = m; }
	const glm::mat3 &mat() const { return mat_; }
	const cocos2d::CCSpriteFrame *frame() const { return frame_; }
	cocos2d::CCSpriteFrame *frame() { return frame_; }

	const AniColor4ub &color() const { return color_; }
	AniColor4ub &color() { return color_; }
	void set_color(const AniColor4ub &c) { color_ = c; }

private:
	int id_;
	int z_;
	cocos2d::CCSpriteFrame *frame_;
	bool visible_;
	glm::mat3 mat_;

	AniColor4ub color_;

};

struct AniResourceZSorter {
	int operator()(const AniResource *a, const AniResource *b) {
		return (a->z() < b->z());
	}
};

}
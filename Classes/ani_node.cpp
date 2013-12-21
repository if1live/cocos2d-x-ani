// Ŭnicode please
#include "stdafx.h"
#include "ani_node.h"
#include "ani_prototype.h"
#include "gl_helper.h"


#if SR_USE_PCH == 0
#include <vector>
#include <algorithm>
#endif

using namespace std;
using namespace cocos2d;
using namespace glm;

namespace sora {;

void AniVertex::SetVertex(float x, float y, cocos2d::CCSpriteFrame *frame, VertexDirectionType dir) {
	this->x = x;
	this->y = y;

	SR_ASSERT(frame->isRotated() == false);
	float tex_width = static_cast<float>(frame->getTexture()->getPixelsWide());
	float tex_height = static_cast<float>(frame->getTexture()->getPixelsHigh());

	//point좌표계더라. 근데 내부 계산은 픽
	float atlas_x = frame->getRectInPixels().origin.x;
	float atlas_y = frame->getRectInPixels().origin.y;
	float atlas_w = frame->getOriginalSizeInPixels().width;
	float atlas_h = frame->getOriginalSizeInPixels().height;

	if (dir == kVertexLeftTop) {
		s = atlas_x;
		t = atlas_y;
	}
	else if (dir == kVertexRightTop) {
		s = atlas_x + atlas_w;
		t = atlas_y;
	}
	else if (dir == kVertexRightBottom) {
		s = atlas_x + atlas_w;
		t = atlas_y + atlas_h;
	}
	else if (dir == kVertexLeftBottom) {
		s = atlas_x;
		t = atlas_y + atlas_h;
	}
	s = s / tex_width;
	t = (t / tex_height);

	r = 255;
	g = 255;
	b = 255;
	a = 255;
}

SimpleAniNode::SimpleAniNode(bool alpha, bool ignore_color)
: alpha_(alpha)
{
	//쉐이더 미리 설정
	CCGLProgram *prog = nullptr;
	if (alpha) {
		if (ignore_color) {
			prog = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureA8Color);
		} else {
			prog = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor);
		}
	} else {
		prog = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture);
	}
	CCAssert(prog != nullptr, "prog need");
	setShaderProgram(prog);
}

SimpleAniNode::~SimpleAniNode()
{
}

bool SimpleAniNode::initWithPrototype(AniPrototype *prototype, int w, int h)
{
	return initWithPrototype(prototype);
}
bool SimpleAniNode::initWithPrototype(AniPrototype *prototype)
{
	scheduleUpdate();

	//orig ani를 복제해서 ani로 대신쓴다. 프로토타입 패턴의 형태를 그냥 갖다쓰자
	//중간에 파서같은걸 추가로 거치는게 더 
	this->set_ani(new Ani(prototype->Create()));

	return true;
}

AniPrototype *SimpleAniNode::prototype()
{
	return ani_->prototype();
}

void SimpleAniNode::update(float dt)
{
	static const float my_dt = 1 / 60.0f; //added by chorice. 패턴 상태와 정확히 싱크를 이루기 위해서.
	ani()->Update(my_dt * tempo());
}

void SimpleAniNode::draw()
{
	if (isVisible() == false) {
		return;
	}

	vector<AniQuad> quad_list;
	vector<unsigned short> index_list;

	IUASSERT(ani() != NULL);
	Ani::ResDictType &res_dict = ani()->GetResourceDict();

	vector<AniResource> res_list;
	for (auto it = res_dict.begin(), e = res_dict.end(); it != e; ++it) {
		if (it->second.visible() == true) {
			res_list.push_back(*&it->second);
		}
	}
	quad_list.reserve(res_list.size());

	std::sort(res_list.begin(), res_list.end(), AniResourceZSorter());

	//visible인것에 대해서 그리기 준비하기
	for (auto it = res_list.begin(), e = res_list.end(); it != e; ++it) {
		AniResource &res = *it;

		const mat3 &matrix = res.mat();
		CCSpriteFrame *frame = res.frame();
		IUASSERT(frame != NULL);

		AniQuad quad;
		float sprite_width = frame->getRect().size.width;
		float sprite_height = frame->getRect().size.height;

		vec3 v0(0, -sprite_height, 1);
		vec3 v1(sprite_width, -sprite_height, 1);
		vec3 v2(sprite_width, 0, 1);
		vec3 v3(0, 0, 1);
		v0 = matrix * v0;
		v1 = matrix * v1;
		v2 = matrix * v2;
		v3 = matrix * v3;

		quad.vert[0].SetVertex(v0.x, v0.y, frame, kVertexLeftBottom);
		quad.vert[1].SetVertex(v1.x, v1.y, frame, kVertexRightBottom);
		quad.vert[2].SetVertex(v2.x, v2.y, frame, kVertexRightTop);
		quad.vert[3].SetVertex(v3.x, v3.y, frame, kVertexLeftTop);

		//색깔 정보 계산하기
		const AniColor4ub &color = res.color();
		const auto &node_color = this->getColor();
		unsigned char node_alpha = this->getOpacity();

		for (int i = 0; i < 4; i++) {
			AniVertex &vert = quad.vert[i];
			vert.r = (unsigned char)(color.r * node_color.r / 255.0f);
			vert.g = (unsigned char)(color.g * node_color.g / 255.0f);
			vert.b = (unsigned char)(color.b * node_color.b / 255.0f);
			vert.a = (unsigned char)(color.a * node_alpha / 255.0f);
		}

		int orig_quad_size = quad_list.size();
		quad_list.push_back(quad);
		index_list.push_back(orig_quad_size * 4 + 0);
		index_list.push_back(orig_quad_size * 4 + 1);
		index_list.push_back(orig_quad_size * 4 + 2);

		index_list.push_back(orig_quad_size * 4 + 0);
		index_list.push_back(orig_quad_size * 4 + 2);
		index_list.push_back(orig_quad_size * 4 + 3);
	}

	int quad_size = quad_list.size();
	if (quad_size == 0) {
		return;
	}

	////////////////////////////////////
	CC_NODE_DRAW_SETUP();

	CCSpriteFrame *frame = (res_dict.begin())->second.frame();
	IUASSERT(frame != NULL);
	GLuint tex_id = frame->getTexture()->getName();
	ccGLBindTexture2D(tex_id);

	if (alpha_ == false) {
		ccGLBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	}

	//Remember, No Old GL.
	//ani는 low-level이기떄문에 retina같은거 수동으로 통제해야됨

	kmGLPushMatrix();
	//깊이를 씹고 그려도 문제없다. 어차피 순서대로 그리는거니까
	//CCDirector::sharedDirector()->setDepthTest(false);

	//float scale = CCDirector::sharedDirector()->getContentScaleFactor();
	//kmGLScalef(scale, scale, 1);

	if (alpha_ == false) {
		ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords);

		AniQuad &first_quad = quad_list[0];
		glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(AniVertex), &first_quad.vert[0].x);
		glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(AniVertex), &first_quad.vert[0].s);
		glDrawElements(GL_TRIANGLES, index_list.size(), GL_UNSIGNED_SHORT, &index_list[0]);
	} else {
		ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex);
		AniQuad &first_quad = quad_list[0];
		glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(AniVertex), &first_quad.vert[0].x);
		glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(AniVertex), &first_quad.vert[0].s);
		glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(AniVertex), &first_quad.vert[0].r);
		glDrawElements(GL_TRIANGLES, index_list.size(), GL_UNSIGNED_SHORT, &index_list[0]);
	}

	kmGLPopMatrix();
	ccGLBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	CHECK_GL_ERROR_DEBUG();
	CC_INCREMENT_GL_DRAWS(1);
}

RGBAAniNode::RGBAAniNode()
	: rt_(nullptr),
	ani_node_(nullptr),
	tex_height_(0),
	tex_width_(0)
{
}
RGBAAniNode::~RGBAAniNode()
{
	ani_node_->retain();
}

bool RGBAAniNode::initWithPrototype(AniPrototype *prototype)
{
	return initWithPrototype(prototype, 128, 128);
}
bool RGBAAniNode::initWithPrototype(AniPrototype *prototype, int w, int h)
{
	scheduleUpdate();
	
	int tex_width = w * (int)CC_CONTENT_SCALE_FACTOR();
	int tex_height = h * (int)CC_CONTENT_SCALE_FACTOR();
	rt_ = CCRenderTexture::create(tex_width, tex_height);
	this->addChild(rt_);
	rt_->getSprite()->getTexture()->setAntiAliasTexParameters();

	//rt + simple => rgba support
	ani_node_ = new SimpleAniNode();
	ani_node_->initWithPrototype(prototype);

	tex_width_ = tex_width;
	tex_height_ = tex_height;

	//쉐이더 미리 설정
	CCGLProgram *prog = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor);
	setShaderProgram(prog);

	return true;
}

void RGBAAniNode::update(float dt)
{
	ani_node_->Update(dt);
}
void RGBAAniNode::draw()
{
	if (isVisible() == false) {
		return;
	}
	if (getOpacity() == 0) {
		// 완전투명==그릴필요 있나?
		return;
	}

	rt_->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);

	kmGLPushMatrix();
	kmGLTranslatef(tex_width_ * 0.5f, tex_height_ * 0.5f, 0);
	ani_node_->draw();
	kmGLPopMatrix();

	rt_->end();

	CCSprite *sprite = rt_->getSprite();
	sprite->setColor(getColor());
	ccBlendFunc blend_func = { GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA };
	sprite->setBlendFunc(blend_func);
	sprite->setOpacity(getOpacity());

	CHECK_GL_ERROR_DEBUG();
}

void RGBAAniNode::reset()
{
	ani_node_->release();
	tex_height_ = -1;
	tex_width_ = -1;
}


}	// namespace sora
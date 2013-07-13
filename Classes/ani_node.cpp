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

    if(dir == kVertexLeftTop) {
        s = atlas_x;
        t = atlas_y;
    } else if(dir == kVertexRightTop) {
        s = atlas_x + atlas_w;
        t = atlas_y;
    } else if(dir == kVertexRightBottom) {
        s = atlas_x + atlas_w;
        t = atlas_y + atlas_h;
    } else if(dir == kVertexLeftBottom) {
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

AniNode::AniNode()
    : rt_(NULL)
{
}

AniNode::~AniNode()
{
    if(rt_ != NULL) {
        rt_->release();
        rt_ = NULL;
    }
}

bool AniNode::initWithPrototype(AniPrototype *prototype)
{
    int default_tex_width = 128;
    int default_tex_height = 128;
    return initWithPrototype(prototype, default_tex_width, default_tex_height);
}

bool AniNode::initWithPrototype(AniPrototype *prototype, int tex_w, int tex_h)
{
	scheduleUpdate();
    //orig ani를 복제해서 ani로 대신쓴다. 프로토타입 패턴의 형태를 그냥 갖다쓰자
    //중간에 파서같은걸 추가로 거치는게 더 
    this->set_ani(new Ani(prototype->Create()));

    //애니메이션이 얼마나 커질지를 알수가 없다. 그냥 감으로 텍스쳐를 생성해야 하나?
    rt_ = new CCRenderTexture();
    int tex_width = 0;
    int tex_height = 0;
    tex_width = tex_w * 2;
    tex_height = tex_h * 2;
    /*
    if(qb::Device::GetInstance().IsIPhoneFamily()) {
    tex_width = tex_w * (int)CC_CONTENT_SCALE_FACTOR();
    tex_height = tex_h * (int)CC_CONTENT_SCALE_FACTOR();
    } else {
    tex_width = tex_w * 2;
    tex_height = tex_h * 2;
    }
    */
    //rt_->initWithWidthAndHeightWithoutDepth(tex_width, tex_height, kCCTexture2DPixelFormat_RGBA8888);
    rt_->initWithWidthAndHeight(tex_width, tex_height, kCCTexture2DPixelFormat_RGBA8888);
    this->addChild(rt_);
    rt_->getSprite()->getTexture()->setAntiAliasTexParameters();

    //상속받은것의 경우 추가 작업을 시행
    Init(tex_width, tex_height);

    return true;
}

AniPrototype *AniNode::prototype()
{
    return ani_->prototype();
}

void AniNode::update(float dt)
{
    ani()->Update(dt);
}

void AniNode::draw()
{
    //RT사용해서 렌더링하는게 잘 안된다. 일단 빼서 렌더링하고 해놓고 추후 고치기
    DrawOnMainRT();

    CCSprite *sprite = rt_->getSprite();
    /*
    //rt갱신은 draw에하는게 프로파일링에는 더 맞겟지?

    if(sprite->isVisible() == true) {
    //rt_->beginWithClear(1, 1, 0, 1);
    //rt_->beginWithClear(0, 0, 0, 0);
    kmGLPushMatrix();

    static bool init = false;
    static kmMat4 matrix_data;
    if(init == false) {
    init = true;
    kmGLGetMatrix(KM_GL_MODELVIEW, &matrix_data);
    }

    kmGLLoadIdentity();
    float scale_x = matrix_data.mat[0];
    float scale_y = matrix_data.mat[5];
    kmGLScalef(scale_x, scale_y, 1);

    float tex_width = rt_->getSprite()->getTexture()->getContentSize().width;
    float tex_height = rt_->getSprite()->getTexture()->getContentSize().height;
    IUASSERT(tex_width > 0);
    IUASSERT(tex_height > 0);

    if(CC_CONTENT_SCALE_FACTOR() == 2.0f) {
    kmGLTranslatef(tex_width, tex_height, 0);
    } else {
    kmGLTranslatef(tex_width/2.0f, tex_height/2.0f, 0);
    }

    DrawOnMainRT();
    kmGLPopMatrix();
    //rt_->end();
    }
    */

    sprite->setColor(getColor());
    //손가락은 이거 커면 잘 나오는데 다른 일반 애니는 이거 켜면 경계가 이상해진다?
    /*
    if(alpha_ == 255) {
    //m_sBlendFunc.src = CC_BLEND_SRC;
    //m_sBlendFunc.dst = CC_BLEND_DST;
    //ccBlendFunc blend_func = { CC_BLEND_SRC, CC_BLEND_DST };
    ccBlendFunc blend_func = { GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA };
    sprite->setBlendFunc(blend_func);
    } else {
    ccBlendFunc blend_func = { GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA };
    sprite->setBlendFunc(blend_func);
    }
    */
    ccBlendFunc blend_func = { GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA };
    sprite->setBlendFunc(blend_func);
    sprite->setOpacity(getOpacity());

    if(getOpacity() == 0) {
        sprite->setVisible(false);
    } else {
        sprite->setVisible(true);
    }
}

//////////////////////////

SimpleAniNode::SimpleAniNode()
{
}
SimpleAniNode::~SimpleAniNode()
{

}

void SimpleAniNode::Init(int tex_w, int tex_h)
{
}

void SimpleAniNode::SimpleDraw(Ani *ani)
{
    vector<AniQuad> quad_list;
    vector<unsigned short> index_list;

    IUASSERT(ani != NULL);
    Ani::ResDictType &res_dict = ani->GetResourceDict();

    vector<AniResource> res_list;
    for(auto it = res_dict.begin(), e = res_dict.end() ; it != e ; ++it) {
        if(it->second.visible() == true) {
            res_list.push_back(*&it->second);
        }
    }
    quad_list.reserve(res_list.size());

    std::sort(res_list.begin(), res_list.end(), AniResourceZSorter());

    //visible인것에 대해서 그리기 준비하기
    for(auto it = res_list.begin(), e = res_list.end() ; it != e ; ++it) {
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
        for(int i = 0 ; i < 4 ; i++) {
            AniVertex &vert = quad.vert[i];
            vert.r = color.r;
            vert.g = color.g;
            vert.b = color.b;
            vert.a = color.a;
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
    if(quad_size == 0) {
        return;
    }

    ////////////////////////////////////

    CCSpriteFrame *frame = (res_dict.begin())->second.frame();
    IUASSERT(frame != NULL);
    GLuint tex_id = frame->getTexture()->getName();
    glBindTexture(GL_TEXTURE_2D, tex_id);

    //Remember, No Old GL.
    //ani는 low-level이기떄문에 retina같은거 수동으로 통제해야됨

    kmGLPushMatrix();
    //float scale = CCDirector::sharedDirector()->getContentScaleFactor();
    //kmGLScalef(scale, scale, 1);

    //색깔까지 지원에 넣으면 아작난다 -_- 좀 고생해서 추적해봐야될듯
    CCGLProgram *prog = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture);
    prog->use();
	prog->setUniformsForBuiltins();
	ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords);

    //깊이를 씹고 그려도 문제없다
    CCDirector::sharedDirector()->setDepthTest(false);

    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    AniQuad &first_quad = quad_list[0];
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(AniVertex), &first_quad.vert[0].x);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(AniVertex), &first_quad.vert[0].s);
    //glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(AniVertex), &first_quad.vert[0].r);
    glDrawElements(GL_TRIANGLES, index_list.size(), GL_UNSIGNED_SHORT, &index_list[0]);

    CCDirector::sharedDirector()->setDepthTest(true);
    kmGLPopMatrix();

	CHECK_GL_ERROR_DEBUG();

}

void SimpleAniNode::DrawOnMainRT()
{
    SimpleDraw(ani());	
}

}

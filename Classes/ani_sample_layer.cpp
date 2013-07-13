﻿// Ŭnicode please 
#include "stdafx.h"
#include "ani_sample_layer.h"

#include "ani_manager.h"
#include "ani_prototype.h"
#include "ani_node.h"

USING_NS_CC;
using namespace sora;

CCScene* AniSampleLayer::scene()
{
	CCScene *scene = CCScene::create();
	AniSampleLayer *layer = AniSampleLayer::create();
	scene->addChild(layer);
	return scene;
}

void AniSampleLayer::update(float dt)
{
	static float elapsed = 0;
	elapsed += dt;

	if(elapsed > 2.0f) {
		AniInterface *pause_ani = ani_list_[kPauseAni];
		if(pause_ani->IsPaused() == false) {
			pause_ani->Pause();
		} else {
			pause_ani->Resume();
		}

		AniInterface *speed_ani = ani_list_[kSpeedAni];
		AniPlayParam param = speed_ani->GetPlayParam();
		if(param.speed == 1.0f) {
			param.speed = 0.5f;
		} else {
			param.speed = 1.0f;
		}
		speed_ani->SetPlayParam(param);

		elapsed = 0;
	}
}

bool AniSampleLayer::init()
{
	if(!CCLayer::init()) {
		return false;
	}
	scheduleUpdate();

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	CCSprite* pSprite = CCSprite::create("HelloWorld.png");
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	this->addChild(pSprite, 0);

	AniManager &ani_mgr = AniManager::GetInstance();
	{
		AniPrototype *ani_prototype_1 = ani_mgr.LoadFile("ani/prop01_movie-clip.xml");
		SimpleAniNode *ani_node = new SimpleAniNode();
		ani_node->initWithPrototype(ani_prototype_1);
		ani_node->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
		this->addChild(ani_node);
		ani_node->release();

		ani_list_[kNormalAni] = ani_node;
	}
	{
		AniPrototype *ani_prototype_1 = ani_mgr.LoadFile("ani/prop02_movie-clip.xml");
		SimpleAniNode *ani_node = new SimpleAniNode();
		ani_node->initWithPrototype(ani_prototype_1);
		ani_node->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
		ani_node->setPosition(ani_node->getPosition() + ccp(100, 0));
		AniPlayParam param;
		param.total_loop_count = 1;
		param.infinite_loop = false;
		ani_node->SetPlayParam(param);

		this->addChild(ani_node);
		ani_node->release();

		ani_list_[kLoopCountAni] = ani_node;
	}

	{
		AniPrototype *ani_prototype_1 = ani_mgr.LoadFile("ani/prop02_movie-clip.xml");
		SimpleAniNode *ani_node = new SimpleAniNode();
		ani_node->initWithPrototype(ani_prototype_1);
		ani_node->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
		ani_node->setPosition(ani_node->getPosition() + ccp(-100, 0));

		this->addChild(ani_node);
		ani_node->release();

		ani_list_[kPauseAni] = ani_node;
	}
	{
		AniPrototype *ani_prototype_1 = ani_mgr.LoadFile("ani/prop01_movie-clip.xml");
		SimpleAniNode *ani_node = new SimpleAniNode();
		ani_node->initWithPrototype(ani_prototype_1);
		ani_node->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
		ani_node->setPosition(ani_node->getPosition() + ccp(0, -100));

		AniPlayParam param;
		param.from_index = 12;
		param.to_index = 24;
		ani_node->SetPlayParam(param);

		this->addChild(ani_node);
		ani_node->release();

		ani_list_[kFrameLimitAni] = ani_node;
	}

	{
		AniPrototype *ani_prototype_1 = ani_mgr.LoadFile("ani/prop01_movie-clip.xml");
		SimpleAniNode *ani_node = new SimpleAniNode();
		ani_node->initWithPrototype(ani_prototype_1);
		ani_node->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
		ani_node->setPosition(ani_node->getPosition() + ccp(+100, +100));

		this->addChild(ani_node);
		ani_node->release();

		ani_list_[kSpeedAni] = ani_node;
	}

	{
		AniPrototype *ani_prototype_1 = ani_mgr.LoadFile("ani/prop01_movie-clip.xml");
		RGBAAniNode *ani_node = new RGBAAniNode();
		ani_node->initWithPrototype(ani_prototype_1);
		ani_node->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
		ani_node->setPosition(ani_node->getPosition() + ccp(0, +100));

		ani_node->setOpacity(128);
		ccColor3B color;
		color.r = 255;
		color.g = 128;
		color.b = 128;
		ani_node->setColor(color);

		this->addChild(ani_node);
		ani_node->release();

		ani_list_[kRGBAAni] = ani_node;
	}
	
	return true;
}

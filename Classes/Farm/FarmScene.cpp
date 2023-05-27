/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#include "FarmScene.h"

#include "Unit/UnitsView.h"

#include <cocos/base/CCDirector.h>
#include <cocos/ui/UIScrollView.h>
#include <cocos/ui/UIButton.h>

FarmScene* FarmScene::create(std::vector<Unit>& units)
{
    FarmScene* scene = new FarmScene(units);
    if (scene && scene->init())
    {
        scene->autorelease();
        return scene;
    }

    CC_SAFE_DELETE(scene);
    return nullptr;
}

FarmScene::FarmScene(std::vector<Unit>& units)
    : BaseScene(units)
{
}

bool FarmScene::init()
{
    if (BaseScene::init() == false)
    {
        return false;
    }

    const cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Node* view = units_view::createView(m_units, cocos2d::Vec2::ZERO, visibleSize);

    view->setAnchorPoint({ 0.0f, 0.0f });

    addChild(view);

    initBack();

    return true;
}
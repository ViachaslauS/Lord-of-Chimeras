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

cocos2d::Scene* FarmScene::create(std::vector<Unit>& units)
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
    cocos2d::ui::ScrollView* view = units_view::createView(m_units, cocos2d::Vec2::ZERO, visibleSize);

    view->setAnchorPoint({ 0.0f, 0.0f });

    addChild(view);

    cocos2d::ui::Button* back_button = cocos2d::ui::Button::create(
        "back_icon.png",
        "back_icon.png",
        "back_icon.png",
        cocos2d::ui::Widget::TextureResType::PLIST
    );

    using TouchType = cocos2d::ui::Widget::TouchEventType;
    back_button->addTouchEventListener([](auto, TouchType event) {
        if (event == TouchType::ENDED)
        {
            cocos2d::Director::getInstance()->popScene();
        }
    });
    back_button->setPosition({ 0.0f, visibleSize.height });
    back_button->setAnchorPoint({ 0.0f, 1.0f });

    addChild(back_button);

    return true;
}
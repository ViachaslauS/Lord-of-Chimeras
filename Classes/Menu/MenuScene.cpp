/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#include "MenuScene.h"

#include "Farm/FarmScene.h"

#include <cocos/base/CCDirector.h>
#include <cocos/ui/UIButton.h>

cocos2d::Scene* MenuScene::create(std::vector<Unit>& units)
{
    MenuScene* scene = new MenuScene(units);
    if (scene && scene->init())
    {
        scene->autorelease();
        return scene;
    }

    CC_SAFE_DELETE(scene);
    return nullptr;
}

MenuScene::MenuScene(std::vector<Unit>& units)
    : BaseScene(units)
{
}

bool MenuScene::init()
{
    if (!BaseScene::init())
    {
        return false;
    }

    auto visible_size = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    auto to_farm_button = cocos2d::ui::Button::create();
    to_farm_button->setTitleText("Farm");
    to_farm_button->setTitleFontSize(24);

    to_farm_button->setPosition(origin + visible_size * 0.2f);
    to_farm_button->addTouchEventListener(CC_CALLBACK_1(MenuScene::toFarm, this));

    addChild(to_farm_button);

    return true;
}

void MenuScene::toFarm(cocos2d::Ref* p_sender)
{
    cocos2d::Director::getInstance()->pushScene(FarmScene::create(m_units));
}
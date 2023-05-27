/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#include "MenuScene.h"

#include "Profile.h"
#include "Farm/FarmScene.h"
#include "Battle/GameMapScene.h"
#include "Unit/UnitLoader.h"

#include <cocos/base/CCDirector.h>
#include <cocos/ui/UIButton.h>

MenuScene* MenuScene::create(std::vector<Unit>& units)
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
    checkUnitsSize();
}

bool MenuScene::init()
{
    if (!BaseScene::init())
    {
        return false;
    }

    initFarmButton();
    initMapButton();

    return true;
}

void MenuScene::initFarmButton()
{
    const cocos2d::Size visible_size = cocos2d::Director::getInstance()->getVisibleSize();
    const cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    cocos2d::ui::Button* to_farm_btn = cocos2d::ui::Button::create();
    to_farm_btn->setTitleText("Farm");
    to_farm_btn->setTitleFontSize(24);

    to_farm_btn->setPosition(origin + visible_size * 0.2f);
    to_farm_btn->addTouchEventListener(CC_CALLBACK_2(MenuScene::toScene<FarmScene>, this));

    addChild(to_farm_btn);
}

void MenuScene::initMapButton()
{
    const cocos2d::Size visible_size = cocos2d::Director::getInstance()->getVisibleSize();
    const cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    cocos2d::ui::Button* to_map_btn = cocos2d::ui::Button::create();
    to_map_btn->setTitleText("Map");
    to_map_btn->setTitleFontSize(24);

    to_map_btn->setPosition(origin + visible_size * 0.5f);
    to_map_btn->addTouchEventListener(CC_CALLBACK_2(MenuScene::toScene<GameMapScene>, this));

    addChild(to_map_btn);
}

void MenuScene::checkUnitsSize()
{
    if (m_units.size() == 0u)
    {
        m_units.push_back(profile::StartUnit);
        unit_loader::saveToFile(m_units, unit_loader::defaultInventoryFile());
    }
}

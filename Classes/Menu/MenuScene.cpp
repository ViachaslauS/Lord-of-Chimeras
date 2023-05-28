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

#include <cocos/2d/CCLabel.h>
#include <cocos/2d/CCSprite.h>

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

    const cocos2d::Size visible_size = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    cocos2d::Sprite* bg = cocos2d::Sprite::create("background/castle.png");
    bg->setAnchorPoint({ 0.5f, 0.5f });
    bg->setPosition(visible_size * 0.5f);

    addChild(bg);

    initFarmButton();
    initMapButton();

    cocos2d::ui::Button* close_btn = cocos2d::ui::Button::create(
        "black_button_large.png",
        "black_button_large.png",
        "black_button_large.png",
        cocos2d::ui::Widget::TextureResType::PLIST
        );

    close_btn->addTouchEventListener(CC_CALLBACK_1(BaseScene::menuCloseCallback, this));
    close_btn->setTitleText("Close game");
    close_btn->setTitleFontSize(20);

    float x = origin.x + visible_size.width - close_btn->getContentSize().width * 0.5f;
    float y = origin.y + close_btn->getContentSize().height * 0.5f;
    close_btn->setPosition(cocos2d::Vec2(x, y));

    addChild(close_btn, 1);

    return true;

    return true;
}

void MenuScene::initFarmButton()
{
    const cocos2d::Size visible_size = cocos2d::Director::getInstance()->getVisibleSize();
    const cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    cocos2d::ui::Button* to_farm_btn = cocos2d::ui::Button::create(
        "black_button_medium.png",
        "black_button_medium.png",
        "black_button_medium.png",
        cocos2d::ui::Widget::TextureResType::PLIST
    );
    to_farm_btn->setTitleText("Farm");
    to_farm_btn->setTitleFontSize(24);

    to_farm_btn->setPosition(origin + visible_size * 0.2f);
    to_farm_btn->addTouchEventListener(CC_CALLBACK_2(MenuScene::toScene<FarmScene>, this));

    addChild(to_farm_btn);

    cocos2d::Label* farm_desc = cocos2d::Label::create();
    farm_desc->setDimensions(300.0f, 300.0f);
    farm_desc->setString("Go to your\nchimeras storage");
    farm_desc->setSystemFontSize(20);
    farm_desc->setAnchorPoint({ 0.5f, 1.0f });
    farm_desc->setHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    farm_desc->setTextColor(cocos2d::Color4B::YELLOW);
    farm_desc->setPosition({ to_farm_btn->getContentSize().width * 0.5f, 0.0f });

    to_farm_btn->addChild(farm_desc);
    
}

void MenuScene::initMapButton()
{
    const cocos2d::Size visible_size = cocos2d::Director::getInstance()->getVisibleSize();
    const cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    cocos2d::ui::Button* to_map_btn = cocos2d::ui::Button::create(
        "black_button_medium.png",
        "black_button_medium.png",
        "black_button_medium.png",
        cocos2d::ui::Widget::TextureResType::PLIST
    );
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

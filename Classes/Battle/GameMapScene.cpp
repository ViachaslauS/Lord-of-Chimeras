/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#include "GameMapScene.h"

#include "Battle/BattlePrepareScene.h"

#include <cocos/2d/CCSprite.h>
#include <cocos/base/CCDirector.h>
#include <cocos/ui/UIButton.h>

namespace
{
    struct MapButtonInfo
    {
        cocos2d::Vec2 relative_pos;
        const char* button_name;
        const char* sprite_name;
    };

    const std::array<MapButtonInfo, 4> buttons_info =
    {
        MapButtonInfo({ cocos2d::Vec2(0.5f, 0.8f), "mountains", "icon_mountains.png" }),
        { cocos2d::Vec2(0.8f, 0.5f), "coast", "icon_beach.png" },
        { cocos2d::Vec2(0.5f, 0.2f), "desert", "icon_desert.png" },
        { cocos2d::Vec2(0.2f, 0.5f), "hills", "icon_hills.png" }
    };
}

GameMapScene* GameMapScene::create(std::vector<Unit>& units)
{
    GameMapScene* scene = new GameMapScene(units);
    if (scene && scene->init())
    {
        scene->autorelease();
        return scene;
    }

    CC_SAFE_DELETE(scene);
    return nullptr;
}

GameMapScene::GameMapScene(std::vector<Unit>& units)
    : BaseScene(units)
{
}

bool GameMapScene::init()
{
    if (BaseScene::init() == false)
    {
        return false;
    }

    const cocos2d::Size visible_size = cocos2d::Director::getInstance()->getVisibleSize();

    cocos2d::Sprite* bg = cocos2d::Sprite::create("background/castle.png");
    CC_ASSERT(bg != nullptr);
    bg->setAnchorPoint({ 0.5f, 0.5f });
    bg->setPosition(visible_size * 0.5f);

    addChild(bg);

    initButtons();

    initBack();

    return true;
}

void GameMapScene::initButtons()
{
    const cocos2d::Size visible_size = cocos2d::Director::getInstance()->getVisibleSize();

    for (size_t i = 0u; i < buttons_info.size(); i++)
    {
        cocos2d::ui::Button* to_location_btn = cocos2d::ui::Button::create(
            buttons_info[i].sprite_name,
            buttons_info[i].sprite_name,
            buttons_info[i].sprite_name,
            cocos2d::ui::Widget::TextureResType::PLIST
        );

        to_location_btn->addTouchEventListener([this, i](auto, cocos2d::ui::Widget::TouchEventType event) {
            if (event == cocos2d::ui::Widget::TouchEventType::ENDED)
            {
                cocos2d::Director::getInstance()->pushScene(BattlePrepareScene::create(m_units, i));
            }
        });

        to_location_btn->setPosition({ visible_size.width * buttons_info[i].relative_pos.x, visible_size.height * buttons_info[i].relative_pos.y });
        addChild(to_location_btn);
    }
}
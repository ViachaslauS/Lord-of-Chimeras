/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#include "GameMapScene.h"

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

    initButtons();

    initBack();
}

void GameMapScene::initButtons()
{
    const cocos2d::Size visible_size = cocos2d::Director::getInstance()->getVisibleSize();

    for (const auto& info : buttons_info)
    {
        cocos2d::ui::Button* to_location_btn = cocos2d::ui::Button::create(
            info.sprite_name,
            info.sprite_name,
            info.sprite_name,
            cocos2d::ui::Widget::TextureResType::PLIST
        );

        to_location_btn->addTouchEventListener([info](auto, cocos2d::ui::Widget::TouchEventType touch) {
            if (touch == cocos2d::ui::Widget::TouchEventType::ENDED)
            {
                // To battle prepare scene
            }
        });

        to_location_btn->setPosition({ visible_size.width * info.relative_pos.x, visible_size.height * info.relative_pos.y });
        addChild(to_location_btn);
    }
}
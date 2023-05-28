/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#include "BaseScene.h"

#include <cocos/base/CCDirector.h>
#include <cocos/ui/UIButton.h>

BaseScene::BaseScene(std::vector<Unit>& units)
    : m_units(units)
{
}

void BaseScene::menuCloseCallback(Ref* p_sender)
{
    cocos2d::Director::getInstance()->end();
}

bool BaseScene::init()
{
    if (Scene::init() == false)
    {
        return false;
    }
}

void BaseScene::initBack()
{
    const cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

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
}
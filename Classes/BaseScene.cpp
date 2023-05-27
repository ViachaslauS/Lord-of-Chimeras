/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#include "BaseScene.h"

#include <cocos/base/CCDirector.h>
#include <cocos/2d/CCMenu.h>
#include <cocos/2d/CCMenuItem.h>
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

    auto closeItem = cocos2d::MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(BaseScene::menuCloseCallback, this));

    const cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
    float y = origin.y + closeItem->getContentSize().height / 2;
    closeItem->setPosition(cocos2d::Vec2(x, y));

    auto menu = cocos2d::Menu::create(closeItem, NULL);
    menu->setPosition(cocos2d::Vec2::ZERO);
    this->addChild(menu, 1);

    return true;
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
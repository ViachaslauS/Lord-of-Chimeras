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

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
    float y = origin.y + closeItem->getContentSize().height / 2;
    closeItem->setPosition(cocos2d::Vec2(x, y));

    auto menu = cocos2d::Menu::create(closeItem, NULL);
    menu->setPosition(cocos2d::Vec2::ZERO);
    this->addChild(menu, 1);

    return true;
}

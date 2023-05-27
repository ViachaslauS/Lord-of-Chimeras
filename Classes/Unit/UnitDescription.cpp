/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#include "UnitDescription.h"

#include <cocos/2d/CCSprite.h>

UnitDescription* UnitDescription::create(const Unit& unit)
{
    UnitDescription* node = new UnitDescription(unit);
    if (node && node->init())
    {
        node->autorelease();
        return node;
    }

    CC_SAFE_DELETE(node);
    return nullptr;
}

UnitDescription::UnitDescription(const Unit& unit)
    : m_unit(unit)
{
}

bool UnitDescription::init()
{
    if (Node::init() == false)
    {
        return false;
    }

    setAnchorPoint({ 0.5f, 0.5f });

    cocos2d::Sprite* background = cocos2d::Sprite::createWithSpriteFrameName("desc_panel.png");
    setContentSize(background->getContentSize());

    background->setAnchorPoint({ 0.5f, 0.5f });
    background->setPosition(getContentSize() * 0.5f);

    addChild(background);

    // ATTENTION: add info about unit
    // ....
    // + add spells desc
}

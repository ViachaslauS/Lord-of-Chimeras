/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#include "SpellDescription.h"

#include <cocos/2d/CCSprite.h>

SpellDescription* SpellDescription::create(const Spell& spell)
{
    SpellDescription* node = new SpellDescription(spell);
    if (node && node->init())
    {
        node->autorelease();
        return node;
    }

    CC_SAFE_DELETE(node);
    return nullptr;
}

SpellDescription::SpellDescription(const Spell& spell)
    : m_spell(spell)
{
}

bool SpellDescription::init()
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
}

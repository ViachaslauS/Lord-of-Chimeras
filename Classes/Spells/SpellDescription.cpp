/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#include "SpellDescription.h"

#include "BattleEnvironment.h"

#include <cocos/2d/CCSprite.h>
#include <cocos/2d/CCLabel.h>

#include <sstream>
#include <iomanip>

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

    cocos2d::Sprite* background = cocos2d::Sprite::createWithSpriteFrameName("desc_spell_panel.png");
    setContentSize(background->getContentSize());

    background->setAnchorPoint({ 0.5f, 0.5f });
    background->setPosition(getContentSize() * 0.5f);

    addChild(background);

    cocos2d::Sprite* spell_icon = cocos2d::Sprite::createWithSpriteFrameName(m_spell.sprite_id);
    spell_icon->setPosition({ 
        spell_icon->getContentSize().width * 0.5f + 10.0f, 
        background->getContentSize().height - spell_icon->getContentSize().height * 0.5f - 4.0f });

    background->addChild(spell_icon);

    background->addChild(addLabel(
        { background->getContentSize().width * 0.5f, background->getContentSize().height - 18.0f },
        m_spell.spell_name
    ));

    background->addChild(addLabel(
        { background->getContentSize().width * 0.5f, background->getContentSize().height - 50.0f },
        std::string("target: ") + std::string(m_spell.to_myself ? "self" : "other")
    ));

    std::stringstream val_stream;
    val_stream << std::fixed << std::setprecision(2) << std::abs(m_spell.value);
    std::string val_string = val_stream.str();

    background->addChild(addLabel(
        { background->getContentSize().width * 0.5f, background->getContentSize().height - 80.0f },
        std::string(m_spell.value > 0 ? "deal: " : "restore: ")
        + val_string + 
        " to " + unit_helper::getValueName(m_spell.value_field)
    ));

    std::stringstream cost_stream;
    cost_stream << std::fixed << std::setprecision(2) << std::abs(m_spell.cost);
    std::string cost_string = cost_stream.str();

    background->addChild(addLabel(
        { background->getContentSize().width * 0.5f, background->getContentSize().height - 100.0f },
        std::string("cost : ")
        + cost_string +
        " " + unit_helper::getValueName(m_spell.cost_field)
    ));

    if (m_spell.influence != EnvironmentInfluence::None)
    {
        background->addChild(addLabel(
            { background->getContentSize().width * 0.5f, background->getContentSize().height - 120.0f },
            std::string("cause ") + unit_helper::getWeatherName(m_spell.influence)
        ));
    }

    // ATTENTION: add info about unit
    // ....

    return true;
}

cocos2d::Label* SpellDescription::addLabel(cocos2d::Vec2 pos, std::string text)
{
    cocos2d::Label* label = cocos2d::Label::create();
    label->setString(text);
    label->setPosition(pos);

    label->setSystemFontSize(20);

    label->setAnchorPoint({ 0.5f, 0.5f });
    label->setTextColor(cocos2d::Color4B::BLACK);

    return label;
}

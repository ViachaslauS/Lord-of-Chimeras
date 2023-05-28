/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#include "UnitDescription.h"

#include "Spells/SpellView.h"
#include "Profile.h"

#include <cocos/2d/CCSprite.h>
#include <cocos/2d/CCLabel.h>

#include <sstream>
#include <iomanip>

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

    cocos2d::Sprite* background = cocos2d::Sprite::createWithSpriteFrameName("desc_unit_panel.png");
    setContentSize(background->getContentSize());
    setAnchorPoint({ 0.5f, 0.5f });

    background->setAnchorPoint({ 0.5f, 0.5f });
    background->setPosition(getContentSize() * 0.5f);

    addChild(background);

    cocos2d::Sprite* unit_icon = cocos2d::Sprite::createWithSpriteFrameName(m_unit.sprite_name);
    unit_icon->setPosition({
        unit_icon->getContentSize().width * 0.5f + 10.0f,
        background->getContentSize().height });

    background->addChild(unit_icon);

    background->addChild(addLabel(
        { background->getContentSize().width * 0.5f, background->getContentSize().height - 15.0f },
        { 0.5f, 0.5f },
        m_unit.unit_name
    ));

    background->addChild(addLabel(
        { 0.0f, background->getContentSize().height - 50.0f },
        { 0.0f, 0.5f },
        "hp: " + convertVal(m_unit.hp)
    ));

    background->addChild(addLabel(
        { 0.0f, background->getContentSize().height - 75.0f },
        { 0.0f, 0.5f },
        "mp: " + convertVal(m_unit.mp)
    ));

    background->addChild(addLabel(
        { background->getContentSize().width, background->getContentSize().height - 50.0f },
        { 1.0f, 0.5f },
        "hp reg: " + convertVal(m_unit.hp_regen)
    ));

    background->addChild(addLabel(
        { background->getContentSize().width, background->getContentSize().height - 75.0f },
        { 1.0f, 0.5f },
        "mp reg: " + convertVal(m_unit.mp_regen)
    ));

    background->addChild(addLabel(
        { 0.0f, background->getContentSize().height - 100.0f },
        { 0.0f, 0.5f },
        "atk: " + convertVal(m_unit.attack)
    ));

    background->addChild(addLabel(
        { background->getContentSize().width, background->getContentSize().height - 100.0f },
        { 1.0f, 0.5f },
        "atk speed: " + convertVal(m_unit.atk_speed)
    ));

    background->addChild(addLabel(
        { 0.0f, background->getContentSize().height - 125.0f },
        { 0.0f, 0.5f },
        "evasion: " + convertVal(m_unit.evasion)
    ));

    background->addChild(addLabel(
        { 0.0f, background->getContentSize().height - 150.0f },
        { 0.0f, 0.5f },
        "accuracy: " + convertVal(m_unit.accuracy)
    ));

    background->addChild(addLabel(
        { 0.0f, background->getContentSize().height - 175.0f },
        { 0.0f, 0.5f },
        "spells count: " + std::to_string(m_unit.spells_id.size())
    ));

    float offset = 200.0f;
    const float step = 20.0f;
    for (const auto& resist : m_unit.resistance)
    {
        const char* infl_name = unit_helper::getWeatherName(static_cast<EnvironmentInfluence>(resist.first));
        background->addChild(addLabel(
            { 0.0f, background->getContentSize().height - offset },
            { 0.0f, 0.5f },
            std::string("resist ") + infl_name + ": " + convertVal(resist.second)
        ));

        offset += step;
    }

    cocos2d::Node* spell_view = spell_view::createView(m_unit.spells_id, { 0.0f, 0.0f }, getContentSize());
    spell_view->setAnchorPoint({ 0.0f, 0.0f });
    spell_view->setPosition({ 0.0f, -10.0f });

    addChild(spell_view);

    return true;
}

cocos2d::Label* UnitDescription::addLabel(cocos2d::Vec2 pos, cocos2d::Vec2 anchor, std::string text)
{
    cocos2d::Label* label = cocos2d::Label::create();
    label->setString(text);
    label->setPosition(pos);

    label->setSystemFontSize(20);

    label->setAnchorPoint(anchor);

    return label;
}

std::string UnitDescription::convertVal(float val)
{
    std::stringstream val_stream;
    val_stream << std::fixed << std::setprecision(2) << val;
    return val_stream.str();
}

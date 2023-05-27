/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#include "Fighter.h"

#include "Helpers/MousePointHandler.h"
#include "Spells/SpellDescription.h"

#include <cocos/base/CCDirector.h>

#include <cocos/ui/UIScale9Sprite.h>
#include <cocos/ui/UIScrollView.h>
#include <cocos/ui/UIButton.h>

#include <cocos/2d/CCSprite.h>
#include <cocos/2d/CCAnimation.h>
#include <cocos/2d/CCActionInterval.h>

namespace
{
    const cocos2d::Size BarNPSize = { 127.0f, 11.0f };

    const cocos2d::Rect NPatch =
    {
        2.0f,
        0.0f,
        123.0f,
        11.0f
    };
}

Fighter* Fighter::create(const Unit& unit, bool touchable_spells)
{
    Fighter* fighter = new Fighter(unit, touchable_spells);
    if (fighter && fighter->init())
    {
        fighter->autorelease();
        return fighter;
    }

    CC_SAFE_DELETE(fighter);
    return nullptr;
}

Fighter::Fighter(const Unit& stats, bool touchable_spells)
    : cocos2d::Node()
    , m_base_unit(stats)
    , m_active_unit(stats)
    , m_touchable_spells(touchable_spells)
{
}

const Unit& Fighter::getBaseUnit() const
{
    return m_base_unit;
}

const Unit& Fighter::getActiveUnit() const
{
    return m_active_unit;
}

void Fighter::setFighterAnimation(cocos2d::Vector<cocos2d::SpriteFrame*>&& frames)
{
    if (frames.size() == 0u)
    {
        return;
    }

    cocos2d::Sprite* fighter_sprite = cocos2d::Sprite::createWithSpriteFrame(frames.at(0)->clone());
    cocos2d::Animation* animation = cocos2d::Animation::createWithSpriteFrames(frames, 0.3f);

    cocos2d::Animate* animate = cocos2d::Animate::create(animation);
    fighter_sprite->runAction(cocos2d::RepeatForever::create(animate));

    fighter_sprite->setAnchorPoint({ 0.5f, 0.75f });

    addChild(fighter_sprite);
}

bool Fighter::isDead() const
{
    return m_active_unit.hp <= 0.0f;
}

bool Fighter::isReadyAttack() const
{
    return m_duration >= m_active_unit.atk_speed;
}

float Fighter::attack()
{
    if (isReadyAttack())
    {
        m_duration = 0.0f;
        return m_active_unit.attack;
    }
}

void Fighter::decreaseStat(UnitField field, float damage)
{
    float old = unit_helper::getValueByField<float>(field, m_active_unit);
    unit_helper::setValueByField(field, m_active_unit, old - damage);
}

int Fighter::wantToCast() const
{
    return m_want_to_cast;
}

bool Fighter::useDesiredSpell()
{
    if (m_want_to_cast >= 0)
    {
        auto spell_it = profile::spells.find(m_want_to_cast);
        CCASSERT(spell_it != profile::spells.end(), "Invalid spell id\n");
        return useSpell(spell_it->second);
    }

    return false;
}

bool Fighter::init()
{
    if (Node::init() == false)
    {
        return false;
    }

    scheduleUpdate();

    initBars();
    initSpells();

    return true;
}

void Fighter::update(float dt)
{
    m_duration += dt;

    if (isDead() == false)
    {
        const float hp = m_active_unit.hp + m_active_unit.hp_regen * dt;
        m_active_unit.hp = cocos2d::clampf(hp, 0.0f, m_base_unit.hp);

        const float mp = m_active_unit.mp + m_active_unit.mp_regen * dt;
        m_active_unit.mp = cocos2d::clampf(mp, 0.0f, m_base_unit.mp);
    }

    updateBar(m_hp_bar, m_active_unit.hp, m_base_unit.hp);
    updateBar(m_mp_bar, m_active_unit.mp, m_base_unit.mp);
}

bool Fighter::useSpell(const Spell& spell)
{
    if (m_want_to_cast == spell.spell_id)
    {
        m_want_to_cast = -1;
    }

    bool found = false;
    for (auto spell_id : m_active_unit.spells_id)
    {
        if (spell_id == spell.spell_id)
        {
            found = true;
            break;
        }
    }

    if (found == false)
    {
        return false;
    }

    const float old_value = unit_helper::getValueByField<float>(spell.cost_field, m_active_unit);
    if (old_value < spell.cost)
    {
        return false;
    }

    unit_helper::setValueByField(spell.cost_field, m_active_unit, old_value - spell.cost);

    return true;
}

void Fighter::setTouchableSpells(bool touchable)
{
    // TODO
}

void Fighter::updateBar(cocos2d::Sprite* bar, float current, float max)
{
    bar->setContentSize({ BarNPSize.width * (cocos2d::clampf(current, 0.0f, max) / max), bar->getContentSize().height });
}

void Fighter::initBars()
{
    m_hp_bar = initBar("hp_", { 0.0f, 120.0f });
    m_mp_bar = initBar("mp_", { 0.0f, 100.0f });
}

cocos2d::ui::Scale9Sprite* Fighter::initBar(const char* prefix, const cocos2d::Vec2& pos)
{
    cocos2d::Sprite* bg = cocos2d::Sprite::createWithSpriteFrameName(std::string(prefix) + "bar_back.png");

    bg->setPosition(pos);
    addChild(bg);

    cocos2d::ui::Scale9Sprite* bar = cocos2d::ui::Scale9Sprite::createWithSpriteFrameName(
        std::string(prefix) + "bar.png",
        NPatch
    );

    bar->setAnchorPoint({ 0.5f, 0.5f });
    bar->setPosition(bg->getContentSize() * 0.5f);
    bar->setContentSize(BarNPSize);

    bg->addChild(bar);

    cocos2d::Sprite* fg = cocos2d::Sprite::createWithSpriteFrameName(std::string(prefix) + "bar_for.png");
    fg->setAnchorPoint({ 0.0f, 0.0f });

    bg->addChild(fg);

    return bar;
}

void Fighter::initSpells()
{
    auto scroll = cocos2d::ui::ScrollView::create();
    scroll->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);

    cocos2d::Size inner_size = { 0.0f, 0.0f };

    for (size_t i = 0u; i < m_active_unit.spells_id.size(); i++)
    {
        const auto& spell_it = profile::spells.find(m_active_unit.spells_id[i]);
        CCASSERT(spell_it != profile::spells.end(), "Invalid spell id\n");
        const Spell& spell = spell_it->second;

        cocos2d::ui::Button* spell_btn = cocos2d::ui::Button::create(
            spell.sprite_id,
            spell.sprite_id,
            spell.sprite_id,
            cocos2d::ui::Widget::TextureResType::PLIST
        );

        spell_btn->setTouchEnabled(m_touchable_spells);
        spell_btn->setAnchorPoint({ 0.5f, 0.5f });

        spell_btn->setPosition({ inner_size.width + spell_btn->getContentSize().width * 0.5f, spell_btn->getContentSize().height * 0.5f });

        using TouchType = cocos2d::ui::Widget::TouchEventType;
        spell_btn->addTouchEventListener([this, i](cocos2d::Ref*, TouchType touch) {
            if (touch == TouchType::ENDED)
            {
                m_want_to_cast = m_active_unit.spells_id[i];
            }
        });

        inner_size.width += spell_btn->getContentSize().width;
        inner_size.height = std::max(inner_size.height, spell_btn->getContentSize().height);

        scroll->addChild(spell_btn);
    }

    auto scroll_listener = [this](cocos2d::Node* root, bool result) {
        if (result == false && m_spell_desc != nullptr)
        {
            removeChild(m_spell_desc);
            m_spell_desc = nullptr;
        }
    };

    auto child_listener = [this](cocos2d::Node* root, bool result, uint32_t idx) {
        if (result && idx != m_curr_spell)
        {
            m_curr_spell = idx;

            if (m_spell_desc != nullptr)
            {
                removeChild(m_spell_desc);
                m_spell_desc = nullptr;
            }

            const auto& spell = profile::spells.find(m_active_unit.spells_id[idx]);
            CCASSERT(spell != profile::spells.end(), "Invalid spell id");
            m_spell_desc = SpellDescription::create(spell->second);
        }
    };

    mouse_point_handler::addListenerToChildren(scroll, scroll_listener, child_listener);

    scroll->setInnerContainerSize(inner_size);
    scroll->setContentSize({ std::min(inner_size.width, 200.0f), inner_size.height });

    scroll->setAnchorPoint({ 0.5f, 0.5f });
    scroll->setPosition({ 0.0f, -200.0f });

    addChild(scroll);
}
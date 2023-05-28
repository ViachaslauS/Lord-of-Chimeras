/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#pragma once

#include "Unit/Unit.h"

#include "Profile.h"

#include <cocos/2d/CCNode.h>

#include <tuple>

namespace cocos2d
{
    class Sprite;
    class AnimationFrame;

    namespace ui
    {
        class Scale9Sprite;
    }
}

class FighterAI;

class Fighter final : public cocos2d::Node
{
    friend class FighterAI;
public:
    static Fighter* create(const Unit& unit, bool touchable_spells);

    const Unit& getBaseUnit() const;
    const Unit& getActiveUnit() const;

    cocos2d::Sprite* setFighterAnimation(cocos2d::Vector<cocos2d::SpriteFrame*>&& frames);

    bool isDead() const;

    bool isReadyAttack() const;
    float attack();

    void decreaseStat(UnitField field, float damage);

    int wantToCast() const;
    bool useDesiredSpell();

private:
    Fighter(const Unit& stats, bool touchable_spells);
    bool init() override;

    void initFighter();

    void update(float dt) override;

    bool useSpell(const Spell& spell);
    
    void setTouchableSpells(bool touchable);

    void updateBar(cocos2d::Sprite* bar, float current, float max);

    void initBars();
    cocos2d::ui::Scale9Sprite* initBar(const char* prefix, const cocos2d::Vec2& pos);

    void initSpells();

private:
    const Unit m_base_unit;
    Unit m_active_unit;

    float m_duration = 0.0f;
    bool m_touchable_spells = false;

    cocos2d::ui::Scale9Sprite* m_hp_bar = nullptr;
    cocos2d::ui::Scale9Sprite* m_mp_bar = nullptr;

    cocos2d::Node* m_unit_desc = nullptr;

    cocos2d::Node* m_spell_desc = nullptr;
    int m_curr_spell = -1;

    int m_want_to_cast = -1;
};
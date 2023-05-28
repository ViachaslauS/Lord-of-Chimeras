/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#pragma once

#include "Profile.h"

#include <cocos/2d/CCNode.h>

namespace cocos2d
{
    class Label;
}

class SpellDescription final : public cocos2d::Node
{
public:
    static SpellDescription* create(const Spell& spell);

private:
    SpellDescription(const Spell& spell);

    bool init() override;

    cocos2d::Label* addLabel(cocos2d::Vec2 pos, std::string text);

private:
    const Spell& m_spell;
};
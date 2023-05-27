/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#pragma once

#include "Profile.h"

#include <cocos/2d/CCNode.h>

class SpellDescription final : public cocos2d::Node
{
public:
    static SpellDescription* create(const Spell& spell);

private:
    SpellDescription(const Spell& spell);

    bool init() override;

private:
    const Spell& m_spell;
};
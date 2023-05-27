/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#include "Unit.h"

#include "Profile.h"

#include <cocos/base/ccRandom.h>

namespace unit_helper
{
    constexpr const char* names[] =
    {
        "Brave knight",
        "Holy knight",
        "Pure knight",
        "Old knight"
    };

    constexpr const char* sprite_names[] =
    {
        "Brave knight",
        "Holy knight",
        "Pure knight",
        "Old knight"
    };

    Unit generateUnit(uint32_t difficulty, uint32_t location_id)
    {
        Unit unit;
        unit.hp = cocos2d::random(difficulty * 50, difficulty * 100);
        unit.mp = cocos2d::random(difficulty * 20, difficulty * 50);

        unit.hp_regen = cocos2d::random(difficulty * 0.1f, difficulty * 2.0f);
        unit.mp_regen = cocos2d::random(difficulty * 0.0f, difficulty * 1.2f);

        unit.attack = cocos2d::random(difficulty * 2, difficulty * 5);
        unit.atk_speed = cocos2d::random(2.0f / difficulty, 10.0f / difficulty);

        unit.evasion = cocos2d::random(difficulty / 10.0f, difficulty / 7.5f);
        unit.accuracy = cocos2d::random(difficulty / 7.0f, difficulty / 5.0f);

        unit.sprite_name = sprite_names[location_id];
        unit.unit_name = names[location_id];

        for (size_t i = 0; i < cocos2d::random(0u, difficulty); i++)
        {
            auto it_spells = profile::spells.begin();
            std::advance(it_spells, cocos2d::random(0u, profile::spells.size() - 1));
            uint32_t id = it_spells->second.spell_id;
            for (auto spell : unit.spells_id)
            {
                if (spell == id)
                {
                    i--;
                    continue;
                }
            }
            unit.spells_id.push_back(id);
        }

        return unit;
    }
}
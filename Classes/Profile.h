/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#pragma once

#include "Unit/Unit.h"

#include "BattleEnvironment.h"

#include <unordered_map>

struct Spell
{
    uint32_t spell_id;

    UnitField cost_field;
    float cost = 0.0f;

    UnitField value_field;
    float value = 0.0f;

    bool to_myself = false;

    const char* sprite_id = "";
    const char* spell_name = "";

    EnvironmentInfluence influence = EnvironmentInfluence::None;
};

// TODO: all of this must be inside xml configs in future
namespace profile
{
    const Unit StartUnit =
    {
        60.0f,       // hp
        20,          // mp
        1.0f,        // hp_reg
        1.0f,        // mp_reg
        15.0f,          // attack
        2.5f,        // atk_speed
        0.0f,        // evasion
        0.8f,        // accuracy
        "unit_0.png",  // sprite
        "Chimera",      // name
        {},          // spells
        {} // resisst
    };

    constexpr uint32_t DifficultyLevels = 5u;

    const std::unordered_map<uint32_t, Spell> spells =
    {
        { 0u, 
            {
                0u, UnitField::MP, 15.0f, UnitField::HP, 10.0f, false, "spell_0.png", "magic punch", EnvironmentInfluence::None
            } 
        },
        { 1u,
            {
                1u, UnitField::MP, 5.0f, UnitField::MP_REGEN, 1.0f, false, "spell_1.png", "water storm", EnvironmentInfluence::Rain
            }
        },
        { 2u,
            {
                2u, UnitField::MP, 10.0f, UnitField::HP_REGEN, 2.0f, false, "spell_2.png", "health curse", EnvironmentInfluence::None
            }
        },
        { 3u,
            {
                3u, UnitField::MP, 50.0f, UnitField::HP, 50.0f, false, "spell_3.png", "slash", EnvironmentInfluence::None
            }
        },
        { 4u,
            {
                4u, UnitField::HP, 25.0f, UnitField::ATTACK, -10.0f, true, "spell_4.png", "berserker", EnvironmentInfluence::Hot
            }
        },
        { 5u,
            {
                5u, UnitField::MP, 75.0f, UnitField::EVASION, -0.2f, true, "spell_5.png", "hide", EnvironmentInfluence::Rain
            }
        },
        { 6u,
            {
                6u, UnitField::MP, -50.0f, UnitField::MP, 30.0f, false, "spell_6.png", "mana drain", EnvironmentInfluence::Cold
            }
        },
        { 7u,
            {
                7u, UnitField::MP, 80.0f, UnitField::HP_REGEN, 7.0f, false, "spell_7.png", "fire storm", EnvironmentInfluence::Drought
            }
        },
        { 8u,
            {
                8u, UnitField::MP, 50.0f, UnitField::ATK_SPEED, -1.0f, false, "spell_8.png", "slowdown", EnvironmentInfluence::Hot
            }
        },
        { 9u,
            {
                9u, UnitField::MP, 50.0f, UnitField::HP, -75.0, true, "spell_9.png", "be strong", EnvironmentInfluence::Hot
            }
        },
        { 10u,
            {
                10u, UnitField::HP, 5.0f, UnitField::MP_REGEN, -2.5f, true, "spell_10.png", "concentrate", EnvironmentInfluence::None
            }
        },
        { 11u,
            {
                11u, UnitField::MP, 50.0f, UnitField::MP_REGEN, 1.5f, false, "spell_11.png", "thunder strike", EnvironmentInfluence::Rain
            }
        },
    };


}
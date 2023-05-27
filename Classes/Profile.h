/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#pragma once

#include "Unit/Unit.h"

#include <unordered_map>

enum class EnvironmentInfluence : uint32_t
{
    None,
    Hot,
    Cold,
    Rain,
    Drought,
};

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
        20.0f,       // hp
        20.0f,       // mp
        0.5f,        // hp_reg
        0.5f,        // mp_reg
        10.0f,          // attack
        1.0f,        // atk_speed
        0.0f,        // evasion
        0.8f,        // accuracy
        "ogre.png",  // sprite
        "Chimera",      // name
        { 0u }           // spells
    };

    constexpr uint32_t DifficultyLevels = 5u;

    const std::unordered_map<uint32_t, Spell> spells =
    {
        { 0u, 
            {
                0u, UnitField::MP, 15.0f, UnitField::HP, 10.0f, false, "spell_0.png", "magic punch", EnvironmentInfluence::None
            } 
        },
    };


}
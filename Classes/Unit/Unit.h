/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#pragma once

#include <BattleEnvironment.h>

#include <unordered_map>

struct Unit
{
    float hp = 10.0f;
    float mp = 10.0f;

    float hp_regen = 0.0f;
    float mp_regen = 0.0f;

    uint32_t attack = 1u;

    float atk_speed = 1.0f;

    float evasion = 0.0f;
    float accuracy = 0.5f;

    const char* sprite_name = "";
    const char* unit_name = "";
};


// TODO: move it to xml config file
namespace standard_unit
{
    constexpr Unit StartUnit =
    {
        20.0f,       // hp
        20.0f,       // mp
        0.5f,        // hp_reg
        0.5f,        // mp_reg
        2u,          // attack
        1.0f,        // atk_speed
        0.0f,        // evasion
        0.8f,        // accuracy
        "ogre.png",  // sprite
        "ogre",      // name
    };
}
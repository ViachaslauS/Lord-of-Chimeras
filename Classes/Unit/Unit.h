/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#pragma once

#include <BattleEnvironment.h>

#include <vector>
#include <unordered_map>

enum class UnitField
{
    HP,
    MP,
    HP_REGEN,
    MP_REGEN,
    ATTACK,
    ATK_SPEED,
    EVASION,
    ACCURACY,

    COUNT
};

struct Unit
{
    float hp = 10.0f;
    float mp = 10.0f;

    float hp_regen = 0.0f;
    float mp_regen = 0.0f;

    float attack = 0.0f;

    float atk_speed = 1.0f;

    float evasion = 0.0f;
    float accuracy = 0.5f;

    const char* sprite_name = "";
    const char* unit_name = "";

    std::vector<uint32_t> spells_id;

    std::unordered_map<uint32_t, float> resistance;
};

namespace unit_helper
{
    Unit generateUnit(uint32_t difficulty, uint32_t location_id);

    template <class T>
    T getValueByField(UnitField field, const Unit& unit)
    {
        switch (field)
        {
            case UnitField::HP: return unit.hp; break;
            case UnitField::MP: return unit.mp; break;
            case UnitField::HP_REGEN: return unit.hp_regen; break;
            case UnitField::MP_REGEN: return unit.mp_regen; break;
            case UnitField::ATTACK: return unit.attack; break;
            case UnitField::ATK_SPEED: return unit.atk_speed; break;
            case UnitField::EVASION: return unit.evasion; break;
            case UnitField::ACCURACY: return unit.accuracy; break;
        }

        return {};
    }

    template <class T>
    void setValueByField(UnitField field, Unit& unit, T value)
    {
        switch (field)
        {
            case UnitField::HP: unit.hp = value; break;
            case UnitField::MP: unit.mp = value; break;
            case UnitField::HP_REGEN: unit.hp_regen = value; break;
            case UnitField::MP_REGEN: unit.mp_regen = value; break;
            case UnitField::ATTACK: unit.attack = value; break;
            case UnitField::ATK_SPEED: unit.atk_speed = value; break;
            case UnitField::EVASION: unit.evasion = value; break;
            case UnitField::ACCURACY: unit.accuracy = value; break;
        }
    }
}
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
    namespace
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

        float calculateValMerge(const Unit& unit_f, const Unit& unit_s, UnitField field)
        {
            const float first = getValueByField<float>(field, unit_f);
            const float second = getValueByField<float>(field, unit_s);

            return std::max(first, second) + std::min(first, second) * 0.1f;
        }

    }
    Unit generateUnit(uint32_t difficulty, uint32_t location_id)
    {
        Unit unit;
        unit.hp = cocos2d::random(difficulty * 50, difficulty * 100);
        unit.mp = cocos2d::random(difficulty * 20, difficulty * 50);

        unit.hp_regen = cocos2d::random(difficulty * 0.0f, difficulty * 5.0f);
        unit.mp_regen = cocos2d::random(0.0f, difficulty * 2.0f);

        unit.attack = cocos2d::random(difficulty * 10, difficulty * 20);
        unit.atk_speed = cocos2d::random(2.0f / difficulty, 10.0f / difficulty);

        unit.evasion = cocos2d::random(difficulty / 10.0f, difficulty / 7.5f);
        unit.accuracy = cocos2d::random(difficulty / 7.0f, difficulty / 5.0f);

        unit.sprite_name = "knight.png";
        unit.unit_name = names[location_id];

        for (size_t i = 0; i < cocos2d::random(0u, difficulty); i++)
        {
            auto it_spells = profile::spells.begin();
            std::advance(it_spells, cocos2d::random(0u, profile::spells.size() - 1));
            uint32_t id = it_spells->second.spell_id;

            bool unique = true;
            for (auto spell : unit.spells_id)
            {
                if (spell == id)
                {
                    unique = false;
                    break;
                }
            }
            if (unique)
            {
                unit.spells_id.push_back(id);
            }
        }

        unit.resistance[cocos2d::random(1u, 4u)] = cocos2d::random(-0.2f * difficulty, 0.1f);
        unit.resistance[location_id] = cocos2d::random(0.0f, 0.2f * difficulty);

        return unit;
    }

    Unit mergeUnits(const Unit& first, const Unit& second)
    {
        Unit newUnit;

        newUnit.hp = calculateValMerge(first, second, UnitField::HP);
        newUnit.mp = calculateValMerge(first, second, UnitField::MP);

        newUnit.hp_regen = calculateValMerge(first, second, UnitField::HP_REGEN);
        newUnit.mp_regen = calculateValMerge(first, second, UnitField::MP_REGEN);

        newUnit.attack = calculateValMerge(first, second, UnitField::ATTACK);
        newUnit.atk_speed = std::min(first.atk_speed, second.atk_speed); // don't play with atk speed...

        newUnit.accuracy = calculateValMerge(first, second, UnitField::ACCURACY);
        newUnit.evasion = calculateValMerge(first, second, UnitField::EVASION);

        newUnit.sprite_name = profile::StartUnit.sprite_name;
        newUnit.unit_name = profile::StartUnit.unit_name;

        for (auto f_res : first.resistance)
        {
            newUnit.resistance[f_res.first] = std::max(newUnit.resistance[f_res.first], f_res.second);
        }

        for (auto s_res : second.resistance)
        {
            newUnit.resistance[s_res.first] = std::max(newUnit.resistance[s_res.first], s_res.second);
        }

        newUnit.spells_id = first.spells_id;
        std::sort(newUnit.spells_id.begin(), newUnit.spells_id.end());

        for (auto sec_spell : second.spells_id)
        {
            if (std::binary_search(newUnit.spells_id.begin(), newUnit.spells_id.end(), sec_spell) == false)
            {
                newUnit.spells_id.push_back(sec_spell);
            }
        }

        return newUnit;
    }

    const char* getValueName(UnitField field)
    {
        switch (field)
        {
        case UnitField::HP: return "hp"; break;
        case UnitField::MP: return "mp"; break;
        case UnitField::HP_REGEN: return "hp regen"; break;
        case UnitField::MP_REGEN: return "mp regen"; break;
        case UnitField::ATTACK: return "attack"; break;
        case UnitField::ATK_SPEED: return "attack speed"; break;
        case UnitField::EVASION: return "evasion"; break;
        case UnitField::ACCURACY: return "accuracy"; break;
        }

        return "";
    }

    const char* getWeatherName(EnvironmentInfluence infl)
    {
        switch (infl)
        {
        case EnvironmentInfluence::None: return "none";
            break;
        case EnvironmentInfluence::Hot: return "hot";
            break;
        case EnvironmentInfluence::Cold: return "cold";
            break;
        case EnvironmentInfluence::Rain: return "rain";
            break;
        case EnvironmentInfluence::Drought: return "drought";
            break;
        }
        return "";
    }
}
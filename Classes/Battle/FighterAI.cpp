/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#include "FighterAI.h"

#include "Fighter.h"

FighterAI::FighterAI(uint32_t difficulty)
    : m_difficulty(difficulty)
{
}

void FighterAI::setFighter(Fighter* fighter)
{
    m_fighter = fighter;
}

void FighterAI::update(float dt)
{
    if (m_fighter == nullptr)
    {
        return;
    }

    m_duration += dt;
    if (m_duration > (5.0f / std::max(1u, m_difficulty)))
    {
        m_duration = 0.0f;
        const auto& spells = m_fighter->getActiveUnit().spells_id;
        if (spells.size() > 0u)
        {
            const uint32_t spellId = cocos2d::random(0u, spells.size() - 1u);

            m_fighter->m_want_to_cast = spells[spellId];
        }
    }
}

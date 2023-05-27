/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#pragma once

#include <inttypes.h>

class Fighter;

class FighterAI final
{
public:
    FighterAI(uint32_t difficulty);

    void setFighter(Fighter* fighter);

    void update(float dt);

private:
    Fighter* m_fighter = nullptr;

    uint32_t m_difficulty = 0u;
    float m_duration = 0.0f;
};
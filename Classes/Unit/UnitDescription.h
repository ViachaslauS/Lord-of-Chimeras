/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#pragma once

#include "Unit/Unit.h"

#include <cocos/2d/CCNode.h>

class UnitDescription final : public cocos2d::Node
{
public:
    static UnitDescription* create(const Unit& unit);

private:
    UnitDescription(const Unit& unit);

    bool init() override;

private:
    const Unit& m_unit;
};
/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#pragma once

#include "Unit/Unit.h"

#include <cocos/2d/CCNode.h>

namespace cocos2d
{
    class Label;
}

class UnitDescription final : public cocos2d::Node
{
public:
    static UnitDescription* create(const Unit& unit);

private:
    UnitDescription(const Unit& unit);

    bool init() override;

    cocos2d::Label* addLabel(cocos2d::Vec2 pos, cocos2d::Vec2 anchor, std::string text);
    std::string convertVal(float val);

private:
    const Unit& m_unit;
};
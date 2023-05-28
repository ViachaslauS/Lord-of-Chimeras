/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#pragma once

#include <cocos/math/Vec2.h>
#include <cocos/math/CCGeometry.h>

#include <vector>

struct Unit;

namespace cocos2d
{
    namespace ui
    {
        class ScrollView;
    }
}

namespace spell_view
{
    cocos2d::Node* createView(const std::vector<uint32_t>& spells, cocos2d::Vec2 pos, cocos2d::Size size);
}
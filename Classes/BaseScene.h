/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#pragma once

#include "Unit/Unit.h"

#include <cocos/2d/CCScene.h>

class BaseScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* create(std::vector<Unit>& units) = delete;

    virtual void menuCloseCallback(cocos2d::Ref* p_sender);

    bool init() override;

protected:
    BaseScene(std::vector<Unit>& units);

protected:
    std::vector<Unit>& m_units;
};
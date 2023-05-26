/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#pragma once

#include "BaseScene.h"

#include "Unit/Unit.h"

#include <vector>

class MenuScene final: public BaseScene
{
public:
    static cocos2d::Scene* create(std::vector<Unit>& units);

    bool init() override;

private:
    MenuScene(std::vector<Unit>& units);

    void toFarm(cocos2d::Ref* p_sender);
};

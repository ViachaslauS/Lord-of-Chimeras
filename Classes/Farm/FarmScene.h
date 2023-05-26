/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#pragma once

#include "BaseScene.h"

#include <vector>

class FarmScene final : public BaseScene
{
public:
    static cocos2d::Scene* create(std::vector<Unit>& units);

    bool init() override;

private:
    FarmScene(std::vector<Unit>& units);

private:
    std::vector<Unit*> m_selected_units;
    Unit* m_current_opened = nullptr;
};

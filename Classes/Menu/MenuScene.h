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
    static MenuScene* create(std::vector<Unit>& units);

    bool init() override;

private:
    MenuScene(std::vector<Unit>& units);

    void initFarmButton();
    void initMapButton();

    void checkUnitsSize();
};

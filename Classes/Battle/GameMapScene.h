/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#pragma once

#include "BaseScene.h"

class GameMapScene final : public BaseScene
{
public:
    static GameMapScene* create(std::vector<Unit>& units);

private:
    GameMapScene(std::vector<Unit>& units);

    bool init() override;
    void initButtons();
};
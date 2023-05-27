/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#pragma once

#include "BaseScene.h"

class BattlePrepareScene final : public BaseScene
{
public:
    static BattlePrepareScene* create(std::vector<Unit>& units, uint32_t map_idx);

private:
    BattlePrepareScene(std::vector<Unit>& units, uint32_t map_idx);

    bool init() override;

    void initLevels();
    void initUnitSelector();

    void updateSelected(uint32_t idx);

private:
    uint32_t m_map_idx = 0u;
    int m_selected_unit_idx = -1;

    cocos2d::Node* m_selected_holder = nullptr;
};
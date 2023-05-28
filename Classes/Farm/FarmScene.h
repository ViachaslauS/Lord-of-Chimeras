/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#pragma once

#include "BaseScene.h"

#include <vector>

namespace cocos2d
{
    class Sprite;

    namespace ui
    {
        class Button;
    }
}

class FarmScene final : public BaseScene
{
public:
    static FarmScene* create(std::vector<Unit>& units);

private:
    FarmScene(std::vector<Unit>& units);

    bool init() override;
    void addUnitToGroup(uint32_t idx);
    cocos2d::Node* initResetBtn();
    cocos2d::Node* initCombineBtn();

    void resetSelected();
    void combine();

    cocos2d::Vec2 getCenterBySelected(const cocos2d::Size& size) const;

private:
    std::vector<uint32_t> m_selected_units;

    cocos2d::Sprite* m_first = nullptr;
    cocos2d::Sprite* m_second = nullptr;
};

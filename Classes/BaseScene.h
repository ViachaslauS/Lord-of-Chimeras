/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#pragma once

#include "Unit/Unit.h"

#include <cocos/2d/CCScene.h>
#include <cocos/ui/UIWidget.h>

class BaseScene : public cocos2d::Scene
{
public:
    static BaseScene* create(std::vector<Unit>& units) = delete;

    virtual void menuCloseCallback(cocos2d::Ref* p_sender);

    bool init() override;

protected:
    BaseScene(std::vector<Unit>& units);

    void initBack();

    template <class T>
    void toScene(cocos2d::Ref*, cocos2d::ui::Widget::TouchEventType touch)
    {
        if (touch == cocos2d::ui::Widget::TouchEventType::ENDED)
        {
            cocos2d::Director::getInstance()->pushScene(T::create(m_units));
        }
    }

protected:
    std::vector<Unit>& m_units;
};
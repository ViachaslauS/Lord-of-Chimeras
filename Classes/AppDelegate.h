/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#pragma once

#include <cocos2d.h>

#include "Unit/Unit.h"

class  AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual void initGLContextAttrs();

    virtual bool applicationDidFinishLaunching();
    virtual void applicationDidEnterBackground();
    virtual void applicationWillEnterForeground();

private:
    void initInventory();
    void initSpriteCaches();

private:
    std::vector<Unit> m_units;
};


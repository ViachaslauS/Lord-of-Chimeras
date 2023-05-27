/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#pragma once

#include <functional>

namespace cocos2d
{
    class Node;
}

namespace mouse_point_handler
{
    using CallBackTest = std::function<void(cocos2d::Node*, bool)>;
    using CallBackChild = std::function<void(cocos2d::Node*, bool, size_t)>;
    void addListenerToChildren(cocos2d::Node* root, CallBackTest test_root_result, CallBackChild test_child_result);
    void addListenerToNode(cocos2d::Node* root, CallBackTest test_result);
}
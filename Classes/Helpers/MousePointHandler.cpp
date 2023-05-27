/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#include "MousePointHandler.h"

#include <cocos/2d/CCNode.h>

#include <cocos/base/CCDirector.h>
#include <cocos/base/CCEventDispatcher.h>
#include <cocos/base/CCEventListenerMouse.h>

namespace mouse_point_handler
{
    namespace
    {
        bool containPoint(cocos2d::Node* node, cocos2d::Vec2 untransformed_location)
        {
            const cocos2d::Rect rect = { 0.0f, 0.0f, node->getContentSize().width, node->getContentSize().height };

            const cocos2d::Vec2 location = node->convertToNodeSpace(untransformed_location);
            return rect.containsPoint(location);
        }
    }

    void addListenerToChildren(cocos2d::Node* root, CallBackTest test_root_result, CallBackChild test_child_result)
    {
        cocos2d::EventListenerMouse* mouseListener = cocos2d::EventListenerMouse::create();
        mouseListener->onMouseMove = [root, test_root_result, test_child_result](cocos2d::Event* event) {
            const cocos2d::EventMouse* mouse = static_cast<cocos2d::EventMouse*>(event);

            const cocos2d::Size visible_size = cocos2d::Director::getInstance()->getVisibleSize();
            const cocos2d::Vec2 untransformed = { mouse->getCursorX(), mouse->getCursorY() };

            const bool contain_result = containPoint(root, untransformed);
            test_root_result(root, contain_result);

            if (contain_result)
            {
                const auto children = root->getChildren();
                for (size_t i = 0u; i < children.size(); i++)
                {
                    test_child_result(children.at(i), containPoint(children.at(i), untransformed), i);
                }
            }
        };

        root->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, root);
    }

    void addListenerToNode(cocos2d::Node* node, CallBackTest test_result)
    {
        cocos2d::EventListenerMouse* mouseListener = cocos2d::EventListenerMouse::create();
        mouseListener->onMouseMove = [node, test_result](cocos2d::Event* event) {
            const cocos2d::EventMouse* mouse = static_cast<cocos2d::EventMouse*>(event);

            const cocos2d::Size visible_size = cocos2d::Director::getInstance()->getVisibleSize();
            const cocos2d::Vec2 untransformed = { mouse->getCursorX(), mouse->getCursorY() };

            const bool contain_result = containPoint(node, untransformed);
            test_result(node, contain_result);
        };
    }
}

/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#include "UnitsView.h"

#include "UnitDescription.h"

#include <cocos/base/CCDirector.h>
#include <cocos/base/CCEventDispatcher.h>
#include <cocos/base/CCEventListenerMouse.h>
#include <cocos/ui/UIScrollView.h>
#include <cocos/ui/UIButton.h>
#include <cocos/2d/CCSprite.h>

namespace units_view
{
    namespace
    {
        constexpr float SelectedScale = 1.2f;
        constexpr cocos2d::ui::Widget::TextureResType PlistType = cocos2d::ui::Widget::TextureResType::PLIST;

        bool containPoint(cocos2d::Node* node, cocos2d::Vec2 untransformed_location)
        {
            const cocos2d::Rect rect = { 0.0f, 0.0f, node->getContentSize().width, node->getContentSize().height };

            const cocos2d::Vec2 location = node->convertToNodeSpace(untransformed_location);
            return rect.containsPoint(location);
        }

        void addMouseListener(cocos2d::Node* root, const std::vector<Unit>& units)
        {
            cocos2d::EventListenerMouse* mouseListener = cocos2d::EventListenerMouse::create();

            mouseListener->onMouseMove = [&units, root, dirty = true](cocos2d::Event* event) mutable {
                const cocos2d::EventMouse* mouse = static_cast<cocos2d::EventMouse*>(event);

                const cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
                const cocos2d::Vec2 untransformed = { mouse->getCursorX(), mouse->getCursorY() };

                if (containPoint(root, untransformed))
                {
                    dirty = true;
                    const auto& children = root->getChildren();

                    for (size_t i = 0u; i < children.size(); i++)
                    {
                        auto child = children.at(i);
                        if (containPoint(child, untransformed))
                        {
                            if (child->getChildren().size() > 0)
                            {
                                continue;
                            }

                            UnitDescription* unit_desc = UnitDescription::create(units[i]);
                            unit_desc->setPosition(child->convertToNodeSpace(visibleSize * 0.5f));
                            
                            child->addChild(unit_desc);
                        }
                        else
                        {
                            children.at(i)->removeAllChildren();
                        }
                    }
                }
                else if (dirty)
                {
                    dirty = false;
                    for (auto child : root->getChildren())
                    {
                        child->removeAllChildren();
                    }
                }
            };

            root->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, root);
        }
    }

    cocos2d::ui::ScrollView* createView(const std::vector<Unit>& units, cocos2d::Vec2 pos, cocos2d::Size size)
    {
        auto scroll = cocos2d::ui::ScrollView::create();
        scroll->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);

        cocos2d::Sprite* background = cocos2d::Sprite::createWithSpriteFrameName("units_view_panel.png");

        const cocos2d::Size bg_size = background->getContentSize();
        background->setScale(size.width / bg_size.width, size.height / bg_size.height);

        cocos2d::Size inner_size = { 0.0f, size.height };

        for (size_t i = 0u; i < units.size(); i++)
        {
            cocos2d::ui::Button* unitButton = cocos2d::ui::Button::create(
                units[i].sprite_name,
                units[i].sprite_name,
                units[i].sprite_name,
                PlistType
            );

            unitButton->setAnchorPoint({ 0.5f, 0.5f });

            const cocos2d::Size max_node_size = unitButton->getContentSize() * SelectedScale;
            unitButton->setPosition({ inner_size.width + max_node_size.width * 0.5f, max_node_size.height * 0.5f });

            inner_size.width += max_node_size.width;
            
            scroll->addChild(unitButton);
        }

        addMouseListener(scroll, units);

        scroll->setContentSize(size);
        scroll->setInnerContainerSize(inner_size);

        return scroll;
    }
}
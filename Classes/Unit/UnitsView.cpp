/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#include "UnitsView.h"

#include "UnitDescription.h"

#include "Helpers/MousePointHandler.h"

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
    }

    cocos2d::Node* createView(const std::vector<Unit>& units, cocos2d::Vec2 pos, cocos2d::Size size)
    {
        auto scroll = cocos2d::ui::ScrollView::create();
        scroll->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);

        cocos2d::Sprite* background = cocos2d::Sprite::createWithSpriteFrameName("units_view_panel.png");

        const cocos2d::Size bg_size = background->getContentSize();
        background->setScale(size.width / bg_size.width, size.height / bg_size.height);

        cocos2d::Size inner_size = { 0.0f, size.height };

        for (size_t i = 0u; i < units.size(); i++)
        {
            cocos2d::ui::Button* unit_button = cocos2d::ui::Button::create(
                units[i].sprite_name,
                units[i].sprite_name,
                units[i].sprite_name,
                PlistType
            );

            unit_button->setAnchorPoint({ 0.5f, 0.5f });

            const cocos2d::Size max_node_size = unit_button->getContentSize() * SelectedScale;
            unit_button->setPosition({ inner_size.width + max_node_size.width * 0.5f, max_node_size.height * 0.5f });

            inner_size.width += max_node_size.width;
            
            scroll->addChild(unit_button);
        }

        auto on_scroll_enter = [dirty = true](cocos2d::Node* root, bool result) mutable {
            if (result)
            {
                dirty = true;
            }
            else if (dirty)
            {
                for (auto child : root->getChildren())
                {
                    child->removeAllChildren();
                }
            }
        };

        auto on_child_enter = [units](cocos2d::Node* root, bool result, uint32_t idx) {
            if (result)
            {
                if (root->getChildren().size() > 0)
                {
                    return;
                }

                const cocos2d::Size visible_size = cocos2d::Director::getInstance()->getVisibleSize();

                UnitDescription* unit_desc = UnitDescription::create(units[idx]);
                unit_desc->setPosition(root->convertToNodeSpace({ 0.0f, visible_size.height * 0.5f }));
                unit_desc->setAnchorPoint({ 0.0f, 0.5f });

                root->addChild(unit_desc);
            }
            else
            {
                root->removeAllChildren();
            }
        };

        mouse_point_handler::addListenerToChildren(scroll, on_scroll_enter, on_child_enter);

        scroll->setContentSize(size);
        scroll->setInnerContainerSize(inner_size);

        return scroll;
    }
}
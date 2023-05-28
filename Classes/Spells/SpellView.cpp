/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#include "SpellView.h"

#include "SpellDescription.h"

#include "Helpers/MousePointHandler.h"

#include <cocos/base/CCDirector.h>
#include <cocos/base/CCEventDispatcher.h>
#include <cocos/base/CCEventListenerMouse.h>
#include <cocos/ui/UIScrollView.h>
#include <cocos/ui/UIButton.h>
#include <cocos/2d/CCSprite.h>

namespace spell_view
{
    namespace
    {
        constexpr float SelectedScale = 1.2f;
        constexpr cocos2d::ui::Widget::TextureResType PlistType = cocos2d::ui::Widget::TextureResType::PLIST;
    }

    cocos2d::Node* createView(const std::vector<uint32_t>& spells, cocos2d::Vec2 pos, cocos2d::Size size)
    {
        auto scroll = cocos2d::ui::ScrollView::create();
        scroll->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
        scroll->setSwallowTouches(false);

        cocos2d::Sprite* background = cocos2d::Sprite::createWithSpriteFrameName("desc_unit_panel.png");

        const cocos2d::Size bg_size = background->getContentSize();
        background->setScale(size.width / bg_size.width, size.height / bg_size.height);

        cocos2d::Size inner_size = { 0.0f, size.height };

        for (size_t i = 0u; i < spells.size(); i++)
        {
            const auto& spell_it = profile::spells.find(spells[i]);
            CCASSERT(spell_it != profile::spells.end(), "Invalid spell id");

            const auto& spell = spell_it->second;
            cocos2d::ui::Button* spell_btn = cocos2d::ui::Button::create(
                spell.sprite_id,
                spell.sprite_id,
                spell.sprite_id,
                PlistType
            );

            spell_btn->setAnchorPoint({ 0.5f, 0.5f });

            const cocos2d::Size max_node_size = spell_btn->getContentSize() * SelectedScale;
            spell_btn->setPosition({ inner_size.width + max_node_size.width * 0.5f, max_node_size.height * 0.5f });

            inner_size.width += max_node_size.width;

            scroll->addChild(spell_btn);
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

        auto on_child_enter = [spells](cocos2d::Node* root, bool result, uint32_t idx) {
            if (result)
            {
                if (root->getChildren().size() > 0)
                {
                    return;
                }

                const cocos2d::Size visible_size = cocos2d::Director::getInstance()->getVisibleSize();

                SpellDescription* unit_desc = SpellDescription::create(profile::spells.find(spells[idx])->second);

                cocos2d::Node* parent = root->getParent()->getParent();
                const cocos2d::Vec2 pos = parent->convertToWorldSpace({ parent->getPosition().x, parent->getPosition().y + parent->getContentSize().height * 0.5f });
                unit_desc->setPosition(root->convertToNodeSpace(pos));
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
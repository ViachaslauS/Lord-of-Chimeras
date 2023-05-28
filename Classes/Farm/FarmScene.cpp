/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#include "FarmScene.h"

#include "Unit/UnitsView.h"
#include "Unit/UnitDescription.h"
#include "Unit/UnitLoader.h"

#include <cocos/base/CCDirector.h>
#include <cocos/2d/CCSprite.h>
#include <cocos/2d/CCLabel.h>

#include <cocos/ui/UIScrollView.h>
#include <cocos/ui/UIButton.h>

namespace
{
    constexpr char* BtnSpriteSmall = "small_button.png";
    constexpr char* BtnSpriteMedium = "medium_button.png";
}

FarmScene* FarmScene::create(std::vector<Unit>& units)
{
    FarmScene* scene = new FarmScene(units);
    if (scene && scene->init())
    {
        scene->autorelease();
        return scene;
    }

    CC_SAFE_DELETE(scene);
    return nullptr;
}

FarmScene::FarmScene(std::vector<Unit>& units)
    : BaseScene(units)
{
}

bool FarmScene::init()
{
    if (BaseScene::init() == false)
    {
        return false;
    }

    const cocos2d::Size visible_size = cocos2d::Director::getInstance()->getVisibleSize();

    cocos2d::Sprite* bg = cocos2d::Sprite::create("background/castle.png");
    bg->setAnchorPoint({ 0.5f, 0.5f });
    bg->setPosition(visible_size * 0.5f);

    addChild(bg);

    cocos2d::Label* farm_desc = cocos2d::Label::create();
    farm_desc->setDimensions(300.0f, 300.0f);
    farm_desc->setString("Click on 2 different chimeras\nand merge them in\none more powerful!");
    farm_desc->setSystemFontSize(20);
    farm_desc->setAnchorPoint({ 0.5f, 0.5f });
    farm_desc->setHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    farm_desc->setVerticalAlignment(cocos2d::TextVAlignment::CENTER);
    farm_desc->setPosition(visible_size * 0.5f);
    farm_desc->setTextColor(cocos2d::Color4B::YELLOW);

    addChild(farm_desc);

    m_first = cocos2d::Sprite::createWithSpriteFrameName("icon_bg.png");
    m_first->setPosition({ visible_size.width * 0.35f, visible_size.height * 0.8f });

    m_second = cocos2d::Sprite::createWithSpriteFrameName("icon_bg.png");
    m_second->setPosition({ visible_size.width * 0.65f, visible_size.height * 0.8f });

    addChild(m_first);
    addChild(m_second);

    cocos2d::Node* view = units_view::createView(m_units, cocos2d::Vec2::ZERO, visible_size);

    view->setAnchorPoint({ 0.0f, 0.0f });

    auto& children = view->getChildren();
    for (size_t i = 0u; i < children.size(); i++)
    {
        cocos2d::ui::Button* child_btn = static_cast<cocos2d::ui::Button*>(children.at(i));
        using TouchEvent = cocos2d::ui::Widget::TouchEventType;
        child_btn->addTouchEventListener([this, i](auto, TouchEvent touch) {
            if (touch == TouchEvent::ENDED)
            {
                addUnitToGroup(i);
            }
            });
    }

    addChild(view);

    addChild(initResetBtn());
    addChild(initCombineBtn());

    initBack();

    return true;
}

void FarmScene::addUnitToGroup(uint32_t idx)
{
    if (m_selected_units.size() >= 2)
    {
        return;
    }

    for (auto selected : m_selected_units)
    {
        if (selected == idx)
        {
            return;
        }
    }

    m_selected_units.push_back(idx);
    cocos2d::Node* base_node = m_selected_units.size() == 1 ? m_first : m_second;

    base_node->removeAllChildren();
    
    UnitDescription* desc = UnitDescription::create(m_units[idx]);

    cocos2d::Vec2 desc_center = getCenterBySelected(desc->getContentSize());
    desc->setPosition(base_node->convertToNodeSpace(desc_center));
    desc->setAnchorPoint({ 0.5f, 0.5f });

    base_node->addChild(desc);

    cocos2d::Sprite* icon = cocos2d::Sprite::createWithSpriteFrameName(m_units[idx].sprite_name);
    icon->setPosition(base_node->getContentSize() * 0.5f);
    base_node->addChild(icon);
}

cocos2d::Node* FarmScene::initResetBtn()
{
    const cocos2d::Size visible_size = cocos2d::Director::getInstance()->getVisibleSize();

    cocos2d::ui::Button* reset_btn = cocos2d::ui::Button::create(
        BtnSpriteSmall,
        BtnSpriteSmall,
        BtnSpriteSmall,
        cocos2d::ui::Widget::TextureResType::PLIST
    );

    using TouchEvent = cocos2d::ui::Widget::TouchEventType;
    reset_btn->addTouchEventListener([this](auto, TouchEvent touch) {
        if (touch == TouchEvent::ENDED)
        {
            resetSelected();
        }
     });

    reset_btn->setTitleText("reset");
    reset_btn->setTitleFontSize(24);

    reset_btn->setPosition({ visible_size.width * 0.5f, visible_size.height * 0.9f });

    return reset_btn;
}

cocos2d::Node* FarmScene::initCombineBtn()
{
    const cocos2d::Size visible_size = cocos2d::Director::getInstance()->getVisibleSize();

    cocos2d::ui::Button* combine_btn = cocos2d::ui::Button::create(
        BtnSpriteMedium,
        BtnSpriteMedium,
        BtnSpriteMedium,
        cocos2d::ui::Widget::TextureResType::PLIST
    );

    using TouchEvent = cocos2d::ui::Widget::TouchEventType;
    combine_btn->addTouchEventListener([this](auto, TouchEvent touch) {
        if (touch == TouchEvent::ENDED)
        {
            combine();
        }
        });

    combine_btn->setTitleText("Merge");
    combine_btn->setTitleFontSize(24);

    combine_btn->setPosition({ visible_size.width * 0.5f, visible_size.height * 0.3f });

    return combine_btn;
}

void FarmScene::resetSelected()
{
    m_selected_units.clear();

    if (m_first != nullptr)
    {
        m_first->removeAllChildren();
    }
    if (m_second != nullptr)
    {
        m_second->removeAllChildren();
    }
}

void FarmScene::combine()
{
    if (m_selected_units.size() != 2u)
    {
        return;
    }
    Unit newUnit = unit_helper::mergeUnits(m_units[m_selected_units[0]], m_units[m_selected_units[1]]);

    m_units[std::max(m_selected_units[0], m_selected_units[1])] = m_units.back();
    m_units.pop_back();
    m_units[std::min(m_selected_units[0], m_selected_units[1])] = m_units.back();
    m_units.pop_back();

    m_units.push_back(newUnit);

    unit_loader::saveToFile(m_units, unit_loader::defaultInventoryFile());

    cocos2d::Director::getInstance()->popScene();
}

cocos2d::Vec2 FarmScene::getCenterBySelected(const cocos2d::Size& size) const
{
    const cocos2d::Size visible_size = cocos2d::Director::getInstance()->getVisibleSize();
    return { (m_selected_units.size() == 1 ?  (size.width * 0.5f) : (visible_size.width - size.width * 0.5f)), visible_size.height * 0.5f };
}

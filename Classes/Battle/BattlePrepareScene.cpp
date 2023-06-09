/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#include "BattlePrepareScene.h"

#include "Battle/BattleScene.h"

#include "Profile.h"

#include "Unit/UnitsView.h"
#include "Unit/UnitDescription.h"

#include <cocos/base/CCDirector.h>
#include <cocos/ui/UIButton.h>
#include <cocos/2d/CCMenu.h>
#include <cocos/2d/CCLabel.h>
#include <cocos/2d/CCSprite.h>

#include <vector>

BattlePrepareScene* BattlePrepareScene::create(std::vector<Unit>& units, uint32_t map_idx)
{
    BattlePrepareScene* scene = new BattlePrepareScene(units, map_idx);
    if (scene && scene->init())
    {
        scene->autorelease();
        return scene;
    }

    CC_SAFE_DELETE(scene);
    return nullptr;
}

BattlePrepareScene::BattlePrepareScene(std::vector<Unit>& units, uint32_t map_idx)
    : BaseScene(units)
    , m_map_idx(map_idx)
{
}

bool BattlePrepareScene::init()
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

    cocos2d::Label* prepare_desc = cocos2d::Label::create();
    prepare_desc->setDimensions(300.0f, 300.0f);
    prepare_desc->setString("Select 1 chimera\nand go to any stage!\nBe careful, if you lose, you \nwill also lose the chimera");
    prepare_desc->setSystemFontSize(20);
    prepare_desc->setAnchorPoint({ 0.5f, 0.5f });
    prepare_desc->setHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    prepare_desc->setVerticalAlignment(cocos2d::TextVAlignment::CENTER);
    prepare_desc->setTextColor(cocos2d::Color4B::YELLOW);
    prepare_desc->setPosition({ visible_size.width * 0.8f, visible_size.height * 0.5f });

    addChild(prepare_desc);

    initUnitSelector();
    initLevels();

    initBack();

    return true;
}

void BattlePrepareScene::initLevels()
{
    const cocos2d::Size visible_size = cocos2d::Director::getInstance()->getVisibleSize();

    cocos2d::Vector<cocos2d::MenuItem*> items;

    for (size_t i = 0u; i < profile::DifficultyLevels; i++)
    {
        cocos2d::Label* level_num = cocos2d::Label::create();
        level_num->setString("Stage " + std::to_string(i + 1));
        level_num->setSystemFontSize(36);

        cocos2d::MenuItem* level_item = cocos2d::MenuItemLabel::create(level_num, [this, i](cocos2d::Ref*) {
            if (m_selected_unit_idx < 0)
            {
                return;
            }

        auto director = cocos2d::Director::getInstance();
        director->popToRootScene();

        BattleScene::BattleCtx ctx;
        ctx.active_unit = m_selected_unit_idx;
        ctx.location_id = m_map_idx;
        ctx.stage_id = i + 1;

        director->replaceScene(BattleScene::create(ctx, m_units));
        });

        items.pushBack(level_item);
    }

    cocos2d::Menu* level_selector = cocos2d::Menu::createWithArray(items);
    level_selector->setAnchorPoint({ 0.5f, 0.5f });
    level_selector->setPosition(visible_size * 0.5f);
    level_selector->alignItemsVertically();

    addChild(level_selector);
}

void BattlePrepareScene::initUnitSelector()
{
    const cocos2d::Size visible_size = cocos2d::Director::getInstance()->getVisibleSize();

    m_selected_holder = cocos2d::Node::create();
    addChild(m_selected_holder);

    cocos2d::Node* units = units_view::createView(m_units, cocos2d::Vec2::ZERO, visible_size);
    auto& children = units->getChildren();

    for (size_t i = 0; i < children.size(); i++)
    {
        cocos2d::ui::Button* child_btn = static_cast<cocos2d::ui::Button*>(children.at(i));

        using TouchType = cocos2d::ui::Widget::TouchEventType;
        child_btn->addTouchEventListener([this, i](auto, cocos2d::ui::Widget::TouchEventType touch) {
            if (touch == TouchType::ENDED)
            {
                updateSelected(i);
            }
        });
    }

    addChild(units);
}

void BattlePrepareScene::updateSelected(uint32_t idx)
{
    CCASSERT(idx < m_units.size(), "[BATTLE_PREPARE]: Invalid unit index\n");
    if (m_selected_unit_idx == idx)
    {
        return;
    }
    m_selected_unit_idx = idx;

    CCASSERT(m_selected_holder, "[BATTLE_PREPARE]: Holder was not initialized\n");

    m_selected_holder->removeAllChildren();

    const cocos2d::Size visible_size = cocos2d::Director::getInstance()->getVisibleSize();

    UnitDescription* desc = UnitDescription::create(m_units[idx]);
    desc->setPosition(convertToNodeSpace({ desc->getContentSize().width * 0.5f, visible_size.height * 0.5f }));

    m_selected_holder->addChild(desc);
}

/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#include "BattleScene.h"

#include "Fighter.h"
#include "Menu/MenuScene.h"
#include "Unit/UnitLoader.h"

#include <cocos/base/CCDirector.h>

#include <cocos/2d/CCActionInterval.h>
#include <cocos/2d/CCActionInstant.h>
#include <cocos/2d/CCLabel.h>
#include <cocos/2d/CCSprite.h>
#include <cocos/2d/CCSpriteFrameCache.h>
#include <cocos/2d/CCActionEase.h>
#include <cocos/2d/CCActionInterval.h>
#include <cocos/2d/CCActionInstant.h>

#include <cocos/ui/UIButton.h>

#include <unordered_map>

namespace
{
    constexpr float BattleStartDelay = 3.0f;
    constexpr uint32_t CultistFrames = 5u;
    constexpr const char* PlayerFileHead = "cultist_priest_idle_";

    constexpr const char* EnemyFileHead = "enemies/knight_";
    const cocos2d::Size EnemyFileSize = { 672.0f, 96.0f };
    constexpr uint32_t EnemyFrames = 6u;

    const std::unordered_map<EnvironmentInfluence, const char*> WeatherSpritesMapping = {
        { EnvironmentInfluence::Hot, "weather_hot.png" },
        { EnvironmentInfluence::Cold, "weather_cold.png" },
        { EnvironmentInfluence::Rain, "weather_rain.png" },
        { EnvironmentInfluence::Drought, "weather_drought.png" },
    };

    const std::array<const char*, 4> map_bg_sprites =
    {
        "background/mountain.png",
        "background/beach.png",
        "background/desert.png",
        "background/forest.png"
    };

    const std::array<const char*, 4> map_enemy_sprites =
    {
        "mountain.png",
        "beach.png",
        "desert.png",
        "forest.png"
    };
}

BattleScene* BattleScene::create(BattleCtx ctx, std::vector<Unit>& units)
{
    BattleScene* scene = new BattleScene(ctx, units);
    if (scene && scene->init())
    {
        scene->autorelease();
        return scene;
    }

    CC_SAFE_DELETE(scene);
    return nullptr;
}

BattleScene::BattleScene(BattleCtx ctx, std::vector<Unit>& units)
    : BaseScene(units)
    , m_ctx(ctx)
    , m_fighter_ai(ctx.stage_id)
{
}

bool BattleScene::init()
{
    if (BaseScene::init() == false)
    {
        return false;
    }

    scheduleUpdate();

    initBackground();

    const cocos2d::Size visible_size = cocos2d::Director::getInstance()->getVisibleSize();

    cocos2d::Label* battle_desc = cocos2d::Label::create();
    battle_desc->setDimensions(300.0f, 300.0f);
    battle_desc->setString("You can use spells (if you have them)\nby clicking on them. You don\'t need to\nattack - only use spells.");
    battle_desc->setSystemFontSize(20);
    battle_desc->setAnchorPoint({ 0.5f, 0.5f });
    battle_desc->setHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    battle_desc->setVerticalAlignment(cocos2d::TextVAlignment::CENTER);
    battle_desc->setTextColor(cocos2d::Color4B::YELLOW);
    battle_desc->setPosition({ visible_size.width * 0.8f, visible_size.height * 0.9f });

    addChild(battle_desc);

    initDurationLabel();

    initPlayer();
    initEnemy();

    cocos2d::ui::Button* leave_btn = cocos2d::ui::Button::create(
        "large_button.png",
        "large_button.png",
        "large_button.png",
        cocos2d::ui::Widget::TextureResType::PLIST
    );

    leave_btn->setTitleText("Give up");
    leave_btn->setTitleFontSize(24);

    leave_btn->setPosition({ visible_size.width * 0.5f, visible_size.height * 0.9f });

    leave_btn->addTouchEventListener(CC_CALLBACK_0(BattleScene::lose, this));

    addChild(leave_btn);

    return true;
}

void BattleScene::update(float dt)
{
    if (m_play_end)
    {
        return;
    }

    m_battle_duration += dt;
    m_duration_label->setString(std::to_string(std::abs(static_cast<int>(m_battle_duration))));

    if (m_started == false && m_battle_duration > 0.0f)
    {
        startBattle();
    }

    if (m_started == false)
    {
        return;
    }

    m_fighter_ai.update(dt);

    tryAttack(m_player, m_enemy);
    tryAttack(m_enemy, m_player);

    if (m_player->wantToCast() >= 0)
    {
        handleCast(m_player, m_enemy);
    }
    if (m_enemy->wantToCast() >= 0)
    {
        handleCast(m_enemy, m_player);
    }

    if (m_enemy->isDead())
    {
        m_play_end = true;
        initTextLabel("You win!", [this]() {
            win();
        }, 1.0f);
        return;
    }
    if (m_player->isDead())
    {
        m_play_end = true;
        initTextLabel("You lose!", [this]() {
           lose();
        }, 1.0f);
        return;
    }
}

void BattleScene::initDurationLabel()
{
    m_duration_label = cocos2d::Label::create();
    m_duration_label->setString(std::to_string(0.0f));

    m_duration_label->setSystemFontSize(64);
    m_duration_label->setAnchorPoint({ 0.5f, 0.5f });

    const cocos2d::Size visible_size = cocos2d::Director::getInstance()->getVisibleSize();
    m_duration_label->setPosition({ visible_size.width * 0.5f, visible_size.height * 0.7f });

    addChild(m_duration_label);

    m_battle_duration = -BattleStartDelay;
}

void BattleScene::initPlayer()
{
    const cocos2d::Size visible_size = cocos2d::Director::getInstance()->getVisibleSize();

    m_player = Fighter::create(m_units[m_ctx.active_unit], true);
    m_player->setPosition({ visible_size.width * 0.3f, visible_size.height * 0.5f });

    addChild(m_player);

    cocos2d::Vector<cocos2d::SpriteFrame*> frames;

    cocos2d::SpriteFrameCache* sprite_cache = cocos2d::SpriteFrameCache::getInstance();

    for (size_t i = 0u; i < CultistFrames; i++)
    {
        frames.pushBack(
            sprite_cache->getSpriteFrameByName(std::string(PlayerFileHead) + std::to_string(i + 1) + ".png")
        );
    }

    auto fighter_sprite = m_player->setFighterAnimation(std::move(frames));

    if (fighter_sprite != nullptr)
    {
        fighter_sprite->setPosition({ 0.0f, -20.0f });
    }
}

void BattleScene::initEnemy()
{
    const cocos2d::Size visible_size = cocos2d::Director::getInstance()->getVisibleSize();

    m_enemy = Fighter::create(unit_helper::generateUnit(m_ctx.stage_id, m_ctx.location_id), false);
    m_fighter_ai.setFighter(m_enemy);

    m_enemy->setPosition({ visible_size.width * 0.7f, visible_size.height * 0.5f });

    addChild(m_enemy);

    cocos2d::Vector<cocos2d::SpriteFrame*> frames;

    cocos2d::SpriteFrameCache* sprite_cache = cocos2d::SpriteFrameCache::getInstance();
    const float frame_step = EnemyFileSize.width / EnemyFrames;
    for (size_t i = 0u; i < EnemyFrames; i++)
    {
        frames.pushBack(
            cocos2d::SpriteFrame::create(std::string(EnemyFileHead) + map_enemy_sprites[m_ctx.location_id],
                cocos2d::Rect(frame_step * i, 0.0f, frame_step, EnemyFileSize.height))
        );
    }

    auto fighter_sprite = m_enemy->setFighterAnimation(std::move(frames));
    if (fighter_sprite != nullptr)
    {
        fighter_sprite->setFlippedX(true);
        fighter_sprite->setScale(2.5f);
        fighter_sprite->setPosition({ 0.0f, 10.0f });
    }
}

void BattleScene::startBattle()
{
    m_started = true;
    initTextLabel("START", [this]() {
    });
}

void BattleScene::initTextLabel(const char* text, std::function<void()> callback, float duration)
{
    const cocos2d::Size visible_size = cocos2d::Director::getInstance()->getVisibleSize();

    cocos2d::Label* start_label = cocos2d::Label::create();
    start_label->setString(text);

    start_label->setSystemFontSize(64);
    start_label->setAnchorPoint({ 0.5f, 0.5f });
    start_label->setPosition(visible_size * 0.5f);
    start_label->setScale(0.0f);

    cocos2d::ScaleTo* scale_action = cocos2d::ScaleTo::create(0.5f, 1.0f);
    cocos2d::FadeOut* fade_out = cocos2d::FadeOut::create(0.2f);

    cocos2d::DelayTime* delay = cocos2d::DelayTime::create(duration);
    cocos2d::ActionInstant* instant_action = cocos2d::RemoveSelf::create();
    cocos2d::ActionInstant* callback_action = cocos2d::CallFunc::create(callback);

    cocos2d::Sequence* sequence = cocos2d::Sequence::create(scale_action, delay, fade_out, instant_action, callback_action, nullptr);
    
    start_label->runAction(sequence);

    addChild(start_label);
}

void BattleScene::initBackground()
{
    cocos2d::Sprite* bg_sprite = cocos2d::Sprite::create(map_bg_sprites[m_ctx.location_id]);
    bg_sprite->setAnchorPoint({ 0.5f, 0.5f });
  
    const cocos2d::Size visible_size = cocos2d::Director::getInstance()->getVisibleSize();
    bg_sprite->setPosition(visible_size * 0.5f);

    addChild(bg_sprite);
}

void BattleScene::tryAttack(Fighter* attacker, Fighter* defencer)
{
    if (attacker->isReadyAttack() == false)
    {
        return;
    }

    float damage = attacker->attack();

    if (damage == 0.0f)
    {
        return;
    }

    float accuracy = attacker->getActiveUnit().accuracy;

    float def_evasion = defencer->getActiveUnit().evasion;

    bool evaded = cocos2d::random(0.0f, 1.0f) < def_evasion;
    if (evaded)
    {
        if (cocos2d::random(0.0f, 1.0f) > accuracy)
        {
            showBattleAction(defencer->getPosition(), "evade.png");
            return;
        }
    }

    defencer->decreaseStat(UnitField::HP, damage);
    showBattleAction(defencer->getPosition(), "hit.png");
}

void BattleScene::win()
{
    Unit newChimera = m_enemy->getBaseUnit();
    newChimera.sprite_name = m_player->getBaseUnit().sprite_name;
    newChimera.unit_name = m_player->getBaseUnit().unit_name;

    m_units.push_back(newChimera);

    unit_loader::saveToFile(m_units, unit_loader::defaultInventoryFile());

    cocos2d::Director::getInstance()->replaceScene(MenuScene::create(m_units));
}

void BattleScene::lose()
{
    m_units[m_ctx.active_unit] = m_units.back();
    m_units.pop_back();

    unit_loader::saveToFile(m_units, unit_loader::defaultInventoryFile());

    cocos2d::Director::getInstance()->replaceScene(MenuScene::create(m_units));
}

void BattleScene::handleCast(Fighter* caster, Fighter* defencer)
{
    const int spellId = caster->wantToCast();

    const bool result = caster->useDesiredSpell();
    if (result)
    {
        const Spell& spell = profile::spells.find(spellId)->second;

        showBattleAction(caster->getPosition(), spell.sprite_id);

        if (spell.to_myself)
        {
            caster->decreaseStat(spell.value_field, spell.value);
        }
        else
        {
            defencer->decreaseStat(spell.value_field, spell.value);
        }
        if (spell.influence != m_currentEnvironment && spell.influence != EnvironmentInfluence::None)
        {
            handleInfluenceChange(spell.influence);
        }
    }
}

void BattleScene::handleInfluenceChange(EnvironmentInfluence newInfluence)
{
    uint32_t currentGroup = selectEnvironmentGroup(m_currentEnvironment);
    uint32_t newGroup = selectEnvironmentGroup(newInfluence);

    if (currentGroup == newGroup)
    {
        m_currentEnvironment = EnvironmentInfluence::None;
    }
    else
    {
        m_currentEnvironment = newInfluence;
    }

    setupWeatherSprite();

    setupInfluence(m_enemy);
    setupInfluence(m_player);
}

void BattleScene::setupInfluence(Fighter* target)
{
    // TODO: add to weather damage to config
    switch (m_currentEnvironment)
    {
    case EnvironmentInfluence::None:
        break;
    case EnvironmentInfluence::Hot:
        damageEnvironmentStat(EnvironmentInfluence::Hot, UnitField::HP_REGEN, target, -1.2f);
        break;
    case EnvironmentInfluence::Cold:
        damageEnvironmentStat(EnvironmentInfluence::Cold, UnitField::MP_REGEN, target, -1.2f);
        damageEnvironmentStat(EnvironmentInfluence::Cold, UnitField::ATK_SPEED, target, 0.3f);
        break;
    case EnvironmentInfluence::Rain:
        damageEnvironmentStat(EnvironmentInfluence::Rain, UnitField::EVASION, target, 0.3f);
        damageEnvironmentStat(EnvironmentInfluence::Rain, UnitField::HP_REGEN, target, 1.0f);
        break;
    case EnvironmentInfluence::Drought:
        damageEnvironmentStat(EnvironmentInfluence::Drought, UnitField::ATTACK, target, 0.2f);
        damageEnvironmentStat(EnvironmentInfluence::Drought, UnitField::ATK_SPEED, target, -0.3f);
        break;
    default:
        break;
    }
}

void BattleScene::setupWeatherSprite()
{
    if (m_weather_sprite != nullptr)
    {
        removeChild(m_weather_sprite);
        m_weather_sprite = nullptr;
    }

    if (m_currentEnvironment == EnvironmentInfluence::None)
    {
        return;
    }

    const auto& env_it = WeatherSpritesMapping.find(m_currentEnvironment);
    CC_ASSERT(env_it != WeatherSpritesMapping.end());

    m_weather_sprite = cocos2d::Sprite::createWithSpriteFrameName(env_it->second);
    m_weather_sprite->setAnchorPoint({ 0.0f, 1.0f });
    m_weather_sprite->setPosition({ 0.0f, getContentSize().height });

    cocos2d::Sprite* weather_info = cocos2d::Sprite::createWithSpriteFrameName("weather_panel.png");
    weather_info->setAnchorPoint({ 0.0f, 0.5f });
    weather_info->setPosition({ m_weather_sprite->getContentSize().width, m_weather_sprite->getContentSize().height * 0.5f });
    
    const cocos2d::Size info_size = weather_info->getContentSize();

    switch (m_currentEnvironment)
    {
    case EnvironmentInfluence::None:
        break;
    case EnvironmentInfluence::Hot:
    {
        auto hp_regen = createLabel("decrease hp regen");
        hp_regen->setPosition(info_size * 0.5f);
        weather_info->addChild(hp_regen);
        break;
    }
    case EnvironmentInfluence::Cold:
    {
        auto mp_regen = createLabel("decrease mp regen");
        mp_regen->setPosition({ info_size.width * 0.5f, info_size.height * 0.3f });
        weather_info->addChild(mp_regen);

        auto atk_speed = createLabel("decrease atk speed");
        atk_speed->setPosition({ info_size.width * 0.5f, info_size.height * 0.6f });
        weather_info->addChild(atk_speed);
        break;
    }
    case EnvironmentInfluence::Rain:
    {
        auto evasion = createLabel("increase evasion");
        evasion->setPosition({ info_size.width * 0.5f, info_size.height * 0.3f });
        weather_info->addChild(evasion);

        auto hp_regen = createLabel("increase hp regen");
        hp_regen->setPosition({ info_size.width * 0.5f, info_size.height * 0.6f });
        weather_info->addChild(hp_regen);
        break;
    }
    case EnvironmentInfluence::Drought:
    {
        auto attack = createLabel("increase attack");
        attack->setPosition({ info_size.width * 0.5f, info_size.height * 0.3f });
        weather_info->addChild(attack);

        auto atk_speed = createLabel("increase atk speed");
        atk_speed->setPosition({ info_size.width * 0.5f, info_size.height * 0.6f });
        weather_info->addChild(atk_speed);
        break;
    }
    }

    m_weather_sprite->addChild(weather_info);
    addChild(m_weather_sprite);
}

void BattleScene::showBattleAction(cocos2d::Vec2 pos, const char* img_name)
{
    cocos2d::Sprite* target_sprite = cocos2d::Sprite::createWithSpriteFrameName(img_name);
    target_sprite->setAnchorPoint({ 0.5f, 0.5f });

    target_sprite->setScale(0.0f);

    cocos2d::ScaleTo* scale_action = cocos2d::ScaleTo::create(0.5f, 1.0f);
    cocos2d::EaseBackOut* ease_action = cocos2d::EaseBackOut::create(scale_action);

    cocos2d::FadeOut* fade_out = cocos2d::FadeOut::create(0.2f);

    cocos2d::ActionInstant* instant_action = cocos2d::RemoveSelf::create();

    cocos2d::Sequence* sequence = cocos2d::Sequence::create(ease_action, fade_out, instant_action, nullptr);

    target_sprite->runAction(sequence);

    target_sprite->setPosition(pos);
    addChild(target_sprite);
}

uint32_t BattleScene::selectEnvironmentGroup(EnvironmentInfluence influence)
{
    switch (influence)
    {
    case EnvironmentInfluence::None: return 0u;
        break;
    case EnvironmentInfluence::Hot: return 1u;
        break;
    case EnvironmentInfluence::Cold: return 1u;
        break;
    case EnvironmentInfluence::Rain: return 2u;
        break;
    case EnvironmentInfluence::Drought: return 2u;
    default:
        break;
    }

    return 0u;
}

void BattleScene::damageEnvironmentStat(EnvironmentInfluence influence, UnitField field, Fighter* target, float increase_by_percent)
{
    const Unit& active_unit = target->getActiveUnit();
    const auto& unit_it = active_unit.resistance.find(static_cast<uint32_t>(influence));

    float resist = 0.0f;
    if (unit_it != active_unit.resistance.end())
    {
        resist = unit_it->second;
    }

    float old = unit_helper::getValueByField<float>(field, active_unit);
    float new_val = old * (increase_by_percent + std::abs(resist / std::max(increase_by_percent, 0.01f)));

    target->decreaseStat(field, -new_val);
}

cocos2d::Node* BattleScene::createLabel(std::string text)
{
    cocos2d::Label* label = cocos2d::Label::create();
    label->setString(text);
    label->setSystemFontSize(20);
    label->setAnchorPoint({ 0.5f, 0.5f });

    return label;
}

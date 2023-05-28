/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#pragma once

#include "BaseScene.h"

#include "BattleEnvironment.h"
#include "FighterAI.h"
#include "Profile.h"

#include <memory>

class Fighter;

namespace cocos2d
{
    class Label;
    class Sprite;
}

class BattleScene final : public BaseScene
{
public:
    struct BattleCtx
    {
        uint32_t active_unit = 0u;

        uint32_t location_id = 0u;
        uint32_t stage_id = 0u;
    };

    static BattleScene* create(BattleCtx ctx, std::vector<Unit>& units);

private:
    BattleScene(BattleCtx ctx, std::vector<Unit>& units);

    bool init() override;
    void update(float dt) override;

private:
    void initDurationLabel();
    void initPlayer();
    void initEnemy();

    void startBattle();

    void initTextLabel(const char* text, std::function<void()> callback, float duration = 0.0f);
    void initBackground();

    void tryAttack(Fighter* attacker, Fighter* defencer);

    void win();
    void lose();

    void handleCast(Fighter* caster, Fighter* defencer);
    void handleInfluenceChange(EnvironmentInfluence newInfluence);
    void setupInfluence(Fighter* target);

    void setupWeatherSprite();

    void showBattleAction(cocos2d::Vec2 pos, const char* img_name);

    uint32_t selectEnvironmentGroup(EnvironmentInfluence influence);

    void damageEnvironmentStat(EnvironmentInfluence influence, UnitField field, Fighter* target, float increase_by_percent);

    cocos2d::Node* createLabel(std::string text);
   
private:
    BattleCtx m_ctx;

    Fighter* m_player = nullptr;

    FighterAI m_fighter_ai;
    Fighter* m_enemy = nullptr;

    bool m_started = false;
    bool m_play_end = false;

    float m_battle_duration = 0.0f;

    cocos2d::Label* m_duration_label = nullptr;
    cocos2d::Sprite* m_weather_sprite = nullptr;

    EnvironmentInfluence m_currentEnvironment = EnvironmentInfluence::None;
};

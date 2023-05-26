/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#include "UnitLoader.h"

#include <cocos/platform/CCFileUtils.h>

#include <tinyxml2.h>

namespace unit_loader
{
    namespace
    {
        constexpr const char* UnitName = "unit";

        constexpr const char* HPAttr = "hp";
        constexpr const char* MPAttr = "mp";
        constexpr const char* HPRegenAttr = "hp_regen";
        constexpr const char* MPRegenAttr = "mp_regen";
        constexpr const char* AttackAttr = "attack";
        constexpr const char* AtkSpeedAttr = "atk_speed";
        constexpr const char* EvasionAttr = "evasion";
        constexpr const char* AccuracyAttr = "accuracy";
        constexpr const char* SpriteAttr = "sprite";
        constexpr const char* NameAttr = "name";

    }

    const char* checkName(const char* name)
    {
        return name == nullptr ? "" : name;
    }

    Unit loadUnit(tinyxml2::XMLElement* element)
    {
        Unit unit;

        tinyxml2::XMLUtil::ToFloat(element->Attribute(HPAttr), &unit.hp);
        tinyxml2::XMLUtil::ToFloat(element->Attribute(MPAttr), &unit.mp);

        tinyxml2::XMLUtil::ToFloat(element->Attribute(HPRegenAttr), &unit.hp_regen);
        tinyxml2::XMLUtil::ToFloat(element->Attribute(MPRegenAttr), &unit.mp_regen);

        tinyxml2::XMLUtil::ToUnsigned(element->Attribute(AttackAttr), &unit.attack);

        tinyxml2::XMLUtil::ToFloat(element->Attribute(AtkSpeedAttr), &unit.atk_speed);

        tinyxml2::XMLUtil::ToFloat(element->Attribute(EvasionAttr), &unit.evasion);
        tinyxml2::XMLUtil::ToFloat(element->Attribute(AccuracyAttr), &unit.accuracy);

        unit.sprite_name = checkName(element->Attribute(SpriteAttr));
        unit.unit_name = checkName(element->Attribute(NameAttr));

        return unit;
    }

    void saveUnit(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* root, const Unit& unit)
    {
        tinyxml2::XMLElement* element = doc.NewElement(UnitName);

        element->SetAttribute(HPAttr, unit.hp);
        element->SetAttribute(MPAttr, unit.mp);

        element->SetAttribute(HPRegenAttr, unit.hp_regen);
        element->SetAttribute(MPRegenAttr, unit.mp_regen);

        element->SetAttribute(AttackAttr, unit.attack);
        element->SetAttribute(AtkSpeedAttr, unit.atk_speed);

        element->SetAttribute(EvasionAttr, unit.evasion);
        element->SetAttribute(AccuracyAttr, unit.accuracy);

        element->SetAttribute(SpriteAttr, unit.sprite_name);
        element->SetAttribute(NameAttr, unit.unit_name);

        root->InsertEndChild(element);
    }

    Units loadFromFile(const char* filename)
    {
        CCASSERT(filename != nullptr, "Invalid file name");

        Units inventory;

        tinyxml2::XMLDocument doc;

        std::string path = cocos2d::FileUtils::getInstance()->fullPathForFilename(filename);

        const tinyxml2::XMLError result = doc.LoadFile(path.c_str());
        CCLOG("[UNIT LOADER]: %s load result is %d\n", filename, result);

        tinyxml2::XMLElement* root = doc.RootElement();
        if (root == nullptr)
        {
            return inventory;
        }

        for (tinyxml2::XMLElement* item = root->FirstChildElement(UnitName); item != nullptr; item = item->NextSiblingElement(UnitName))
        {
            inventory.push_back(loadUnit(item));
        }

        return inventory;
    }

    void saveToFile(const Units& inventory, const char* filename)
    {
        CCASSERT(filename != nullptr, "Invalid file name");
        tinyxml2::XMLDocument doc;

        doc.NewDeclaration();

        tinyxml2::XMLElement* root = doc.NewElement("root");
        doc.InsertFirstChild(root);

        for (const auto& unit : inventory)
        {
            saveUnit(doc, root, unit);
        }

        std::string path = cocos2d::FileUtils::getInstance()->fullPathForFilename(filename);

        const tinyxml2::XMLError result = doc.SaveFile(path.c_str());
        CCLOG("[UNIT LOADER]: %s save result is %d\n", filename, result);
    }
}
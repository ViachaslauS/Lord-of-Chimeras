/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#pragma once

#include "Unit.h"

#include <vector>

namespace tinyxml2
{
    class XMLDocument;
    class XMLElement;
}

namespace unit_loader
{
    const char* defaultInventoryFile();

    using Units = std::vector<Unit>;

    Unit loadUnit(tinyxml2::XMLElement* element);
    void saveUnit(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* root, const Unit& unit);

    Units loadFromFile(const char* filename);
    void saveToFile(const Units& inventory, const char* filename);
}
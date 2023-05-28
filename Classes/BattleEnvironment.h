/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#pragma once

#include <inttypes.h>

enum class EnvironmentInfluence : uint32_t
{
    None,
    Hot,
    Cold,
    Rain,
    Drought,
};
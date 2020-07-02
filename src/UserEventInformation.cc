#include "UserEventInformation.hh"

#include "spdlog/logger.h"

void g4e::UserEventInformation::Print() const
{
    using namespace fmt;
    print("HasTrueDISInfo: {:<5}  x={:<10} Q2={:<10}\n",  GetEventInfoConst().HasTrueDISInfo, GetEventInfoConst().TrueDISInfo.x, GetEventInfoConst().TrueDISInfo.Q2);
}

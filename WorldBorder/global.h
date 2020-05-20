#pragma once

#include <yaml.h>
#include <string>
#include <map>
#include <log.h>
#include "playerdb.h"
#include <Actor/Actor.h>
#include <Actor/Player.h>
#include <Packet/TextPacket.h>
#include <boost/algorithm/string/replace.hpp>


struct Settings {
  long long minX = -30000;
  long long maxX = -30000;

  long long minZ = 30000;
  long long maxZ = 30000;

  int informBefore = 10;

  std::string messageBorder = "You've reached world border!";
  std::string messageBorderApproach = "You will reach border at %coord% pos in %number% blocks!";


  template <typename IO> static inline bool io(IO f, Settings &settings, YAML::Node &node) {
    return f(settings.minX, node["minX"]) && f(settings.minZ, node["minZ"]) && f(settings.maxX, node["maxX"]) &&
           f(settings.maxZ, node["maxZ"]) && f(settings.messageBorder, node["reachBorderMessage"]) &&
           f(settings.messageBorderApproach, node["messageBorderApproach"]) &&
           f(settings.informBefore, node["informBefore"]);
  }
};

DEF_LOGGER("WB");

extern Settings settings;
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

struct WorldBorder {
  long long minX = -30000;
  long long maxX = 30000;

  long long minZ = -30000;
  long long maxZ = 30000;

  template <typename IO> static inline bool io(IO f, WorldBorder &overworld, YAML::Node &node) {
    return f(overworld.minX, node["minX"]) && f(overworld.minZ, node["minZ"]) && f(overworld.maxX, node["maxX"]) &&
           f(overworld.maxZ, node["maxZ"]);
  }
};

struct Settings {
  int informBefore = 10;

  std::string messageBorder         = "You've reached world border!";
  std::string messageBorderApproach = "You will reach border at %coord% pos in %number% blocks!";

  WorldBorder overworld;

  WorldBorder nether;

  WorldBorder end;

  template <typename IO> static inline bool io(IO f, Settings &settings, YAML::Node &node) {
    return f(settings.messageBorder, node["reachBorderMessage"]) &&
           f(settings.messageBorderApproach, node["messageBorderApproach"]) &&
           f(settings.informBefore, node["informBefore"]) && f(settings.overworld, node["overworld"]) &&
           f(settings.nether, node["nether"]) && f(settings.end, node["end"]);
  }
};

DEF_LOGGER("WB");

extern Settings settings;
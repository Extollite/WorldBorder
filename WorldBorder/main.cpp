#include "global.h"

#include <dllentry.h>

Settings settings;
DEFAULT_SETTINGS(settings);

void dllenter() {}
void dllexit() {}

std::unordered_map<int, WorldBorder> borders;

void PreInit() { borders[0] = settings.overworld;
  borders[1] = settings.nether;
  borders[2]               = settings.end;
}
void PostInit() {}



void teleport(Player& thi, float x, float y, float z) {
  Vec3 teleportTo;
  teleportTo.x = x;
  teleportTo.y = y - 0.9;
  teleportTo.z = z;
  thi.teleport(teleportTo, {0}, thi.getDimensionId());
  auto pk = TextPacket::createTextPacket<TextPacketType::JukeboxPopup>(settings.messageBorder);
  thi.sendNetworkPacket(pk);
}

void reachMsg(Player& thi, std::string pos, float number) {
  std::string out(settings.messageBorderApproach);
  boost::replace_all(out, "%coord%", pos);
  boost::replace_all(out, "%number%", std::to_string((int) (number)));
  auto pk = TextPacket::createTextPacket<TextPacketType::JukeboxPopup>(out);
  thi.sendNetworkPacket(pk);
}

THook(void *, "?move@Player@@UEAAXAEBVVec3@@@Z", Player &thi, Vec3 const &newPos) {
  if (thi.getCommandPermissionLevel() > CommandPermissionLevel::Any) return original(thi, newPos);
  Vec3 prevPos = thi.getPos();
  void *ret    = original(thi, newPos);
  Vec3 currPos = thi.getPos();
  if (std::fabs(currPos.x - prevPos.x) >= 0.00001 ||
      std::fabs(currPos.z - prevPos.z) >= 0.00001) {
    WorldBorder border = borders[thi.getDimensionId().value];
    if (currPos.x >= border.maxX) {
      teleport(thi, prevPos.x - 1, prevPos.y, prevPos.z);
    } else if (currPos.x <= border.minX) {
      teleport(thi, prevPos.x + 1, prevPos.y, prevPos.z);
    } else if (currPos.z >= border.maxZ) {
      teleport(thi, prevPos.x, prevPos.y, prevPos.z - 1);
    } else if (currPos.z <= border.minZ) {
      teleport(thi, prevPos.x, prevPos.y, prevPos.z + 1);
    }
    
    std::string coord = "X";
    float min         = std::fabs(border.maxX - currPos.x);
    if (std::fabs(border.minX - currPos.x) < min) {
      min = std::fabs(border.minX - currPos.x);
    }
    if (std::fabs(border.maxZ - currPos.z) < min) {
      min = std::fabs(border.maxZ - currPos.z);
      coord = "Z";
    }
    if (std::fabs(border.minZ - currPos.z) < min) {
      min   = std::fabs(border.minZ - currPos.z);
      coord = "Z";
    }
    if (min <= settings.informBefore + 1)
        reachMsg(thi, coord, min);
  }
  return ret;
}

THook(bool, "?teleportTo@Player@@UEAAXAEBVVec3@@_NHH@Z", Player &thi, Vec3 const & pos, bool b1, int i1, int i2) {
  if (thi.getCommandPermissionLevel() > CommandPermissionLevel::Any) return original(thi, pos, b1, i1, i2);
  Vec3 prevPos = thi.getPos();
  WorldBorder border = borders[thi.getDimensionId().value];
  if (pos.x >= border.maxX || pos.x <= border.minX || pos.z >= border.maxZ || pos.z <= border.minZ) {
    teleport(thi, prevPos.x, prevPos.y, prevPos.z);
    return false;
  }
  return original(thi, pos, b1, i1, i2);
}

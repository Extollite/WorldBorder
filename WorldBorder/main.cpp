#include "global.h"

#include <dllentry.h>

Settings settings;
DEFAULT_SETTINGS(settings);

void dllenter() {}
void dllexit() {}

void PreInit() {}
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
    if (currPos.x >= settings.maxX) {
      teleport(thi, prevPos.x - 1, prevPos.y, prevPos.z);
    } else if (currPos.x <= settings.minX) {
      teleport(thi, prevPos.x + 1, prevPos.y, prevPos.z);
    } else if (currPos.z >= settings.maxZ) {
      teleport(thi, prevPos.x, prevPos.y, prevPos.z - 1);
    } else if (currPos.z <= settings.minZ) {
      teleport(thi, prevPos.x, prevPos.y, prevPos.z + 1);
    }
    
    if (std::fabs(settings.maxX - currPos.x) <= 10) {
      reachMsg(thi, "X", std::fabs(settings.maxX - currPos.x));
    } else if (std::fabs(settings.minX - currPos.x) <= 10) {
      reachMsg(thi, "X", std::fabs(settings.minX - currPos.x));
    } else if (std::fabs(settings.maxZ - currPos.z) <= 10) {
      reachMsg(thi, "Z", std::fabs(settings.maxZ - currPos.z));
    } else if (std::fabs(settings.minZ - currPos.z) <= 10) {
      reachMsg(thi, "Z", std::fabs(settings.minZ - currPos.z));
    }
  }
  return ret;
}

THook(bool, "?teleportTo@Player@@UEAAXAEBVVec3@@_NHH@Z", Player &thi, Vec3 const & pos, bool b1, int i1, int i2) {
  if (thi.getCommandPermissionLevel() > CommandPermissionLevel::Any) return original(thi, pos, b1, i1, i2);
  Vec3 prevPos = thi.getPos();
  if (pos.x >= settings.maxX || pos.x <= settings.minX || pos.z >= settings.maxZ || pos.z <= settings.minZ) {
    teleport(thi, prevPos.x, prevPos.y, prevPos.z);
    return false;
  }
  return original(thi, pos, b1, i1, i2);
}

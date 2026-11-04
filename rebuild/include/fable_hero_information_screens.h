#pragma once
#include "rebuild_abi.h"
class CWorld;
namespace NHeroInformationScreens {
// Original non-polymorphic manager, only its retail OpenFlag prefix is modeled.
// FableWin +4C / Ego_r +40; retail +34 follows three 12-byte STLport vectors.
class CManager {
public:
    explicit CManager(CWorld& world);
    ~CManager();
    void RemoveAllPlayerModes() const; // retail 0066E985
    void Close(); // retail 0066E73D
private:
    friend struct HeroInformationManagerTestAccess;
    fable_u8 unknown00_[0x34];
    bool OpenFlag; // +34; remainder and full allocation size not modeled.
};
}

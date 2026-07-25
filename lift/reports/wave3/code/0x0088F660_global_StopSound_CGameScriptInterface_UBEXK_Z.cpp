#include <cstdint>

class CGameScriptInterface
{
public:
    virtual void StopSound(unsigned long soundId) const;
};

extern CGameScriptInterface* DAT_013B8394;

void CGameScriptInterface::StopSound(unsigned long soundId) const
{
    DAT_013B8394->StopSound(soundId);
}
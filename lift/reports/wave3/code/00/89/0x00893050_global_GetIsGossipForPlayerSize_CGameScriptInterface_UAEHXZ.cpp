#include <cstdint>

extern std::int32_t DAT_013bae3c;

class CGameScriptInterface
{
public:
    int __thiscall GetIsGossipForPlayerSize();
};

int __thiscall CGameScriptInterface::GetIsGossipForPlayerSize()
{
    return DAT_013bae3c;
}
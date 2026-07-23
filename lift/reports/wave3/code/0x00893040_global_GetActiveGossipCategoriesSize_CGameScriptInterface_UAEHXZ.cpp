#include <cstdint>

class CGameScriptInterface
{
public:
    int __thiscall GetActiveGossipCategoriesSize();
};

namespace
{
inline constexpr auto DAT_013BAE30 = reinterpret_cast<int*>(0x013BAE30);
}

int __thiscall CGameScriptInterface::GetActiveGossipCategoriesSize()
{
    return *DAT_013BAE30;
}
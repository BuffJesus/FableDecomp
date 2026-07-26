class CGameScriptInterface
{
public:
    void __thiscall SetCheapHeadLooking(bool cheapHeadLooking);
};

namespace
{
bool& DAT_013bae1b = *reinterpret_cast<bool*>(0x013BAE1B);
}

void __thiscall CGameScriptInterface::SetCheapHeadLooking(bool cheapHeadLooking)
{
    DAT_013bae1b = cheapHeadLooking;
}
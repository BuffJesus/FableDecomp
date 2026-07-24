class CGameScriptInterface
{
public:
    void __thiscall SetCountBetMoneyDown(bool countBetMoneyDown);
};

namespace
{
bool& DAT_013bae24 = *reinterpret_cast<bool*>(0x013BAE24);
}

void __thiscall CGameScriptInterface::SetCountBetMoneyDown(bool countBetMoneyDown)
{
    DAT_013bae24 = countBetMoneyDown;
}
class CGameScriptInterface
{
public:
    virtual void __thiscall SetTradingPriceMult(float tradingPriceMult);
};

extern float _DAT_013833d4;

void __thiscall CGameScriptInterface::SetTradingPriceMult(float tradingPriceMult)
{
    _DAT_013833d4 = tradingPriceMult;
}
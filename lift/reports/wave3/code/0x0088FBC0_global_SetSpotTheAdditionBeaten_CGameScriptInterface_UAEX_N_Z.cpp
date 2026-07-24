class CGameScriptInterface
{
public:
    virtual void __thiscall SetSpotTheAdditionBeaten(bool beaten);
};

extern bool DAT_013bae25;

void __thiscall CGameScriptInterface::SetSpotTheAdditionBeaten(bool beaten)
{
    DAT_013bae25 = beaten;
}
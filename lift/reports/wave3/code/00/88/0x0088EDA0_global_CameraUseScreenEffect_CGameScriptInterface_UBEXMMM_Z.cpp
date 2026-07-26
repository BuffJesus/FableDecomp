class CGameScriptInterface
{
public:
    void __thiscall CameraUseScreenEffect(float effectIntensity, float fadeInDuration, float fadeOutDuration) const;
};

void __thiscall CGameScriptInterface::CameraUseScreenEffect(
    float effectIntensity,
    float fadeInDuration,
    float fadeOutDuration) const
{
    (void)effectIntensity;
    (void)fadeInDuration;
    (void)fadeOutDuration;
}
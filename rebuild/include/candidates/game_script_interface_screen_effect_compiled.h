#ifndef FABLETLC_GAME_SCRIPT_INTERFACE_SCREEN_EFFECT_COMPILED_H
#define FABLETLC_GAME_SCRIPT_INTERFACE_SCREEN_EFFECT_COMPILED_H

class CGameScriptInterface
{
public:
    virtual void CameraCancelScreenEffect() const;
    virtual void CameraUseScreenEffect(
        float effectIntensity,
        float fadeInDuration,
        float fadeOutDuration) const;
};

#endif

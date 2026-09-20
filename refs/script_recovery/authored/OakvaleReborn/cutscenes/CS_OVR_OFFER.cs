# CS_OVR_OFFER -- beat 3, the offer. Run by stranger.lua offer() with actors
# HERO + Stranger (the created creature, acquired). Staged on the Father-intro
# markers by the house (the square) with the intro cameras. The yes/no question
# and the answer line are Lua (GiveHeroYesNoQuestion cannot run inside a macro).
PlayMusic MUSIC_SET_NULL
FadeOut 0.5,0
CameraPause FALSE
RemoveExtras TRUE,MK_OVI_ID_GIRL
HERO.Teleport MK_OVI_ID_HERO,FALSE
Stranger.Teleport MK_OVI_ID_DAD
Stranger.LookToThing HERO,FOREVER
HERO.LookToThing Stranger,FOREVER
DoScriptFrame 2
DoCameraPreloading
DoScriptFrame 1
NoLoadUseCamera CAM_OVI_ID_STANDUP
FadeIn
GamePause 1.0
Stranger.Speak HERO,'TEXT_OVR_OFFER_010'
GamePause 0.5
UseCamera CAM_OVI_ID_DADMID
Stranger.Speak HERO,'TEXT_OVR_OFFER_020'
GamePause 0.4
# the sword: drawn into his hand for the terms, held until Lua takes it back after the answer
Stranger.HoldInHand OBJECT_HERO_SWORD_FIRST,TRUE
Stranger.PlayAnimation CS_HOLD_SWORD
NoLoadUseCamera CAM_OVI_ID_HERODAD
Stranger.Speak HERO,'TEXT_OVR_OFFER_030'
GamePause 0.6
UseCamera CAM_OVI_ID_DAD
Stranger.Speak HERO,'TEXT_OVR_OFFER_040'
GamePause 0.8
UseCamera CAM_OVI_ID_WIDE
# the villagers return; the sword stays in his hand until Lua takes it back after the answer
RemoveExtras FALSE,RETURN

[SkipCond]
FadeOut
StayFadedOut
GamePause 0.5
HERO.FadeIn 0
Stranger.FadeIn 0
RemoveExtras FALSE,RETURN
PlayMusic MUSIC_SET_NULL,FALSE

[SetupCond]
""

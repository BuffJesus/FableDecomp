# CS_OVR_COLDOPEN -- beat 0, the Stranger on the hill. Runs from DoMission before
# the Father wakes the hero (OVR_LiveFather waits for ColdOpenDone). Actors: HERO
# (asleep, kept out of frame). The hill / cliff staging is the retail raid's:
# the archer's cliff marker and its spline camera. Framing unverified until S1/S2.
PlayMusic MUSIC_SET_NULL
FadeOut 0.5,0
CameraPause FALSE
SetTime 6
Create CREATURE_PROPHET_01,MK_OIF_BANARCH,STRANGER
STRANGER.LookToThing HERO,FOREVER
DoScriptFrame 2
DoCameraPreloading
DoScriptFrame 1
NoLoadUseCamera CAM_OIF_SHOT3
FadeIn
GamePause 2.0
STRANGER.Speak HERO,'TEXT_OVR_COLD_010'
GamePause 0.8
UseCamera CAM_OIF_SHOT4NEW
STRANGER.Speak HERO,'TEXT_OVR_COLD_020'
GamePause 1.2
FadeOut 1.0,0
GamePause 1.0
Remove STRANGER
SetTime 12

[SkipCond]
FadeOut
StayFadedOut
GamePause 0.5
Remove STRANGER
SetTime 12
PlayMusic MUSIC_SET_NULL,FALSE

[SetupCond]
""

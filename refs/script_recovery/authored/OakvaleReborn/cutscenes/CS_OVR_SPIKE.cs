# CS_OVR_SPIKE -- Phase 0 spike S1: the first cutscene ever APPENDED to script.bin.
# Every marker, camera and text key here is retail (borrowed from
# CS_OAKVALE_INTRO_FATHER) so the only new thing under test is the appended
# CCutsceneDef itself. Run from Lua with actors HERO + Father, e.g.
#   Scene.RunMacro(quest, "CS_OVR_SPIKE", { HERO = heroCtrl, Father = fatherCtrl })
# Pass = FableScriptExtender.log shows ENTERING/EXITED BLOCKING for CS_OVR_SPIKE,
# the camera cuts to CAM_OVI_ID_STANDUP then CAM_OVIF_SHOT2, and the Father line
# is heard.
PlayMusic MUSIC_SET_NULL
FadeOut 0.5,0
CameraPause FALSE
Hero.Teleport MK_OVI_ID_HERO,FALSE
Father.Teleport MK_OVI_ID_DAD
Father.LookToThing Hero,FOREVER
DoScriptFrame 2
NoLoadUseCamera CAM_OVI_ID_STANDUP
FadeIn
GamePause 1.0
Father.Speak Father,'TEXT_QST_048_FATHER_INTRO_10'
GamePause 0.5
UseCamera CAM_OVIF_SHOT2
GamePause 2.0
FadeOut 0.5,0

[SkipCond]
FadeOut
StayFadedOut
GamePause 0.5
PlayMusic MUSIC_SET_NULL,FALSE

[SetupCond]
""

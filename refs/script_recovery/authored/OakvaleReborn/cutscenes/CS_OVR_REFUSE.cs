# CS_OVR_REFUSE -- beat 4b, "someone always pays". Runs from
# OVR_Theresa.finishTheresaChildhood in place of CS_OAKVALE_INTRO_THERESA + the
# raid FMV, actors HERO + Theresa, at Theresa's departure trigger. The staging
# is the retail raid scene's (hill, fence, the bandit markers below); the
# Stranger takes the bandits' place and Father is the retail NOVI_LiveFather
# thing registered as an actor. Ends faded out; the caller swaps the sections.
RegisterActor NOVI_Theresa
RegisterActor NOVI_LiveFather
CameraPause FALSE
FadeOut
GamePause 0.5
SetDoorOpen NOVI_BlockingGate,TRUE
RemoveExtras TRUE,MK_OVI_ID_GIRL
UseCamera CAM_OIF_SHOT1
Theresa.Teleport MK_OIF_THERESA
HERO.Teleport MK_OIF_HERO,FALSE
DoScriptFrame 1
DoCameraPreloading
DoScriptFrame 1
StartTimeCode
EnableSounds FALSE
PlayMusic MUSIC_SET_CUTSCENE_FEET
FadeIn
# the vision
Theresa.PlayAnimation CS_SURPRISED
Theresa.InteractiveSpeak HERO,'TEXT_OVR_REFUSE_010'
GamePause 0.4
HERO.PlayAnimation CS_TURN_REACTION
WaitActiveDialog
# the run to the fence, night falling
NoLoadUseCamera CAM_OIF_SHOT2NEW
Theresa.Teleport MK_OIF_THERESA2
HERO.Teleport MK_OIF_HERO2
SetTime 22
GamePause 1.7
UseCamera CAM_OIF_SHOT5NEW
Theresa.RunTo MK_OIF_THERESA3,0.0
GamePause 2.6
# the fire he set
CreateEffect ENFLAME_COLUMN,MK_OIF_B1A
CreateEffect ENFLAME_COLUMN,MK_OIF_BBA
CreateEffect CAMPFIRE_LIT_01,MK_OIF_B3A
PlaySound MK_OIF_B1A,SND_MAN_06_NONSPEECH_TERRORSCREAM_02
NoLoadUseCamera CAM_OIF_SHOT6
GamePause 1.5
HERO.PlayAnimation CS_FIRST_LOOK_THROUGH_FENCE,TRUE,TRUE,FALSE,FALSE
NoLoadUseCamera CAM_OIF_SHOT7NEW
GamePause 0.9
# the square: Father down, the Stranger with Theresa
FadeOut
GamePause 0.5
Create CREATURE_PROPHET_01,MK_OIF_B2B,STRANGER
NOVI_LiveFather.Teleport MK_OIF_B1B
NOVI_LiveFather.PlayAnimation STANDARD_DEAD,TRUE,TRUE
Theresa.Teleport MK_OIF_B2END
Theresa.PlayLoopingAnim ST_OPINION_FEAR_IDLE_COWERING,-1
HERO.Teleport MK_OIF_THERESA
HERO.ClearCommands
STRANGER.LookToThing HERO,FOREVER
FadeIn
UseCamera CAM_OIF_SHOT8
GamePause 1.0
HERO.PlayAnimation CS_RISING_HEAD_SCARED,TRUE,TRUE,FALSE,FALSE
STRANGER.InteractiveSpeak HERO,'TEXT_OVR_REFUSE_020'
GamePause 0.5
UseCamera CAM_OIF_SHOT9
STRANGER.PlayAnimation ST_HOLDING_ANOTHER
STRANGER.InteractiveSpeak HERO,'TEXT_OVR_REFUSE_030'
GamePause 0.6
Theresa.FadeOut 1
STRANGER.FadeOut 1
GamePause 1.2
HERO.PlayCombatAnim CS_TURNS_TO_HIDE,FALSE,TRUE,FALSE,FALSE
UseCamera CAM_OIF_SHOT10
GamePause 1.5
FadeOut
GamePause 1.0
Remove STRANGER
RemoveExtras FALSE,RETURN
HERO.Teleport MK_OIF_THERESA2

[SkipCond]
FadeOut
GamePause 1.0
Remove STRANGER
RemoveExtras FALSE,RETURN
HERO.Teleport MK_OIF_THERESA2

[SetupCond]
Create CREATURE_YOUNG_SISTER,MK_OIF_THERESA,Theresa,true
RegisterActor Theresa

-- GuildMasterGameFlow.lua
-- Entity script for GuildMasterGameFlow (V_GuildMaster.c)
-- Functional parity port of NScript::CV_GuildMasterScript::CGuildMasterGameFlow

local MY_SCRIPT_NAME = "GuildMasterGameFlow"

local TITLE_CHICKEN_CHASER = 19
local EGP_WAITING_FOR_WIZARD_BATTLE = 1450

local STATE_IDLE = 1
local STATE_FOCAL_SITES_INTRO = 2
local STATE_FOCAL_SITES_ACTIVE = 3

function Init(quest, me)
    quest:Log("GuildMasterGameFlow: Init() started.")

    -- Entity properties matching CV_GuildMasterScript::CGuildMasterGameFlow::Init
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetAsKillable(me, false, true)
    quest:EntitySetAsDamageable(me, false)
    quest:SetIsPushableByHero(me, false)
    quest:SetIsThingForcePushable(me, false)
    quest:EntitySetAsUseMovementInActions(me, false)

    quest:Log("GuildMasterGameFlow: Init() finished.")
end

-- Functional parity port of NScript::CV_GuildMasterScript::GetGuildMasterSpeech
local function GetGuildMasterSpeech(quest)
    local postSavePos = quest:GetMasterGameState("PostSavePosition") or 0
    local returnString = ""

    if postSavePos <= 700 then
        if postSavePos == 700 then
            returnString = "TEXT_QST_081_BANDIT_CAMP"
        elseif postSavePos > 400 then
            if postSavePos == 450 then
                returnString = "TEXT_QST_081_ORCHARD_FARM"
            elseif postSavePos == 500 then
                returnString = "TEXT_QST_081_WAITING_FOR_TRADER_ESCORT"
            elseif postSavePos == 550 then
                returnString = "TEXT_QST_081_TRADER_ESCORT"
            elseif postSavePos == 600 then
                if quest:IsQuestCompleted("QS_GuardianSisterInfo2_SisterInBanditCamp") then
                    returnString = "TEXT_QST_081_BANDIT_CAMP"
                else
                    returnString = "TEXT_QST_081_SECOND_MAZE_MEETING"
                end
            end
        else
            if postSavePos == 400 then
                returnString = "TEXT_QST_081_WAITING_FOR_ORCHARD_FARM"
            elseif postSavePos == 100 then
                returnString = "TEXT_QST_081_TRAINING"
            elseif postSavePos == 150 then
                returnString = "TEXT_QST_081_WAITING_FOR_WASP_BOSS"
            elseif postSavePos == 200 then
                returnString = "TEXT_QST_081_WASP_BOSS"
            elseif postSavePos == 300 then
                returnString = "TEXT_QST_081_MAZE_MEETING"
            end
        end
    elseif postSavePos > 1216 then
        if postSavePos > 1600 then
            if postSavePos > 2400 then
                if postSavePos == 2500 then
                    local heroSoulsPos = quest:GetMasterGameState("CurrentHeroSoulsPosition") or 0
                    if heroSoulsPos ~= 0 then
                        if heroSoulsPos == 1 then
                            returnString = "TEXT_QST_081_HERO_SOULS_1ST_SOUL_TALK_TO_THUNDER"
                        elseif heroSoulsPos == 2 then
                            returnString = "TEXT_QST_081_HERO_SOULS_1ST_SOUL_VISIT_THE_ARENA"
                        elseif heroSoulsPos == 3 then
                            returnString = "TEXT_QST_081_HERO_SOULS_2ND_SOUL_TALK_TO_BRIAR_KILLED_THUNDER"
                        elseif heroSoulsPos == 4 then
                            returnString = "TEXT_QST_081_HERO_SOULS_2ND_SOUL_TALK_TO_BRIAR_GOT_ARENA_SOUL"
                        elseif heroSoulsPos == 5 then
                            returnString = "TEXT_QST_081_HERO_SOULS_2ND_SOUL_VISIT_YOUR_MOTHER"
                        elseif heroSoulsPos == 6 then
                            returnString = "TEXT_QST_081_HERO_SOULS_3RD_SOUL_TALK_TO_BRIAR"
                        elseif heroSoulsPos == 7 then
                            returnString = "TEXT_QST_081_HERO_SOULS_3RD_SOUL_TALK_TO_SCYTHE"
                        end
                    else
                        returnString = "TEXT_QST_081_HERO_SOULS"
                    end
                elseif postSavePos == 2600 then
                    returnString = "TEXT_QST_081_DRAGON_FIGHT_10"
                end
            else
                if postSavePos == 1700 then
                    returnString = "TEXT_QST_081_JACK_BOSS_FIGHT"
                elseif postSavePos == 2100 then
                    returnString = "TEXT_QST_081_SHIP_SUMMONING"
                elseif postSavePos == 2300 or postSavePos == 2400 then
                    returnString = "TEXT_QST_081_THE_ORACLE"
                end
            end
        else
            if postSavePos == 1600 then
                returnString = "TEXT_QST_081_FOCAL_SITES"
            elseif postSavePos > 1450 then
                if postSavePos == 1500 then
                    returnString = "TEXT_QST_081_WIZARD_BATTLE"
                elseif postSavePos == 1550 then
                    returnString = "TEXT_QST_081_WAITING_FOR_FOCAL_SITES"
                end
            else
                if postSavePos == 1450 then
                    returnString = "TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE"
                elseif postSavePos == 1233 then
                    returnString = "TEXT_QST_081_FINALGRAVEYARD"
                elseif postSavePos == 1250 then
                    returnString = "TEXT_QST_081_PRISON"
                elseif postSavePos == 1300 then
                    if quest:GetMasterGameState("SeenAbbeyMotherAtGuild") then
                        returnString = "TEXT_QST_081_HOOK_COAST"
                    else
                        returnString = "TEXT_QST_081_BEFORE_HOOK_COAST"
                    end
                end
            end
        end
    elseif postSavePos == 1216 then
        returnString = "TEXT_QST_081_WAITING_FOR_FINALGRAVEYARD"
    elseif postSavePos <= 870 then
        if postSavePos == 870 then
            returnString = "TEXT_QST_081_WHITE_BALVERINE_WITCHWOOD"
        else
            if postSavePos == 733 then
                returnString = "TEXT_QST_081_WAITING_FOR_BANDIT_CAMP_TWINBLADE"
            elseif postSavePos == 766 then
                returnString = "TEXT_QST_081_BANDIT_CAMP_TWINBLADE"
            elseif postSavePos == 800 then
                if quest:IsQuestActive("V_TrophyDealer") then
                    returnString = "TEXT_QST_081_TROPHY_DEALER"
                else
                    returnString = "TEXT_QST_081_MAZE_TELEPORT_TO_WW"
                end
            elseif postSavePos == 850 then
                returnString = "TEXT_QST_081_TROPHY_DEALER"
            elseif postSavePos == 856 then
                returnString = "TEXT_QST_081_WITCHWOOD_POST_TROPHY_DEALER"
            elseif postSavePos == 865 then
                returnString = "TEXT_QST_081_WHITE_BALVERINE_KHG"
            end
        end
    elseif postSavePos > 1050 then
        if postSavePos == 1100 then
            returnString = "TEXT_QST_081_MINION_CLIFFTOP_CHASE"
        elseif postSavePos == 1200 then
            returnString = "TEXT_QST_081_GRAVEYARD"
        end
    elseif postSavePos == 1050 then
        returnString = "TEXT_QST_081_WAITING_FOR_MCC"
    else
        if postSavePos == 875 then
            returnString = "TEXT_QST_081_WITCHWOOD_WAITING_FOR_ARENA"
        elseif postSavePos == 900 then
            returnString = "TEXT_QST_081_ARENA"
        elseif postSavePos == 1000 then
            returnString = "TEXT_QST_081_MEET_SISTER"
        end
    end

    -- Repeat dialogue handling: 25% chance to deliver cyclic gameplay advice
    local lastDialogue = quest:GetStateString("LastDialogueSaid") or ""
    if returnString ~= "" and returnString == lastDialogue then
        if math.random(0, 3) == 0 then
            if not quest:GetStateBool("GMDial_2") then
                quest:SetStateBool("GMDial_2", true)
                returnString = "TEXT_QST_081_INFO_TELEPORTERS"
            elseif not quest:GetStateBool("GMDial_3") then
                quest:SetStateBool("GMDial_3", true)
                returnString = "TEXT_QST_081_INFO_WEAPONS"
            elseif not quest:GetStateBool("GMDial_4") then
                quest:SetStateBool("GMDial_4", true)
                returnString = "TEXT_QST_081_INFO_LOG_BOOK"
            elseif quest:GetHeroTitle() == TITLE_CHICKEN_CHASER and not quest:GetStateBool("GMDial_1") then
                quest:SetStateBool("GMDial_1", true)
                returnString = "TEXT_QST_081_INFO_HERO_TITLE"
            end
        end
    end

    quest:SetStateString("LastDialogueSaid", returnString)
    return returnString
end

-- Manual setting up and removal routine for speaking in a cinematic movie sequence
local function DoSpeak(quest, me, hero, speechKey)
    if not speechKey or speechKey == "" then
        return
    end

    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)

    if quest:GetHealth(me) > 0.0 then
        me:Speak(hero, speechKey, 0, false, true, false)
    end

    quest:PauseAllNonScriptedEntities(false)
    quest:EndMovieSequence()
end

-- Interactive Yes/No prompt sequence for Wizard Battle quest card offer
local function HandleWizardBattleTalk(quest, me, hero)
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)

    if quest:GetHealth(me) > 0.0 then
        me:Speak(hero, "TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE", 0, false, true, false)
    end

    local answer = quest:GiveHeroYesNoQuestion(
        "TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE_QUESTION_TAKE_CARD",
        "TEXT_OBJECT_HERO_ANSWER_YES",
        "TEXT_OBJECT_HERO_ANSWER_NO",
        ""
    )

    if answer == 1 then
        if quest:GetHealth(me) > 0.0 then
            me:Speak(hero, "TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE_YES", 0, false, true, false)
        end
        quest:GiveQuestCardDirectly("OBJECT_QUEST_CARD_WIZARD_BATTLE", "Q_WizardBattle", false)
        quest:TakeObjectFromHero("OBJECT_MAZE_JOURNAL")
    else
        if quest:GetHealth(me) > 0.0 then
            me:Speak(hero, "TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE_NO", 0, false, true, false)
        end
    end

    quest:PauseAllNonScriptedEntities(false)
    quest:EndMovieSequence()
end

function Main(quest, me)
    quest:Log("GuildMasterGameFlow: Main() started.")

    -- Cache Hero outside loop to prevent memory leaks
    local hero = quest:GetHero()
    if not hero then
        quest:Log("!!! ERROR: GuildMasterGameFlow Main - Could not get Hero. Aborting.")
        return
    end

    -- Acquire script control and take exclusive control over ambient AI
    me:AcquireControl()
    me:TakeExclusiveControl()

    local state = STATE_IDLE
    if quest:IsQuestActive("Q_EndGameFocalSites") then
        if quest:GetStateBool("FocalSitesSetupDone") then
            state = STATE_FOCAL_SITES_ACTIVE
        else
            state = STATE_FOCAL_SITES_INTRO
        end
    end

    local teleporterResidue = nil
    local teleporterEffect = nil
    local focalSitesHSP = nil

    while true do
        -- -----------------------------------------------------------
        -- STATE 1: STATE_IDLE (Ambient Guildmaster)
        -- -----------------------------------------------------------
        if state == STATE_IDLE then
            if quest:IsQuestActive("Q_EndGameFocalSites") then
                state = STATE_FOCAL_SITES_INTRO
            else
                -- Hit check: reacted to damage or offensive abilities, excluding Heal Life
                local wasHit = me:MsgIsHitByHero()
                    or (me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHealLifeFromHero())

                if wasHit then
                    DoSpeak(quest, me, hero, "TEXT_QST_081_ATTACKED")
                elseif me:IsTalkedToByHero() then
                    local postSavePos = quest:GetMasterGameState("PostSavePosition") or 0
                    if postSavePos == EGP_WAITING_FOR_WIZARD_BATTLE then
                        HandleWizardBattleTalk(quest, me, hero)
                    else
                        local speech = GetGuildMasterSpeech(quest)
                        DoSpeak(quest, me, hero, speech)
                    end
                end
            end

        -- -----------------------------------------------------------
        -- STATE 2: STATE_FOCAL_SITES_INTRO (Intro cutscene & setup)
        -- -----------------------------------------------------------
        elseif state == STATE_FOCAL_SITES_INTRO then
            quest:Log("GuildMasterGameFlow: Initiating Focal Sites intro sequence...")

            -- Release scripted control handle before cutscene so cutscene system can acquire ME & HERO
            me:ReleaseControl()
            quest:StartCutscene({ HERO = hero, GUILDDUDE = me }, nil, true)
            quest:RunCutscene("CS_FOCALSITEINTRO", true, false)
            quest:EndCutscene()

            -- Re-acquire control after cutscene
            me:AcquireControl()
            me:TakeExclusiveControl()

            quest:SetTeleportingAsActive(false)
            quest:GiveQuestCardDirectly("OBJECT_QUEST_CARD_ACTIVATE_FOCAL_SITES", "Q_EndGameFocalSites", false)
            quest:SetQuestCardObjective("Q_EndGameFocalSites", "TEXT_QUEST_ACTIVATE_FOCAL_SITES_OBJECTIVE_01", "", "")

            teleporterResidue = quest:GetThingWithScriptName("TeleporterResidue")
            if teleporterResidue and teleporterResidue:IsAlive() then
                teleporterEffect = quest:CreateEffectOnThing("NEWTELEPORTER2", teleporterResidue, "")
                quest:MiniMapAddMarker(teleporterResidue, "HUD_ORB_QUEST_CORE")
            end

            focalSitesHSP = quest:GetThingWithScriptName("FocalSitesHSP")
            quest:SetStateBool("FocalSitesSetupDone", true)

            state = STATE_FOCAL_SITES_ACTIVE
            quest:Log("GuildMasterGameFlow: Focal Sites setup completed. Entering active portal state.")

        -- -----------------------------------------------------------
        -- STATE 3: STATE_FOCAL_SITES_ACTIVE (Portal & guidance loop)
        -- -----------------------------------------------------------
        elseif state == STATE_FOCAL_SITES_ACTIVE then
            if not quest:IsQuestActive("Q_EndGameFocalSites") then
                quest:Log("GuildMasterGameFlow: Q_EndGameFocalSites no longer active. Cleaning up portal...")

                if teleporterEffect then
                    quest:RemoveThing(teleporterEffect)
                    teleporterEffect = nil
                end

                if not teleporterResidue or not teleporterResidue:IsAlive() then
                    teleporterResidue = quest:GetThingWithScriptName("TeleporterResidue")
                end
                if teleporterResidue and teleporterResidue:IsAlive() then
                    quest:MiniMapRemoveMarker(teleporterResidue)
                end

                quest:SetStateBool("FocalSitesSetupDone", false)
                state = STATE_IDLE
            else
                -- Resolve entity references if needed
                if not teleporterResidue or not teleporterResidue:IsAlive() then
                    teleporterResidue = quest:GetThingWithScriptName("TeleporterResidue")
                    if teleporterResidue and teleporterResidue:IsAlive() and not teleporterEffect then
                        teleporterEffect = quest:CreateEffectOnThing("NEWTELEPORTER2", teleporterResidue, "")
                        quest:MiniMapAddMarker(teleporterResidue, "HUD_ORB_QUEST_CORE")
                    end
                end

                if not focalSitesHSP or not focalSitesHSP:IsAlive() then
                    focalSitesHSP = quest:GetThingWithScriptName("FocalSitesHSP")
                end

                -- Proximity portal check: teleport hero to FocalSitesHSP when standing on teleporter residue
                if teleporterResidue and teleporterResidue:IsAlive() and focalSitesHSP and focalSitesHSP:IsAlive() then
                    if quest:IsDistanceBetweenThingsUnder(hero, teleporterResidue, 3.0) then
                        quest:EntityTeleportToThing(hero, focalSitesHSP)
                    end
                end

                -- Talk check
                if me:IsTalkedToByHero() then
                    local speech = GetGuildMasterSpeech(quest)
                    DoSpeak(quest, me, hero, speech)
                else
                    -- Hit check
                    local wasHit = me:MsgIsHitByHero()
                        or (me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHealLifeFromHero())

                    if wasHit then
                        DoSpeak(quest, me, hero, "TEXT_QST_081_ATTACKED")
                    end
                end
            end
        end

        if not quest:NewScriptFrame(me) then
            me:ReleaseControl()
            return
        end
    end
end

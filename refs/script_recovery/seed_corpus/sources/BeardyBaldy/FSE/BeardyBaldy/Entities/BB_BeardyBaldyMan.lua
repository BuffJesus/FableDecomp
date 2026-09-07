-- BB_BeardyBaldyMan.lua
-- Entity script for BB_BeardyBaldyMan in Bowerstone South (BowerstoneSlumsWarehouses)
-- Functional parity port of NScript::CV_BeardyBaldyScript::CBB_BeardyBaldyMan

local MY_SCRIPT_NAME = "BB_BeardyBaldyMan"
local REGION_NAME = "BowerstoneSlumsWarehouses"

-- Constants from main.def
local DEF = {
    WANDER_MIN                  = 0.0,
    WANDER_MAX                  = 50.0,
    HAIRCUT_STYLE_1             = "OBJECT_HERO_HAIR_BASIN_01",
    HAIRCUT_CARD_1              = "OBJECT_HAIRCUT_CARD_MOP",
    HAIRCUT_STYLE_2             = "OBJECT_HERO_HAIR_MOHAWK_01",
    HAIRCUT_CARD_2              = "OBJECT_HAIRCUT_CARD_07",
    BEARD_STYLE_1               = "OBJECT_HERO_BEARD_MUTTON_01",
    BEARD_CARD_1                = "OBJECT_BEARD_CARD_04",
    BEARD_STYLE_2               = "OBJECT_HERO_BEARD_LONG_01",
    BEARD_CARD_2                = "OBJECT_BEARD_CARD_05",
    TASH_STYLE_1                = "OBJECT_HERO_TASHTRADER_01",
    TASH_CARD_1                 = "OBJECT_TASH_CARD_02",
    TASH_STYLE_2                = "OBJECT_HERO_TASHSHERIFF_01",
    TASH_CARD_2                 = "OBJECT_TASH_CARD_04",
    REWARD_FULL                 = "OBJECT_TATTOO_CARD_BS_BACK_01",
    REWARD_HALF                 = "OBJECT_BEARD_CARD_08",
    EXPERIENCE                  = 100,
    RANDOM_SPEECH_TIMER_MAX     = 30,
    RANDOM_SPEECH_TIMER_VAR     = 10,
}

-- Text group selection methods
local GROUP_SELECT_NONE              = 0
local GROUP_SELECT_RANDOM            = 1
local GROUP_SELECT_RANDOM_NO_REPEAT  = 2
local GROUP_SELECT_SEQUENTIAL        = 3
local GROUP_SELECT_FIRST_ONLY        = 4

-- Movement types
local ENTITY_MOVE_WALK = 0
local ENTITY_MOVE_RUN  = 1

-- Odd hairdos list matching IsHeroWearingAnyOddHairdo
local ODD_HAIRDOS = {
    "OBJECT_HERO_HAIR_BUZZ_01",
    "OBJECT_HERO_HAIR_BASIN_01",
    "OBJECT_HERO_HAIR_MOHAWK_01",
    "OBJECT_HERO_HAIR_PONYTAIL_01",
    "OBJECT_HERO_HAIR_PLATS_01",
}

-- Tash styles list matching IsHeroWearingAnyTash
local TASH_STYLES = {
    "OBJECT_HERO_TASHSMITH_01",
    "OBJECT_HERO_TASHTRADER_01",
    "OBJECT_HERO_TASHKHG_01",
    "OBJECT_HERO_TASHSHERIFF_01",
    "OBJECT_HERO_TASHCHINESE_01",
    "OBJECT_HERO_TASHSMALL_01",
}

-- Beard styles list matching CV_BordelloScript::IsHeroWearingBeard
local BEARD_STYLES = {
    "OBJECT_HERO_BEARD_01",
    "OBJECT_HERO_BEARD_02",
    "OBJECT_HERO_BEARD_03",
    "OBJECT_HERO_BEARD_MUTTON_01",
    "OBJECT_HERO_BEARD_LONG_01",
    "OBJECT_HERO_BEARD_CHIN_01",
    "OBJECT_HERO_BEARD_TRAMP_01",
    "OBJECT_HERO_BEARD_WATSON_01",
}

-- 10 Random ambient speech keys
local RANDOM_SPEECH = {
    "TEXT_QST_014_RANDOM_00",
    "TEXT_QST_014_RANDOM_10",
    "TEXT_QST_014_RANDOM_20",
    "TEXT_QST_014_RANDOM_30",
    "TEXT_QST_014_RANDOM_40",
    "TEXT_QST_014_RANDOM_50",
    "TEXT_QST_014_RANDOM_60",
    "TEXT_QST_014_RANDOM_70",
    "TEXT_QST_014_RANDOM_80",
    "TEXT_QST_014_RANDOM_90",
}

local lastRandomSpeechIdx1 = 10
local lastRandomSpeechIdx2 = 10

-- Helper: Check if hero is wearing any odd hairdo
local function IsHeroWearingAnyOddHairdo(quest, hero)
    if not hero then return false end
    for _, style in ipairs(ODD_HAIRDOS) do
        if quest:IsWearingHairstyle(hero, style) then
            return true
        end
    end
    return false
end

-- Helper: Check if hero is wearing any moustache
local function IsHeroWearingAnyTash(quest, hero)
    if not hero then return false end
    for _, style in ipairs(TASH_STYLES) do
        if quest:IsWearingHairstyle(hero, style) then
            return true
        end
    end
    return false
end

-- Helper: Check if hero is wearing any beard (matching CV_BordelloScript::IsHeroWearingBeard)
local function IsHeroWearingBeard(quest, hero)
    if not hero then return false end
    for _, style in ipairs(BEARD_STYLES) do
        if quest:IsWearingHairstyle(hero, style) then
            return true
        end
    end
    return false
end

-- Helper: Get non-repeating random speech line (matching CBB_BeardyBaldyMan::GetRandomSpeech)
local function GetRandomSpeech(quest)
    local idx = math.random(0, 9)
    while idx == lastRandomSpeechIdx1 or idx == lastRandomSpeechIdx2 do
        idx = math.random(0, 9)
    end
    lastRandomSpeechIdx2 = lastRandomSpeechIdx1
    lastRandomSpeechIdx1 = idx
    return RANDOM_SPEECH[idx + 1]
end

-- Helper: Reset random speech timer (matching ResetRandomSpeechTime)
local function ResetRandomSpeechTime(quest, timerId)
    local delay = DEF.RANDOM_SPEECH_TIMER_MAX - math.random(0, DEF.RANDOM_SPEECH_TIMER_VAR - 1)
    quest:SetTimer(timerId, delay)
end

function Init(quest, me)
    quest:Log("BB_BeardyBaldyMan: Init() started.")
    quest:SetThingHasInformation(me, true, true, false)
    quest:SetThingPersistent(me, true)
    lastRandomSpeechIdx1 = 10
    lastRandomSpeechIdx2 = 10
    me:MakeBehavioral()
    quest:Log("BB_BeardyBaldyMan: Init() completed.")
end

function Main(quest, me)
    quest:Log("BB_BeardyBaldyMan: Main() started.")

    local hero = quest:GetHero()
    if not hero then
        quest:Log("!!! ERROR: BB_BeardyBaldyMan Main - could not get Hero. Aborting.")
        return
    end
    quest:Log("BB_BeardyBaldyMan: Hero retrieved successfully.")

    local speechTimer = quest:RegisterTimer()
    quest:Log("BB_BeardyBaldyMan: speechTimer registered: " .. tostring(speechTimer))

    ResetRandomSpeechTime(quest, speechTimer)
    quest:Log("BB_BeardyBaldyMan: ResetRandomSpeechTime completed.")

    while true do
        -- 1. Check if attacked by Hero (matching WatchForAttack logic)
        local wasHit = me:MsgIsHitByHero()
            or (me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHealLifeFromHero())

        if wasHit then
            quest:SetStateBool("AttackedByHero", true)
        end

        if quest:GetStateBool("AttackedByHero") then
            quest:Log("BB_BeardyBaldyMan: Handling AttackedByHero...")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            me:AcquireControl()
            if quest:GetHealth(me) > 0.0 then
                me:Speak(hero, "TEXT_QST_014_ATTACK", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false)
            end
            me:ReleaseControl()
            quest:SetStateBool("AttackedByHero", false)
            ResetRandomSpeechTime(quest, speechTimer)
            quest:PauseAllNonScriptedEntities(false)
            quest:EndMovieSequence()

        -- 2. Ambient random speech timer check
        elseif quest:GetTimer(speechTimer) <= 0 then
            quest:Log("BB_BeardyBaldyMan: Handling ambient random speech...")
            local text = GetRandomSpeech(quest)
            quest:EntitySetFacingAngleTowardsThing(me, hero)
            local convoID = quest:StartAmbientConversation(me, hero, false, false)
            if convoID and convoID ~= -1 then
                quest:AddLineToConversation(convoID, text, me, hero, false)
            end
            ResetRandomSpeechTime(quest, speechTimer)
        end

        -- 3. Phase state machine
        local questPhase = quest:GetStateInt("QuestPhase") or 0

        -- Initialize wandering for current phase
        if quest:GetStateBool("InitialisePhase") then
            local pos = me:GetPos()
            quest:SetWanderCentrePoint(me, pos)
            quest:SetWanderMinDistance(me, DEF.WANDER_MIN)
            quest:SetWanderMaxDistance(me, DEF.WANDER_MAX)
            quest:SetScriptingStateGroup(me, 4) -- ESSG_WANDER_NEAR
            quest:SetStateBool("InitialisePhase", false)
        end

        -------------------------------------------------------------
        -- PHASE 0: Hairdo Challenge
        -------------------------------------------------------------
        if questPhase == 0 then
            if me:IsTalkedToByHero() then
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                me:AcquireControl()

                if quest:GetStateBool("Phase1RequirementsComplete") then
                    if quest:GetHealth(me) > 0.0 then
                        me:Speak(hero, "TEXT_QST_014_PHASE1_REMINDER", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false)
                    end
                else
                    if quest:GetStateBool("IntroComplete") then
                        if quest:GetHealth(me) > 0.0 then
                            me:Speak(hero, "TEXT_QST_014_PHASE1_INTRO_REMINDER", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false)
                        end
                    else
                        if quest:GetHealth(me) > 0.0 then
                            me:Speak(hero, "TEXT_QST_014_PHASE1_INTRO1", GROUP_SELECT_NONE, false, true, false)
                        end
                        quest:SetStateBool("IntroComplete", true)
                    end

                    local ans = quest:GiveHeroYesNoQuestion("TEXT_QST_014_QUEST_ON_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "")
                    ResetRandomSpeechTime(quest, speechTimer)

                    if ans == 1 then
                        -- Player answered YES
                        if quest:GetHealth(me) > 0.0 then
                            me:Speak(hero, "TEXT_QST_014_PHASE1_INTRO2", GROUP_SELECT_NONE, false, true, false)
                        end

                        if not quest:IsWearingHairstyle(hero, DEF.HAIRCUT_STYLE_1) then
                            quest:GiveHeroObject(DEF.HAIRCUT_CARD_1, -1, false)
                            quest:SetStateString("RequiredHairdo", DEF.HAIRCUT_STYLE_1)
                        else
                            quest:GiveHeroObject(DEF.HAIRCUT_CARD_2, -1, false)
                            quest:SetStateString("RequiredHairdo", DEF.HAIRCUT_STYLE_2)
                        end

                        if quest:GetHealth(me) > 0.0 then
                            me:Speak(hero, "TEXT_QST_014_PHASE1_INTRO3", GROUP_SELECT_NONE, false, true, false)
                        end
                        quest:SetStateBool("Phase1RequirementsComplete", true)
                    else
                        -- Player answered NO
                        if quest:GetHealth(me) > 0.0 then
                            me:Speak(hero, "TEXT_QST_014_PHASE1_DENIED", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false)
                        end
                    end
                end

                me:ReleaseControl()
                quest:PauseAllNonScriptedEntities(false)
                quest:EndMovieSequence()

                if quest:GetStateBool("Phase1RequirementsComplete") then
                    quest:SetStateInt("QuestPhase", 1)
                    quest:SetStateBool("InitialisePhase", true)
                end
            end

        -------------------------------------------------------------
        -- PHASE 1: Beard Challenge
        -------------------------------------------------------------
        elseif questPhase == 1 then
            if me:IsTalkedToByHero() then
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                me:AcquireControl()

                if quest:GetStateBool("Phase2RequirementsComplete") then
                    if quest:GetHealth(me) > 0.0 then
                        me:Speak(hero, "TEXT_QST_014_PHASE2_REMINDER", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false)
                    end
                else
                    local reqHair = quest:GetStateString("RequiredHairdo")
                    if quest:IsWearingHairstyle(hero, reqHair) then
                        if quest:GetHealth(me) > 0.0 then
                            me:Speak(hero, "TEXT_QST_014_PHASE2_INTRO1", GROUP_SELECT_NONE, false, true, false)
                        end

                        if not quest:IsWearingHairstyle(hero, DEF.BEARD_STYLE_1) then
                            quest:GiveHeroObject(DEF.BEARD_CARD_1, -1, false)
                            quest:SetStateString("RequiredBeard", DEF.BEARD_STYLE_1)
                        else
                            quest:GiveHeroObject(DEF.BEARD_CARD_2, -1, false)
                            quest:SetStateString("RequiredBeard", DEF.BEARD_STYLE_2)
                        end

                        if quest:GetHealth(me) > 0.0 then
                            me:Speak(hero, "TEXT_QST_014_PHASE2_INTRO2", GROUP_SELECT_NONE, false, true, false)
                        end
                        quest:SetStateBool("Phase2RequirementsComplete", true)
                    else
                        if quest:GetHealth(me) > 0.0 then
                            me:Speak(hero, "TEXT_QST_014_PHASE2_WRONG_HAIR", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false)
                        end
                    end
                end

                me:ReleaseControl()
                quest:PauseAllNonScriptedEntities(false)
                quest:EndMovieSequence()

                if quest:GetStateBool("Phase2RequirementsComplete") then
                    quest:SetStateInt("QuestPhase", 2)
                    quest:SetStateBool("InitialisePhase", true)
                end
            end

        -------------------------------------------------------------
        -- PHASE 2: Moustache Challenge
        -------------------------------------------------------------
        elseif questPhase == 2 then
            if me:IsTalkedToByHero() then
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                me:AcquireControl()

                if quest:GetStateBool("Phase3RequirementsComplete") then
                    if quest:GetHealth(me) > 0.0 then
                        me:Speak(hero, "TEXT_QST_014_PHASE3_REMINDER", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false)
                    end
                else
                    local reqHair = quest:GetStateString("RequiredHairdo")
                    local reqBeard = quest:GetStateString("RequiredBeard")
                    local hasBeard = IsHeroWearingBeard(quest, hero)

                    if not hasBeard then
                        if quest:GetHealth(me) > 0.0 then
                            me:Speak(hero, "TEXT_QST_014_PHASE3_NO_BEARD", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false)
                        end
                    else
                        local wearingHair = quest:IsWearingHairstyle(hero, reqHair)
                        local wearingBeard = quest:IsWearingHairstyle(hero, reqBeard)

                        if wearingHair and wearingBeard then
                            if quest:GetHealth(me) > 0.0 then
                                me:Speak(hero, "TEXT_QST_014_PHASE3_INTRO1", GROUP_SELECT_NONE, false, true, false)
                            end

                            if IsHeroWearingAnyTash(quest, hero) then
                                if quest:GetHealth(me) > 0.0 then
                                    me:Speak(hero, "TEXT_QST_014_PHASE3_INTRO2", GROUP_SELECT_NONE, false, true, false)
                                end
                            else
                                if quest:GetHealth(me) > 0.0 then
                                    me:Speak(hero, "TEXT_QST_014_PHASE3_INTRO3", GROUP_SELECT_NONE, false, true, false)
                                end
                            end

                            if not quest:IsWearingHairstyle(hero, DEF.TASH_STYLE_1) then
                                quest:GiveHeroObject(DEF.TASH_CARD_1, -1, false)
                                quest:SetStateString("RequiredTash", DEF.TASH_STYLE_1)
                            else
                                quest:GiveHeroObject(DEF.TASH_CARD_2, -1, false)
                                quest:SetStateString("RequiredTash", DEF.TASH_STYLE_2)
                            end

                            if quest:GetHealth(me) > 0.0 then
                                me:Speak(hero, "TEXT_QST_014_PHASE3_INTRO4", GROUP_SELECT_NONE, false, true, false)
                            end
                            quest:SetStateBool("Phase3RequirementsComplete", true)
                        elseif wearingHair and not wearingBeard then
                            if quest:GetHealth(me) > 0.0 then
                                me:Speak(hero, "TEXT_QST_014_PHASE3_WRONG_BEARD", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false)
                            end
                        elseif not wearingHair and wearingBeard then
                            if quest:GetHealth(me) > 0.0 then
                                me:Speak(hero, "TEXT_QST_014_PHASE3_WRONG_HAIR", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false)
                            end
                        else
                            if quest:GetHealth(me) > 0.0 then
                                me:Speak(hero, "TEXT_QST_014_PHASE3_WRONG_HAIR_AND_BEARD", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false)
                            end
                        end
                    end
                end

                me:ReleaseControl()
                quest:PauseAllNonScriptedEntities(false)
                quest:EndMovieSequence()

                if quest:GetStateBool("Phase3RequirementsComplete") then
                    quest:SetStateInt("QuestPhase", 3)
                    quest:SetStateBool("InitialisePhase", true)
                end
            end

        -------------------------------------------------------------
        -- PHASE 3: Finale Evaluation & Exit
        -------------------------------------------------------------
        elseif questPhase == 3 then
            if me:IsTalkedToByHero() then
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                me:AcquireControl()

                local allHairChanged = quest:GetStateBool("AllHairChanged")
                if allHairChanged then
                    local hasBeard = IsHeroWearingBeard(quest, hero)
                    local hasTash = IsHeroWearingAnyTash(quest, hero)
                    local hasOddHair = IsHeroWearingAnyOddHairdo(quest, hero)

                    if hasBeard and hasTash and hasOddHair then
                        if quest:GetHealth(me) > 0.0 then
                            me:Speak(hero, "TEXT_QST_014_END_FINALE1", GROUP_SELECT_NONE, false, true, false)
                        end
                        quest:GiveHeroObject(DEF.REWARD_FULL, -1, false)
                        quest:SetStateBool("QuestComplete", true)
                    elseif (quest:GetStateInt("IncorrectHairComboCount") > 3) and allHairChanged then
                        if quest:GetHealth(me) > 0.0 then
                            me:Speak(hero, "TEXT_QST_014_END_FINALE2", GROUP_SELECT_NONE, false, true, false)
                        end
                        quest:GiveHeroObject(DEF.REWARD_HALF, -1, false)
                        quest:SetStateBool("QuestComplete", true)
                    else
                        local count = (quest:GetStateInt("IncorrectHairComboCount") or 0) + 1
                        quest:SetStateInt("IncorrectHairComboCount", count)
                        if quest:GetHealth(me) > 0.0 then
                            me:Speak(hero, "TEXT_QST_014_END_WRONG", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false)
                        end
                    end
                else
                    if quest:GetHealth(me) > 0.0 then
                        me:Speak(hero, "TEXT_QST_014_END_NO_TASH", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false)
                    end
                end

                me:ReleaseControl()
                quest:PauseAllNonScriptedEntities(false)
                quest:EndMovieSequence()
            end

            -- Check if quest completed to execute exit sequence
            if quest:GetStateBool("QuestComplete") then
                quest:GiveHeroExperience(DEF.EXPERIENCE)
                quest:ClearThingHasInformation(me)

                local exitThing = quest:GetNearestWithScriptName(me, "M_BB_ExitPoint")
                if exitThing and exitThing:IsAlive() then
                    me:AcquireControl()
                    me:MoveToPosition_NonBlocking(exitThing:GetPos(), 1.0, ENTITY_MOVE_RUN)

                    while not quest:IsDistanceBetweenThingsUnder(me, exitThing, 2.0) and not quest:MsgOnRegionLoaded() do
                        if not quest:NewScriptFrame(me) then
                            me:ReleaseControl()
                            quest:DeregisterTimer(speechTimer)
                            return
                        end
                    end

                    me:ReleaseControl()
                    quest:FadeOutAndKillEntity(me, false, 1.0, true)
                    quest:SetStateBool("BeardyBaldyLeft", true)
                    quest:DeregisterTimer(speechTimer)
                    return
                else
                    quest:FadeOutAndKillEntity(me, false, 1.0, true)
                    quest:SetStateBool("BeardyBaldyLeft", true)
                    quest:DeregisterTimer(speechTimer)
                    return
                end
            end
        end

        if not quest:NewScriptFrame(me) then
            quest:DeregisterTimer(speechTimer)
            return
        end
    end
end

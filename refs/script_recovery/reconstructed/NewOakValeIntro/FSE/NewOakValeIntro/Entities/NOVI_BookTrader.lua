-- NOVI_BookTrader — retail NScript::CQ_NewOakValeIntroScript::CNOVI_BookTrader
-- Init 0x00DB3F00, Main 0x00DB3FA0
-- Evidence level: reconstructed-source (Ghidra decompile of retail Fable.exe + PDB names)
--
-- The Oakvale trader who sells the box of chocolates for 3 gold (TEXT_QST_048_TRADER_*). He walks
-- back to his home position, faces NOVI_Theresa, occasionally shouts "roll up" at a nearby hero,
-- runs the buy-sweets yes/no dialogue, and scolds the hero (bad deed) if hit.

local NOVI = require("NewOakValeIntro.common")
local F = require("NewOakValeIntro.fields")
local Deeds = require("NewOakValeIntro.deeds")

-- Constants
local HERO_SCRIPT_NAME     = "SCRIPT_NAME_HERO"
local THERESA_SCRIPT_NAME  = "NOVI_Theresa"
local SWEETS_OBJECT        = "OBJECT_CHOCOLATE_BOX_UNGIVEABLE"
local SWEETS_PRICE         = 3        -- GetHeroGold() < 3 check; GiveHeroGold(-3)
local HOME_LEAVE_DISTANCE  = 0.1      -- immediate 0.1: "am I away from home?"
local HOME_ARRIVE_DISTANCE = 2.0      -- immediate 2.0: walk home until within this
local HOME_MOVE_RADIUS     = 0.0      -- MoveToPosition second arg (immediate 0)
local HOME_MOVE_TYPE       = 0        -- EScriptEntityMoveType immediate 0
local SHOUT_HERO_DISTANCE  = 20.0     -- immediate 20.0
local SHOUT_CHANCE_MODULO  = 200      -- rand() % 200 == 0
local SPEAK_MIN_HEALTH     = 0.0      -- _DAT_0122dedc (retail .rdata = 0.0)
local SPEECH_METHOD        = 0
local BAD_DEED_HIT_ME      = 2        -- AddBadDeed(PARENT, 2)
local IDLE_PRIORITY        = 3        -- StartScriptingEntity(me, res, 3) each loop iteration
local ACTION_PRIORITY      = 4        -- StartScriptingEntity(me, res, 4) for the hit reaction / shout
local DEFAULT_PRIORITY     = nil      -- dropped by the decompiler
local SHOUT_ANIM           = "ST_OPINION_NEUTRAL_SHOUTING_WITH_HANDS_CUPPED"
local SHOUT_TIMER_VALUE    = 3     -- native push 3 at 0x00db4d5d

-- Text keys
local TEXT_ON_HIT          = "TEXT_QST_048_TRADER_ON_HIT"
local TEXT_INTRO           = "TEXT_QST_048_TRADER_INTRO"
local TEXT_INTRO_10        = "TEXT_QST_048_TRADER_INTRO_10"
local TEXT_STILL_GOT       = "TEXT_QST_048_TRADER_STILL_GOT"
local TEXT_BUY_SWEETS_Q    = "TEXT_QST_048_TRADER_BUY_SWEETS"
local TEXT_NOT_ENOUGH_CASH = "TEXT_QST_048_TRADER_NOT_ENOUGH_CASH"
local TEXT_GIVES_SWEETS    = "TEXT_QST_048_TRADER_GIVES_SWEETS"
local TEXT_BUY_LATER       = "TEXT_QST_048_TRADER_BUY_LATER"
local TEXT_ROLL_UP         = "TEXT_QST_048_TRADER_ROLL_UP"
local TEXT_OBJECTIVE_04    = "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_04"
local TEXT_ANSWER_YES      = "TEXT_OBJECT_HERO_ANSWER_YES"
local TEXT_ANSWER_NO       = "TEXT_OBJECT_HERO_ANSWER_NO"

-- Retail entity field (this+0x1c)
local DoneIntro = false

function Init(quest, me)
    DoneIntro = false
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsKillable(me, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:SetThingHasInformation(me, false)    -- retail (me,0,1,0)
    quest:SetIsPushableByHero(me, false)
    quest:EntitySetAsUseMovementInActions(me, false)
    quest:EntitySetDeedReactionsEnabled(me, false)
end

local function speak_if_alive(quest, me, key)
    if quest:GetHealth(me) > SPEAK_MIN_HEALTH then
        me:SpeakAndWait(key, SPEECH_METHOD)
    end
end

local function begin_cutscene(quest)
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
end

local function end_cutscene(quest)
    quest:PauseAllNonScriptedEntities(false)
    quest:EndMovieSequence()   -- inference: retail movie object destructor
end

local function face_theresa(quest, me)
    quest:EntitySetFacingAngleTowardsThing(me, quest:GetThingWithScriptName(THERESA_SCRIPT_NAME))
end

local function hero_hit_me(quest, me)
    if me:MsgIsHitByHero() then return true end
    if me:MsgIsHitByAnySpecialAbilityFromHero() then
        if not me:MsgIsHitByHeroSpecialAbility(14) then return true end
    end
    return false
end

-- Phase: walk back to the home position, then face Theresa
local function return_home(quest, me)
    local home = me:GetHomePos()
    local away = NOVI.distance_from_thing_to_position_over(me, home, HOME_LEAVE_DISTANCE)
    if not away then return true end
    while true do
        local far = NOVI.distance_from_thing_to_position_over(me, home, HOME_ARRIVE_DISTANCE)
        if not far then break end
        if not NOVI.frame(quest, me) then return false end
        me:MoveToPosition(home, HOME_MOVE_RADIUS, HOME_MOVE_TYPE)
        while me:IsPerformingScriptTask() do
            if not NOVI.frame(quest, me) then return false end
        end
    end
    face_theresa(quest, me)
    return true
end

-- Phase: hero hit him
local function react_to_hit(quest, me)
    local hero = quest:GetHero()
    quest:EntitySetThingAsAllyOfThing(me, hero)    -- twice in retail, args dropped
    quest:EntitySetThingAsAllyOfThing(hero, me)
    Deeds.add_bad(quest, me, BAD_DEED_HIT_ME)
    if not NOVI.acquire(quest, me, ACTION_PRIORITY) then return false end
    begin_cutscene(quest)
    speak_if_alive(quest, me, TEXT_ON_HIT)
    face_theresa(quest, me)
    end_cutscene(quest)
    return true
end

-- Phase: the sweets sale dialogue
local function sell_sweets(quest, me)
    if not NOVI.acquire(quest, me, DEFAULT_PRIORITY) then return false end
    begin_cutscene(quest)   -- slot 0x5ec(1)/(0) here; inference = PauseAllNonScriptedEntities
    if F.get(quest, F.GivenSweets) then
        speak_if_alive(quest, me, TEXT_INTRO_10)
        face_theresa(quest, me)
        end_cutscene(quest)
        return true
    end
    if not DoneIntro then
        speak_if_alive(quest, me, TEXT_INTRO)
        DoneIntro = true
    else
        speak_if_alive(quest, me, TEXT_STILL_GOT)
    end
    quest:GiveHeroYesNoQuestion(TEXT_BUY_SWEETS_Q, TEXT_ANSWER_YES, TEXT_ANSWER_NO, "")
    local answer = quest:MsgIsQuestionAnsweredYesOrNo()
    while answer < 0 do
        if not NOVI.frame(quest, me) then end_cutscene(quest); return false end
        answer = quest:MsgIsQuestionAnsweredYesOrNo()
    end
    if answer == 1 then
        if quest:GetHeroGold() < SWEETS_PRICE then
            speak_if_alive(quest, me, TEXT_NOT_ENOUGH_CASH)
        else
            speak_if_alive(quest, me, TEXT_GIVES_SWEETS)
            quest:GiveHeroObject(SWEETS_OBJECT, -1)   -- retail (name, -1, 0)
            quest:GiveHeroGold(-SWEETS_PRICE)
            quest:SetQuestCardObjective(quest:GetActiveQuestName(), TEXT_OBJECTIVE_04, "", "")
            F.set(quest, F.GivenSweets, true)
            quest:ClearThingHasInformation(me)
        end
    else
        speak_if_alive(quest, me, TEXT_BUY_LATER)
    end
    face_theresa(quest, me)
    end_cutscene(quest)
    return true
end

-- Phase: random "roll up!" shout at a nearby hero
local function maybe_shout(quest, me)
    local shout_timer = F.get(quest, F.TalkIntermittentTimer)
    if quest:GetTimer(shout_timer) ~= 0 then return true end
    if math.random(0, SHOUT_CHANCE_MODULO - 1) ~= 0 then return true end   -- rand() % 200 == 0
    if not NOVI.hero_within(quest, me, SHOUT_HERO_DISTANCE) then return true end
    quest:SetTimer(shout_timer, SHOUT_TIMER_VALUE)
    local conv = quest:AddNewConversation(me)   -- retail (me, 0, 0)
    quest:AddPersonToConversation(conv, quest:GetHero())
    if not NOVI.acquire(quest, me, ACTION_PRIORITY) then return false end
    me:PlayAnimation(SHOUT_ANIM)   -- retail bools (0,0,0,1,DAT_01375748,false) unbound
    quest:AddLineToConversation(conv, TEXT_ROLL_UP, me, quest:GetHero())
    return true
end

function Main(quest, me)
    if not NOVI.frame(quest, me) then return end
    while true do
        if not NOVI.acquire(quest, me, IDLE_PRIORITY) then NOVI.release(quest, me); return end
        if not return_home(quest, me) then NOVI.release(quest, me); return end

        if hero_hit_me(quest, me) then
            if not react_to_hit(quest, me) then NOVI.release(quest, me); return end
        elseif me:IsTalkedToByHero() then
            if not sell_sweets(quest, me) then NOVI.release(quest, me); return end
        end

        if not maybe_shout(quest, me) then NOVI.release(quest, me); return end

        if not NOVI.frame(quest, me) then NOVI.release(quest, me); return end
    end
end

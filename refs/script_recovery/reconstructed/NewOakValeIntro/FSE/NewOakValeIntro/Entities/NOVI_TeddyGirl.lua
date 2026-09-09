-- NOVI_TeddyGirl — retail NScript::CQ_NewOakValeIntroScript::CNOVI_TeddyGirl
-- Init 0x00DAF000, Main 0x00DAF080, GivenTeddy 0x00DB0600 (private helper)
-- Evidence level: reconstructed-source (Ghidra decompile of retail Fable.exe + PDB names)
--
-- The little girl in Oakvale who lost her teddy bear. She begs the hero for it while he is
-- carrying OBJECT_TEDDY_BEAR_UNGIVEABLE (yes/no prompt or hero presents the item), thanks him and
-- records TeddySolution "B" when she gets it, complains and walks off to NOVI_AffairWife when the
-- bully ruined it (TeddySolution "C"), and scolds the hero (bad deed) when he hits her.

local NOVI = require("NewOakValeIntro.common")
local F = require("NewOakValeIntro.fields")
local Deeds = require("NewOakValeIntro.deeds")

-- Constants (values read from the decompile / retail .rdata)
local TEDDY_OBJECT          = "OBJECT_TEDDY_BEAR_UNGIVEABLE"
local BULLY_SCRIPT_NAME     = "NOVI_Bully"
local WIFE_SCRIPT_NAME      = "NOVI_AffairWife"
local HERO_SCRIPT_NAME      = "SCRIPT_NAME_HERO"
local BULLY_NEAR_DISTANCE   = 10.0   -- immediate 10.0 @ 0x00DAF080 (ruined-teddy trigger)
local HERO_FAR_DISTANCE     = 20.0   -- immediate 20.0 (stop waiting for the walk-off)
local WIFE_ARRIVE_RADIUS    = 3.0    -- 0x40400000 in the MoveToThing call
local WIFE_MOVE_TYPE        = 1      -- EScriptEntityMoveType immediate 1
local SPEAK_MIN_HEALTH      = 0.0    -- _DAT_0122dedc (retail .rdata = 0.0): speak only while alive
local SPEECH_METHOD         = 0      -- ETextGroupSelectionMethod immediate 0 on every Speak
local BAD_DEED_HIT_ME       = 2      -- AddBadDeed(PARENT, 2)
local EXCLUDED_HIT_ABILITY  = 14     -- sibling NOVI hit predicates pass 0x0e
local WALK_OFF_PRIORITY     = 4      -- StartScriptingEntity(me, res, 4) before the walk-off
local DEFAULT_PRIORITY      = nil    -- priority argument dropped by the decompiler elsewhere

-- Text keys
local TEXT_LOST_TEDDY       = "TEXT_QST_048_TEDDYGIRL_LOST_TEDDY"
local TEXT_PLEA_POST_BEATEN = "TEXT_QST_048_TEDDYGIRL_PLEA_POST_BEATEN"
local TEXT_FOUND_TEDDY      = "TEXT_QST_048_TEDDYGIRL_FOUND_TEDDY"
local TEXT_REPEAT_PLEA      = "TEXT_QST_048_TEDDYGIRL_REPEAT_PLEA"
local TEXT_REPEAT_FOUND     = "TEXT_QST_048_TEDDYGIRL_REPEAT_FOUND"
local TEXT_DONT_WANT        = "TEXT_QST_048_TEDDYGIRL_DONT_WANT"
local TEXT_BAD_FEELING      = "TEXT_QST_048_TEDDYGIRL_BAD_FEELING"
local TEXT_TEDDY_RUINED     = "TEXT_QST_048_TEDDYGIRL_TEDDY_RUINED"
local TEXT_DONT_HIT         = "TEXT_QST_048_TEDDYGIRL_DONT_HIT"
local TEXT_GIVE_TEDDY_Q     = "TEXT_QST_048_GIVE_TEDDY_TO_GIRL"
local TEXT_ANSWER_YES       = "TEXT_OBJECT_HERO_ANSWER_YES"
local TEXT_ANSWER_NO        = "TEXT_OBJECT_HERO_ANSWER_NO"

-- Retail entity fields (this+0x1c..0x1f), initialised in Init
local DoneIntro             = false  -- 0x1c
local FoundTeddy            = false  -- 0x1d
local SpokeAboutFindingTeddy = false -- 0x1e (never read or written by Main; retail leaves it false)
local HeroHitMe             = false  -- 0x1f

-- Reference to the bully, resolved once at the top of Main (retail keeps it for the whole loop)
local bully = nil

function Init(quest, me)
    DoneIntro = false
    FoundTeddy = false
    SpokeAboutFindingTeddy = false
    HeroHitMe = false
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsKillable(me, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:SetThingHasInformation(me, false)   -- retail passes (me,0,1,0); extra args unbound
end

-- Retail: Speak(hero, key, method, ...) + IsPerformingScriptTask wait loop, guarded by GetHealth(me) > 0.
local function speak_if_alive(quest, me, key)
    if quest:GetHealth(me) > SPEAK_MIN_HEALTH then
        me:SpeakAndWait(key, SPEECH_METHOD)   -- host-blocking: retail's per-frame terminate check is inside
    end
end

-- Retail: StartMovieSequence("") + PauseAllNonScriptedEntities(true); the movie object is released by
-- its destructor at scope end (modelled as EndMovieSequence, inference).
local function begin_cutscene(quest)
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
end

local function end_cutscene(quest)
    quest:PauseAllNonScriptedEntities(false)
    quest:EndMovieSequence()
end

-- CNOVI_TeddyGirl::GivenTeddy @ 0x00DB0600
local function given_teddy(quest, me)
    quest:TakeObjectFromHero(TEDDY_OBJECT)
    FoundTeddy = true
    Deeds.add_good(quest, me)
    quest:ClearThingHasInformation(me)
    F.set_master(quest, F.master.TeddySolution, "B")
end

local function hero_hit_me(quest, me)
    -- MsgIsHitBy(hero) || (MsgIsHitByAnySpecialAbilityFrom(hero) && !MsgIsHitBySpecialAbilityFrom(<ability>, hero))
    if me:MsgIsHitByHero() then return true end
    if me:MsgIsHitByAnySpecialAbilityFromHero() then
        if not me:MsgIsHitByHeroSpecialAbility(EXCLUDED_HIT_ABILITY) then return true end
    end
    return false
end

-- Phase: hero talks to her while carrying the teddy -> plea + yes/no offer
local function offer_teddy(quest, me)
    NOVI.acquire(quest, me, DEFAULT_PRIORITY)
    begin_cutscene(quest)
    if not DoneIntro then
        if not HeroHitMe then
            speak_if_alive(quest, me, TEXT_LOST_TEDDY)
        else
            speak_if_alive(quest, me, TEXT_PLEA_POST_BEATEN)
        end
        DoneIntro = true
    end
    quest:GiveHeroYesNoQuestion(TEXT_GIVE_TEDDY_Q, TEXT_ANSWER_YES, TEXT_ANSWER_NO, "")
    local answer = quest:MsgIsQuestionAnsweredYesOrNo()
    while answer < 0 do
        if not NOVI.frame(quest, me) then end_cutscene(quest); return false end
        answer = quest:MsgIsQuestionAnsweredYesOrNo()
    end
    if answer == 1 then
        speak_if_alive(quest, me, TEXT_FOUND_TEDDY)
        given_teddy(quest, me)
    else
        speak_if_alive(quest, me, TEXT_REPEAT_PLEA)
    end
    end_cutscene(quest)
    return true
end

-- Phase: hero presents an item to her
local function receive_item(quest, me, item)
    if item == TEDDY_OBJECT then
        NOVI.acquire(quest, me, DEFAULT_PRIORITY)
        DoneIntro = true
        begin_cutscene(quest)
        speak_if_alive(quest, me, TEXT_FOUND_TEDDY)
        given_teddy(quest, me)
        end_cutscene(quest)
    elseif not FoundTeddy then
        -- retail re-polls MsgIsPresentedWithItem and compares the item again (operand dropped; inference: != teddy)
        NOVI.acquire(quest, me, DEFAULT_PRIORITY)
        begin_cutscene(quest)
        speak_if_alive(quest, me, TEXT_DONT_WANT)
        end_cutscene(quest)
    end
end

-- Phase: the bully ruined the teddy -> complain and walk off to the affair wife, then despawn
local function teddy_ruined(quest, me)
    local conv = quest:AddNewConversation(me)   -- retail args dropped
    quest:AddPersonToConversation(conv, quest:GetHero())
    quest:AddLineToConversation(conv, TEXT_TEDDY_RUINED, me, quest:GetHero())
    F.set_master(quest, F.master.TeddySolution, "C")
    NOVI.acquire(quest, me, WALK_OFF_PRIORITY)
    local wife = quest:GetThingWithScriptName(WIFE_SCRIPT_NAME)
    me:MoveToThing(wife, WIFE_ARRIVE_RADIUS, WIFE_MOVE_TYPE)   -- retail also passes (false,false,false,true)
    while quest:IsCameraPosOnScreen(me:GetPos()) do
        local far = NOVI.things_over(quest, me, quest:GetHero(), HERO_FAR_DISTANCE)
        if far then break end
        if not NOVI.frame(quest, me) then return false end
    end
    quest:RemoveThing(me)   -- retail arg dropped; inference: me
    return true
end

-- Phase: hero talks to her without the teddy
local function chat(quest, me)
    NOVI.acquire(quest, me, DEFAULT_PRIORITY)
    begin_cutscene(quest)
    if not FoundTeddy and HeroHitMe then
        speak_if_alive(quest, me, TEXT_PLEA_POST_BEATEN)
        DoneIntro = true
    elseif DoneIntro then
        if not FoundTeddy then
            if F.get(quest, F.TeddyRuined) then
                speak_if_alive(quest, me, TEXT_BAD_FEELING)
                quest:ClearThingHasInformation(me)   -- retail arg dropped; inference: me
            else
                speak_if_alive(quest, me, TEXT_REPEAT_PLEA)
            end
        else
            speak_if_alive(quest, me, TEXT_REPEAT_FOUND)
        end
    else
        speak_if_alive(quest, me, TEXT_LOST_TEDDY)
        DoneIntro = true
    end
    end_cutscene(quest)
end

-- Phase: hero hit her
local function scold_hero(quest, me)
    local hero = quest:GetHero()
    quest:EntitySetThingAsAllyOfThing(me, hero)   -- retail calls it twice, args dropped (inference: both directions)
    quest:EntitySetThingAsAllyOfThing(hero, me)
    HeroHitMe = true
    Deeds.add_bad(quest, me, BAD_DEED_HIT_ME)
    NOVI.acquire(quest, me, DEFAULT_PRIORITY)
    begin_cutscene(quest)
    speak_if_alive(quest, me, TEXT_DONT_HIT)
    end_cutscene(quest)
end

function Main(quest, me)
    if not NOVI.frame(quest, me) then return end
    bully = quest:GetThingWithScriptName(BULLY_SCRIPT_NAME)
    while true do
        local talked = me:IsTalkedToByHero()
        local has_teddy = talked and quest:IsObjectInThingsPossession(quest:GetHero(), TEDDY_OBJECT)
        if has_teddy then
            if not offer_teddy(quest, me) then NOVI.release(quest, me); return end
        else
            local presented, item = me:MsgIsPresentedWithItem()
            if presented then receive_item(quest, me, item) end
        end

        if F.get(quest, F.SpokeAboutFindingTeddy)
           and quest:IsDistanceBetweenThingsUnder(me, bully, BULLY_NEAR_DISTANCE) then
            if not teddy_ruined(quest, me) then NOVI.release(quest, me); return end
        end

        if me:IsTalkedToByHero() then
            chat(quest, me)
        end

        if hero_hit_me(quest, me) then
            scold_hero(quest, me)
        end

        if not NOVI.frame(quest, me) then NOVI.release(quest, me); return end
    end
end

-- Readable native conversion: Gate2Guard1. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    EXPERIENCE_GRANT_MEDIUM = 64,  -- 50
}

-- per-entity fields (native class members; one Lua state per entity instance)
local aiState

-- Gate2Guard1.Main (retail 0x00d0d910)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult2, predicateResult4, predicateResult, taskRunning, scratchValue, timerId
    local scratchValue46, scratchValue47, scratchValue49, scratchValue50, resource, movie3, movie4
    local function ReleaseEverything()
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource)
    end
    scratchValue50 = 0
    if not quest:NewScriptFrame(me) then return end
    resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    if not quest:IsActiveThreadTerminating() then
        quest:EntitySetAsKillable(me, false, true)
        timerId = quest:RegisterTimer()
        quest:SetTimer(timerId, 0)
        scratchValue = 0
        scratchValue49 = 0
        predicateResult2 = quest:IsActiveThreadTerminating()
        scratchValue47 = 0
        repeat
            if predicateResult2 then
                quest:DeregisterTimer(timerId)
                resources:ReleaseResource(resource)
                return
            end
            if not quest:GetStateBool("Gate2Open") then
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                scratchValue46 = scratchValue47 | 1
                scratchValue50 = scratchValue46
                if me:MsgIsHitByHero() then
                    goto LAB_00d0daf6
                else
                    scratchValue46 = scratchValue47 | 3
                    scratchValue50 = scratchValue46
                    if me:MsgIsHitByAnySpecialAbilityFromHero() then
                        scratchValue46 = scratchValue47 | 7
                        scratchValue50 = scratchValue46
                        if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00d0daf6 end
                    end
                    predicateResult4 = false
                end
                goto FLOW_past_lab_00d0daf6
                ::LAB_00d0daf6::
                predicateResult4 = true
                ::FLOW_past_lab_00d0daf6::
                if scratchValue46 & 4 ~= 0 then
                    scratchValue46 = scratchValue46 & 0xfffffffb
                    scratchValue50 = scratchValue46
                end
                if scratchValue46 & 2 ~= 0 then
                    scratchValue46 = scratchValue46 & 0xfffffffd
                    scratchValue50 = scratchValue46
                end
                if scratchValue46 & 1 ~= 0 then
                    scratchValue50 = scratchValue46 & 0xfffffffe
                end
                if predicateResult4 then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                    movie3 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    local switch1 = scratchValue49
                    repeat
                        if switch1 == 0 then
                            if quest:GetHealth(resources:ScriptThing(resource)) <= 0.0 then
                                goto LAB_00d0dc78
                            end
                            goto FLOW_past_lab_00d0dc78
                            ::LAB_00d0dc78::
                            me:SetFriendsWithEverythingFlag(true)
                            scratchValue49 = 1
                            break
                            ::FLOW_past_lab_00d0dc78::
                            if not me:Speak(hero, "TEXT_QST_009_BANDIT2_HIT_FIRST", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d0e8c4 end
                            if not quest:IsActiveThreadTerminating() then goto LAB_00d0dc78 end
                            quest:PauseAllNonScriptedEntities(false)
                            goto LAB_00d0e8d0
                        elseif switch1 == 1 then
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                if not me:Speak(hero, "TEXT_QST_009_BANDIT2_HIT_SECOND", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d0e8c4 end
                                if quest:IsActiveThreadTerminating() then goto LAB_00d0e8c4 end
                            end
                            me:SetFriendsWithEverythingFlag(true)
                            break
                        elseif switch1 == 2 then
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                if not me:Speak(hero, "TEXT_QST_009_BANDIT2_HIT_THIRD", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d0e8c4 end
                                if quest:IsActiveThreadTerminating() then goto LAB_00d0e8c4 end
                            end
                            me:SetFriendsWithEverythingFlag(true)
                            break
                        elseif switch1 == 3 then
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                if not me:Speak(hero, "TEXT_QST_009_BANDIT2_HIT_FOURTH", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d0e8c4 end
                                if quest:IsActiveThreadTerminating() then goto LAB_00d0e8c4 end
                            end
                            me:SetFriendsWithEverythingFlag(false)
                            quest:ModifyThingHealth(me, 10000.0, false)
                            quest:EntitySetAsKillable(me, true, true)
                            quest:GiveThingBestEnemyTarget(me, hero)
                            quest:ClearThingHasInformation(me)
                        end
                    until true
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie3)
                end
                if quest:GetTimer(timerId) < 1 then
                    if quest:IsDistanceBetweenThingsUnder(hero, me, 8.0) then
                        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                        local conversationId = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(conversationId, hero)
                        if scratchValue == 0 then
                            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                            scratchValue = 1
                            quest:AddLineToConversation(conversationId, "TEXT_QST_009_BANDIT2_COMMENT_FIRST", me, hero, false)
                        else
                            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                            quest:AddLineToConversation(conversationId, "TEXT_QST_009_BANDIT2_COMMENT_SECOND", me, hero, false)
                        end
                        quest:SetTimer(timerId, 10)
                    end
                end
                if not me:IsTalkedToByHero() then quest:NewScriptFrame(me); predicateResult2 = quest:IsActiveThreadTerminating(); scratchValue47 = scratchValue50; goto continue_2 end
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                quest:SetStateBool("SpokenToSecondGuard", true)
                resources:PrepareResource(resource)
                while not resources:TryAcquire(resource, me, 4) do
                    if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                end
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                local predicateResult13 = quest:IsActiveThreadTerminating()
                if quest:IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", hero) then
                    if predicateResult13 then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                    quest:GiveHeroExperience(quest:ReadGlobalGameData(SCRIPT_DEF.EXPERIENCE_GRANT_MEDIUM))
                    local movie = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                        me:Speak(hero, "TEXT_QST_009_BANDIT2_GIVEN_CAMP_PASS", GROUP_SELECT_FIRST, false, true, false)
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00d0e942 end
                        end
                        if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00d0e942 end
                        goto FLOW_past_lab_00d0e942
                        ::LAB_00d0e942::
                        resources:DestroyMovie(movie)
                        quest:DeregisterTimer(timerId)
                        resources:ReleaseResource(resource)
                        do return end
                        ::FLOW_past_lab_00d0e942::
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    quest:SetStateBool("Gate2Open", true)
                    quest:CreateThread("OpenGate", {args = {2.0, "Gate2Outer"}})  -- native parent-quest worker OpenGate, bound values
                    if scratchValue50 & 32 ~= 0 then
                        scratchValue47 = scratchValue50 & 0xffffffdf
                    end
                    if scratchValue47 & 16 ~= 0 then
                        scratchValue47 = scratchValue47 & 0xffffffef
                    end
                    if scratchValue47 & 8 ~= 0 then
                        scratchValue50 = scratchValue47 & 0xfffffff7
                    end
                    local conversationId2 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId2, hero)
                    quest:AddLineToConversation(conversationId2, "TEXT_QST_009_BANDIT2_PASS", me, hero, false)
                    quest:ClearThingHasInformation(me)
                else
                    if predicateResult13 then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                    local movie2 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                        me:Speak(hero, "TEXT_QST_009_BANDIT2_GATE_BLOCKED", GROUP_SELECT_FIRST, false, true, false)
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00d0e909 end
                        end
                        if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00d0e909 end
                        goto FLOW_past_lab_00d0e909
                        ::LAB_00d0e909::
                        resources:DestroyMovie(movie2)
                        quest:DeregisterTimer(timerId)
                        resources:ReleaseResource(resource)
                        do return end
                        ::FLOW_past_lab_00d0e909::
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie2)
                end
                resources:PrepareResource(resource)
                while not resources:TryAcquire(resource, me, 4) do
                    if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                end
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
            else
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                if me:IsTalkedToByHero() then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                    movie4 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                        if not me:Speak(hero, "TEXT_QST_009_BANDIT2_ASIDE", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d0e996 end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d0e996 end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie4)
                end
                local scratchValue48 = scratchValue50
                scratchValue50 = scratchValue50 | 64
                if me:MsgIsHitByHero() then
                    goto LAB_00d0e82e
                else
                    scratchValue46 = scratchValue48 | 192
                    scratchValue50 = scratchValue46
                    if me:MsgIsHitByAnySpecialAbilityFromHero() then
                        scratchValue46 = scratchValue48 | 448
                        scratchValue50 = scratchValue46
                        if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00d0e82e end
                    end
                    predicateResult = false
                end
                goto FLOW_past_lab_00d0e82e
                ::LAB_00d0e82e::
                predicateResult = true
                ::FLOW_past_lab_00d0e82e::
                if scratchValue46 & 256 ~= 0 then
                    scratchValue46 = scratchValue46 & 0xfffffeff
                    scratchValue50 = scratchValue46
                end
                if scratchValue46 < 0 then
                    scratchValue46 = scratchValue46 & 0xffffff7f
                    scratchValue50 = scratchValue46
                end
                if scratchValue46 & 64 ~= 0 then
                    scratchValue50 = scratchValue46 & 0xffffffbf
                end
                if predicateResult then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                    movie4 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    if quest:GetHealth(resources:ScriptThing(resource)) <= 0.0 then goto LAB_00d0eade end
                    me:Speak(hero, "TEXT_QST_009_BANDIT2_ATTACKED_NEW", GROUP_SELECT_FIRST, false, true, false)
                    taskRunning = me:IsPerformingScriptTask()
                    goto LAB_00d0ea82
                end
            end
            quest:NewScriptFrame(me)
            predicateResult2 = quest:IsActiveThreadTerminating()
            scratchValue47 = scratchValue50
            ::continue_2::
        until false
    end
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00d0ea82::
    if taskRunning then
        if not quest:NewScriptFrame(me) then goto LAB_00d0e996 end
        taskRunning = me:IsPerformingScriptTask()
        goto LAB_00d0ea82
    end
    if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00d0eabf end
    ::LAB_00d0eade::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie4)
    me:SetFriendsWithEverythingFlag(false)
    quest:EntitySetAsKillable(me, true, true)
    quest:GiveThingBestEnemyTarget(me, hero)
    quest:ClearThingHasInformation(me)
    resources:PrepareResource(resource)
    repeat
        quest:NewScriptFrame(me)
    until quest:IsActiveThreadTerminating()
    quest:DeregisterTimer(timerId)
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00d0e8c4::
    quest:PauseAllNonScriptedEntities(false)
    ::LAB_00d0e8d0::
    resources:DestroyMovie(movie3)
    quest:DeregisterTimer(timerId)
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00d0e996::
    quest:PauseAllNonScriptedEntities(false)
    ::LAB_00d0eabf::
    resources:DestroyMovie(movie4)
    quest:DeregisterTimer(timerId)
    resources:ReleaseResource(resource)
end

-- Gate2Guard1.Init (retail 0x00d072c0)
function Init(quest, me)
    aiState = 0
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetInFaction(me, "FACTION_TWINBLADE_CAMP_BANDITS")
    quest:SetIsPushableByHero(me, false)
end

-- Gate2Guard1.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- Gate2Guard1.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end


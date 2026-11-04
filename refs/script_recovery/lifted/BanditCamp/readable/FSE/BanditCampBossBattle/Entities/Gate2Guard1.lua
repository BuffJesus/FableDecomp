-- Readable native conversion: Gate2Guard1. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    EXPERIENCE_GRANT_MEDIUM = 64,  -- 50
}

-- per-entity fields (native class members; one Lua state per entity instance)
local aiState

-- Gate2Guard1.Main (retail 0x00d0d910)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult2, predicateResult4, predicateResult, taskRunning, scratchValue3, dist
    local scratchValue, timerId, hero17, line, speechResult, scratchValue46, resource, movie3
    local movie4
    local function ReleaseEverything()
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource)
    end
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
        scratchValue3 = 0
        scratchValue46 = 0
        predicateResult2 = quest:IsActiveThreadTerminating()
        repeat
            if predicateResult2 then
                quest:DeregisterTimer(timerId)
                resources:ReleaseResource(resource)
                return
            end
            if not quest:GetStateBool("Gate2Open") then
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                if me:MsgIsHitByHero() then
                    goto LAB_00d0daf6
                else
                    if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00d0daf6 end
                    predicateResult4 = false
                end
                goto FLOW_past_lab_00d0daf6
                ::LAB_00d0daf6::
                predicateResult4 = true
                ::FLOW_past_lab_00d0daf6::
                if predicateResult4 then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                    movie3 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    local switch1 = scratchValue46
                    repeat
                        if switch1 == 0 then
                            local scratchValue48 = resources:ScriptThing(resource)
                            if quest:GetHealth(scratchValue48) <= 0.0 then
                                goto LAB_00d0dc78
                            end
                            goto FLOW_past_lab_00d0dc78
                            ::LAB_00d0dc78::
                            me:SetFriendsWithEverythingFlag(true)
                            scratchValue46 = 1
                            break
                            ::FLOW_past_lab_00d0dc78::
                            line = "TEXT_QST_009_BANDIT2_HIT_FIRST"
                            local hero = quest:GetHero()
                            if not me:Speak(hero, line, 0, false, true, false) then
                                goto LAB_00d0e8c4
                            end
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then goto LAB_00d0e8c4 end
                            end
                            if not quest:IsActiveThreadTerminating() then goto LAB_00d0dc78 end
                            quest:PauseAllNonScriptedEntities(false)
                            goto LAB_00d0e8d0
                        elseif switch1 == 1 then
                            local scratchValue52 = resources:ScriptThing(resource)
                            if 0.0 < quest:GetHealth(scratchValue52) then
                                line = "TEXT_QST_009_BANDIT2_HIT_SECOND"
                                local hero2 = quest:GetHero()
                                if not me:Speak(hero2, line, 0, false, true, false) then
                                    goto LAB_00d0e8c4
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00d0e8c4 end
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00d0e8c4 end
                            end
                            me:SetFriendsWithEverythingFlag(true)
                            break
                        elseif switch1 == 2 then
                            local scratchValue47 = resources:ScriptThing(resource)
                            if 0.0 < quest:GetHealth(scratchValue47) then
                                line = "TEXT_QST_009_BANDIT2_HIT_THIRD"
                                local hero3 = quest:GetHero()
                                if not me:Speak(hero3, line, 0, false, true, false) then
                                    goto LAB_00d0e8c4
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00d0e8c4 end
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00d0e8c4 end
                            end
                            me:SetFriendsWithEverythingFlag(true)
                            break
                        elseif switch1 == 3 then
                            local scratchValue49 = resources:ScriptThing(resource)
                            if 0.0 < quest:GetHealth(scratchValue49) then
                                line = "TEXT_QST_009_BANDIT2_HIT_FOURTH"
                                local hero4 = quest:GetHero()
                                if not me:Speak(hero4, line, 0, false, true, false) then
                                    goto LAB_00d0e8c4
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00d0e8c4 end
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00d0e8c4 end
                            end
                            me:SetFriendsWithEverythingFlag(false)
                            quest:ModifyThingHealth(me, 10000.0, false)
                            quest:EntitySetAsKillable(me, true, true)
                            local hero5 = quest:GetHero()
                            quest:GiveThingBestEnemyTarget(me, hero5)
                            quest:ClearThingHasInformation(me)
                        end
                    until true
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie3)
                end
                if quest:GetTimer(timerId) < 1 then
                    dist = 8.0
                    local hero6 = quest:GetHero()
                    if quest:IsDistanceBetweenThingsUnder(hero6, me, 8.0) then
                        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                        local conversationId = quest:AddNewConversation(me, false, false)
                        local hero7 = quest:GetHero()
                        quest:AddPersonToConversation(conversationId, hero7)
                        if scratchValue3 == 0 then
                            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                            scratchValue3 = 1
                            local hero8 = quest:GetHero()
                            quest:AddLineToConversation(conversationId, "TEXT_QST_009_BANDIT2_COMMENT_FIRST", me, hero8, false)
                        else
                            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                            local hero9 = quest:GetHero()
                            quest:AddLineToConversation(conversationId, "TEXT_QST_009_BANDIT2_COMMENT_SECOND", me, hero9, false)
                        end
                        quest:SetTimer(timerId, 10)
                    end
                end
                if not me:IsTalkedToByHero() then quest:NewScriptFrame(me); predicateResult2 = quest:IsActiveThreadTerminating(); scratchValue = timerId; goto continue_2 end
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                quest:SetStateBool("SpokenToSecondGuard", true)
                resources:PrepareResource(resource)
                while not resources:TryAcquire(resource, me, 4) do
                    if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                end
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                local hero10 = quest:GetHero()
                local isObjectInThingsPossession = quest:IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", hero10)
                local predicateResult13 = quest:IsActiveThreadTerminating()
                if isObjectInThingsPossession then
                    if predicateResult13 then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                    quest:GiveHeroExperience(quest:ReadGlobalGameData(SCRIPT_DEF.EXPERIENCE_GRANT_MEDIUM))
                    local movie = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    local scratchValue50 = resources:ScriptThing(resource)
                    if 0.0 < quest:GetHealth(scratchValue50) then
                        line = "TEXT_QST_009_BANDIT2_GIVEN_CAMP_PASS"
                        local hero11 = quest:GetHero()
                        if not me:Speak(hero11, line, 0, false, true, false) then
                            quest:PauseAllNonScriptedEntities(false)
                            goto LAB_00d0e942
                        end
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
                    local conversationId2 = quest:AddNewConversation(me, false, false)
                    local hero12 = quest:GetHero()
                    quest:AddPersonToConversation(conversationId2, hero12)
                    local hero13 = quest:GetHero()
                    quest:AddLineToConversation(conversationId2, "TEXT_QST_009_BANDIT2_PASS", me, hero13, false)
                    quest:ClearThingHasInformation(me)
                else
                    if predicateResult13 then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                    local movie2 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    local scratchValue51 = resources:ScriptThing(resource)
                    if 0.0 < quest:GetHealth(scratchValue51) then
                        line = "TEXT_QST_009_BANDIT2_GATE_BLOCKED"
                        local hero14 = quest:GetHero()
                        if not me:Speak(hero14, line, 0, false, true, false) then
                            quest:PauseAllNonScriptedEntities(false)
                            goto LAB_00d0e909
                        end
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
                    local x_stk_54_1 = resources:ScriptThing(resource)
                    if 0.0 < quest:GetHealth(x_stk_54_1) then
                        line = "TEXT_QST_009_BANDIT2_ASIDE"
                        local hero15 = quest:GetHero()
                        if not me:Speak(hero15, line, 0, false, true, false) then
                            goto LAB_00d0e996
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00d0e996 end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d0e996 end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie4)
                end
                if me:MsgIsHitByHero() then
                    goto LAB_00d0e82e
                else
                    if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00d0e82e end
                    predicateResult = false
                end
                goto FLOW_past_lab_00d0e82e
                ::LAB_00d0e82e::
                predicateResult = true
                ::FLOW_past_lab_00d0e82e::
                if predicateResult then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                    movie4 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    local x_stk_54_2 = resources:ScriptThing(resource)
                    if quest:GetHealth(x_stk_54_2) <= 0.0 then goto LAB_00d0eade end
                    line = "TEXT_QST_009_BANDIT2_ATTACKED_NEW"
                    local hero16 = quest:GetHero()
                    speechResult = me:Speak(hero16, line, 0, false, true, false)
                    taskRunning = me:IsPerformingScriptTask()
                    goto LAB_00d0ea82
                end
            end
            quest:NewScriptFrame(me)
            predicateResult2 = quest:IsActiveThreadTerminating()
            scratchValue = timerId
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
    hero17 = quest:GetHero()
    quest:GiveThingBestEnemyTarget(me, hero17)
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


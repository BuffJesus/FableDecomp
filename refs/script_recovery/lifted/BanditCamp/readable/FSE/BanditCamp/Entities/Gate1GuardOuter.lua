-- Readable native conversion: Gate1GuardOuter. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    EXPERIENCE_GRANT_MEDIUM = 64,  -- 50
}

-- per-entity fields (native class members; one Lua state per entity instance)
local aiState

-- Gate1GuardOuter.Main (retail 0x00d01630)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult2, timerId, scratchValue4, gate1GuardInner, movie2, resource
    quest:NewScriptFrame(me)
    if not quest:IsActiveThreadTerminating() then
        resource = resources:NewResource()
        resources:PrepareResource(resource)
        while not resources:TryAcquire(resource, me, 4) do
            if not quest:NewScriptFrame(me) then goto LAB_00d02a56 end
        end
        if not quest:IsActiveThreadTerminating() then
            gate1GuardInner = quest:GetThingWithScriptName("Gate1GuardInner")
            timerId = quest:RegisterTimer()
            quest:SetTimer(timerId, 0)
            if not quest:IsActiveThreadTerminating() then goto LAB_00d01776 end
            goto FLOW_hoist_lab_00d01776_1
        end
        goto FLOW_hoist_lab_00d01776_2
    end
    goto FLOW_past_lab_00d01776
    ::LAB_00d01776::
    if not quest:GetStateBool("Gate1Open") then
        if not quest:IsActiveThreadTerminating() then
            if quest:GetTimer(timerId) < 1 then
                if quest:IsActiveThreadTerminating() then goto LAB_00d02851 end
                quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                local conversationId = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId, hero)
                if true then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d02851 end
                    -- TODO(native): pCVar11 = *(this + 4)
--[[unresolved native value]]
                    -- TODO(native): pCVar10 = (**(*pCVar11 + 0x118))(pCVar11)
--[[unresolved native value]]
                    quest:AddLineToConversation(conversationId, "TEXT_QST_009_BANDIT1_CALL_OVER_FIRST", me, gate1GuardInner, false)
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00d02851 end
                    -- TODO(native): pCVar11 = *(this + 4)
--[[unresolved native value]]
                    -- TODO(native): pCVar10 = (**(*pCVar11 + 0x118))(pCVar11)
--[[unresolved native value]]
                    quest:AddLineToConversation(conversationId, "TEXT_QST_009_BANDIT1_CALL_OVER_SECOND", me, nil --[[missing]], false)
                end
                quest:SetTimer(timerId, 10)
            end
            if not me:IsTalkedToByHero() then goto LAB_00d0274f end
            if not quest:IsActiveThreadTerminating() then
                movie2 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                if quest:IsWearingClothingItem(hero, "OBJECT_HERO_SHIRT_BANDITCAMP") then
                    goto LAB_00d01abe
                else
                    if quest:IsWearingClothingItem(hero, "OBJECT_HERO_TROUSERS_BANDITCAMP") then goto LAB_00d01abe end
                    if quest:IsWearingClothingItem(hero, "OBJECT_HERO_HAT_BANDITCAMP") then goto LAB_00d01abe end
                    if quest:IsWearingClothingItem(hero, "OBJECT_HERO_BOOTS_BANDITCAMP") then goto LAB_00d01abe end
                    predicateResult2 = true
                    if quest:IsWearingClothingItem(hero, "OBJECT_HERO_GLOVES_BANDITCAMP") then goto LAB_00d01abe end
                end
                goto FLOW_past_lab_00d01abe
                ::LAB_00d01abe::
                predicateResult2 = false
                ::FLOW_past_lab_00d01abe::
                if predicateResult2 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d0287c end
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                        if not me:Speak(hero, "TEXT_QST_009_BANDIT1_NO_DISGUISE", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d0287c end
                        goto LAB_00d01be9
                    end
                    goto FLOW_hoist_lab_00d01be9_1
                end
                goto FLOW_past_lab_00d01be9
                ::LAB_00d01be9::
                if quest:IsActiveThreadTerminating() then goto LAB_00d0287c end
                ::FLOW_hoist_lab_00d01be9_1::
                ::LAB_00d01bf8::
                ::LAB_00d02592::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
                goto LAB_00d0274f
                ::FLOW_past_lab_00d01be9::
                if quest:IsWearingClothingItem(hero, "OBJECT_HERO_SHIRT_BANDITCAMP") then
                    if not quest:IsWearingClothingItem(hero, "OBJECT_HERO_TROUSERS_BANDITCAMP") then
                        if not quest:IsActiveThreadTerminating() then
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                if not me:Speak(hero, "TEXT_QST_009_BANDIT1_NO_TROUSERS", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d0287c end
                                goto LAB_00d01be9
                            end
                            goto LAB_00d01bf8
                        end
                        goto LAB_00d0287c
                    end
                    if not quest:IsWearingClothingItem(hero, "OBJECT_HERO_HAT_BANDITCAMP") then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d0287c end
                        if quest:GetHealth(resources:ScriptThing(resource)) <= 0.0 then goto LAB_00d01bf8 end
                        if not me:Speak(hero, "TEXT_QST_009_BANDIT1_NO_HAT", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d0287c end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d0287c end
                        goto LAB_00d02592
                    end
                    if not quest:IsWearingClothingItem(hero, "OBJECT_HERO_BOOTS_BANDITCAMP") then
                        if not quest:IsActiveThreadTerminating() then
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                if not me:Speak(hero, "TEXT_QST_009_BANDIT1_NO_BOOTS", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d0287c end
                                goto LAB_00d01be9
                            end
                            goto LAB_00d01bf8
                        end
                        goto LAB_00d0287c
                    end
                    local predicateResult = quest:IsActiveThreadTerminating()
                    if not quest:IsWearingClothingItem(hero, "OBJECT_HERO_GLOVES_BANDITCAMP") then
                        if not predicateResult then
                            if quest:GetHealth(resources:ScriptThing(resource)) <= 0.0 then goto LAB_00d01bf8 end
                            if not me:Speak(hero, "TEXT_QST_009_BANDIT1_NO_GLOVES", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d0287c end
                            if not quest:IsActiveThreadTerminating() then goto LAB_00d02592 end
                        end
                        goto LAB_00d0287c
                    end
                    if not predicateResult then
                        if 0 == 1 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d0287c end
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                if not me:Speak(hero, "TEXT_QST_009_BANDIT1_TRIED_AND_IN", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d0287c end
                                if quest:IsActiveThreadTerminating() then goto LAB_00d0287c end
                            end
                            goto LAB_00d022ba
                        end
                        goto FLOW_past_lab_00d022ba
                        ::LAB_00d022ba::
                        if not quest:IsSleepingTime(quest:GetNearestWithDefName(hero, "VILLAGE_BANDIT_CAMP_MAIN")) then
                            goto LAB_00d023d3
                        end
                        goto FLOW_past_lab_00d023d3
                        ::LAB_00d023d3::
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                            if not me:Speak(hero, "TEXT_QST_009_BANDIT1_GATE_OPEN", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d02870 end
                            if quest:IsActiveThreadTerminating() then goto LAB_00d028a8 end
                        end
                        quest:OpenDoor(quest:GetThingWithScriptName("Gate1"))
                        quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_BANDIT_CAMP_OBJECTIVE_05", "BanditCampBoss", "BanditCampEntrance")
                        quest:SetStateBool("Gate1Open", true)
                        quest:GiveHeroExperience(quest:ReadGlobalGameData(SCRIPT_DEF.EXPERIENCE_GRANT_MEDIUM))
                        quest:ClearThingHasInformation(me)
                        goto LAB_00d02592
                        ::FLOW_past_lab_00d023d3::
                        if not quest:IsActiveThreadTerminating() then
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                if not me:Speak(hero, "TEXT_QST_009_BANDIT1_BANDITS_SLEEPING", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d02870 end
                                if quest:IsActiveThreadTerminating() then goto LAB_00d028a8 end
                            end
                            goto LAB_00d023d3
                        end
                        ::LAB_00d028a8::
                        goto LAB_00d0287c
                        ::FLOW_past_lab_00d022ba::
                        if not quest:IsActiveThreadTerminating() then
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                if not me:Speak(hero, "TEXT_QST_009_BANDIT1_STRAIGHT_IN", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d0287c end
                                if quest:IsActiveThreadTerminating() then goto LAB_00d0287c end
                            end
                            goto LAB_00d022ba
                        end
                    end
                    goto LAB_00d0287c
                end
                if not quest:IsActiveThreadTerminating() then
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                        if not me:Speak(hero, "TEXT_QST_009_BANDIT1_NO_SHIRT", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d0287c end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d0287c end
                    end
                    goto LAB_00d02592
                end
                quest:PauseAllNonScriptedEntities(false)
                goto LAB_00d02889
            end
            goto LAB_00d02851
        end
        quest:DeregisterTimer(timerId)
        goto LAB_00d02a4d
    end
    if not quest:IsActiveThreadTerminating() then
        if scratchValue4:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d02851 end
            local movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                me:Speak(hero, "TEXT_QST_009_BANDIT1_ASIDE", GROUP_SELECT_FIRST, false, true, false)
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        goto LAB_00d02851
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    goto LAB_00d02851
                end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
        end
        goto LAB_00d0274f
    end
    goto FLOW_past_lab_00d0274f
    ::LAB_00d0274f::
    if me:MsgIsHitByHero() then goto LAB_00d027e0 end
    if me:MsgIsHitByAnySpecialAbilityFromHero() then
        if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00d027e0 end
    end
    predicateResult2 = false
    goto FLOW_past_lab_00d027e0
    ::LAB_00d027e0::
    predicateResult2 = true
    ::FLOW_past_lab_00d027e0::
    if not predicateResult2 then
        -- TODO(native): if (*(char *)(*(int *)(this + 0x14) + 0x48) == '\0') goto code_r0x00d0283a;
        if not quest:IsActiveThreadTerminating() then
            quest:GiveThingBestEnemyTarget(me, hero)
            resources:PrepareResource(resource)
            repeat
                quest:NewScriptFrame(me)
            until quest:IsActiveThreadTerminating()
        end
        goto LAB_00d02851
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d02851 end
    if 0.0 < quest:GetHealth(me) then
        local conversationId2 = quest:AddNewConversation(me, false, false)
        quest:AddPersonToConversation(conversationId2, hero)
        quest:AddLineToConversation(conversationId2, "TEXT_QST_009_BANDIT1_ATTACKED_NEW", me, hero, false)
    end
    quest:SetStateBool("AttackedOuterGateGuards", true)
    quest:GiveThingBestEnemyTarget(me, hero)
    quest:ClearThingHasInformation(me)
    resources:PrepareResource(resource)
    repeat
        quest:NewScriptFrame(me)
    until quest:IsActiveThreadTerminating()
    quest:DeregisterTimer(timerId)
    goto LAB_00d02a4d
    ::FLOW_past_lab_00d0274f::
    ::FLOW_hoist_lab_00d01776_1::
    ::LAB_00d02851::
    quest:DeregisterTimer(timerId)
    ::LAB_00d02a4d::
    ::FLOW_hoist_lab_00d01776_2::
    ::LAB_00d02a56::
    resources:ReleaseResource(resource)
    ::FLOW_past_lab_00d01776::
    do return end
    ::LAB_00d02870::
    ::LAB_00d0287c::
    quest:PauseAllNonScriptedEntities(false)
    ::LAB_00d02889::
    resources:DestroyMovie(movie2)
    goto LAB_00d02851
    -- TODO(native): code_r0x00d0283a:
    if not quest:NewScriptFrame(me) then goto LAB_00d02851 end
    goto LAB_00d01776
end

-- Gate1GuardOuter.Init (retail 0x00d01590)
function Init(quest, me)
    aiState = 0
    if not quest:GetStateBool("Gate1Open") then
        quest:SetThingHasInformation(me, false, true, false)
    end
    quest:EntitySetInFaction(me, "FACTION_TWINBLADE_CAMP_BANDITS")
    quest:SetIsPushableByHero(me, false)
    quest:SetStateBool("AttackedOuterGateGuards", false)
end

-- Gate1GuardOuter.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- Gate1GuardOuter.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end


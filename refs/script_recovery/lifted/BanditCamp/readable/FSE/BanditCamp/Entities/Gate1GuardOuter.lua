-- Readable native conversion: Gate1GuardOuter. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    EXPERIENCE_GRANT_MEDIUM = 64,  -- 50
}

-- per-entity fields (native class members; one Lua state per entity instance)
local aiState

-- Gate1GuardOuter.Main (retail 0x00d01630)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult, scratchValue, scratchValue14, timerId, hero20, gate1, hero26, pQuestName
    local line, getNearestWithDefName, movie2, resource, scratchValue38
    quest:NewScriptFrame(me)
    if not quest:IsActiveThreadTerminating() then
        resource = resources:NewResource()
        resources:PrepareResource(resource)
        while not resources:TryAcquire(resource, me, 4) do
            if not quest:NewScriptFrame(me) then goto LAB_00d02a4d end
        end
        if not quest:IsActiveThreadTerminating() then
            timerId = quest:RegisterTimer()
            quest:SetTimer(timerId, 0)
            scratchValue = 0
            scratchValue14 = 0
            if not quest:IsActiveThreadTerminating() then goto LAB_00d01776 end
            goto FLOW_past_lab_00d0274f
        end
        goto LAB_00d02a4d
    end
    goto FLOW_past_lab_00d01776
    ::LAB_00d01776::
    if not quest:GetStateBool("Gate1Open") then
        if not quest:IsActiveThreadTerminating() then
            if quest:GetTimer(timerId) < 1 then
                if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d0274f end
                hero26 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, hero26, false)
                local conversationId = quest:AddNewConversation(me, false, false)
                local hero = quest:GetHero()
                quest:AddPersonToConversation(conversationId, hero)
                if scratchValue == 0 then
                    if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d0274f end
                    scratchValue = 1
                    local hero2 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_009_BANDIT1_CALL_OVER_FIRST", me, hero2, false)
                else
                    if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d0274f end
                    local hero3 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_009_BANDIT1_CALL_OVER_SECOND", me, hero3, false)
                end
                quest:SetTimer(timerId, 10)
            end
            if not me:IsTalkedToByHero() then goto LAB_00d0274f end
            if not quest:IsActiveThreadTerminating() then
                movie2 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                local hero4 = quest:GetHero()
                if quest:IsWearingClothingItem(hero4, "OBJECT_HERO_SHIRT_BANDITCAMP") then
                    goto LAB_00d01abe
                else
                    local hero5 = quest:GetHero()
                    if quest:IsWearingClothingItem(hero5, "OBJECT_HERO_TROUSERS_BANDITCAMP") then goto LAB_00d01abe end
                    local hero6 = quest:GetHero()
                    if quest:IsWearingClothingItem(hero6, "OBJECT_HERO_HAT_BANDITCAMP") then goto LAB_00d01abe end
                    local hero7 = quest:GetHero()
                    if quest:IsWearingClothingItem(hero7, "OBJECT_HERO_BOOTS_BANDITCAMP") then goto LAB_00d01abe end
                    local hero8 = quest:GetHero()
                    predicateResult = true
                    if quest:IsWearingClothingItem(hero8, "OBJECT_HERO_GLOVES_BANDITCAMP") then goto LAB_00d01abe end
                end
                goto FLOW_past_lab_00d01abe
                ::LAB_00d01abe::
                predicateResult = false
                ::FLOW_past_lab_00d01abe::
                if predicateResult then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d02870 end
                    local scratchValue33 = resources:ScriptThing(resource)
                    if 0.0 < quest:GetHealth(scratchValue33) then
                        line = "TEXT_QST_009_BANDIT1_NO_DISGUISE"
                        local hero9 = quest:GetHero()
                        if not me:Speak(hero9, line, 0, false, true, false) then
                            goto LAB_00d02870
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00d02870 end
                        end
                        goto LAB_00d01be9
                    end
                    goto FLOW_hoist_lab_00d01be9_1
                end
                goto FLOW_past_lab_00d01be9
                ::LAB_00d01be9::
                if quest:IsActiveThreadTerminating() then goto LAB_00d02870 end
                ::FLOW_hoist_lab_00d01be9_1::
                scratchValue14 = 1
                ::LAB_00d02592::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
                goto LAB_00d0274f
                ::FLOW_past_lab_00d01be9::
                local hero10 = quest:GetHero()
                if quest:IsWearingClothingItem(hero10, "OBJECT_HERO_SHIRT_BANDITCAMP") then
                    local hero11 = quest:GetHero()
                    if not quest:IsWearingClothingItem(hero11, "OBJECT_HERO_TROUSERS_BANDITCAMP") then
                        if not quest:IsActiveThreadTerminating() then
                            local scratchValue40 = resources:ScriptThing(resource)
                            if 0.0 < quest:GetHealth(scratchValue40) then
                                line = "TEXT_QST_009_BANDIT1_NO_TROUSERS"
                                local hero12 = quest:GetHero()
                                if not me:Speak(hero12, line, 0, false, true, false) then
                                    goto LAB_00d02870
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00d02870 end
                                end
                                goto LAB_00d01be9
                            end
                            goto FLOW_hoist_lab_00d01be9_1
                        end
                        goto LAB_00d02870
                    end
                    local hero13 = quest:GetHero()
                    if not quest:IsWearingClothingItem(hero13, "OBJECT_HERO_HAT_BANDITCAMP") then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d02870 end
                        local scratchValue34 = resources:ScriptThing(resource)
                        if quest:GetHealth(scratchValue34) <= 0.0 then goto FLOW_hoist_lab_00d01be9_1 end
                        line = "TEXT_QST_009_BANDIT1_NO_HAT"
                        local hero14 = quest:GetHero()
                        if not me:Speak(hero14, line, 0, false, true, false) then
                            goto LAB_00d02870
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00d02870 end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d02870 end
                        scratchValue14 = 1
                        goto LAB_00d02592
                    end
                    local hero15 = quest:GetHero()
                    if not quest:IsWearingClothingItem(hero15, "OBJECT_HERO_BOOTS_BANDITCAMP") then
                        if not quest:IsActiveThreadTerminating() then
                            local scratchValue37 = resources:ScriptThing(resource)
                            if 0.0 < quest:GetHealth(scratchValue37) then
                                line = "TEXT_QST_009_BANDIT1_NO_BOOTS"
                                local hero16 = quest:GetHero()
                                if not me:Speak(hero16, line, 0, false, true, false) then
                                    goto LAB_00d02870
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00d02870 end
                                end
                                goto LAB_00d01be9
                            end
                            goto FLOW_hoist_lab_00d01be9_1
                        end
                        goto LAB_00d02870
                    end
                    local hero17 = quest:GetHero()
                    local isWearingClothingItem9 = quest:IsWearingClothingItem(hero17, "OBJECT_HERO_GLOVES_BANDITCAMP")
                    local isWearingClothingItem = quest:IsActiveThreadTerminating()
                    if not isWearingClothingItem9 then
                        if not isWearingClothingItem then
                            local scratchValue30 = resources:ScriptThing(resource)
                            if quest:GetHealth(scratchValue30) <= 0.0 then goto FLOW_hoist_lab_00d01be9_1 end
                            line = "TEXT_QST_009_BANDIT1_NO_GLOVES"
                            local hero18 = quest:GetHero()
                            if not me:Speak(hero18, line, 0, false, true, false) then
                                goto LAB_00d02870
                            end
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then goto LAB_00d02870 end
                            end
                            if not quest:IsActiveThreadTerminating() then scratchValue14 = 1; goto LAB_00d02592 end
                        end
                        goto LAB_00d02870
                    end
                    if not isWearingClothingItem then
                        if scratchValue14 == 1 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d02870 end
                            local scratchValue36 = resources:ScriptThing(resource)
                            if 0.0 < quest:GetHealth(scratchValue36) then
                                line = "TEXT_QST_009_BANDIT1_TRIED_AND_IN"
                                local hero19 = quest:GetHero()
                                if not me:Speak(hero19, line, 0, false, true, false) then
                                    goto LAB_00d02870
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00d02870 end
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00d02870 end
                            end
                        else
                            goto FLOW_past_lab_00d022ba
                        end
                        ::LAB_00d022ba::
                        hero20 = quest:GetHero()
                        getNearestWithDefName = quest:GetNearestWithDefName(hero20, "VILLAGE_BANDIT_CAMP_MAIN")
                        if quest:IsSleepingTime(getNearestWithDefName) then
                            goto FLOW_past_lab_00d023d3
                        end
                        ::LAB_00d023d3::
                        scratchValue38 = resources:ScriptThing(resource)
                        if 0.0 < quest:GetHealth(scratchValue38) then
                            line = "TEXT_QST_009_BANDIT1_GATE_OPEN"
                            local hero21 = quest:GetHero()
                            if not me:Speak(hero21, line, 0, false, true, false) then
                                goto LAB_00d02870
                            end
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then goto LAB_00d02870 end
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00d028a8 end
                        end
                        gate1 = quest:GetThingWithScriptName("Gate1")
                        quest:OpenDoor(gate1)
                        pQuestName = quest:GetActiveQuestName()
                        quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_BANDIT_CAMP_OBJECTIVE_05", "BanditCampBoss", "BanditCampEntrance")
                        quest:SetStateBool("Gate1Open", true)
                        quest:GiveHeroExperience(quest:ReadGlobalGameData(SCRIPT_DEF.EXPERIENCE_GRANT_MEDIUM))
                        quest:ClearThingHasInformation(me)
                        goto LAB_00d02592
                        ::FLOW_past_lab_00d023d3::
                        if not quest:IsActiveThreadTerminating() then
                            local scratchValue39 = resources:ScriptThing(resource)
                            if 0.0 < quest:GetHealth(scratchValue39) then
                                line = "TEXT_QST_009_BANDIT1_BANDITS_SLEEPING"
                                local hero22 = quest:GetHero()
                                if not me:Speak(hero22, line, 0, false, true, false) then
                                    goto LAB_00d02870
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00d02870 end
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00d028a8 end
                            end
                            goto LAB_00d023d3
                        end
                        ::LAB_00d028a8::
                        goto LAB_00d02870
                        ::FLOW_past_lab_00d022ba::
                        if not quest:IsActiveThreadTerminating() then
                            local scratchValue32 = resources:ScriptThing(resource)
                            if 0.0 < quest:GetHealth(scratchValue32) then
                                line = "TEXT_QST_009_BANDIT1_STRAIGHT_IN"
                                local hero23 = quest:GetHero()
                                if not me:Speak(hero23, line, 0, false, true, false) then
                                    goto LAB_00d02870
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00d02870 end
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00d02870 end
                            end
                            goto LAB_00d022ba
                        end
                    end
                    goto LAB_00d02870
                end
                if not quest:IsActiveThreadTerminating() then
                    local scratchValue31 = resources:ScriptThing(resource)
                    if 0.0 < quest:GetHealth(scratchValue31) then
                        line = "TEXT_QST_009_BANDIT1_NO_SHIRT"
                        local hero24 = quest:GetHero()
                        if not me:Speak(hero24, line, 0, false, true, false) then
                            goto LAB_00d02870
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00d02870 end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d02870 end
                    end
                    scratchValue14 = 1
                    goto LAB_00d02592
                end
                quest:PauseAllNonScriptedEntities(false)
                goto LAB_00d02889
            end
            goto FLOW_past_lab_00d0274f
        end
        quest:DeregisterTimer(timerId)
        goto LAB_00d02a4d
    end
    if not quest:IsActiveThreadTerminating() then
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d0274f end
            local movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            local scratchValue35 = resources:ScriptThing(resource)
            if 0.0 < quest:GetHealth(scratchValue35) then
                line = "TEXT_QST_009_BANDIT1_ASIDE"
                local hero25 = quest:GetHero()
                if not me:Speak(hero25, line, 0, false, true, false) then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    goto FLOW_past_lab_00d0274f
                end
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        goto FLOW_past_lab_00d0274f
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    goto FLOW_past_lab_00d0274f
                end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
        end
    else
        goto FLOW_past_lab_00d0274f
    end
    ::LAB_00d0274f::
    if not (not not (me:MsgIsHitByHero() or (me:MsgIsHitByAnySpecialAbilityFromHero() and (not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL))))) then
        if not quest:GetStateBool("AttackedOuterGateGuards") then goto LAB_00d0283a end
        if not quest:IsActiveThreadTerminating() then
            hero26 = quest:GetHero()
            quest:GiveThingBestEnemyTarget(me, hero26)
            resources:PrepareResource(resource)
            repeat
                quest:NewScriptFrame(me)
            until quest:IsActiveThreadTerminating()
        end
        goto FLOW_past_lab_00d0274f
    end
    if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d0274f end
    if 0.0 < quest:GetHealth(me) then
        if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d0274f end
        local conversationId2 = quest:AddNewConversation(me, false, false)
        hero26 = quest:GetHero()
        quest:AddPersonToConversation(conversationId2, hero26)
        hero26 = quest:GetHero()
        quest:AddLineToConversation(conversationId2, "TEXT_QST_009_BANDIT1_ATTACKED_NEW", me, hero26, false)
    end
    quest:SetStateBool("AttackedOuterGateGuards", true)
    hero26 = quest:GetHero()
    quest:GiveThingBestEnemyTarget(me, hero26)
    quest:ClearThingHasInformation(me)
    resources:PrepareResource(resource)
    repeat
        quest:NewScriptFrame(me)
    until quest:IsActiveThreadTerminating()
    quest:DeregisterTimer(timerId)
    goto LAB_00d02a4d
    ::FLOW_past_lab_00d0274f::
    quest:DeregisterTimer(timerId)
    ::LAB_00d02a4d::
    resources:ReleaseResource(resource)
    ::FLOW_past_lab_00d01776::
    do return end
    ::LAB_00d02870::
    quest:PauseAllNonScriptedEntities(false)
    ::LAB_00d02889::
    resources:DestroyMovie(movie2)
    goto FLOW_past_lab_00d0274f
    ::LAB_00d0283a::
    if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00d0274f end
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


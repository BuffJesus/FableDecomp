-- Readable native conversion: BCGameMaster. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    EXPERIENCE_GRANT_MEDIUM = 64,  -- 50
}

-- per-entity fields (native class members; one Lua state per entity instance)
local givenPass_

-- BCGameMaster.Main (retail 0x00d067a0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local isObjectInThingsPossession, isObjectInThingsPossession2, predicateResult
    local isObjectInThingsPossession3, isObjectInThingsPossession4, isObjectInThingsPossession5
    local taskRunning, taskRunning2, sequence, sequence22, sequence32, hero, hero6, speechResult
    local speechResult2, movie, x_stk_2c_3
    predicateResult = false
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    if not givenPass_ then
        if quest:IsActiveThreadTerminating() then goto LAB_00d071d7 end
        givenPass_ = true
        quest:GiveThingHeroRewardItem(me, "OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", "")
    end
    quest:SetThingHasInformation(me, false, true, false)
    if not (quest:GetStateBool("PubGameChatted")) then
        predicateResult = true
        hero = quest:GetHero()
        isObjectInThingsPossession = quest:IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", hero)
        sequence = isObjectInThingsPossession
        if not sequence then
            isObjectInThingsPossession = true
            sequence = quest:GetStateBool("Gate2Open")
        end
        if sequence then goto LAB_00d068cd end
        goto FLOW_past_lab_00d068cd
    end
    ::LAB_00d068cd::
    isObjectInThingsPossession = false
    ::FLOW_past_lab_00d068cd::
    if predicateResult then
    end
    if isObjectInThingsPossession then
        if not quest:IsActiveThreadTerminating() then
            resources:PrepareResource(resource)
            while not resources:TryAcquire(resource, me, 4) do
                if not quest:NewScriptFrame(me) then goto LAB_00d071d7 end
            end
            if not quest:IsActiveThreadTerminating() then
                repeat
                    if me:IsTalkedToByHero() then
                        goto LAB_00d069c6
                    else
                        local hero2 = quest:GetHero()
                        isObjectInThingsPossession2 = quest:IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", hero2)
                        sequence22 = isObjectInThingsPossession2
                        if not sequence22 then
                            isObjectInThingsPossession2 = true
                            sequence22 = quest:GetStateBool("Gate2Open")
                        end
                        if sequence22 then goto LAB_00d069c6 end
                    end
                    goto FLOW_past_lab_00d069c6
                    ::LAB_00d069c6::
                    isObjectInThingsPossession2 = false
                    ::FLOW_past_lab_00d069c6::
                    if not isObjectInThingsPossession2 then
                        if quest:IsActiveThreadTerminating() then break end
                        local hero3 = quest:GetHero()
                        isObjectInThingsPossession3 = quest:IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", hero3)
                        sequence32 = isObjectInThingsPossession3
                        if not sequence32 then
                            isObjectInThingsPossession3 = true
                            sequence32 = quest:GetStateBool("Gate2Open")
                        end
                        if sequence32 then
                            isObjectInThingsPossession3 = false
                        end
                        if not isObjectInThingsPossession3 then
                            if not quest:IsActiveThreadTerminating() then
                                resources:PrepareResource(resource)
                                quest:ClearThingHasInformation(me)
                                repeat
                                    quest:NewScriptFrame(me)
                                until quest:IsActiveThreadTerminating()
                                resources:ReleaseResource(resource)
                                return
                            end
                            break
                        end
                        if quest:IsActiveThreadTerminating() then break end
                        movie = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        if not quest:GetStateBool("SpokenToSecondGuard") then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d06bc5 end
                            local x_stk_2c_1 = resources:ScriptThing(resource)
                            if quest:GetHealth(x_stk_2c_1) <= 0.0 then goto LAB_00d06c8d end
                            local hero4 = quest:GetHero()
                            speechResult = me:Speak(hero4, "TEXT_QST_009_GAMES_MASTER_INTRO_GUARD_NOT_SPOKEN", GROUP_SELECT_FIRST, false, true, false)
                            taskRunning = me:IsPerformingScriptTask()
                            goto LAB_00d06c55
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d06bc5 end
                        local x_stk_2c_2 = resources:ScriptThing(resource)
                        if quest:GetHealth(x_stk_2c_2) <= 0.0 then goto LAB_00d06c8d end
                        local hero5 = quest:GetHero()
                        speechResult2 = me:Speak(hero5, "TEXT_QST_009_GAMES_MASTER_INTRO_GUARD_SPOKEN", GROUP_SELECT_FIRST, false, true, false)
                        taskRunning2 = me:IsPerformingScriptTask()
                        goto LAB_00d06bbe
                    end
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        resources:ReleaseResource(resource)
                        do return end
                    end
                until false
            end
        end
        goto LAB_00d071d7
    end
    ::LAB_00d06cd0::
    hero6 = quest:GetHero()
    isObjectInThingsPossession4 = quest:IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", hero6)
    if isObjectInThingsPossession4 or quest:GetStateBool("Gate2Open") then
        isObjectInThingsPossession4 = true
    end
    if isObjectInThingsPossession4 then
        if not quest:IsActiveThreadTerminating() then
            resources:PrepareResource(resource)
            quest:ClearThingHasInformation(me)
            repeat
                quest:NewScriptFrame(me)
            until quest:IsActiveThreadTerminating()
            resources:ReleaseResource(resource)
            return
        end
    else
        quest:ClearThingHasInformation(me)
        quest:SetPrizeTavernTable(false)
        while not quest:GetSpotTheAdditionBeaten() do
            if not quest:NewScriptFrame(me) then goto LAB_00d071d7 end
            local hero7 = quest:GetHero()
            isObjectInThingsPossession5 = quest:IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", hero7)
            if isObjectInThingsPossession5 or quest:GetStateBool("Gate2Open") then
                isObjectInThingsPossession5 = true
            end
            if isObjectInThingsPossession5 then
                if not quest:IsActiveThreadTerminating() then
                    quest:SetPrizeTavernTable(true)
                    resources:PrepareResource(resource)
                    repeat
                        quest:NewScriptFrame(me)
                    until quest:IsActiveThreadTerminating()
                    resources:ReleaseResource(resource)
                    return
                end
                goto LAB_00d071d7
            end
        end
        if not (quest:IsActiveThreadTerminating()) then
            quest:SetPrizeTavernTable(true)
            quest:SetQuitTavernGame(true)
            while true do
                if not (0.0 == quest:GetBestTimeGuessTheAddition() or quest:IsHeroInTavernGame()) then break end
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    resources:ReleaseResource(resource)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00d071d7 end
            movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            resources:PrepareResource(resource)
            while not resources:TryAcquire(resource, me, 4) do
                if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00d06c8d end
            end
            if quest:IsActiveThreadTerminating() then
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                resources:ReleaseResource(resource)
                return
            end
            x_stk_2c_3 = resources:ScriptThing(resource)
            if 0.0 < quest:GetHealth(x_stk_2c_3) then
                local hero8 = quest:GetHero()
                if not me:Speak(hero8, "TEXT_QST_009_GAMES_MASTER_WON", GROUP_SELECT_FIRST, false, true, false) then
                    goto LAB_00d06bc5
                end
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then goto LAB_00d06bc5 end
                end
                if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d06c8d end
            end
            quest:GiveHeroObject("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", -1, false)
            quest:RemoveItemFromContainer(me, "OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS")
            quest:GiveHeroExperience(quest:ReadGlobalGameData(SCRIPT_DEF.EXPERIENCE_GRANT_MEDIUM))
            resources:PrepareResource(resource)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            repeat
                quest:NewScriptFrame(me)
            until quest:IsActiveThreadTerminating()
        end
    end
    ::LAB_00d071d7::
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00d06bbe::
    if not taskRunning2 then goto LAB_00d06c7b end
    if not quest:NewScriptFrame(me) then goto LAB_00d06bc5 end
    taskRunning2 = me:IsPerformingScriptTask()
    goto LAB_00d06bbe
    ::LAB_00d06c55::
    if not taskRunning then goto LAB_00d06c7b end
    if not (not quest:NewScriptFrame(me)) then
        taskRunning = me:IsPerformingScriptTask()
        goto LAB_00d06c55
    end
    ::LAB_00d06bc5::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00d06c7b::
    if not (not quest:IsActiveThreadTerminating()) then
        goto FLOW_past_lab_00d06c8d
    end
    ::LAB_00d06c8d::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:PrepareResource(resource)
    quest:SetStateBool("PubGameChatted", true)
    goto LAB_00d06cd0
    ::FLOW_past_lab_00d06c8d::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:ReleaseResource(resource)
end

-- BCGameMaster.Init (retail 0x00d06760)
function Init(quest, me)
    givenPass_ = false
end

-- BCGameMaster.OnPersist (retail 0x00d0ee10)
function OnPersist(quest, me, context)
    local givenPass = givenPass_ or false
    givenPass_ = quest:PersistTransferBool(context, "GivenPass", givenPass)
end

-- BCGameMaster.OnPredicateFail (retail 0x00d06770)
function OnPredicateFail(quest, me)
end


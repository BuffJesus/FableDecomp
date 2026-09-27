-- Readable native conversion: BCGameMaster. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    EXPERIENCE_GRANT_MEDIUM = 64,  -- 50
}

-- per-entity fields (native class members; one Lua state per entity instance)
local givenPass

-- BCGameMaster.Main (retail 0x00d067a0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local isObjectInThingsPossession, isObjectInThingsPossession2, isObjectInThingsPossession3
    local isObjectInThingsPossession4, isObjectInThingsPossession5, taskRunning, taskRunning2
    local sequence, sequence22, sequence32, movie
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    if not givenPass then
        givenPass = true
        quest:GiveThingHeroRewardItem(me, "OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", "")
    end
    quest:SetThingHasInformation(me, false, true, false)
    if quest:GetStateBool("PubGameChatted") then goto LAB_00d068cd end
    isObjectInThingsPossession = quest:IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", hero)
    sequence = isObjectInThingsPossession
    if not sequence then
        isObjectInThingsPossession = true
        sequence = quest:GetStateBool("Gate2Open")
    end
    if sequence then goto LAB_00d068cd end
    goto FLOW_past_lab_00d068cd
    ::LAB_00d068cd::
    isObjectInThingsPossession = false
    ::FLOW_past_lab_00d068cd::
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
                        isObjectInThingsPossession2 = quest:IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", hero)
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
                        isObjectInThingsPossession3 = quest:IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", hero)
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
                        movie = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        if not quest:GetStateBool("SpokenToSecondGuard") then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d06bc5 end
                            if quest:GetHealth(resources:ScriptThing(resource)) <= 0.0 then goto LAB_00d06c8d end
                            me:Speak(hero, "TEXT_QST_009_GAMES_MASTER_INTRO_GUARD_NOT_SPOKEN", GROUP_SELECT_FIRST, false, true, false)
                            taskRunning = me:IsPerformingScriptTask()
                            goto LAB_00d06c55
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d06bc5 end
                        if quest:GetHealth(resources:ScriptThing(resource)) <= 0.0 then goto LAB_00d06c8d end
                        me:Speak(hero, "TEXT_QST_009_GAMES_MASTER_INTRO_GUARD_SPOKEN", GROUP_SELECT_FIRST, false, true, false)
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
    isObjectInThingsPossession4 = quest:IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", hero)
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
        goto LAB_00d071d7
    end
    quest:ClearThingHasInformation(me)
    quest:SetPrizeTavernTable(false)
    while not quest:GetSpotTheAdditionBeaten() do
        if not quest:NewScriptFrame(me) then goto LAB_00d071d7 end
        isObjectInThingsPossession5 = quest:IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", hero)
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
    if quest:IsActiveThreadTerminating() then goto LAB_00d071d7 end
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
        if not quest:NewScriptFrame(me) then goto LAB_00d070e0 end
    end
    if quest:IsActiveThreadTerminating() then
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        resources:ReleaseResource(resource)
        return
    end
    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
        if not me:Speak(hero, "TEXT_QST_009_GAMES_MASTER_WON", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d06bc5 end
        if quest:IsActiveThreadTerminating() then goto LAB_00d070e0 end
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
    if not quest:NewScriptFrame(me) then goto LAB_00d06bc5 end
    taskRunning = me:IsPerformingScriptTask()
    goto LAB_00d06c55
    ::LAB_00d06bc5::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00d06c7b::
    if not quest:IsActiveThreadTerminating() then goto LAB_00d06c8d end
    goto FLOW_past_lab_00d06c8d
    ::LAB_00d06c8d::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:PrepareResource(resource)
    quest:SetStateBool("PubGameChatted", true)
    goto LAB_00d06cd0
    ::FLOW_past_lab_00d06c8d::
    ::LAB_00d070e0::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:ReleaseResource(resource)
end

-- BCGameMaster.Init (retail 0x00d06760)
function Init(quest, me)
    givenPass = false
end

-- BCGameMaster.OnPersist (retail 0x00d0ee10)
function OnPersist(quest, me, context)
    quest:SetStateBool("GivenPass", quest:PersistTransferBool(context, "GivenPass", quest:GetStateBool("GivenPass")))
end

-- BCGameMaster.OnPredicateFail (retail 0x00d06770)
function OnPredicateFail(quest, me)
end

